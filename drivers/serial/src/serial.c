/*****************************************************************************
 * Filename:    serial.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 2008
 *
 * Description: The hardware independent code of the serial driver. Developed
 *              specifically for the platform software driver framework.
 *
 *****************************************************************************/

#include "rei.h"
#include "framework.h"
#include "driver.h"
#include "uart.h"

#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE  FILE_SERIAL_FWK

#define SERIAL_MAX_ARGUMENT     8
#define SERIAL_COMMAND_SIZE     256
#define SERIAL_HELP_SIZE        256

#warning "START: Things to be fixed when GPIO is ported to MiniOS"
#if defined( PLATFORM_FLINT_MMP2 )
#define SERIAL_PORT         UART_PORT_3
#define SERIAL_TX           GPIO_52
#define SERIAL_RX           GPIO_51
#define SERIAL_TX_FN        1
#define SERIAL_RX_FN        1
#define SERIAL_CLOCK        CLOCK_UART3
#define SERIAL_IRQ          IRQ_SOURCE( HWIRQ_UART3 )
#elif defined( PLATFORM_VP_MMP2 )
#define SERIAL_PORT         UART_PORT_2
#define SERIAL_TX           GPIO_48
#define SERIAL_RX           GPIO_47
#define SERIAL_TX_FN        1
#define SERIAL_RX_FN        1
#define SERIAL_CLOCK        CLOCK_UART2
#define SERIAL_IRQ          IRQ_SOURCE( HWIRQ_UART2 )
#else
#warning "Platform not yet supported (defaults to CAPROCK)"
#define GPIO_54             ((92<<16)|54)
#define GPIO_53             ((93<<16)|53)
#define SERIAL_PORT         UART_PORT_1
#define SERIAL_TX           GPIO_54
#define SERIAL_RX           GPIO_53
#define SERIAL_TX_FN        1
#define SERIAL_RX_FN        1
#define SERIAL_CLOCK        CLOCK_UART1
#define SERIAL_IRQ          IRQ_SOURCE( HWIRQ_UART1 )
#endif

void UartClockEnable(RE_PeripheralClock clock);
void UartClockDisable(RE_PeripheralClock clock);
void UartPadEnable(RE_Line line);
void UartPadDisable(RE_Line line);
#warning "STOP: Things to be fixed when GPIO is ported to MiniOS"

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
    DRIVER_STATE    State;      /* current driver state */
} SerialStatus_t;

typedef struct {
    DWORD BiCount;   /* break interrupt count */
    DWORD FeCount;   /* framing error count */
    DWORD PeCount;   /* parity error count */
    DWORD OeCount;   /* overflow error count */
    DWORD TxCount;   /* transmitted data byte count */
    DWORD RxCount;   /* received data byte count */
} SerialStats_t;

typedef struct {
    UART_PORT           Port;       /* device port setting */       
    UART_DATABIT        DataBits;   /* data width setting */
    UART_STOPBIT        StopBits;   /* stop bits setting */
    UART_PARITY         Parity;     /* parity setting */
    UART_BAUD           Baud;       /* baud rate setting */
    RE_Line             TxLine;
    RE_Line             RxLine;
    BYTE                TxLineMode;
    BYTE                RxLineMode;
    RE_PeripheralClock  Clock;
    RE_IntId            Irq;
    RE_IRQHandler       *IrqHandler;
    BOOL                IsConsole;  /* console function flag */
} SerialConfig_t;

typedef struct {
    char            Buffer[SERIAL_COMMAND_SIZE];   /* console buffer */
    unsigned int    BufferCount;                    /* console buffer count */ 
} SerialConsole_t;

typedef struct {
    /* driver interface data structure instance */
    Driver_t Driver;
    /* required data structure instances for the driver interface */
    SerialStatus_t  Status;
    SerialStats_t   Statistics;
    SerialConfig_t  Config;
    /* driver specific (private) fields */
    SerialConsole_t Console;
    RE_Semaphore    TxMutex;
} SerialDriver_t;

