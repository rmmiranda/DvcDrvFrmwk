/*****************************************************************************
 * Filename:    rei_rtc.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Jan 07, 2009
 *
 * Description: This file defines the REI(runtime environment interface) for 
 *              RTC. The original data structures and functions are defined in 
 *              file armos/nessus/inc_pub/timer.h and armos/nessus/inc_pub/rtc.h. 
 *              Please refer to it for the usage of RTC.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *
 ****************************************************************************/
#ifndef __REI_RTC_H__
#define __REI_RTC_H__

#include "rei_basetype.h"

/* REI interface for RTC and clock tick*/
#define RE_RimSetUTCTime                RimSetUTCTime
#define RE_RimGetUTCTime                RimGetUTCTime
#define RE_RimGetNativeTicks            RimGetNativeTicks
#define RE_RimGetNativeTicksPerSecond   RimGetNativeTicksPerSecond

#define RE_ReadRtcDword                 ReadRtcDword
#define RE_RtcDelay                     RtcDelay
#define RE_WaitForRtcTicks              WaitForRtcTicks
#define RE_imGetUptime                  RimGetUptime

#define   TIME_START(x)         {x = RimGetNativeTicks();}
#define   TIME_ELAPSE(x)        (((RimGetNativeTicks() - x)*1000)/RimGetNativeTicksPerSecond())

/* ==========================================================================*/
/* DO NOT use these declarations directly in platform software. Use the REI 
 * defines instead */
 
/* Original function declarations */
extern BOOL     RimSetUTCTime( QWORD time );
extern QWORD    RimGetUTCTime( void );

extern DWORD    RimGetNativeTicks( void );
extern DWORD    RimGetNativeTicksPerSecond( void );

extern DWORD    ReadRtcDword( void );

extern void     RtcDelay( DWORD timeInMilliseconds );

extern void     WaitForRtcTicks( WORD ticks );
extern QWORD    RimGetUptime( void );


#endif //__REI_TIMER_H__
