/*****************************************************************************
 * Filename:    uart_mmp.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     February 2010
 *
 * Description: The MMP specific code of the uart driver. Developed
 *              specifically for the platform software driver framework.
 *
 *****************************************************************************/

#include "rei.h"
#include "framework.h"
#include "uart.h"
#include "uart_mmp.h"

#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE  FILE_UART_FWK

#define RXFIFO_BUFFER_SIZE  64

/*
 ***************************************************
 ***************************************************
 *
 * Data structure definitions
 *
 ***************************************************
 ***************************************************
 */

typedef struct {

    BYTE    RxFifo[RXFIFO_BUFFER_SIZE]; /* internal circfular buffer for received data */ 
    DWORD   RxCount;                    /* number of bytes presently in the buffer */
    DWORD   RxFifoRdIdx;                /* the next available read index in the buffer */ 
    DWORD   RxFifoWrIdx;                /* the next available write index in the buffer */

    DWORD   PeripheralBus;              /* width (8- or 32-bit) peripheral bus */ 
    DWORD   TxFifoTrigLevel;            /* tx fifo threshold bytes to trigger transfer */ 
    DWORD   RxFifoTrigLevel;            /* rx fifo threshold bytes to trigger transfer */
    DWORD   TrailingBytes;              /* trailing bytes to be inserted manually or not */

    BOOL    IsInit;                     /* determines if port has been initialized */ 

} UartPort_t;

/*
 ***************************************************
 ***************************************************
 *
 * Static global variable declarations
 *
 ***************************************************
 ***************************************************
 */

static UartPort_t UartPorts[UART_PORT_MAX];

/*
 ***************************************************
 ***************************************************
 *
 * Exportable global variable declarations
 *
 ***************************************************
 ***************************************************
 */

/*
 ***************************************************
 ***************************************************
 *
 * Static function definitions
 *
 ***************************************************
 ***************************************************
 */

static void uartHwReceive(int port)
{
    UartPort_t *device;
    DWORD      ier;
    DWORD      lsr;

    if( port >= UART_PORT_MAX ) {
        /* invalid device port */
        return;
    }

    if( !UartPorts[port].IsInit ) {
        /* device port has not been initialized */
        return;
    }

    /* get a handle to the controller port */
    device = &UartPorts[port];

    /* store current IER values */
    ier = ReadRegDword( UART_IER_REG(port) );

    /* disable all port interrupts */
    WriteRegDwordAnd( UART_IER_REG(port),
                      ~(UART_IER_DMAE |
                        UART_IER_RTOIE |
                        UART_IER_MIE |
                        UART_IER_RLSE |
                        UART_IER_TIE |
                        UART_IER_RAVIE) );

    /* retrieve current LSR value */
    lsr = ReadRegDword( UART_LSR_REG(port) );

    /* read until there are nothing to be read */
    while( lsr & UART_LSR_DR ) {

        /* check for line errors */
        if( lsr & (UART_LSR_BI | UART_LSR_FE | UART_LSR_PE | UART_LSR_OE) ) {
            lsr = ReadRegDword( UART_LSR_REG(port) );
            break;
        }

        /* copy received data to the circular buffer */
        if( device->RxCount < RXFIFO_BUFFER_SIZE ) {
            device->RxFifo[device->RxFifoWrIdx++] = ( (char)(ReadRegDword(UART_RBR_REG(port)) & UART_RBR_BYTE0) );
            if( device->RxFifoWrIdx >= RXFIFO_BUFFER_SIZE ) {
                device->RxFifoWrIdx = 0;
            }
            device->RxCount++;
        } else {
            /* circular buffer is already full */
            ReadRegDword( UART_RBR_REG(port) );
        }
        lsr = ReadRegDword( UART_LSR_REG(port) );
    }

    while( lsr & UART_LSR_FIFOE ) {
        /* consume invalid data */
        ReadRegDword( UART_RBR_REG(port) );
        /* re-read the LSR */
        lsr = ReadRegDword( UART_LSR_REG(port) );
    }

    /* restore previous IER values */
    WriteRegDword( UART_IER_REG(port), ier );

    /* return successfully */
    return;
}

/*
 ***************************************************
 ***************************************************
 *
 * Exportable function definitions (BEGIN)
 *
 ***************************************************
 ***************************************************
 */

/**
 * Initializes an instance of the UART device port.
 *
 * @param port - The device port to be initialized.
 *
 * @return Zero if the device driver configuration is successful. Otherwise,
 *         returns a negative value, from which tha absolute value refers to
 *         to a specific error. 
 *
 */
