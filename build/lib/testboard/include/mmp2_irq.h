/******************************************************************************
 * Filename:    mmp2_irq.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Jackson Mo
 *
 * Created:     Oct 2009
 *
 * Description: Define all HW and SW interrupts used in Nessus.
 *****************************************************************************/
#ifndef __MMP2_IRQ_H__
#define __MMP2_IRQ_H__

#if !defined( PROCESSOR_MMP2 )
 #error "This file is for MMP2 processor only"
#endif

#if !defined( __MMP_IRQ_H__ )
 #error "Should not include this header directly. Include mmp_irq.h instead"
#endif


/*-----------------------------------------------------------------------------
 * Macros
 *---------------------------------------------------------------------------*/
#define GET_SOURCE( x, y )      (y)
#define IRQ_SOURCE( x )         GET_SOURCE( x )


/*-----------------------------------------------------------------------------
 * Constants
 *---------------------------------------------------------------------------*/
// Primary IRQs
#define HWIRQ_SSP1              PRIMARY_IRQ, 0
#define HWIRQ_SSP2              PRIMARY_IRQ, 1
#define HWIRQ_SSPA1             PRIMARY_IRQ, 2
#define HWIRQ_SSPA2             PRIMARY_IRQ, 3
#define HWIRQ_INT4              PRIMARY_IRQ, 4  // Reserved by interrupt controller
#define HWIRQ_INT5              PRIMARY_IRQ, 5  // Reserved by interrupt controller
//#define HWIRQ_INT6              PRIMARY_IRQ, 6
#define HWIRQ_TWSI1             PRIMARY_IRQ, 7
#define HWIRQ_2D3D_GRAPH_CTRL   PRIMARY_IRQ, 8
#define HWIRQ_KEYPAD            PRIMARY_IRQ, 9
#define HWIRQ_ROTARY_CTRL       PRIMARY_IRQ, 10
#define HWIRQ_TRACKBALL         PRIMARY_IRQ, 11
#define HWIRQ_ONEWIRE_MASTER    PRIMARY_IRQ, 12
#define HWIRQ_TIMER1_1          PRIMARY_IRQ, 13
#define HWIRQ_TIMER1_2          PRIMARY_IRQ, 14
#define HWIRQ_TIMER1_3          PRIMARY_IRQ, 15
#define HWIRQ_IPC1              PRIMARY_IRQ, 16
#define HWIRQ_INT17             PRIMARY_IRQ, 17 // Reserved by interrupt controller
//#define HWIRQ_INT18             PRIMARY_IRQ, 18
#define HWIRQ_HDMI              PRIMARY_IRQ, 19
#define HWIRQ_SSP3              PRIMARY_IRQ, 20
#define HWIRQ_SSP4              PRIMARY_IRQ, 21
#define HWIRQ_USB_HSIC1         PRIMARY_IRQ, 22
#define HWIRQ_USB_HSIC2         PRIMARY_IRQ, 23
#define HWIRQ_UART3             PRIMARY_IRQ, 24
//#define HWIRQ_INT25             PRIMARY_IRQ, 25
#define HWIRQ_VMETA             PRIMARY_IRQ, 26
#define HWIRQ_UART1             PRIMARY_IRQ, 27
#define HWIRQ_UART2             PRIMARY_IRQ, 28
#define HWIRQ_MIPI_DSI          PRIMARY_IRQ, 29
#define HWIRQ_CCIC_2            PRIMARY_IRQ, 30
#define HWIRQ_TIMER2_1          PRIMARY_IRQ, 31
#define HWIRQ_TIMER2_2          PRIMARY_IRQ, 32
#define HWIRQ_TIMER2_3          PRIMARY_IRQ, 33
#define HWIRQ_FSIC              PRIMARY_IRQ, 34
#define HWIRQ_INT35             PRIMARY_IRQ, 35 // Reserved by interrupt controller
#define HWIRQ_WDT1              PRIMARY_IRQ, 36
#define HWIRQ_NFS_SMC_DMA       PRIMARY_IRQ, 37
#define HWIRQ_USIM              PRIMARY_IRQ, 38
#define HWIRQ_MMC1              PRIMARY_IRQ, 39
#define HWIRQ_SP_TO_PJ4         PRIMARY_IRQ, 40
#define HWIRQ_DISPLAY_CTRL      PRIMARY_IRQ, 41
#define HWIRQ_CCIC1             PRIMARY_IRQ, 42
#define HWIRQ_IRE               PRIMARY_IRQ, 43
#define HWIRQ_USB               PRIMARY_IRQ, 44
#define HWIRQ_NFC               PRIMARY_IRQ, 45
#define HWIRQ_UART4             PRIMARY_IRQ, 46
#define HWIRQ_DMA_FIQ           PRIMARY_IRQ, 47
#define HWIRQ_DMA_IRQ           PRIMARY_IRQ, 48 // Reserved by interrupt controller
#define HWIRQ_GPIO              PRIMARY_IRQ, 49
#define HWIRQ_PJ4_TO_SP         PRIMARY_IRQ, 50
#define HWIRQ_INT51             PRIMARY_IRQ, 51 // Reserved by interrupt controller
#define HWIRQ_MMC2              PRIMARY_IRQ, 52
#define HWIRQ_MMC3              PRIMARY_IRQ, 53
#define HWIRQ_MMC4              PRIMARY_IRQ, 54
#define HWIRQ_MIPI_HSI          PRIMARY_IRQ, 55
#define HWIRQ_IPC2              PRIMARY_IRQ, 56
//#define HWIRQ_INT57             PRIMARY_IRQ, 57
#define HWIRQ_MSP_CARD          PRIMARY_IRQ, 58
#define HWIRQ_MIPI_SLIM_DMA     PRIMARY_IRQ, 59
#define HWIRQ_PJ4_FREQ_CHANGE   PRIMARY_IRQ, 60
//#define HWIRQ_INT61             PRIMARY_IRQ, 61
#define HWIRQ_MIPI_SLIM         PRIMARY_IRQ, 62
#define HWIRQ_SMC               PRIMARY_IRQ, 63

