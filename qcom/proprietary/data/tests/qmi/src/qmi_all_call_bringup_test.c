#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "qmi.h"
#include "qmi_wds_srvc.h"


int qmi_client_handle;

/*qmi message library handle*/
int qmi_handle = QMI_INVALID_CLIENT_HANDLE;

int main(int argc, char *argv[])
{
  int return_code = 0;
  int qmi_client_handle;
  int qmi_err_code;
  int rc;
  qmi_wds_start_nw_if_params_type start_nw_params;
  qmi_wds_call_end_reason_type    call_end_reason;

  qmi_wds_indication_reg_req_type req_type;
  qmi_wds_embms_tmgi_type tmgi;
  qmi_wds_embms_tmgi_list_type list_type;
  qmi_wds_embms_tmgi_list_ind_type list_query;

  (void)argc; (void)argv;
  
  call_end_reason.call_end_reason_verbose.verbose_reason  
    = QMI_WDS_VERBOSE_CE_INVALID;

  qmi_handle = qmi_init(NULL, NULL);

  if (qmi_handle < 0)
  {
    fprintf (stderr,"Unable to acquire qmi handle \n");
    fprintf (stderr, "Test failed!!\n");
    return -1;
  }
   
  /* Initialize connection to first QMI control connection (corresponds to SMD_DATA_5 for data transfer */
  if ((rc=qmi_connection_init (QMI_PORT_RMNET_1, &qmi_err_code)) < 0)
  {
    fprintf (stderr,"Unable to open QMI RMNET_1 port rc=%d, qmi_err_code=%x",rc,qmi_err_code);
    return_code = -1;
    goto exit0;
  }

  /* Initialize connection to second QMI control connection (corresponds to SMD_DATA_6 for data transfer */

  fprintf (stdout,"Starting WDS services.... \n");

  /* Bring up WDS service for first port */
  if ((qmi_client_handle = qmi_wds_srvc_init_client (QMI_PORT_RMNET_1,NULL,NULL,&qmi_err_code)) < 0)
  {
    fprintf (stderr,"Unable to start WDS service on port #1 rc = %d, qmi_err_code=%d\n",qmi_client_handle,qmi_err_code);
    return_code = -1;
    goto exit3;
  }


  fprintf (stdout, "sucessfully opened all QMI WDS client\n");

  /* Start N/W interface #1 synchronously, use start_nw_params to specify 
  ** the PDP profile index you want so associate with the call
  */

  start_nw_params.params_mask = 0;
  if (  (rc = qmi_wds_start_nw_if (qmi_client_handle,
                                   &start_nw_params,
                                   NULL,
                                   NULL,
                                   &call_end_reason,
                                   &qmi_err_code)) < 0)
  {
    fprintf (stderr,"unable to start NW IF #1, rc=%d, qmi_err_code = %x\n",rc,qmi_err_code);
    fprintf (stderr,"Verbose Call End Reason Code is = %d \n",
             call_end_reason.call_end_reason_verbose.verbose_reason);
    return_code = -1;
    goto exit3;
  }
  else
  {
    fprintf (stdout,"Sucessfully started NW IF #1\n");
  }


  fprintf (stdout,"Network interface started... will stop in 3 seconds\n");

  sleep (3);

  fprintf (stdout,"Stopping all network interfaces\n");


  if (  (rc = qmi_wds_stop_nw_if (qmi_client_handle,NULL,NULL,&qmi_err_code)) < 0)
  {
    fprintf (stderr,"unable to stop NW IF, rc=%d, qmi_err_code = %x\n",rc,qmi_err_code);
    return_code = -1;
    goto exit3;

  } 


  fprintf(stdout, "Indication Registering...\n");
  req_type.param_mask = QMI_WDS_EMBMS_TMGI_INDICATION_REG_LIST_PARAM_MASK;
  req_type.tmgi_list_pref = (qmi_wds_embms_tmgi_ind_pref_type)QMI_WDS_EMBMS_TMGI_IND_REPORT;
  if((rc = qmi_wds_indication_register(qmi_client_handle,req_type ,&qmi_err_code)) < 0)
  {
    fprintf(stderr, "unable to register indication, rc=%d, qmi_err_code =%x\n", rc, qmi_err_code);
    return_code = -1;
    goto exit3;
  }
  sleep(2);

  fprintf(stdout, "TMGI activating....\n");

  qmi_wds_embms_actdeact_tmgi_info_type act_tmgi;
  memset(&act_tmgi, 0, sizeof(act_tmgi));
  act_tmgi.dbg_trace_id = 1;
  act_tmgi.earfcn_list_len = 1;
  act_tmgi.earfcn_list[0] = 11;
  act_tmgi.tmgi_list_len = 1;
  act_tmgi.tmgi_list->session_id_valid = 0;
  act_tmgi.tmgi_list->tmgi[0] ='A';
  act_tmgi.tmgi_list->tmgi[1] ='C';
  act_tmgi.tmgi_list->tmgi[2] ='T';
  act_tmgi.tmgi_list->tmgi[3] ='M';
  act_tmgi.tmgi_list->tmgi[4] ='G';
  act_tmgi.tmgi_list->tmgi[5] ='I';

  if((rc = qmi_wds_embms_tmgi_activate(qmi_client_handle, &act_tmgi, &qmi_err_code)) < 0)
  {
    fprintf(stderr, "unable to activate TMGI, rc=%d, qmi_err_code=%x\n", rc, qmi_err_code);
    return_code = -1;
    goto exit3;
  }
  sleep(2);

  fprintf(stdout, "TMGI deactivating...\n");

  qmi_wds_embms_actdeact_tmgi_info_type deact_tmgi;
  memset(&deact_tmgi, 0, sizeof(deact_tmgi));
  deact_tmgi.dbg_trace_id = 1;
  act_tmgi.tmgi_list_len = 1;
  act_tmgi.deactivate_tmgi_list->session_id_valid = 0;
  act_tmgi.deactivate_tmgi_list->tmgi[0] ='D';
  act_tmgi.deactivate_tmgi_list->tmgi[1] ='E';
  act_tmgi.deactivate_tmgi_list->tmgi[2] ='A';
  act_tmgi.deactivate_tmgi_list->tmgi[3] ='C';
  act_tmgi.deactivate_tmgi_list->tmgi[4] ='T';
  act_tmgi.deactivate_tmgi_list->tmgi[5] ='I';
  if((rc = qmi_wds_embms_tmgi_deactivate(qmi_client_handle, &deact_tmgi, &qmi_err_code)) < 0)
  {
    fprintf(stderr, "unable to deactivate TMGI, rc=%d, qmi_err_code=%x\n", rc, qmi_err_code);
    return_code = -1;
    goto exit3;
  }
  sleep(2);

  fprintf(stdout, "TMGI active list query....\n");
  list_type = (qmi_wds_embms_tmgi_list_type)QMI_WDS_EMBMS_TMGI_LIST_ACTIVE;  
  /*QMI_WDS_EMBMS_TMGI_LIST_AVAILABLE */

  if((rc = qmi_wds_embms_tmgi_list_query(qmi_client_handle, list_type, &list_query, &qmi_err_code)) < 0)
  {
    fprintf(stderr, "unable to query list TMGI, rc=%d, qmi_err_code=%x\n", rc, qmi_err_code);
    return_code = -1;
    goto exit3;
  }

  fprintf (stdout,"Interfaces sucessfully stopped, closing clients\n");

exit3: 
  if (  (rc=qmi_wds_srvc_release_client (qmi_client_handle,&qmi_err_code)) < 0)
  {
    fprintf  (stderr,"Unable to release QMI client handle for port #1, rc=%d, qmi_err_code=%d\n",rc,qmi_err_code);
    return_code = -1;
  }

exit0:
  if (return_code == 0)
  {
    fprintf (stdout, "Test ran sucessfully!!\n");
  }
  else
  {
    fprintf (stderr, "Test failed!!\n");
  }
  sleep(2);
  qmi_release(qmi_handle);
  return return_code;
}


