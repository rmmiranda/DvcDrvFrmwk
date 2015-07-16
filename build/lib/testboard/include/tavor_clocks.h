/*****************************************************************************
 * Filename:    tavor_clocks.h
 *
 * Copyright 2006, Research In Motion Ltd
 *
 * Author:      Chris Book
 *
 * Created:     March, 2006
 *
 * Description: Tavor peripheral clock API
 ****************************************************************************/
#ifndef __TAVOR_CLOCKS_H__
#define __TAVOR_CLOCKS_H__

typedef enum {
    CLOCK_USB         =  0,
    CLOCK_I2C         =  1,
    CLOCK_LCD         =  2,
    CLOCK_USIM0       =  3,
    CLOCK_USIM1       =  4,
    CLOCK_CAMERA      =  5,
    CLOCK_MMC0        =  6,
    CLOCK_MMC1        =  7,
    CLOCK_KEYPAD      =  8,
    CLOCK_UART1       =  9,
    CLOCK_UART2       = 10,
    CLOCK_UART3       = 11,
    CLOCK_SSP1        = 12,
    CLOCK_SSP2        = 13,
    CLOCK_SSP3        = 14,
    CLOCK_SSP4        = 15,
    CLOCK_MSL         = 16,
    CLOCK_ROLLER      = 17,
    CLOCK_FAKE        = 18,
    CLOCK_GPS         = 19,
    CLOCK_GRAPHICS    = 20,
    CLOCK_ANTIOCH     = 21,
    CLOCK_CADDO       = 22,
    CLOCK_NAND        = 23,
    CLOCK_ABC         = 24,
    CLOCK_VBROM       = 25,
    CLOCK_TVOUT       = 26,
#if defined( PROCESSOR_TAVOR_PV ) || ( defined( PROCESSOR_TAVOR_PV2 ) || defined( PROCESSOR_NEVO ) || defined( PROCESSOR_FAMILY_MMP ) )
    CLOCK_MVED        = 27,
    CLOCK_MMC2        = 28,
#endif
#if ( defined( PROCESSOR_TAVOR_PV2 ) || defined( PROCESSOR_NEVO ) || defined( PROCESSOR_FAMILY_MMP ) ) && defined( AUDIO_BUFFER_UNIT )
    CLOCK_ABU         = 29,
#endif
    // ...
    NUM_PERIPHERALS
} PeripheralClock;


#define CLOCKS_PREVENT_SLEEP_MASK ( ~( 1 << CLOCK_MSL | 1 << CLOCK_ABC) )

#endif

/**
 * @file
 *
 * Tavor peripheral clock API
 *
 */