typedef struct {
    char            *Name;                  /* name of the serial command */
    DrvCommandFunc  Function;               /* serial command callback function */
    char            Help[SERIAL_HELP_SIZE]; /* help information of the serial command */
} SerialCommand_t;

/*
 ***************************************************
 ***************************************************
 *
 * Static global variable declarations
 *
 ***************************************************
 ***************************************************
 */

static SerialDriver_t SerialDriver;

/*********************************/
/*********************************/
/*** TODO: Needs to be removed ***/
/*********************************/
/*********************************/
#define CHAR2INT(x)     (((x)>='0' && (x)<='9') ? ((x)-'0')    : (\
                         ((x)>='A' && (x)<='F') ? ((x)-'A'+10) : (\
                         ((x)>='a' && (x)<='f') ? ((x)-'a'+10) : 0\
                        )))

DWORD convert2int(char *str)
{
    DWORD num = 0;
    int index = 0;
    int multi = 10;

    if( str[0]=='0' && (str[1]=='x' || str[1]=='X') ) {
        index = 2;
        multi = 16;
    } else if( str[0]=='0' && str[1]!='\0' ) {
        index = 1;
        multi = 8;
    }

    while( str[index] != '\0' ) {
        num = (DWORD) ((multi * num) + CHAR2INT(str[index]));
        index++; 
    }

    return num; 
}

void PlatformBugdispEntry(const char *, DWORD, DWORD);   
int cmd_bugdisp( int argc, char *argv[] )
{
    char *cmd = "pf.fwk.info";
    DWORD arg1 = 0;
    DWORD arg2 = 0;
    int argcnt = argc;

    argcnt--;
    if( argcnt-- > 0 )
        cmd = argv[1];

    if( argcnt-- > 0 )
        arg1 = convert2int(argv[2]);

    if( argcnt-- > 0 )
        arg2 = convert2int(argv[3]);

    PlatformBugdispEntry(cmd, arg1, arg2);

    return 1;
}

static SerialCommand_t SerialCommandList[] = {
    { "x", cmd_bugdisp, "Tests the Driver Framework's debug module." },
};

#define SERIAL_MAX_COMMAND      (sizeof(SerialCommandList)/sizeof(SerialCommand_t))
/*********************************/
/*********************************/
/*********************************/
/*********************************/
/*********************************/

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
 * static function definitions
 *
 ***************************************************
 ***************************************************
 */
static SerialDriver_t *serial_get_instance(Driver_t *driver)
{
    if( driver == &(SerialDriver.Driver) ) {
        /* found driver instance */
        return &(SerialDriver);
    }

    /* driver instance could not be found */
    return NULL;
}

static void serial_put_char(SerialDriver_t *driver, const char c)
{
    char carriage_return = '\r';

    if( c == '\n' ) {
        /* precede a crriage return for every newline character */
        driver->Statistics.TxCount += UartHwWrite( driver->Config.Port, &carriage_return, 1 );
    }

    /* send the character to the serial driver port */
    driver->Statistics.TxCount += UartHwWrite( driver->Config.Port, &c, 1 );
}

static char serial_get_char(SerialDriver_t *driver)
{
    char    c;
    DWORD   count;

    /* do a serial driver read */
    count = UartHwRead( driver->Config.Port, &c, 1 );
    if( count > 0 ) {
        /* update driver statistics */
        driver->Statistics.RxCount += count;
        /* return the retrieved character */
        return c;
    }

    /* otherwise return a null character */
    return '\0';
}

static void serial_puts(SerialDriver_t *driver, const char *s)
{
    while( *s ) {
        serial_put_char( driver, *s++ );
    }
}

static void serial_prompt(SerialDriver_t *driver)
{
    serial_puts( driver, "Platform:>" );
}