int UartHwInitialize(int port)
{
    int index;

    if( port >= UART_PORT_MAX ) {
        /* invalid device port */
        return (-DRIVER_INVALID_HW_PORT);
    }

    if( UartPorts[port].IsInit ) {
        /* device port already been initialized */
        return (DRIVER_NO_ERROR);
    }

    /* initialize rx buffer and counter */
    UartPorts[port].RxCount = 0;
    UartPorts[port].RxFifoRdIdx = 0;
    UartPorts[port].RxFifoWrIdx = 0;
    for( index=0; index<RXFIFO_BUFFER_SIZE; index++ ) {
        UartPorts[port].RxFifo[index] = 0;
    }

    /* set fifo bus, trailing bytes and trigger levels to default */
    UartPorts[port].PeripheralBus = UART_FCR_BUS_8;
    UartPorts[port].TxFifoTrigLevel = UART_FCR_TIL_EMPTY;
    UartPorts[port].RxFifoTrigLevel = UART_FCR_ITL_1;
    UartPorts[port].TrailingBytes = UART_FCR_TRAIL_CPU;

    /* disable controller and all interrupts */
    WriteRegDword( UART_IER_REG(port), 0x00000000 );

    /* set line control register to 0x00000000 */
    WriteRegDword( UART_LCR_REG(port), 0x00000000 );

    /* no modem control, set to default values */
    WriteRegDword( UART_MCR_REG(port), 0x00000000 );

#if 1
    while( ReadRegDword(UART_LSR_REG(port)) & UART_LSR_DR ) {
        /* consume invalid data */
        ReadRegDword( UART_RBR_REG(port) );
    }
#endif

    /* set initialization flag */
    UartPorts[port].IsInit = TRUE;

    /* return succesfully */
    return (DRIVER_NO_ERROR);

}

/**
 * Enables an instance of the UART device port.
 *
 * @param port - The device port to be enabled.
 * @param flag - Placeholder for future use.
 *
 * @return Zero if the device driver configuration is successful. Otherwise,
 *         returns a negative value, from which tha absolute value refers to
 *         to a specific error. 
 *
 */
int UartHwEnable(int port, int flag)
{
    if( port >= UART_PORT_MAX ) {
        /* invalid device port */
        return (-DRIVER_INVALID_HW_PORT);
    }

    if( !UartPorts[port].IsInit ) {
        /* device port has not been initialized */
        return (-DRIVER_UNINIT_HW_PORT);
    }

    /* enable all receiver related interrupts */
    WriteRegDwordOr( UART_IER_REG(port),
                     (UART_IER_RTOIE |
                      UART_IER_RLSE |
                      UART_IER_RAVIE) );

    /* connect uart interrupt line to the interrupt controller */
    WriteRegDwordOr( UART_MCR_REG(port), UART_MCR_OUT2 );

    /* enable the fifos */
    WriteRegDword( UART_FCR_REG(port), 
                   (UartPorts[port].PeripheralBus |
                    UartPorts[port].TxFifoTrigLevel |
                    UartPorts[port].RxFifoTrigLevel |
                    UartPorts[port].TrailingBytes |
                    UART_FCR_RESETTF |
                    UART_FCR_RESETRF |
                    UART_FCR_TRFIFOE) );

    /* enable the uart controller port */
    WriteRegDwordOr( UART_IER_REG(port), UART_IER_UUE );

    /* return successfully */
    return (DRIVER_NO_ERROR);
}

/**
 * Disables an instance of the UART device port.
 *
 * @param port   - The device port to be enabled.
 * @param normal - Method by which the device port will be disabled.
 *
 * @return Zero if the device driver configuration is successful. Otherwise,
 *         returns a negative value, from which tha absolute value refers to
 *         to a specific error. 
 *
 */
int UartHwDisable(int port, int normal)
{
    if( port >= UART_PORT_MAX ) {
        /* invalid device port */
        return (-DRIVER_INVALID_HW_PORT);
    }

    if( !UartPorts[port].IsInit ) {
        /* device port has not been initialized */
        return (-DRIVER_UNINIT_HW_PORT);
    }

    /* disable all receiver related interrupts */
    WriteRegDwordAnd( UART_IER_REG(port),
                      ~(UART_IER_RTOIE |
                        UART_IER_RLSE |
                        UART_IER_RAVIE) );

    /* disconnect uart interrupt line to the interrupt controller */
    WriteRegDwordAnd( UART_MCR_REG(port), ~UART_MCR_OUT2 );

    /* reset and disable fifos */
    WriteRegDword( UART_FCR_REG(port), 
                   (UartPorts[port].PeripheralBus |
                    UartPorts[port].TxFifoTrigLevel |
                    UartPorts[port].RxFifoTrigLevel |
                    UartPorts[port].TrailingBytes |
                    UART_FCR_RESETTF |
                    UART_FCR_RESETRF) );

    /* disable uart controller */
    WriteRegDwordAnd( UART_IER_REG(port), ~UART_IER_UUE );

    /* return successfully */
    return (DRIVER_NO_ERROR);
}

