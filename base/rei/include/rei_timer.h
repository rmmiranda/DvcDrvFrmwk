/*****************************************************************************
 * Filename:    rei_timer.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Jan 07, 2009
 *
 * Description: This file defines the REI(runtime environment interface) for 
 *              OS timers. The original data structures 
 *              and functions are defined in file armos/nessus/inc_pub/ntimers.h. 
 *              Please refer to it for the usage of OS timers.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *
 ****************************************************************************/
#ifndef __REI_TIMER_H__
#define __REI_TIMER_H__

#include "rei_basetype.h"

/* REI interface for OS timer*/
#define    RE_OS_TIMER_IMMEDIATE        = 0 ,    /**< enum expire immediately during powerdown */
#define    RE_OS_TIMER_HOLD_FOR_POWERUP = 1      /**< enum expiry held during powerdown */
 
#define RE_RTC_TICKS_PER_SECOND    (32768u)
#define RE_RTC_TICKS_PER_MINUTE    (RE_RTC_TICKS_PER_SECOND * 60)
#define RE_RTC_TICKS_PER_HOUR      (RE_RTC_TICKS_PER_MINUTE * 60)
#define RE_RTC_TICKS_PER_DAY       (RE_RTC_TICKS_PER_HOUR * 24)

#define RE_MSEC_TO_RTC_TICKS( X ) ((DWORD)(((QWORD)(X)*RTC_TICKS_PER_SECOND)/1000))
#define RE_SECONDS_TO_RTC_TICKS( X ) ((DWORD)(((QWORD)(X)*RTC_TICKS_PER_SECOND)))

/* from timer.h */
#define TIMER_PERIODIC  (1)  ///< Timer is periodic.
#define TIMER_ONE_SHOT  (2)  ///< Timer is a one-shot timer.

/* REI defines for OS timer functions*/
#define     RE_TimerCreate          RimTimerCreate
#define     RE_TimerDelete          RimTimerDelete
#define     RE_TimerStart           RimTimerStart
#define     RE_TimerStop            RimTimerStop
#define     RE_TimerGetHandle       RimTimerGetHandle
#define     RE_TicksPerSecond       RimGetNativeTicksPerSecond
/* Timer data structures. */
/* See the extra timers in nessus/inc_pub/ntimers_enum.h */
/* ==========================================================================*/
/* DO NOT use these declarations directly in platform software. Use the REI 
 * defines instead */
/* Original function declarations */
extern DWORD   RimTimerCreate( void );
extern BOOL    RimTimerStart( DWORD handle, DWORD time, DWORD type, DWORD cookie );
extern BOOL    RimTimerStop( DWORD handle );
extern BOOL    RimTimerDelete( DWORD handle );
extern DWORD   RimTimerGetHandle( DWORD cookie );
extern DWORD   RimGetNativeTicksPerSecond( void );
#endif //__REI_TIMER_H__