static BOOL escapeMode = FALSE;
static int serial_process_console( SerialDriver_t *driver )
{
    char            *buffer = driver->Console.Buffer;
    unsigned int    *count = &(driver->Console.BufferCount); 
    char            c = 0;
    int             argc = 0;
    char            *argv[SERIAL_MAX_ARGUMENT];
    int             index = 0;
    BOOL            commandFound = FALSE;

    /* Step 1 --- Scan at most one line */

    /* lock the serial port write mutex */
    RE_RimLockSemaphore( &driver->TxMutex );

    while( c!=0x0d && c!=0x0a ) {

        /* read back a character */
        c = serial_get_char( driver );
        //PRINTN( "c=0x%02x", (char) c);

        /* handle special characters */
        switch( c ) {

            /* newline and carriage return */
            case 0x0a:
            case 0x0d:
                /* append string terminator on the console buffer */
                buffer[*count] = '\0';
                /* send newline to the serial driver port */
                serial_put_char( driver, '\n' );
                break;

            /* null */
            case 0x00:
                /* no incoming data */
                /* unlock the serial port write mutex */
                RE_RimUnlockSemaphore( &driver->TxMutex );
                return( 0 );

            /* control-C */
            case 0x03:
                /* cancel pending command string */
                while( (*count)-- ) {
                    /* send delete to the serial driver port */
                    serial_put_char( driver, (char) 0x7F );
                }
                /* reset console buffer and index */
                *count = 0;
                buffer[0] = '\0';
                continue;
    
            /* backspace and delete */
            case 0x08:
            case 0x7f:
                if( *count ) {
                    /* drop last character from console buffer */
                    (*count)--;
                    /* send delete to the serial driver port */
                    serial_put_char( driver, (char) 0x7F );
                }
                continue;

            /* control-D */
            case 0x04:
                /* decrement debug level */
                DrvDecDebugLevel();
                continue;

            /* control-I  */
            case 0x09:
                /* increment debug level */
                DrvIncDebugLevel();
                continue;

            /* escape sequence */
            case 0x1b:
            case 0x5b:
                escapeMode = TRUE;
                break;

            /* normal character */
            default:
                if( escapeMode ) {
                    escapeMode = FALSE;
                    break;
                }
                if( *count < SERIAL_COMMAND_SIZE ) {
                    /* echo back */
                    serial_put_char( driver, c );
                    /* store in console buffer */ 
                    buffer[(*count)++] = c;
                } else {
                    /* send bell alert since console buffer is full */
                    serial_put_char( driver, '\a' ); 
                }
                continue;

        } /* switch */

    } /* while */

    /* unlock the serial port write mutex */
    RE_RimUnlockSemaphore( &driver->TxMutex );

    /* Step 2 --- Parse the scanned line into command and argument tokens */

    while( argc < SERIAL_MAX_ARGUMENT ) {

        /* skip white spaces */
        while( (*buffer == ' ') || (*buffer == '\t') ) {
            buffer++;
        }

        /* check for the end of the command string */
        if( *buffer == '\0' ) {
            argv[argc] = NULL;
            break;  
        }

        /* found a token */
        argv[argc] = buffer;

        /* increment argument counter */
        argc++;

        /* copy current token string to the array */
        while( (*buffer) && (*buffer != ' ') && (*buffer != '\t') ) {
            /* increment command string pointer */
            buffer++;
        }

        /* check again for the end of the command string */
        if( *buffer == '\0' ) {
            argv[argc] = NULL;
            break;  
        }

        /* terminate current token string */
        *buffer = '\0';

        /* increment command string to process next token */
        buffer++;

    } /* while */

    /* Step 3 --- Find the requested command from a predefined list */

    if( argc ) {

        for( index=0; index<SERIAL_MAX_COMMAND; index++ ) {

            /* get a handle to the requested command string */
            char *commandOnArgv = argv[0];
            /* get the name of the command from the list */
            char *commandOnList = SerialCommandList[index].Name;

            /* string compare the requested command and the
             * current command from the list
             */
            while( *commandOnList == *commandOnArgv++ ) {
                if( *commandOnList++ == '\0') {
                    /* command found */
                    commandFound = TRUE;
                    break;
                }
            }

            /* break out if command has been found */
            if( commandFound ) {
                break;
            }
        }
    }

    /* Step 4 --- Run the requested command */

    if( commandFound ) {
        if( SerialCommandList[index].Function(argc, argv) < 0 ) {
            /* command found but failed execution */
            serial_puts( driver, argv[0] );
            serial_puts( driver, ": Command failed" );
        }
    } else {
        if( argc ) {
            /* command not found */
            serial_puts( driver, argv[0] );
            serial_puts( driver, ": Command not found\n" );
        }
    }

    /* Step 5 --- Prepare serial console for next command */

    /* reset console buffer and index */
    *count = 0;
    buffer[0] = '\0';

    /* display the serial prompt */
    serial_prompt( driver );

    return( 0 );

}

