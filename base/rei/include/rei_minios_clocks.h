/*****************************************************************************
 * Filename:    rei_minios_clocks.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      
 *
 * Created:     February 2010
 *
 * Description: This file defines the REI(runtime environment interface) for 
 *              clock control.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *
 ****************************************************************************/
 
#ifndef __REI_MINIOS_CLOCKS_H__
#define __REI_MINIOS_CLOCKS_H__

#include "rei_basetype.h"
#include "clocks.h"

/* ==========================================================================*/

/* REI interface for Clock */
void RE_ClockEnable( RE_PeripheralClock peripheral );
void RE_ClockDisable( RE_PeripheralClock peripheral );

// RE_PeripheralClock  - This enum is defined in base/arch/<proc>/clocks.h


/* ==========================================================================*/


void RE_DumpClockList ( void );

#endif // __REI_CLOCKS_H__
