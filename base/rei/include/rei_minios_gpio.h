/*****************************************************************************
 * Filename:    rei_gpio_minios.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Daniel Hadi
 *
 * Created:     Feb 24, 2010
 *
 * Description: This file defines the REI(runtime environment interface) for 
 *              PAD control.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *              See nessus gpio.h and <platform>_gpio_def.h for details
 *
 ****************************************************************************/
 
#ifndef __REI_GPIO_MINIOS_H__
#define __REI_GPIO_MINIOS_H__

#include "rei_basetype.h"

//This file will include the proper header files based on processor and platform. 
#include "gpio_processor.h"


/* ==========================================================================*/
/* REI interface for PAD*/
/* PAD data structure REI defines */

typedef enum {
    PAD_INTERRUPT_NONE,
    PAD_INTERRUPT_RISING_EDGE,
    PAD_INTERRUPT_FALLING_EDGE,
    PAD_INTERRUPT_BOTH_EDGES,
    PAD_INTERRUPT_HIGH_LEVEL,
    PAD_INTERRUPT_LOW_LEVEL
} RE_PadInterruptType;

typedef enum {
    PAD_DIRECTION_INPUT,
    PAD_DIRECTION_OUTPUT
} RE_PadDirection;

typedef enum {
    PAD_OUTPUT,
    PAD_OUTPUT_SLEEP_OFF,
    PAD_OUTPUT_ENABLE,
    PAD_INPUT_ENABLE,
    PAD_PULL_UP,
    PAD_PULL_DOWN,
    PAD_INPUT,
    PAD_RISING_EDGE_LATCH,
    PAD_FALLING_EDGE_LATCH
} RE_PadAttribute;

typedef struct RE_PadConfiguration_struct {
    DWORD                       altFunctionMode;
    RE_PadDirection             initialDirection;
    BOOL                        initialLevel;
    BOOL                        pullUpEnable;
    BOOL                        pullDownEnable;
    RE_PadInterruptType         interruptType;
    void                        (*interruptHandler)(void);  // NULL = no change
    BOOL                        interruptsInitiallyEnabled;
    BOOL                        interruptDebounceEnabled;
} RE_PadConfiguration;

typedef DWORD RE_Line;


/* ==========================================================================*/

/* The following enums are required for the PAD data structures and functions */

typedef enum {
    PAD_LATCH_FALLING_EDGE = 0,
    PAD_LATCH_RISING_EDGE = 1,
    PAD_LATCH_NONE = 2
} RE_PadLatchState;

typedef enum {
    DEFAULT_STRENGTH_SLEW,
    PAD_DRIVE_SLOW,
    PAD_DRIVE_MEDIUM,
    PAD_DRIVE_FAST
} RE_PadDriveStrengthSlewRate;

typedef enum {
    SLEEP_SEL_DEFAULT,
    SLEEP_SEL_APPS,
    SLEEP_SEL_COMMS
} RE_PadSleepSel;

typedef enum {
    SLEEP_DATA_DEFAULT,
    SLEEP_DATA_HIGH,
    SLEEP_DATA_LOW
} RE_PadSleepData;

typedef enum {
    SLEEP_DIRECTION_DEFAULT,
    SLEEP_DIRECTION_INPUT,
    SLEEP_DIRECTION_OUTPUT
} RE_PadSleepDirection;

typedef enum {
    EDGE_DETECT_DEFAULT,
    EDGE_DETECT_ENABLE,
    EDGE_DETECT_DISABLE
} RE_PadEdgeDetect;

typedef enum {
    PULL_SEL_DEFAULT,
    PULL_SEL_MFPR,      // Pull resistors on pad controlled by MFPR
    PULL_SEL_ALT_MODE   // Pull resistors on pad controlled by AF module
} RE_PadPullSel;

typedef enum {
    USE_RDH_DEFAULT,
    USE_RDH_ENABLE,
    USE_RDH_DISABLE
} RE_PadUseRdh;

typedef struct {
    RE_PadDriveStrengthSlewRate     drive;          // Select drive strength and slew rate
    RE_PadSleepSel                  sleepSel;       // Whether BPMU or CPMU controls the sleep mode
    RE_PadSleepData                 sleepData;      // Data on the pin during sleep
    RE_PadSleepDirection            sleepDirection; // Pad direction during sleep
    RE_PadPullSel                   pullSel;        // Select between MFPR or alt mode module controlling the pulls on the pad
    RE_PadUseRdh                    useRdh;         // Determines if the RDH firewall bit is used
    RE_PadEdgeDetect                edgeDetectEnable; // Whether the edge detect detects a rising or falling edge
    BOOL                            detectRisingEdge;   // Whether to detect a rising edge.  Ignored unless edgeDetectEnable == EDGE_DETECT_ENABLE
    BOOL                            detectFallingEdge;   // Whether to detect a falling edge.  Ignored unless edgeDetectEnable == EDGE_DETECT_ENABLE
} RE_DeviceSpecificPadConfiguration;

/* ==========================================================================*/


/* REI interface for PAD*/
/* PAD function REI defines */

void RE_PadConfigure( RE_Line line, const RE_PadConfiguration *config,
                   const RE_DeviceSpecificPadConfiguration *devSpecificConfig );
void RE_PadSet( RE_Line line, RE_PadAttribute attribute );
void RE_PadClear( RE_Line line, RE_PadAttribute attribute );
BOOL RE_PadRead( RE_Line line, RE_PadAttribute attribute );
void RE_PadSetMode( RE_Line line, DWORD mode );
void RE_GpioInit (void);
void RE_PadInterruptEnable( RE_Line line );
void RE_PadInterruptDisable( RE_Line line );
void RE_PadInterruptClear( RE_Line line );


#endif // __REI_GPIO_MINIOS_H__