// DMA IRQs
//   PDMA IRQs
#define HWIRQ_PDMA_CHANNEL0     DMA_IRQ, 0
#define HWIRQ_PDMA_CHANNEL1     DMA_IRQ, 1
#define HWIRQ_PDMA_CHANNEL2     DMA_IRQ, 2
#define HWIRQ_PDMA_CHANNEL3     DMA_IRQ, 3
#define HWIRQ_PDMA_CHANNEL4     DMA_IRQ, 4
#define HWIRQ_PDMA_CHANNEL5     DMA_IRQ, 5
#define HWIRQ_PDMA_CHANNEL6     DMA_IRQ, 6
#define HWIRQ_PDMA_CHANNEL7     DMA_IRQ, 7
#define HWIRQ_PDMA_CHANNEL8     DMA_IRQ, 8
#define HWIRQ_PDMA_CHANNEL9     DMA_IRQ, 9
#define HWIRQ_PDMA_CHANNEL10    DMA_IRQ, 10
#define HWIRQ_PDMA_CHANNEL11    DMA_IRQ, 11
#define HWIRQ_PDMA_CHANNEL12    DMA_IRQ, 12
#define HWIRQ_PDMA_CHANNEL13    DMA_IRQ, 13
#define HWIRQ_PDMA_CHANNEL14    DMA_IRQ, 14
#define HWIRQ_PDMA_CHANNEL15    DMA_IRQ, 15

//   MDMA IRQs
#define HWIRQ_MDMA_CHANNEL0     DMA_IRQ, 16
#define HWIRQ_MDMA_CHANNEL1     DMA_IRQ, 17

//   ADMA IRQs
#define HWIRQ_ADMA_CHANNEL0     DMA_IRQ, 18
#define HWIRQ_ADMA_CHANNEL1     DMA_IRQ, 19
#define HWIRQ_ADMA_CHANNEL2     DMA_IRQ, 20
#define HWIRQ_ADMA_CHANNEL3     DMA_IRQ, 21

//   VDMA IRQs
#define HWIRQ_VDMA_CHANNEL0     DMA_IRQ, 22
#define HWIRQ_VDMA_CHANNEL1     DMA_IRQ, 23

// INT4 IRQs
#define HWIRQ_USB_CHARGER       SECONDARY_INT4, 0
#define HWIRQ_PMIC              SECONDARY_INT4, 1

// INT5 IRQs
#define HWIRQ_RTC_ALARM         SECONDARY_INT5, 0
#define HWIRQ_RTC_INT           SECONDARY_INT5, 1

// INT17 IRQs
#define HWIRQ_TWSI2             SECONDARY_INT17, 0
#define HWIRQ_TWSI3             SECONDARY_INT17, 1
#define HWIRQ_TWSI4             SECONDARY_INT17, 2
#define HWIRQ_TWSI5             SECONDARY_INT17, 3
#define HWIRQ_TWSI6             SECONDARY_INT17, 4

// INT35 IRQs
#define HWIRQ_PERFMON           SECONDARY_INT35, 0
#define HWIRQ_L2_PA_ECC_IRQ     SECONDARY_INT35, 1
#define HWIRQ_L2_ECC_IRQ        SECONDARY_INT35, 2
#define HWIRQ_L2_UECC_IRQ       SECONDARY_INT35, 3
#define HWIRQ_DMEM              SECONDARY_INT35, 4
#define HWIRQ_FABRIC0_TIMEOUT   SECONDARY_INT35, 5
#define HWIRQ_FABRIC1_TIMEOUT   SECONDARY_INT35, 6
#define HWIRQ_FABRIC2_TIMEOUT   SECONDARY_INT35, 7
//#define HWIRQ_RESERVED          SECONDARY_INT35, 8
#define HWIRQ_THERMAL_INT       SECONDARY_INT35, 9
#define HWIRQ_MAIN_PMU_INT      SECONDARY_INT35, 10
#define HWIRQ_WDT2_INT          SECONDARY_INT35, 11
#define HWIRQ_CORESIGHT_INT     SECONDARY_INT35, 12
#define HWIRQ_COMMTX_INT        SECONDARY_INT35, 13
#define HWIRQ_COMMRX_INT        SECONDARY_INT35, 14

// INT51 IRQs
#define HWIRQ_SSP1_SRDY_INT     SECONDARY_INT51, 0
#define HWIRQ_SSP3_SRDY_INT     SECONDARY_INT51, 1


/*---------------------------------------------------------------------------*/
#endif
