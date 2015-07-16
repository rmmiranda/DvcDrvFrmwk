/*****************************************************************************
 * Filename:    rei_critical.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Jan 07, 2009
 *
 * Description: This file defines the REI(runtime environment interface) for 
 *              critical section and semaphores. The original data structures 
 *              and functions are defined in file armos/common/include/critical.h. 
 *              Please refer to it for the usage of message queue.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *
 ****************************************************************************/
#ifndef __REI_CRITCAL_H__
#define __REI_CRITCAL_H__

#include "rei_basetype.h"

/* REI interface for semaphores and critical section*/
/* Functions */
#define RE_RimInitSemaphore         RimInitSemaphore
#define RE_RimLockSemaphore         RimLockSemaphore
#define RE_RimUnlockSemaphore       RimUnlockSemaphore

#define RE_RimInitCountingSemaphore         RimInitCountingSemaphore
#define RE_RimLockCountingSemaphore         RimLockCountingSemaphore
#define RE_RimLockCountingSemaphoreWithTimeout       RimLockCountingSemaphoreWithTimeout
#define RE_RimUnlockCountingSemaphore       RimUnlockCountingSemaphore

#define RE_RimEnterCriticalSection         RimEnterCriticalSection
#define RE_RimLeaveCriticalSection         RimLeaveCriticalSection
#define RE_RimCheckCriticalSection         RimCheckCriticalSection

/* Semaphore data structures */
#define RE_Semaphore            Semaphore
#define RE_CountingSemaphore    CountingSemaphore

/* ==========================================================================*/
/* DO NOT use these declarations directly in platform software. Use the REI 
 * defines instead */
#define __BASETYPE_H__
#ifndef IPIFUNC
#define IPIFUNC
#endif
#include "critical.h"

#endif //__REI_CRITCAL_H__