/*
 ***************************************************
 ***************************************************
 *
 * Exportable function definitions
 *
 ***************************************************
 ***************************************************
 */

void *SerialGetStatus(Driver_t *driver)
{
    SerialDriver_t *serialDrv;

    /* get handle to the driver instance */
    serialDrv = serial_get_instance(driver);

    if( serialDrv ) {
        /* return a void pointer to the status structure */
        return (void *)&(serialDrv->Status);
    }

    /* no driver instance found */
    return NULL;
}

void *SerialGetStats(Driver_t *driver)
{
    SerialDriver_t *serialDrv;

    /* get handle to the driver instance */
    serialDrv = serial_get_instance(driver);

    if( serialDrv ) {
        /* return a void pointer to the statistics structure */
        return (void *)&(serialDrv->Statistics);
    }

    /* no driver instance found */
    return NULL;
}

void *SerialGetConfig(Driver_t *driver)
{
    SerialDriver_t *serialDrv;

    /* get handle to the driver instance */
    serialDrv = serial_get_instance(driver);

    if( serialDrv ) {
        /* return a void pointer to the config structure */
        return (void *)&(serialDrv->Config);
    }

    /* no driver instance found */
    return NULL;
}

int SerialEnable(Driver_t *driver, int flag)
{
    SerialDriver_t  *serialDrv;
    int             error;

    /* get handle to the driver instance */
    serialDrv = serial_get_instance(driver);

    if( serialDrv ) {

        if( serialDrv->Status.State == DRIVER_ENABLED) {
            /* driver already enabled */
            return (DRIVER_NO_ERROR);
        }

        /* set directions and mode of all pads */
        RE_PadSet( serialDrv->Config.TxLine, PAD_OUTPUT_ENABLE );
        RE_PadSet( serialDrv->Config.RxLine, PAD_INPUT_ENABLE );
        RE_PadSetMode( serialDrv->Config.TxLine, serialDrv->Config.TxLineMode );
        RE_PadSetMode( serialDrv->Config.RxLine, serialDrv->Config.TxLineMode );

        /* disable pull ups and pull downs */
        RE_PadSet( serialDrv->Config.TxLine, PAD_PULL_UP );
        RE_PadSet( serialDrv->Config.RxLine, PAD_PULL_UP );
        RE_PadClear( serialDrv->Config.TxLine, PAD_PULL_DOWN );
        RE_PadClear( serialDrv->Config.RxLine, PAD_PULL_DOWN );

        /* enable peripheral clock */
        //RE_ClockEnable( serialDrv->Config.Clock );
        UartClockEnable( serialDrv->Config.Clock );

        /* configure device controller */
        error = UartHwConfigure( serialDrv->Config.Port,
            serialDrv->Config.DataBits,
            serialDrv->Config.StopBits,
            serialDrv->Config.Parity,
            serialDrv->Config.Baud );

        if( error == DRIVER_NO_ERROR ) {
  
            /* enable device controller */
            error = UartHwEnable( serialDrv->Config.Port, flag );

            if( error == DRIVER_NO_ERROR ) {

                /* enable interrupt line */
                RE_EnableInterrupt( PRIMARY_IRQ, serialDrv->Config.Irq );

                /* set driver state appropriately */
                serialDrv->Status.State = DRIVER_ENABLED;

                /* display a serial prompt if it is a console */
                if( serialDrv->Config.IsConsole ) {
                    serial_puts( serialDrv, "Serial Debugger is ready for use ...\n\n" );
                    //serial_prompt( serialDrv );
                }

                /* return successfully */
                return (DRIVER_NO_ERROR);
            }
        }

        /* if we got here, device controller error encountered */

        /* disable peripheral clock */
        //RE_ClockDisable( serialDrv->Config.Clock );
        UartClockDisable( serialDrv->Config.Clock );

        /* set all pads as gpio inputs */
        RE_PadSet( serialDrv->Config.TxLine, PAD_INPUT_ENABLE );
        RE_PadSet( serialDrv->Config.RxLine, PAD_INPUT_ENABLE );
        RE_PadSetMode( serialDrv->Config.TxLine, 0 );
        RE_PadSetMode( serialDrv->Config.RxLine, 0 );

        /* disable pull ups but enable pull downs so lines do not float */
        RE_PadClear( serialDrv->Config.TxLine, PAD_PULL_UP );
        RE_PadClear( serialDrv->Config.RxLine, PAD_PULL_UP );
        RE_PadSet( serialDrv->Config.TxLine, PAD_PULL_DOWN );
        RE_PadSet( serialDrv->Config.RxLine, PAD_PULL_DOWN );

        /* return the error code */
        return (error);

    }

    /* no driver instance found */
    return (-DRIVER_NO_INSTANCE);
}

