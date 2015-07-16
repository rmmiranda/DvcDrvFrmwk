/*****************************************************************************
 * Filename:    uart_tavorpv.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 2008
 *
 * Description: The Tavor PV sepcific code of the uart driver. Developed
 *              specifically for the platform software driver framework.
 *
 *****************************************************************************/

#include "rei.h"
#include "framework.h"
#include "uart_tavorpv.h"
#include "serial.h"

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

    UartRegister_t  *RegMap;            /* pointer to the register base address */

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

static UartPort_t UartPorts[UART_MAX_PORT];

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
    UartRegister_t  *reg;
    UartPort_t      *device;
    DWORD           ier;
    DWORD           lsr;

    if( port >= UART_MAX_PORT ) {
        /* invalid device port */
        return;
    }

    if( !UartPorts[port].IsInit ) {
        /* device port has not been initialized */
        return;
    }

    /* get a handle to the controller port */
    device = &UartPorts[port];

    /* get a handle to controller register set */
    reg = UartPorts[port].RegMap;

    /* store current IER values */
    ier = reg->IER;

    /* disable all port interrupts */
    reg->IER &= ~( UART_IER_DMAE | UART_IER_RTOIE | UART_IER_MIE |
                   UART_IER_RLSE | UART_IER_TIE | UART_IER_RAVIE );

    /* retrieve current LSR value */
    lsr = reg->LSR;

    /* read until there are nothing to be read */
    while( lsr & UART_LSR_DR ) {

        /* check for line errors */
        if( lsr & (UART_LSR_BI | UART_LSR_FE | UART_LSR_PE | UART_LSR_OE) ) {
            lsr = reg->LSR;
            break;
        }

        /* copy received data to the circular buffer */
        if( device->RxCount < RXFIFO_BUFFER_SIZE ) {
            device->RxFifo[device->RxFifoWrIdx++] = ( (char)(reg->RBR8 & UART_RBR_BYTE0) );
            if( device->RxFifoWrIdx >= RXFIFO_BUFFER_SIZE ) {
                device->RxFifoWrIdx = 0;
            }
            device->RxCount++;
        } else {
            /* circular buffer is already full */
            reg->RBR8;
        }
        lsr = reg->LSR;
    }

    while( lsr & UART_LSR_FIFOE ) {
        /* consume invalid data */
        reg->RBR8;
        /* re-read the LSR */
        lsr = reg->LSR;
    }

    /* restore previous IER values */
    reg->IER = ier;

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
    UartRegister_t  *reg;
    int index;

    if( port >= UART_MAX_PORT ) {
        /* invalid device port */
        return (-DRIVER_INVALID_HW_PORT);
    }

    if( UartPorts[port].IsInit ) {
        /* device port already been initialized */
        return (DRIVER_NO_ERROR);
    }

    /* set register map appropriately */
    switch( port ) {
        case UART_PORT0:
            UartPorts[port].RegMap = (UartRegister_t *) UART_REG_ADDR0;
            break;
        case UART_PORT1:
            UartPorts[port].RegMap = (UartRegister_t *) UART_REG_ADDR1;
            break;
        case UART_PORT2:
        default:
            UartPorts[port].RegMap = (UartRegister_t *) UART_REG_ADDR2;
            break;
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
    UartPorts[port].TxFifoTrigLevel = UART_FCR_TIL_HALF;
    UartPorts[port].RxFifoTrigLevel = UART_FCR_ITL_8;
    UartPorts[port].TrailingBytes = UART_FCR_TRAIL_PXA;

    /* get a handle to controller register set */
    reg = UartPorts[port].RegMap;

    /* disable controller and all interrupts */
    reg->IER = 0;

    /* reset and disable fifos */
    reg->FCR = ( UartPorts[port].PeripheralBus |
                 UartPorts[port].TxFifoTrigLevel |
                 UartPorts[port].RxFifoTrigLevel |
                 UartPorts[port].TrailingBytes |
                 UART_FCR_RESETTF |
                 UART_FCR_RESETRF );

    /* set line control to 8-1-N setting */
    reg->LCR = UART_LCR_WLS_8;
    reg->LCR &= ~( UART_LCR_STB | UART_LCR_PEN );

    /* no modem control, set to default values */
    reg->MCR = 0;

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
    UartRegister_t  *reg;

    if( port >= UART_MAX_PORT ) {
        /* invalid device port */
        return (-DRIVER_INVALID_HW_PORT);
    }

    if( !UartPorts[port].IsInit ) {
        /* device port has not been initialized */
        return (-DRIVER_UNINIT_HW_PORT);
    }

    /* get a handle to controller register set */
    reg = UartPorts[port].RegMap;

#ifdef NESSUS
    /* enable all receiver related interrupts */
    reg->IER |= ( UART_IER_RTOIE | UART_IER_RLSE | UART_IER_RAVIE );

    /* connect uart interrupt line to the interrupt controller */
    reg->MCR |= UART_MCR_OUT2;
#endif

    /* enable the fifos */
    reg->FCR = ( UartPorts[port].PeripheralBus |
                 UartPorts[port].TxFifoTrigLevel |
                 UartPorts[port].RxFifoTrigLevel |
                 UartPorts[port].TrailingBytes |
                 UART_FCR_TRFIFOE );

    /* enable the uart controller port */
    reg->IER |= UART_IER_UUE;

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
    UartRegister_t  *reg;

    if( port >= UART_MAX_PORT ) {
        /* invalid device port */
        return (-DRIVER_INVALID_HW_PORT);
    }

    if( !UartPorts[port].IsInit ) {
        /* device port has not been initialized */
        return (-DRIVER_UNINIT_HW_PORT);
    }

    /* get a handle to controller register set */
    reg = UartPorts[port].RegMap;

#ifdef NESSUS
    /* disable all receiver related interrupts */
    reg->IER &= ~( UART_IER_RTOIE | UART_IER_RLSE | UART_IER_RAVIE );

    /* disconnect uart interrupt line to the interrupt controller */
    reg->MCR &= ~UART_MCR_OUT2;
#endif

    /* reset and disable fifos */
    reg->FCR = ( UartPorts[port].PeripheralBus |
                 UartPorts[port].TxFifoTrigLevel |
                 UartPorts[port].RxFifoTrigLevel |
                 UartPorts[port].TrailingBytes |
                 UART_FCR_RESETTF |
                 UART_FCR_RESETRF );

    /* disable uart controller */
    reg->IER &= ~UART_IER_UUE;

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
    COMM_DATABIT dataBits,
    COMM_STOPBIT stopBits,
    COMM_PARITY parity,
    COMM_BAUD baud)
{
    UartRegister_t  *reg;
    DWORD           lcr;
    DWORD           divisor;
    BOOL            hsMode = FALSE;

    if( port >= UART_MAX_PORT ) {
        /* invalid device port */
        return (-DRIVER_INVALID_HW_PORT);
    }

    if( !UartPorts[port].IsInit ) {
        /* device port has not been initialized */
        return (-DRIVER_UNINIT_HW_PORT);
    }

    /* get a handle to controller register set */
    reg = UartPorts[port].RegMap;

    /* retrieve current LCR */
    lcr = reg->LCR;

    /* set up word length */
    lcr &= ~UART_LCR_WLS_MASK;
    switch( dataBits ) {
        case COMM_DATABIT_7:
            lcr |= UART_LCR_WLS_7;
            break;
        case COMM_DATABIT_8:
        default:
            lcr |= UART_LCR_WLS_8;
            break;
    }

    /* set up stop bits */
    lcr &= ~UART_LCR_STB;
    if( stopBits == COMM_STOPBIT_0 ) {
        lcr |= UART_LCR_STB;
    }

    /* set up parity enable */
    lcr &= ~UART_LCR_PEN;
    if( parity != COMM_PARITY_NONE ) {
        lcr |= UART_LCR_PEN;
        /* disable sticky parity */
        lcr &= ~UART_LCR_STKYP;
        /* set up parity */
        lcr &= ~UART_LCR_EPS;
        if( parity == COMM_PARITY_EVEN ) {
            lcr |= UART_LCR_EPS;
        }
    }

    /* do not send break signals */
    lcr &= ~UART_LCR_SB;

    /* set up baud rate divisor */
    switch( baud ) {
        case COMM_BAUD_9P6K:
        case COMM_BAUD_19P2K:
        case COMM_BAUD_38P4K:
        case COMM_BAUD_57P6K:
        case COMM_BAUD_115P2K:
        case COMM_BAUD_230K:
        case COMM_BAUD_460K:
        case COMM_BAUD_921K:
            divisor = ( (UART_FULL_SPEED_CLOCK >> 4) / baud );
            break;
        case COMM_BAUD_1P8M:
        case COMM_BAUD_3P6M:
            divisor = ( (UART_HIGH_SPEED_CLOCK >> 4) / baud );
            hsMode = TRUE;
            break;
        default:
            divisor = 1;
            break;
    }

    /* disable controller port */
    UartHwDisable( port, 0 );

    /* enable access to the DLH and DLL */
    reg->LCR |= UART_LCR_DLAB;

    /* set up DLH and DLL */
    reg->DLH = ( (divisor >> 8) & UART_DLH_MASK );
    reg->DLL = ( divisor & UART_DLL_MASK );

    /* disable back access to the DLH and DLL */
    reg->LCR &= ~UART_LCR_DLAB;

    /* set new values for LCR */
    reg->LCR = lcr;

    /* set up high speed mode */
    if( hsMode ) {
        reg->IER |= UART_IER_HSE;
    } else {
        reg->IER &= ~UART_IER_HSE;
    }

    /* enable back controller port */
    UartHwEnable( port, 0 );

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
    UartRegister_t  *reg;
    DWORD           ier;
    DWORD           count;

    if( port >= UART_MAX_PORT ) {
        /* invalid device port */
        return (0);
    }

    if( !UartPorts[port].IsInit ) {
        /* device port has not been initialized */
        return (0);
    }

    /* get a handle to controller register set */
    reg = UartPorts[port].RegMap;

    /* store current IER values */
    ier = reg->IER;

    /* disable all port interrupts */
    reg->IER &= ~( UART_IER_DMAE | UART_IER_RTOIE | UART_IER_MIE |
                   UART_IER_RLSE | UART_IER_TIE | UART_IER_RAVIE );

    for( count = 0; count < length; count++, buffer++ ) {

        while( (reg->LSR & UART_LSR_TEMT) == 0 ) {
            /* transmitter data has not been shifted out */
            if( reg->LSR & UART_LSR_TDRQ ) {
                /* do not block since tx fifo is less than half full */
                break;
            }
        }

        /* if we got here, we can write data to tx fifo */
        reg->THR8 = ( *buffer & UART_THR_BYTE0 );
    }

    /* restore previous IER values */
    reg->IER = ier;

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
    UartPort_t  *device;
    DWORD       count = length;
    DWORD       index;

    if( port >= UART_MAX_PORT ) {
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
int UartHwPollEvents(int port, COMM_EVENT *events)
{
    UartRegister_t  *reg;
    DWORD           lsr;

    if( port >= UART_MAX_PORT ) {
        /* invalid device port */
        return (-DRIVER_INVALID_HW_PORT);
    }

    if( !UartPorts[port].IsInit ) {
        /* device port has not been initialized */
        return (-DRIVER_UNINIT_HW_PORT);
    }

    /* get a handle to controller register set */
    reg = UartPorts[port].RegMap;

    /* retrieve current LSR */
    lsr = reg->LSR;

    /* priority 1: check for line error */
    if( lsr & (UART_LSR_BI | UART_LSR_FE | UART_LSR_PE | UART_LSR_OE) ) {

        if( lsr & UART_LSR_BI ) {
            /* publish break interrupt event */
            *events |= COMM_EVENT_BREAK_INTERRUPT;
        }
        if( lsr & UART_LSR_FE ) {
            /* publish framing error event */
            *events |= COMM_EVENT_FRAMING_ERROR;
        }
        if( lsr & UART_LSR_PE ) {
            /* publish parity error event */
            *events |= COMM_EVENT_PARITY_ERROR;
        }
        if( lsr & UART_LSR_OE ) {
            /* publish overrun error event */
            *events |= COMM_EVENT_OVERRUN_ERROR;
        }

        /* re-read the LSR to clear the errors */
        lsr = reg->LSR;

        while( lsr & UART_LSR_FIFOE ) {
            /* consume invalid data */
            reg->RBR8;
            /* re-read the LSR */
            lsr = reg->LSR;
        }
    }

    /* priority 2: check if data is available */
    if( lsr & UART_LSR_DR ) {

        /* transfer controller data to port buffer */ 
        uartHwReceive( port );

        /* publish data available event */
        *events |= COMM_EVENT_DATA_READY;
    }

    /* priority 3: check for receiver timeout */
    if( reg->IIR & UART_IIR_TOD ) {

        /* reset and disable fifos */
        reg->FCR = ( UartPorts[port].PeripheralBus |
                     UartPorts[port].TxFifoTrigLevel |
                     UartPorts[port].RxFifoTrigLevel |
                     UartPorts[port].TrailingBytes |
                     UART_FCR_RESETRF |
                     UART_FCR_TRFIFOE );

        /* publish receiver data timeout event */
        *events |= COMM_EVENT_RX_TIMEOUT;
    }

    /* return successfully */
    return (DRIVER_NO_ERROR);
}



#define TAVORPV_REG(addr)       (*((volatile DWORD *)(addr)))

#define TAVORPV_D0CKEN_A_REG    TAVORPV_REG(0x4134000C)
void UartHwClockControl(DWORD clock, BOOL enable)
{
    if( enable ) {
        // enable clock
        TAVORPV_D0CKEN_A_REG |= (1 << clock);
    } else {
        // disable clock
        TAVORPV_D0CKEN_A_REG &= ~(1 << clock);
    }

    // The enabling of the clock could actually get delayed for 5us
    // if a timer expired and is currenty using the SGPR to notify
    // the interrupt controller.  To work around this, read back the
    // D0CKEN register to ensure that the write is completed.
    // Simplicity Issue #414809.
    TAVORPV_D0CKEN_A_REG;

    // It also takes 3 cycles of the enabled frequency for the clock to
    // become active, minus the time it takes to read back the D0CKEN
    // register above (3/20Mhz, 15ns)
    //
    // Regsiter	RIM Enable Bit	Description	        Min CLK Freq. (MHz)	Min Period (ns)
    // D0CKEN_A	21	UART2 Clock Enable 	            14.857	            67.31
    // D0CKEN_A	22	UART1 Clock Enable 	            14.857	            67.31
    // D0CKEN_A	23	UART3 Clock Enable 	            14.857	            67.31
    //
    // We're just going to assume a worst case of ((67.31ns * 3) - 15ns)
    // and delay approx 187ns
    RE_HwDelay( 187 );
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

