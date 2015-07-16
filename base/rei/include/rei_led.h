/*****************************************************************************
 * Filename:    rei_led.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Jan 07, 2009
 *
 * Description: This file defines the REI(runtime environment interface) for 
 *              LED.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *
 ****************************************************************************/
 
#ifndef __REI_LED_H__
#define __REI_LED_H__

/* REI interface for LED */
#define     RE_LED_On     LED_On
#define     RE_LED_Off    LED_Off

/* DO NOT use these functions directly in platform software */
/* Original LED function declarations */
extern void LED_On(void);
extern void LED_Off(void);
#endif
