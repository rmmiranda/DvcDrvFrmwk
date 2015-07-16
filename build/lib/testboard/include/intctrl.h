/*****************************************************************************
 * Filename:    intctrl.h
 *
 * Copyright 2003, Research In Motion Ltd
 *
 * Author:      Conrad Kreek
 *
 * Created:     December 2003
 *
 * Description: Pulls in processor-specific interrupt headers.
 ****************************************************************************/
#ifndef __INTCTRL_H__
#define __INTCTRL_H__

#include <basetype.h>
//#include "eventlog.h"

#ifdef PROCESSOR_MSM6500
 #include "msm6500_irq.h"
 #include "msm6500_intctrl.h"
#endif

#ifdef PROCESSOR_MSM6250
 #include "msm6250_irq.h"
 #include "msm6250_intctrl.h"
#endif

#ifdef PROCESSOR_MSM6550
 #include "msm6550_irq.h"
 #include "msm6550_intctrl.h"
#endif

#if defined( PROCESSOR_MSM7600 )
 #include "msm7600_irq.h"
 #include "msm7600_intctrl.h"
#endif

#if defined( PROCESSOR_QSD8650 )
 #include "qsd8650_irq.h"
 #include "qsd8650_intctrl.h"
#endif

#ifdef PROCESSOR_HERMONB0
 #ifdef CORE_BLACKFIN
  #include "hnb0_msa_irq.h"
  #include "hnb0_msa_intctrl.h"
 #else
  #include "hnb0_irq.h"
  #include "hnb0_intctrl.h"
 #endif
#endif

#if defined( PROCESSOR_FAMILY_TAVOR )
 #if defined( CORE_BLACKFIN )
  #if ( defined( PROCESSOR_TAVOR_PV2 ) || defined( PROCESSOR_NEVO ) )
   #include "tavorpv2_msa_irq.h"
  #else
   #include "tavor_msa_irq.h"
  #endif
  #include "hnb0_msa_intctrl.h"
 #elif defined( CORE_ARM9 )
  #if defined( CORE_SEAGULL )
   #include "tavorpv2_seagull_irq.h"
  #else
   #include "tavor_harbell_irq.h"
  #endif
  #include "tavor_harbell_intctrl.h"
 #else
  #include "tavor_irq.h"
  #include "tavor_intctrl.h"
 #endif
#endif

#if defined( PROCESSOR_FAMILY_MMP )
 #include "mmp_irq.h"
#endif

#ifdef PROCESSOR_ARGONLV
 #ifdef CORE_STARCORE
  #include "argonlv_starcore_irq.h"
 #else
  #include "argonlv_irq.h"
 #endif
 #include "argonlv_intctrl.h"
#endif

#ifdef PROCESSOR_WM1830
  #include "wm1830_irq.h"
#endif

typedef enum {
    //Common to all cores
    PRIMARY_IRQ,

    //Hermon/ Tavor XSCALE interrupt controllers
    SECONDARY_IRQ,
    GPIO_IRQ,       //also PROCESSOR_MSM6500, PROCESSOR_MSM6250, PROCESSOR_MSM6550
    DMA_IRQ,
    GSM_IRQ,

    //Hermon / Tavor MSA interrupt controllers
    SECONDARY_L_IRQ,
    SECONDARY_H_IRQ,
    WCDMA_H_IRQ,
    WCDMA_L_IRQ,
    //Tavor MSA interrupt controller
    WCDMA_HL_IRQ,

    //Tavor Virtual controllers for cross-core interrupts
    REMOTE_XSCALE_IRQ,
    REMOTE_ARM9_IRQ,
    REMOTE_MSA_IRQ,

    //Argon LV controllers
    MU_GP_IRQ,
    SDMA_IRQ,
    BDMA_IRQ,
    IPU_IRQ,

    // MMP2
    SECONDARY_INT4,     // PMIC or USB
    SECONDARY_INT5,     // RTC or RTC Alarm
    SECONDARY_INT17,    // TWSI 2, 3, 4, 5, 6
    SECONDARY_INT35,    // COMMRX, COMMTX, CoreSight, Wdt Timer2, Main PMU, Thermal, Fabric Timeout, DDR_L2_IRQ, PJ4 PerfMon
    SECONDARY_INT51,    // SSP1_SRDY, SSP3_SRDY

    MAX_CONTROLLERS

}IntController;

#define IrqType IntController

// A generic interrupt identifier.  This structure
// would vary depending on how each core defines identifiers.

typedef DWORD IntId;
typedef DWORD IntPriority;


/** Bunch of useful macros. */
#define GET_IRQ_TYPE( irqType, irqNum )     irqType
#define HWIRQ_TYPE( hwirq )                 ( GET_IRQ_TYPE( hwirq ) )
#define GET_IRQ_INDEX( irqType, irqNum )    irqNum
#define HWIRQ_INDEX( hwirq )                ( GET_IRQ_INDEX( hwirq ) )
#define HWIRQ_CREATEINDEX( group, prio )    ( (group) << 16 | (prio) )
#define HWIRQ_GROUP( irqNum )               ( ( (irqNum) >> 16 ) & 0xFFFF )
#define HWIRQ_PRIORITY( irqNum )            ( (irqNum) & 0xFFFF )

/** Interrupt request handler. */
typedef void IRQHandler( void );

/**
 * Registers handler on a specified HW/SW interrupt controller.
 *
 * @param controller Controller on which interrupt should be registered
 * @param id Index/Identifier of the interrupt
 * @param function Pointer to interrupt handler function.
 */
void        RegisterIntHandler( IntController controller, IntId id, IRQHandler *function );

/**
 * Enables an interrupt id on the specified interrupt controller
 *
 * @param controller - interrupt controller
 * @param id - interrupt identifier
 */
void        EnableInterrupt( IntController controller, IntId id );

/**
 * Enables an interrupt id on the specified interrupt controller
 *
 * @param controller - interrupt controller
 * @param id - interrupt identifier
 */
void        DisableInterrupt( IntController controller, IntId id );

/**
 * Clears an interrupt id on the specified interrupt controller
 *
 * @param controller - interrupt controller
 * @param id - interrupt identifier
 */
void        ClearInterrupt( IntController controller, IntId id );

/**
 * Causes an interrupt to be asserted on the specified controller
 * To perform a cross assert on a different core, use the
 * AssertCrossCoreInt() API
 *
 * @param controller - interrupt controller
 * @param id - interrupt identifier
 */
void        AssertInterrupt( IntController controller, IntId id );

/**
 * Asserts an interrupt id on the specified interrupt controller
 *
 * @param controller - interrupt controller
 * @param id - interrupt identifier
 */
void        AssertCrossCoreInt( IntController controller, IntId id );

/**
 * Determines if interrupt 'id' is pending on the specified
 * interrupt controller
 *
 * @param controller - interrupt controller
 * @param id - interrupt identifier
 */
BOOL        IsInterruptPending( IntController controller, IntId id );

/**
 * Determines if interrupt 'id' is masked on the specified interrupt
 * controller
 *
 * @param controller - interrupt controller
 * @param id - interrupt identifier
 */
BOOL        IsInterruptMasked( IntController controller, IntId id );

/**
 * Sets the priority for the specified interrupt this is not yet supported
 * on every platform but is needed for the arm9 core on tavor
 *
 * @param controller - interrupt controller
 * @param id - interrupt identifier
 * @param priority - interrupt priority
 */
void        SetInterruptPriority( IntController controller, IntId id, IntPriority priority );

#endif

/**
 * @file
 *
 * Pulls in processor-specific interrupt headers.
 *
 */