int SerialDisable(Driver_t *driver, int normal)
{
    SerialDriver_t  *serialDrv;
    int             error;

    /* get handle to the driver instance */
    serialDrv = serial_get_instance(driver);

    if( serialDrv ) {

        if( serialDrv->Status.State == DRIVER_DISABLED) {
            /* driver already disabled */
            return (DRIVER_NO_ERROR);
        }

        /* disable interrupt line */
        RE_DisableInterrupt( PRIMARY_IRQ, serialDrv->Config.Irq );

        /* disable device controller */
        error = UartHwDisable( serialDrv->Config.Port, normal );

        if( error != DRIVER_NO_ERROR ) {
 
            /* enable back interrupt line */
            RE_EnableInterrupt( PRIMARY_IRQ, serialDrv->Config.Irq );

            /* device controller error encountered */
            return (error);
        }

        /* disable peripheral clock */
        //RE_ClockDisable( serialDrv->Config.Clock );
        UartClockDisable( serialDrv->Config.Clock );

        /* set all pads as gpio inputs */
        RE_PadSet( serialDrv->Config.TxLine, PAD_INPUT_ENABLE );
        RE_PadSet( serialDrv->Config.RxLine, PAD_INPUT_ENABLE );
        RE_PadSetMode( serialDrv->Config.TxLine, 0 );
        RE_PadSetMode( serialDrv->Config.RxLine, 0 );

        /* disable pull ups but enable pull downs so lines do not float */
        RE_PadClear( serialDrv->Config.TxLine, PAD_PULL_UP );
        RE_PadClear( serialDrv->Config.RxLine, PAD_PULL_UP );
        RE_PadSet( serialDrv->Config.TxLine, PAD_PULL_DOWN );
        RE_PadSet( serialDrv->Config.RxLine, PAD_PULL_DOWN );

        /* set driver state appropriately */
        serialDrv->Status.State = DRIVER_DISABLED;

        /* return successfully */
        return (DRIVER_NO_ERROR);
    }

    /* no driver instance found */
    return (-DRIVER_NO_INSTANCE);
}

