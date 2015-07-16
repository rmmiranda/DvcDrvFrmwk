/*****************************************************************************
 * Filename:    clocks.h
 *
 * Copyright 2004, Research In Motion Ltd
 *
 * Author:      Chris Book
 *
 * Created:     April 7, 2005
 *
 * Description: Generic peripheral clock API
 ****************************************************************************/
#ifndef __CLOCKS_H__
#define __CLOCKS_H__

#include <basetype.h>

#if defined( PROCESSOR_HERMONB0 ) && ( defined( CORE_XSCALE ) || defined( CORE_PJ4 ) )
#include "hnb0_clocks.h"
#elif defined( PROCESSOR_HERMONB0 ) && defined( CORE_BLACKFIN )
#include "hnb0_msa_clocks.h"
#elif defined( PROCESSOR_FAMILY_TAVOR ) && defined( CORE_BLACKFIN )
#include "tavor_msa_clocks.h"
#include "tavor_comms_clocks.h"
#elif defined( PROCESSOR_FAMILY_TAVOR ) && ( defined( CORE_XSCALE ) || defined( CORE_PJ4 ) ) || defined( PROCESSOR_FAMILY_MMP )
#include "tavor_clocks.h"
#elif defined( PROCESSOR_FAMILY_TAVOR ) && defined( CORE_ARM9 ) && defined( CORE_SEAGULL )
#include "tavorpv2_seagull_clocks.h"
#elif defined( PROCESSOR_FAMILY_TAVOR ) && defined( CORE_ARM9 )
#include "tavor_harbell_clocks.h"
#include "tavor_comms_clocks.h"
#elif defined( PROCESSOR_ARGONLV ) && defined( CORE_ARM11 )
#include "argonlv_clocks.h"
#elif defined( PROCESSOR_ARGONLV ) && defined( CORE_STARCORE )
#include "argonlv_starcore_clocks.h"
#elif defined( PROCESSOR_WM1830 )
#include "wm1830_clocks.h"
#else
// hack to fix builds that include this file but don't implement the api
typedef WORD PeripheralClock;
#endif

// Returns running status of desired clock
BOOL ClockRunning( PeripheralClock peripheral );

// Returns uptime of current clock; or 0 if off
QWORD ClockUptime ( PeripheralClock peripheral );

// Enables all clocks needed by the specified peripheral
void ClockEnable( PeripheralClock peripheral );

// Disables all clocks needed by the specified peripheral
void ClockDisable( PeripheralClock peripheral );

// Used to prevent sleep based on certain clocks being on
BOOL ClocksAllowSleep( void );

BOOL IsEverySlaveProcessorIdle( void );

// Print out clocks that are running.
void DumpRunningClocks( void );

#endif
/**
 * @file
 *
 * Generic peripheral clock API
 *
 */
