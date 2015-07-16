/*****************************************************************************
 * Filename:    rei_delay.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Jan 07, 2009
 *
 * Description: This file defines the REI(runtime environment interface) for 
 *              delay.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *
 ****************************************************************************/
 
#ifndef __REI_DELAY_H__
#define __REI_DELAY_H__

#include "rei_basetype.h"

/* REI interface for Delay */
#define RE_NANOSECOND                  1UL             /*ns */
#define RE_MICROSECOND                 1000UL          /*us */
#define RE_MILLISECOND                 1000000UL       /*ms*/ 
#define RE_SEC                         1000000000UL    /*s*/
 
#define     RE_HwDelay     HwDelay
#define     RE_RimSleep    RimSleep
/* DO NOT use these functions directly in platform software */
/* Original delay function declaration */
extern void HwDelay( SDWORD delay );   
extern void RimSleep( WORD time );
#endif
