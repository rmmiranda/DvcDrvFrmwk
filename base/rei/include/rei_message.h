/*****************************************************************************
 * Filename:    rei_message.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Jan 07, 2009
 *
 * Description: This file defines the REI(runtime environment interface) for 
 *              message queue. The original data structures and functions are 
 *              defined in file armos/common/unc_pub/message.h. Please refer 
 *              to it for the usage of message queue.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *
 ****************************************************************************/
#ifndef __REI_MESSAGE_H__
#define __REI_MESSAGE_H__

#include "rei_basetype.h"

/* REI interface for Message Queue data structure*/
/* REI define for Message Q functions */
#define RE_RimPostMessage               RimPostMessage
#define RE_RimGetMessage                RimGetMessage
#define RE_RimPeekMessage               RimPeekMessage
#define RE_RimWatchdogHit               RimWatchdogHit
#define RE_RimPriorityMessageCount      RimPriorityMessageCount

/* REI define for Message data structure*/
#define RE_MESSAGE  MESSAGE

/* ==========================================================================*/
/* DO NOT use these declarations directly in platform software. Use the REI 
 * defines instead */
#include "message.h"
/* Message map defines */
typedef void (*MsgCallbackFunc)(MESSAGE * msg);

typedef struct 
{
  DWORD             EventId;
  MsgCallbackFunc   cbFunc;    
}MsgHandlerMap;

/* Driver messages */
#define PLATFORM_MSG_SERIAL_DRV     0x8001
#define PLATFORM_MSG_CAM_DRV        0x8002
#define PLATFORM_MSG_DISP_DRV       0x8003
#define PLATFORM_MSG_HDMI_DRV       0x8004


/* Test application messages */
#define PLATFORM_MSG_CAM_TEST_APP       0x8010
#define PLATFORM_MSG_DIS_TEST_APP       0x8011

/* Thread ID */
#define PLATFORM_THREAD_MAX_MSG     30
#define PLATFORM_THREAD_TOTAL       2

/* Thread ID - Library Specific */
/* TODO: include threadid.h in the build process
#include "threadid.h"

#define PLATFORM_THREAD_ID_DRV  BASE_PLATFORM_THREAD
#define PLATFORM_THREAD_ID_APP0 BASE_PLATFORM_APP0_THREAD
*/


#ifdef NESSUS
#define PLATFORM_THREAD_ID_DRV   0xED
#define PLATFORM_THREAD_ID_APP0  0xEF
#endif
#ifdef MINIOS
    /* Fixes issue in thread enumeration differences between Nessus and Minios */
    #include "threads.h"
    #define PLATFORM_THREAD_ID_APP0     PLATFORM_APP0_THREAD
#endif



#endif //__REI_MESSAGE_H__
