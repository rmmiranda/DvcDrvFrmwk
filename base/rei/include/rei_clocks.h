/*****************************************************************************
 * Filename:    rei_clocks.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     February 2009
 *
 * Description: This file defines the REI(runtime environment interface) for 
 *              clock control.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *
 ****************************************************************************/
 
#ifndef __REI_CLOCKS_H__
#define __REI_CLOCKS_H__

#include "rei_basetype.h"

/* REI interface for Clock */
#define RE_ClockEnable                  ClockEnable
#define RE_ClockDisable                 ClockDisable
#define RE_ClocksAllowSleep             ClocksAllowSleep
#define RE_IsEverySlaveProcessorIdle    IsEverySlaveProcessorIdle
#define RE_DumpRunningClocks            DumpRunningClocks

/* REI structure defines for LDO */
#define RE_PeripheralClock              PeripheralClock

/* ==========================================================================*/
/* DO NOT use these declarations directly in platform software. Use the REI 
 * defines instead */
#include "clocks.h"

extern void ClockEnable( PeripheralClock peripheral );
extern void ClockDisable( PeripheralClock peripheral );
extern BOOL ClocksAllowSleep( void );
extern BOOL IsEverySlaveProcessorIdle( void );
extern void DumpRunningClocks( void );

#endif // __REI_CLOCKS_H__
