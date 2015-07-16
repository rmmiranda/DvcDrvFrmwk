/*****************************************************************************
 * Filename:    uart_tavorpv.h
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 2008
 *
 * Description: The Tavor PV specific header file for the uart driver. Developed
 *              specifically for the platform software driver framework.
 *
 *****************************************************************************/

#ifndef __UART_TAVORPV_H__
#define __UART_TAVORPV_H__

#include "rei.h"

#define UART_RX_FIFO_DEPTH      64 // bytes
#define UART_TX_FIFO_WIDTH      8  // bits
#define UART_RX_FIFO_WIDTH      11 // bits
#define UART_TX_FIFO_DEPTH      64 // bytes

#define UART_HIGH_SPEED_CLOCK   59429000
#define UART_FULL_SPEED_CLOCK   14857000

#define UART_REG_ADDR0          0x40100000
#define UART_REG_ADDR1          0x40200000
#define UART_REG_ADDR2          0x40300000

#ifdef NESSUS
#define UART_CLOCK0             9
#define UART_CLOCK1             10
#define UART_CLOCK2             11
#else
#define UART_CLOCK0             22
#define UART_CLOCK1             21
#define UART_CLOCK2             23
#endif

#define UART_HWIRQ0             22
#define UART_HWIRQ1             21
#define UART_HWIRQ2             20
 
#define UART_PORT0              0
#define UART_PORT1              1
#define UART_PORT2              2
#define UART_MAX_PORT           3

/**
 * UART register map structure.
 */
typedef struct {
    union {
        IOBYTE rbr8;  // receive buffer register (8bit access)
        IOWORD rbr16; // receive buffer register (16bit access)
        IODWORD rbr32;// receive buffer register (32bit access)
        IOBYTE thr8;  // transmit holding register (8bit access)
        IOWORD thr16; // transmit holding register (16bit access)
        IODWORD thr32;// transmit holding register (32bit access)
        IODWORD dll;  // divisor latch register (low byte)
    } u1;
    union {	
        IODWORD ier;  // interrupt enable register
        IODWORD dlh;  // divisor latch register (high byte)
    } u2;
    union {
        IODWORD iir;  // interrupt id register
        IODWORD fcr;  // fifo control register
    } u3;
    IODWORD LCR;      // line control register
    IODWORD MCR;      // modem control register
    IODWORD LSR;      // line status register
    IODWORD MSR;      // modem status register
    IODWORD SPR;      // scratch pad register
    IODWORD ISR;      // serial infrared select register
    IODWORD FOR;      // fifo occupancy register
    IODWORD ABR;      // autobaud control register
    IODWORD ACR;      // autobaud count register
} UartRegister_t;

#define RBR8    u1.rbr8
#define RBR16   u1.rbr16
#define RBR32   u1.rbr32
#define THR8    u1.thr8
#define THR16   u1.thr16
#define THR32   u1.thr32
#define DLL     u1.dll
#define IER     u2.ier
#define DLH     u2.dlh
#define IIR     u3.iir
#define FCR     u3.fcr

/**
 * Macros for receive buffer register.
 */
#define UART_RBR_MASK           0xFFFFFFFF
#define UART_RBR_BYTE0          0x000000FF
#define UART_RBR_BYTE1          0x0000FF00
#define UART_RBR_BYTE2          0x00FF0000
#define UART_RBR_BYTE3          0xFF000000

/**
 * Macros for transmit holding register.
 */
#define UART_THR_MASK           0xFFFFFFFF
#define UART_THR_BYTE0          0x000000FF
#define UART_THR_BYTE1          0x0000FF00
#define UART_THR_BYTE2          0x00FF0000
#define UART_THR_BYTE3          0xFF000000

/**
 * Macros for divisor latch low register.
 */
#define UART_DLL_MASK           0x000000FF
#define UART_DLL_DLL            0x000000FF

/**
 * Macros for divisor latch high register.
 */
#define UART_DLH_MASK           0x000000FF
#define UART_DLH_DLH            0x000000FF

/**
 * Macros for interrupt enable register.
 */
#define UART_IER_MASK           0x000001FF
#define UART_IER_HSE            0x00000100
#define UART_IER_DMAE           0x00000080
#define UART_IER_UUE            0x00000040
#define UART_IER_NRZE           0x00000020
#define UART_IER_RTOIE          0x00000010
#define UART_IER_MIE            0x00000008
#define UART_IER_RLSE           0x00000004
#define UART_IER_TIE            0x00000002
#define UART_IER_RAVIE          0x00000001

/**
 * Macros for interrupt id register.
 */
