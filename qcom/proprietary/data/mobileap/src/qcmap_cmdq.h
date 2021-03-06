/******************************************************************************

                          QCMAP _ C M D Q . H

******************************************************************************/
/*===========================================================================
 
Copyright (c) 2012 Qualcomm Technologies, Inc. All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential 

===========================================================================*/

/******************************************************************************

  @file    qcmp_cmdq.h
  @brief   QCMAP command queues header file

  DESCRIPTION
  Header file for QCMAP command queues.

******************************************************************************/

/******************************************************************************

                      EDIT HISTORY FOR FILE

  $Id:$

when       who        what, where, why
--------   ---        -------------------------------------------------------
5/21/2012  sc         Initial version

******************************************************************************/

#ifndef __QCMAP_CMDQ_H__
#define __QCMAP_CMDQ_H__


#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================
                              INCLUDE FILES
===========================================================================*/
#include "comdef.h"

#include "ds_cmdq.h"
#include "dssocket.h"
#include "ps_ipfltr_defs.h"
#include "ps_iface.h"
#include "qualcomm_mobile_access_point_v01.h"
#include "comdef.h"

/*===========================================================================
                     GLOBAL DEFINITIONS AND DECLARATIONS
===========================================================================*/

#define QCMAP_CMDQ_SUCCESS (0)

/*--------------------------------------------------------------------------- 
   Type of a Executive event data
---------------------------------------------------------------------------*/
typedef struct qcmap_cmdq_cmd_data_s {	
  sint15            dss_nethandle;              /* Application ID         */
  dss_iface_id_type            iface_id;        /* Interface ID structure */
  sint15            dss_errno;                  /* Type of network err    */
} qcmap_cmdq_cmd_data_t;

/*--------------------------------------------------------------------------- 
   Type of a Executive command
---------------------------------------------------------------------------*/
typedef struct qcmap_cmdq_cmd_s {
  ds_cmd_t                  cmd;                /* Command object         */
  qcmap_cmdq_cmd_data_t    data;                /* Command data           */
  int                       tracker;            /* 1 if alloc, else 0     */
} qcmap_cmdq_cmd_t;

/*--------------------------------------------------------------------------- 
   Type representing collection of state information for module
---------------------------------------------------------------------------*/
struct qcmap_cmdq_state_s {
  struct ds_cmdq_info_s       cmdq;  /* Command queue for async processing */  
};


/*===========================================================================
                            GLOBAL FUNCTION DECLARATIONS
===========================================================================*/

/*===========================================================================
  FUNCTION  qcmap_cmdq_get_cmd
===========================================================================*/
/*!
@brief
  Function to get a command buffer for asynchronous processing

@return
  void

@note

  - Dependencies
    - None  

  - Side Effects
    - Allocated heap memory
*/
/*=========================================================================*/
qcmap_cmdq_cmd_t * qcmap_cmdq_get_cmd ( void );

/*===========================================================================
  FUNCTION  qcmap_cmdq_release_cmd
===========================================================================*/
/*!
@brief
  Function to release a command buffer 

@return
  void

@note

  - Dependencies
    - None  

  - Side Effects
    - Returns memory to heap
*/
/*=========================================================================*/
void qcmap_cmdq_release_cmd ( qcmap_cmdq_cmd_t * );

/*===========================================================================
  FUNCTION  qcmap_cmdq_put_cmd
===========================================================================*/
/*!
@brief
  Function to post a command buffer for asynchronous processing

@return
  int - QCMAP_SUCCESS on successful operation, QCMAP_FAILURE otherwise

@note

  - Dependencies
    - None  

  - Side Effects
    - None
*/
/*=========================================================================*/
int qcmap_cmdq_put_cmd ( const qcmap_cmdq_cmd_t * cmdbuf );

/*===========================================================================
  FUNCTION  qcmap_cmdq_wait
===========================================================================*/
/*!
@brief
  Forces calling thread to wait on exit of command queue thread.

@return
  void

@note

  - Dependencies
    - None  

  - Side Effects
    - Calling thread is blocked indefinitely
*/
/*=========================================================================*/
void qcmap_cmdq_wait ( void );

/*===========================================================================
  FUNCTION  qcmap_cmdq_init
===========================================================================*/
/*!
@brief
 Main initialization routine of the executive control module. 

@return
  void

@note

  - Dependencies
    - None  

  - Side Effects
    - None
*/
/*=========================================================================*/
extern void qcmap_cmdq_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __QCMAP_CMDQ_H__ */
