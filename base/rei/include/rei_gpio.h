/*****************************************************************************
 * Filename:    rei_gpio.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Jan 07, 2009
 *
 * Description: This file defines the REI(runtime environment interface) for 
 *              PAD control.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *
 ****************************************************************************/
 
#ifndef __REI_GPIO_H__
#define __REI_GPIO_H__

#include "rei_basetype.h"

/* REI interface for PAD*/
/* PAD data structure REI defines */
#define     RE_PadConfigure     PadConfigure
#define     RE_PadSet           PadSet
#define     RE_PadClear         PadClear
#define     RE_PadRead          PadRead
#define     RE_PadSetMode       PadSetMode

/* PAD function REI defines */
#define RE_PadConfiguration     PadConfiguration
#define RE_Line                 Line
#define RE_PadAttribute         PadAttribute
#define RE_PadInterruptType     PadInterruptType

/* ==========================================================================*/
/* DO NOT use these declarations directly in platform software. Use the REI 
 * defines instead */
#define __BASETYPE_H__
#include "gpio.h"
#endif