/**
 * Configures an instance of the UART device port.
 *
 * @param port     - The device port to be configured.
 * @param dataBits - The new data bits setting for the device instance.
 * @param stopBits - The new stop bits setting for the device instance.
 * @param parity   - The new parity setting for the device instance.
 * @param baud     - The new baud rate setting for the device instance.
 *
 * @return Zero if the device driver configuration is successful. Otherwise,
 *         returns a negative value, from which tha absolute value refers to
 *         to a specific error. 
 *
 */
int UartHwConfigure(
    int port,
    UART_DATABIT dataBits,
    UART_STOPBIT stopBits,
    UART_PARITY parity,
    UART_BAUD baud)
{
    DWORD lcr;
    DWORD divisor;
    //BOOL hsMode = FALSE;

    if( port >= UART_PORT_MAX ) {
        /* invalid device port */
        return (-DRIVER_INVALID_HW_PORT);
    }

    if( !UartPorts[port].IsInit ) {
        /* device port has not been initialized */
        return (-DRIVER_UNINIT_HW_PORT);
    }

    /* retrieve current LCR */
    lcr = ReadRegDword( UART_LCR_REG(port) );

    /* set up word length */
    lcr &= ~UART_LCR_WLS_MASK;
    switch( dataBits ) {
        case UART_DATABIT_7:
            lcr |= UART_LCR_WLS_7;
            break;
        case UART_DATABIT_8:
        default:
            lcr |= UART_LCR_WLS_8;
            break;
    }

    /* set up stop bits */
    lcr &= ~UART_LCR_STB;
    if( stopBits == UART_STOPBIT_0 ) {
        lcr |= UART_LCR_STB;
    }

    /* set up parity enable */
    lcr &= ~UART_LCR_PEN;
    if( parity != UART_PARITY_NONE ) {
        lcr |= UART_LCR_PEN;
        /* disable sticky parity */
        lcr &= ~UART_LCR_STKYP;
        /* set up parity */
        lcr &= ~UART_LCR_EPS;
        if( parity == UART_PARITY_EVEN ) {
            lcr |= UART_LCR_EPS;
        }
    }

    /* do not send break signals */
    lcr &= ~UART_LCR_SB;

    /* set up baud rate divisor */
    switch( baud ) {
        case UART_BAUD_9P6K:
        case UART_BAUD_19P2K:
        case UART_BAUD_38P4K:
        case UART_BAUD_57P6K:
        case UART_BAUD_115P2K:
            divisor = ( (UART_FULL_SPEED_CLOCK >> 4) / baud );
            break;
//        case UART_BAUD_230K:
//        case UART_BAUD_460K:
//        case UART_BAUD_921K:
//        case UART_BAUD_1P8M:
//        case UART_BAUD_3P6M:
//            divisor = ( (UART_HIGH_SPEED_CLOCK >> 4) / baud );
            //hsMode = TRUE;
//            break;
        default:
            divisor = 1;
            break;
    }

    /* disable controller port */
//    UartHwDisable( port, 0 );

    /* enable access to the DLH and DLL */
    WriteRegDwordOr( UART_LCR_REG(port), UART_LCR_DLAB );

    /* set up DLH and DLL */
    WriteRegDword( UART_DLH_REG(port), ((divisor >> 8) & UART_DLH_MASK) );
    WriteRegDword( UART_DLL_REG(port), (divisor & UART_DLL_MASK) );

    /* disable back access to the DLH and DLL */
    WriteRegDwordAnd( UART_LCR_REG(port), ~UART_LCR_DLAB );

    /* set new values for LCR */
    WriteRegDword( UART_LCR_REG(port), lcr );

#if 0
    /* set up high speed mode */
    if( hsMode ) {
        WriteRegDwordOr( UART_IER_REG(port), UART_IER_HSE );
    } else {
        WriteRegDwordAnd( UART_IER_REG(port), ~UART_IER_HSE );
    }
#endif

    /* enable back controller port */
//    UartHwEnable( port, 0 );

    /* return successfully */
    return (DRIVER_NO_ERROR);
}

/**
 * Performs a raw data transfer to an instance of the UART device port.
 *
 * @param port     - The device port to which data will be transferred.
 * @param *buffer  - The pointer to a location where data will be originating.
 * @param length   - The number of data bytes requested to be transferred.
 *
 * @return The number of actual data bytes that have been transferred.
 *
 */