#define UART_IIR_MASK           0x000000FF
#define UART_IIR_FIFOES_MASK    0x000000C0
#define UART_IIR_FIFOES_NONFIFO 0x00000000
#define UART_IIR_FIFOES_FIFO    0x000000C0
#define UART_IIR_EOC            0x00000020
#define UART_IIR_ABL            0x00000010
#define UART_IIR_TOD            0x00000008
#define UART_IIR_IID_MASK       0x00000006
#define UART_IIR_IID_MODEM_STAT 0x00000000
#define UART_IIR_IID_TX_FIFO    0x00000002
#define UART_IIR_IID_RX_DATA    0x00000004
#define UART_IIR_IID_RX_ERROR   0x00000006
#define UART_IIR_nIP            0x00000001

/**
 * Macros for fifo control register.
 */
#define UART_FCR_MASK           0x000000FF
#define UART_FCR_ITL_MASK       0x000000C0
#define UART_FCR_ITL_1          0x00000000
#define UART_FCR_ITL_8          0x00000040
#define UART_FCR_ITL_16         0x00000080
#define UART_FCR_ITL_32         0x000000C0
#define UART_FCR_BUS_8          0x00000000
#define UART_FCR_BUS_32         0x00000020
#define UART_FCR_TRAIL_PXA      0x00000000
#define UART_FCR_TRAIL_DMAC     0x00000010
#define UART_FCR_TIL_HALF       0x00000000
#define UART_FCR_TIL_EMPTY      0x00000008
#define UART_FCR_RESETTF        0x00000004
#define UART_FCR_RESETRF        0x00000002
#define UART_FCR_TRFIFOE        0x00000001

/**
 * Macros for line control register.
 */
#define UART_LCR_MASK           0x000000FF
#define UART_LCR_DLAB           0x00000080
#define UART_LCR_SB             0x00000040
#define UART_LCR_STKYP          0x00000020
#define UART_LCR_EPS            0x00000010
#define UART_LCR_PEN            0x00000008
#define UART_LCR_STB            0x00000004
#define UART_LCR_WLS_MASK       0x00000003
#define UART_LCR_WLS_7          0x00000002
#define UART_LCR_WLS_8          0x00000003

/**
 * Macros for modem control register.
 */
#define UART_MCR_MASK           0x0000003F
#define UART_MCR_AFE            0x00000020
#define UART_MCR_LOOP           0x00000010
#define UART_MCR_OUT2           0x00000008
#define UART_MCR_OUT1           0x00000004
#define UART_MCR_RTS            0x00000002
#define UART_MCR_DTR            0x00000001

/**
 * Macros for line status register.
 */
#define UART_LSR_MASK           0x000000FF
#define UART_LSR_FIFOE          0x00000080
#define UART_LSR_TEMT           0x00000040
#define UART_LSR_TDRQ           0x00000020
#define UART_LSR_BI             0x00000010
#define UART_LSR_FE             0x00000008
#define UART_LSR_PE             0x00000004
#define UART_LSR_OE             0x00000002
#define UART_LSR_DR             0x00000001

/**
 * Macros for modem status register.
 */
#define UART_MSR_MASK           0x000000FF
#define UART_MSR_DCD            0x00000080
#define UART_MSR_RI             0x00000040
#define UART_MSR_DSR            0x00000020
#define UART_MSR_CTS            0x00000010
#define UART_MSR_DDCD           0x00000008
#define UART_MSR_TERI           0x00000004
#define UART_MSR_DDSR           0x00000002
#define UART_MSR_DCTS           0x00000001

/**
 * Macros for scratchpad register.
 */
#define UART_SPR_MASK           0x000000FF
#define UART_SPR_SCRATCH_PAD    0x000000FF

/**
 * Macros for infrared selection register.
 */
#define UART_ISR_MASK           0x0000001F
#define UART_ISR_RXPL           0x00000010
#define UART_ISR_TXPL           0x00000008
#define UART_ISR_XMODE          0x00000004
#define UART_ISR_RCVEIR         0x00000002
#define UART_ISR_XMITIR         0x00000001

/**
 * Macros for receive fifo occupancy register.
 */
#define UART_FOR_MASK           0x0000003F
#define UART_FOR_BYTE_COUNT     0x0000003F

/**
 * Macros for auto-baud control register.
 */
#define UART_ABR_MASK           0x0000000F
#define UART_ABR_ABT            0x00000008
#define UART_ABR_ABUP           0x00000004
#define UART_ABR_ABLIE          0x00000002
#define UART_ABR_ABE            0x00000001

/**
 * Macros for auto-baud count register.
 */
#define UART_ACR_MASK           0x0000FFFF
#define UART_ACR_COUNT_VALUE    0x0000FFFF

/**
 * End of definitions of required macros.
 */



#endif // __UART_TAVORPV_H__