int SerialControl(Driver_t *driver, int control, va_list args)
{
    SerialDriver_t *serialDrv;

    /* get handle to the driver instance */
    serialDrv = serial_get_instance(driver);

    if( serialDrv ) {

        /* handle control command/request */
        switch( control ) {

            case DRIVER_PORT_CONFIGURE:
                {
                    /* configure device controller port */
                    return UartHwConfigure( serialDrv->Config.Port,
                        serialDrv->Config.DataBits,
                        serialDrv->Config.StopBits,
                        serialDrv->Config.Parity,
                        serialDrv->Config.Baud );
                }

            case DRIVER_PORT_WRITE:
                {
                    char *buffer;
                    DWORD length;
                    DWORD count;
    
                    /* retrieve control arguments */
                    buffer = VA_ARG( args, char * );
                    length = VA_ARG( args, DWORD );
                 
                    /* lock the serial port write mutex */
                    RE_RimLockSemaphore( &serialDrv->TxMutex );

                    if( !serialDrv->Config.IsConsole ) {

                        /* writing raw serial data */ 

                        /* transfer data to the device controller port */
                        count = UartHwWrite( serialDrv->Config.Port,
                            buffer,
                            length);

                        /* update the number of bytes transmitted */
                        serialDrv->Statistics.TxCount += count;

                        /* unlock the serial port write mutex */
                        RE_RimUnlockSemaphore( &serialDrv->TxMutex );

                        /* return the number of bytes transmitted */
                        return (count);
                    } else {
                        /* process serial console data */
                        buffer[length] = '\0';
                        serial_puts( serialDrv, buffer );

                        /* unlock the serial port write mutex */
                        RE_RimUnlockSemaphore( &serialDrv->TxMutex );

                        return( 0 );
                    }
                }

            case DRIVER_PORT_READ:
                {
                    if( !serialDrv->Config.IsConsole ) {
                        /* reading raw serial data */ 
                        char *buffer;
                        DWORD length;
                        DWORD count;
    
                        /* retrieve control arguments */
                        buffer = VA_ARG( args, char * );
                        length = VA_ARG( args, DWORD );
                 
                        /* transfer data from the device controller port */
                        count = UartHwRead( serialDrv->Config.Port,
                            buffer,
                            length);
                    
                        /* update the number of bytes received */
                        serialDrv->Statistics.RxCount += count;

                        /* return the number of bytes received */
                        return( count );
                    } else {
                        /* processing console serial data */
                        serial_process_console( serialDrv );
                        /* do not return any read bytes */
                        return( 0 );
                    }
                }

            default:
                /* unknown driver specific command */
                break;
        }

        /* return successfully */
        return DRIVER_NO_ERROR;
    }

    /* no driver instance found */
    return (-DRIVER_NO_INSTANCE);
}

void SerialMsgHandler(RE_MESSAGE *msg)
{
    DWORD subMsg = msg->SubMsg;
    SerialDriver_t *serialDrv = (SerialDriver_t *) msg->DataPtr; 

    if( subMsg == 0x0 ) {
        if( !serialDrv->Config.IsConsole ) {
            /* reading raw serial data */ 
        } else {
            /* processing console serial data */
            serial_process_console( serialDrv );
        }
    }

    return;
}

RE_MESSAGE msg;
void SerialPollEvents(Driver_t *driver)
{
    SerialDriver_t  *serialDrv;
    UART_EVENT      events = 0;
    int             error;

    /* get handle to the driver instance */
    serialDrv = serial_get_instance(driver);

    if( serialDrv ) {

        /* poll device controller events */
        error = UartHwPollEvents( serialDrv->Config.Port, &events );

        if( error == DRIVER_NO_ERROR ) {

            /* handle break interrupt events */
            if( events & UART_EVENT_BREAK_INTERRUPT ) {
                serialDrv->Statistics.BiCount++;
            }

            /* handle framing error events */
            if( events & UART_EVENT_FRAMING_ERROR ) {
                serialDrv->Statistics.FeCount++;
            }

            /* handle parity error events */
            if( events & UART_EVENT_PARITY_ERROR ) {
                serialDrv->Statistics.PeCount++;
            }

            /* handle overrun error events */
            if( events & UART_EVENT_OVERRUN_ERROR ) {
                serialDrv->Statistics.OeCount++;
            }

            /* handle data available events */
            if( events & UART_EVENT_DATA_READY ) {
                msg.Event   = (DWORD) PLATFORM_MSG_SERIAL_DRV;
                msg.SubMsg  = (DWORD) 0x0;
                msg.DataPtr = (char *) serialDrv;
                RE_RimPostMessage( PLATFORM_THREAD_ID_DRV, &msg );
            }
        }
    }

    /* return regardless */
    return;
}