DWORD UartHwWrite(
    int port,
    char const *buffer,
    DWORD length)
{
    DWORD ier;
    DWORD count;

    if( port >= UART_PORT_MAX ) {
        /* invalid device port */
        return (0);
    }

    if( !UartPorts[port].IsInit ) {
        /* device port has not been initialized */
        return (0);
    }

    /* store current IER values */
    ier = ReadRegDword( UART_IER_REG(port) );

    /* disable all port interrupts */
    WriteRegDwordAnd( UART_IER_REG(port),
                      ~(UART_IER_DMAE |
                        UART_IER_RTOIE |
                        UART_IER_MIE |
                        UART_IER_RLSE |
                        UART_IER_TIE |
                        UART_IER_RAVIE) );

    for( count = 0; count < length; count++, buffer++ ) {

        while( (ReadRegDword(UART_LSR_REG(port)) & UART_LSR_TEMT) == 0 ) {
            /* transmitter data has not been shifted out */
            if( ReadRegDword(UART_LSR_REG(port)) & UART_LSR_TDRQ ) {
                /* do not block since tx fifo is less than half full */
                break;
            }
        }

        /* if we got here, we can write data to tx fifo */
        WriteRegByte( UART_THR_REG(port), (BYTE)(*buffer & UART_THR_BYTE0) );
    }

    /* restore previous IER values */
    WriteRegDword( UART_IER_REG(port), ier );

    /* return the number of actual bytes transmitted */
    return count;
}

/**
 * Performs a raw data transfer from an instance of the UART device port.
 *
 * @param port     - The device port from which data will be transferred.
 * @param *buffer  - The pointer to a location where data will be stored.
 * @param length   - The number of data bytes requested to be transferred.
 *
 * @return The number of actual data bytes that have been transferred.
 *
 */
DWORD UartHwRead(
    int port,
    char *buffer,
    DWORD length)
{
    UartPort_t *device;
    DWORD      count = length;
    DWORD      index;

    if( port >= UART_PORT_MAX ) {
        /* invalid device port */
        return (0);
    }

    if( !UartPorts[port].IsInit ) {
        /* device port has not been initialized */
        return (0);
    }

    /* get a handle to the controller port */
    device = &UartPorts[port];

    /* read only what is actually available */
    if( device->RxCount < length ) {
        count = device->RxCount;
    } 

    /* copy data from circular buffer */
    for( index=0; index<count; index++ ) {
        if( device->RxCount ) {
            buffer[index] = device->RxFifo[device->RxFifoRdIdx++];
            if( device->RxFifoRdIdx >= RXFIFO_BUFFER_SIZE ) {
                device->RxFifoRdIdx = 0;
            }
            device->RxCount--; 
        } else {
            /* circular buffer is already empty */
            break;
        }
    }

    /* return actual bytes read */
    return count;
}

/**
 * Polls the instance of the UART device port for hardware events.
 *
 * @param port    - The device port from which events will be polled.
 * @param *events - The pointer to a location where events will be stored as
 *                  bitmasked values.
 *
 * @return Zero if the device driver configuration is successful. Otherwise,
 *         returns a negative value, from which tha absolute value refers to
 *         to a specific error. 
 *
 */
