/*****************************************************************************
 * Filename:    uart.h
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 2008
 *
 * Description: The universal header file for the uart driver. Developed
 *              specifically for the platform software driver framework.
 *
 *****************************************************************************/

#ifndef __UART_H__
#define __UART_H__

/**
 * Port settings.
 */
typedef enum {
    UART_PORT_1 = 1,
    UART_PORT_2,
    UART_PORT_3,
} UART_PORT;

/**
 * Data bits settings.
 */
typedef enum {
    UART_DATABIT_7,
    UART_DATABIT_8,
} UART_DATABIT;

/**
 * Stop bits settings.
 */
typedef enum {
    UART_STOPBIT_0,
    UART_STOPBIT_1,
} UART_STOPBIT;

/**
 * Parity settings.
 */
typedef enum {
    UART_PARITY_NONE,
    UART_PARITY_EVEN,
    UART_PARITY_ODD,
} UART_PARITY;

/**
 * Baud rate settings.
 */
typedef enum {
    UART_BAUD_9P6K    = 9600,
    UART_BAUD_19P2K   = 19200,
    UART_BAUD_38P4K   = 38400,
    UART_BAUD_57P6K   = 57600,
    UART_BAUD_115P2K  = 115200,
    UART_BAUD_230K    = 230400,
    UART_BAUD_460K    = 460800,
    UART_BAUD_921K    = 921600,
    UART_BAUD_1P8M    = 1842000,
    UART_BAUD_3P6M    = 3686400,
} UART_BAUD;

/**
 * Bitmasked event values.
 */
typedef DWORD UART_EVENT;
#define UART_EVENT_DATA_READY       (1 << 0)    /* receiver data available */
#define UART_EVENT_DATA_REQUEST     (1 << 1)    /* transmitter data request */
#define UART_EVENT_RX_FULL          (1 << 2)    /* receiver full */
#define UART_EVENT_TX_EMPTY         (1 << 3)    /* transmitter empty */
#define UART_EVENT_BREAK_INTERRUPT  (1 << 4)    /* break interrupt */
#define UART_EVENT_FRAMING_ERROR    (1 << 5)    /* framing error */
#define UART_EVENT_PARITY_ERROR     (1 << 6)    /* parity error */
#define UART_EVENT_OVERRUN_ERROR    (1 << 7)    /* receiver overrun error */
#define UART_EVENT_UNDERRUN_ERROR   (1 << 8)    /* transmitter underrun error */
#define UART_EVENT_RX_TIMEOUT       (1 << 9)    /* received data timeout */

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
int UartHwInitialize(int port);

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
int UartHwEnable(int port, int flag);

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
int UartHwDisable(int port, int normal);

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
    UART_BAUD baud);

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
    DWORD length);

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
    DWORD length);


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
int UartHwPollEvents(int port, UART_EVENT *events);

void UartHwClockControl(DWORD clock, BOOL enable);

#endif // __UART_H__