void SerialIrqHandler(void)
{
    SerialPollEvents( &(SerialDriver.Driver) );
}

Driver_t *SerialInitialize(void)
{
    SerialDriver_t *serialDrv;
    Driver_t        *driver = NULL;

    /* get a handle to the driver instance */
    serialDrv = &SerialDriver;

    /* set appropriately all driver status data */
    serialDrv->Status.State         = DRIVER_UNINIT;
         
    /* reset all driver statistical data */
    serialDrv->Statistics.BiCount   = 0;
    serialDrv->Statistics.FeCount   = 0;
    serialDrv->Statistics.PeCount   = 0;
    serialDrv->Statistics.OeCount   = 0;
    serialDrv->Statistics.TxCount   = 0;
    serialDrv->Statistics.RxCount   = 0;

    /* initialize driver configuration data */
    serialDrv->Config.Port          = SERIAL_PORT;
    serialDrv->Config.DataBits      = UART_DATABIT_8;
    serialDrv->Config.StopBits      = UART_STOPBIT_1;
    serialDrv->Config.Parity        = UART_PARITY_NONE;
    serialDrv->Config.Baud          = UART_BAUD_115P2K;

    /* initialize platform specific configuration data */
    serialDrv->Config.TxLine        = SERIAL_TX;
    serialDrv->Config.RxLine        = SERIAL_RX;
    serialDrv->Config.TxLineMode    = SERIAL_TX_FN; 
    serialDrv->Config.RxLineMode    = SERIAL_RX_FN;

    serialDrv->Config.Clock         = (RE_PeripheralClock) SERIAL_CLOCK;
    serialDrv->Config.Irq           = SERIAL_IRQ;
    serialDrv->Config.IrqHandler    = SerialIrqHandler;

    /* enable console functionalites */
    serialDrv->Config.IsConsole     = TRUE;

    /* initialize console-specific data */
    serialDrv->Console.Buffer[0]    = '\0';
    serialDrv->Console.BufferCount  = 0;

    /* initialize mutex of serial port writes */
    RE_RimInitSemaphore( &serialDrv->TxMutex );

    /* setup the driver interface functions */
    serialDrv->Driver.GetStatus     = SerialGetStatus;
    serialDrv->Driver.GetStats      = SerialGetStats;
    serialDrv->Driver.GetConfig     = SerialGetConfig;
    serialDrv->Driver.Enable        = SerialEnable;
    serialDrv->Driver.Disable       = SerialDisable;
    serialDrv->Driver.Control       = SerialControl;

    /* turn on the clock to initialize the device controller */
    //RE_ClockEnable( serialDrv->Config.Clock );
    (*((volatile DWORD *)(0xD4015064))) = ( 0x00000002 | 0x00000001 );
    UartClockEnable( serialDrv->Config.Clock );

    /* initialize controller device */
    if( UartHwInitialize(serialDrv->Config.Port) == DRIVER_NO_ERROR ) {

        /* register interrupt handler */
        RE_RegisterIntHandler( PRIMARY_IRQ,
            serialDrv->Config.Irq,
            serialDrv->Config.IrqHandler);

        /* expose driver interface instance for public use */
        driver = &(serialDrv->Driver); 

        /* set appropriately all driver status data */
        serialDrv->Status.State = DRIVER_DISABLED;
    }

    /* turn off back the clock */
    //RE_ClockDisable( serialDrv->Config.Clock );
    UartClockDisable( serialDrv->Config.Clock );

    /* register message handler */
    PfMsgRegister(PLATFORM_THREAD_ID_DRV, PLATFORM_MSG_SERIAL_DRV, SerialMsgHandler);

    return (driver);
}

