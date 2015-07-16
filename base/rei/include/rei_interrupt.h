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
 *              interrupt. inc_pub/intctrl.h
 *              Please refer to it for the usage of interrupt.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *
 ****************************************************************************/
#ifndef __REI_INTERRUPT_H__
#define __REI_INTERRUPT_H__

#include "rei_basetype.h"

/* REI interface for interrupt*/
#define  RE_CoreIntEnable           CoreIntEnable
#define  RE_CoreIntDisable          CoreIntDisable

#define  RE_EnableInterrupt         EnableInterrupt
#define  RE_DisableInterrupt        DisableInterrupt
#define  RE_RegisterIntHandler      RegisterIntHandler
         
#define RE_IntController        IntController
#define RE_IRQHandler           IRQHandler
#define RE_IntId                IntId

/* ==========================================================================*/
/* DO NOT use these declarations directly in platform software. Use the REI 
 * defines instead */
#if !defined(RIM_NESSUS)
#define RIM_NESSUS 
#endif
#define __BASETYPE_H__
#include "intctrl.h"

#endif //__REI_TIMER_H__