int UartHwPollEvents(int port, UART_EVENT *events)
{
    DWORD lsr;

    if( port >= UART_PORT_MAX ) {
        /* invalid device port */
        return (-DRIVER_INVALID_HW_PORT);
    }

    if( !UartPorts[port].IsInit ) {
        /* device port has not been initialized */
        return (-DRIVER_UNINIT_HW_PORT);
    }

    /* retrieve current LSR */
    lsr = ReadRegDword( UART_LSR_REG(port) );

    /* priority 1: check for line error */
    if( lsr & (UART_LSR_BI | UART_LSR_FE | UART_LSR_PE | UART_LSR_OE) ) {

        if( lsr & UART_LSR_BI ) {
            /* publish break interrupt event */
            *events |= UART_EVENT_BREAK_INTERRUPT;
        }
        if( lsr & UART_LSR_FE ) {
            /* publish framing error event */
            *events |= UART_EVENT_FRAMING_ERROR;
        }
        if( lsr & UART_LSR_PE ) {
            /* publish parity error event */
            *events |= UART_EVENT_PARITY_ERROR;
        }
        if( lsr & UART_LSR_OE ) {
            /* publish overrun error event */
            *events |= UART_EVENT_OVERRUN_ERROR;
        }

        /* re-read the LSR to clear the errors */
        lsr = ReadRegDword( UART_LSR_REG(port) );

        while( lsr & UART_LSR_FIFOE ) {
            /* consume invalid data */
            ReadRegDword( UART_RBR_REG(port) );
            /* re-read the LSR */
            lsr = ReadRegDword( UART_LSR_REG(port) );
        }
    }

    /* priority 2: check if data is available */
    if( lsr & UART_LSR_DR ) {

        /* transfer controller data to port buffer */ 
        uartHwReceive( port );

        /* publish data available event */
        *events |= UART_EVENT_DATA_READY;
    }

    /* priority 3: check for receiver timeout */
    if( ReadRegDword(UART_IIR_REG(port)) & UART_IIR_TOD ) {

        /* reset and disable fifos */
        WriteRegDword( UART_FCR_REG(port), 
                       (UartPorts[port].PeripheralBus |
                        UartPorts[port].TxFifoTrigLevel |
                        UartPorts[port].RxFifoTrigLevel |
                        UartPorts[port].TrailingBytes |
                        UART_FCR_RESETRF |
                        UART_FCR_TRFIFOE) );

        /* publish receiver data timeout event */
        *events |= UART_EVENT_RX_TIMEOUT;
    }

    /* return successfully */
    return (DRIVER_NO_ERROR);
}

/*
 ***************************************************
 ***************************************************
 ***************************************************
 ***************************************************
 ***************************************************
 */

#define APB_CLOCK_UART1         0xD401502C
#define APB_CLOCK_UART2         0xD4015030
#define APB_CLOCK_UART3         0xD4015034

#define APB_CLOCK_MASK          0x00000077
#define APB_CLOCK_FNCLK_14MHZ   0x00000000
#define APB_CLOCK_FNCLK_26MHZ   0x00000001
#define APB_CLOCK_FNCLK_38MHZ   0x00000001
#define APB_CLOCK_RESET         0x00000004
#define APB_CLOCK_FNCLK_ON      0x00000002
#define APB_CLOCK_APBCLK_ON     0x00000001

#define PMU_CLOCK_UART          0xD4050014
#define PMU_CLOCK_UARTDIVN      0x1FFF0000
#define PMU_CLOCK_UARTDIVD      0x00001FFF

void UartClockEnable(RE_PeripheralClock clock)
{
    DWORD accu;
    DWORD loop;

    /* determine UART clock control register */
    switch( clock ) {
        case CLOCK_UART1:
            accu = APB_CLOCK_UART1;
            break;
        case CLOCK_UART2:
            accu = APB_CLOCK_UART2;
            break;
        case CLOCK_UART3:
        default:
            accu = APB_CLOCK_UART3;
            break;
    }

    /* set the functional clock freq to 19.23Mhz */
    //WriteRegDword( PMU_CLOCK_UART, 0x0DC102B1 );
    WriteRegDword( accu, APB_CLOCK_FNCLK_14MHZ );

    /* enable both functional and bus clocks */
    WriteRegDwordOr( accu, (APB_CLOCK_FNCLK_ON | APB_CLOCK_APBCLK_ON) );

    /* reset the clocks */
    WriteRegDwordAnd( accu, ~APB_CLOCK_RESET );

    /* delay for 3 clock periods of the slowest frequency */
    /* For 14.7456MHz clock, 3 clock periods is approx 200 ns */
//    RE_HwDelay( 200 );
    for( loop=0; loop<200; loop++ );

    return;
}

void UartClockDisable(RE_PeripheralClock clock)
{
    DWORD accu;
    DWORD loop;

    /* determine UART clock control register */
    switch( clock ) {
        case CLOCK_UART1:
            accu = APB_CLOCK_UART1;
            break;
        case CLOCK_UART2:
            accu = APB_CLOCK_UART2;
            break;
        case CLOCK_UART3:
        default:
            accu = APB_CLOCK_UART3;
            break;
    }

    /* disable first the functional clock */
    WriteRegDwordAnd( accu, ~APB_CLOCK_FNCLK_ON );

    /* delay for 5 clock periods of the slowest frequency */
    /* For 14.7456MHz clock, 5 clock periods is approx 333 ns */
    //RE_HwDelay( 333 );
    for( loop=0; loop<333; loop++ );

    /* then disable the bus clock */
    WriteRegDwordAnd( accu, ~APB_CLOCK_APBCLK_ON );

    /* reset the clocks */
    WriteRegDwordOr( accu, APB_CLOCK_RESET );

    return;
}

/*
 ***************************************************
 ***************************************************
 *
 * Exportable function definitions (END)
 *
 ***************************************************
 ***************************************************
 */

