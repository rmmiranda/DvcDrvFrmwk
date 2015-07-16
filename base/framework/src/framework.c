/*****************************************************************************
 * Filename:    framework.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 2008
 *
 * Description: Contains the platform software driver framework core code.
 *
 *****************************************************************************/

#include <string.h>

#include "rei.h" 
#include "framework.h"
#include "driver.h"

#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE  FILE_ID_FRAMEWORK

#define MAX_IODRV_NAME_LEN    15

/*
 ***************************************************
 ***************************************************
 *
 * Data structure definitions
 *
 ***************************************************
 ***************************************************
 */

typedef struct iodriver_t {

    /* Name refers to the name of the IO driver instance.
     */
    char Name[MAX_IODRV_NAME_LEN];

    /* Id refers to the numeric code of the IO driver.
     */           
    IODRV_ID Id;

    /* Driver is a pointer to the driver interface data structure.
     */
    Driver_t *Driver; 

    /* Initialize is a pointer to a function that performs
     * device driver initialization as well as the under-
     * lying hardware device.
     *
     * The function shall accept one output parameter, for
     * which the function is called successfully, shall point
     * to a valid driver interface data structure instance.
     *
     * The function shall be impelemented specific to a high
     * level driver and the code shall reside in xxxDriver.c
     * file (eq: gpsDriver.c).
     */    
    Driver_t *(*Initialize)(void);

} IODriver_t;

/*
 ***************************************************
 ***************************************************
 *
 * Static global variable declarations
 *
 ***************************************************
 ***************************************************
 */

#if defined( DRV_SERIAL ) && defined( DRV_UART )
extern Driver_t *SerialInitialize(void);
#endif
#if defined( DRV_CAMERA )
extern Driver_t * DrvFwkCamInit(void);
#endif
#if defined( DRV_DISPLAY )
//void DispInitialize(Driver_t *driver);
extern Driver_t * DrvFwkDispInit(void);
#endif
#if defined( DRV_I2C )
extern Driver_t * DrvFwkI2cInit(void);
#endif

static IODriver_t IODriverTable[] = {
#if defined( DRV_CAMERA )
	{"Camera",   IODRV_ID_CAMERA,  0,  &DrvFwkCamInit },
#endif
#if defined( DRV_DISPLAY )
   	{ "Display",  IODRV_ID_DISPLAY,   0, &DrvFwkDispInit},
#endif
#if defined( DRV_I2C )
   	{ "I2C",  IODRV_ID_I2C,   0, &DrvFwkI2cInit },
#endif
   	{ "NULL", (IODRV_ID)0, 0, 0}
};

#define IODRIVER_TABLE_SIZE     (sizeof(IODriverTable)/sizeof(IODriver_t) - 1) 

/* Driver framework boolean initialization flag */
static BOOL FrameworkInit = FALSE;

/* Debug module driver for the driver framework */
static Driver_t *DebugDrv = NULL;

/* Debug level to control debug driver prints */
static DBG_LVL DebugLevel = DBG_LVL_ALL;

/* Driver framework build date */
static const char FrameworkBuildDate[] = __DATE__;

/* Driver framework build time */
static const char FrameworkBuildTime[] = __TIME__;

/* Driver framework builder */
static const char FrameworkBuildUser[] = BUILD_USER;

/*
 ***************************************************
 ***************************************************
 *
 * Function prototype declarations
 *
 ***************************************************
 ***************************************************
 */

void DrvFrameworkDebug(char *cmd, DWORD arg1, DWORD arg2);

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

/*
 ***************************************************
 ***************************************************
 *
 * Exportable function definitions
 *
 ***************************************************
 ***************************************************
 */

/**
 * Prepares the entire driver framework for platform software use.
 *
 * @param void.
 *
 * @return Positive integer if the call is successful. Otherwise, negative integer.
 *
 */
int DrvFrameworkInit(void)
{
    IODriver_t  *iodriver;
    int        index = 0;

    /* initialize serial debugger */
#if defined( DRV_SERIAL ) && defined( DRV_UART )
    DebugDrv = SerialInitialize();
#endif
    if( DebugDrv ) {
        /* serial driver exists, enable it */
        if( DrvEnable( DebugDrv, 0 ) != DRIVER_NO_ERROR ) {
            /* cannot enable serial driver */
            DebugDrv = NULL;
        }
    }

    /* initialize debug level to show all messages */
    DebugLevel = DBG_LVL_ALL;

    /*******************************************/
    /*******************************************/
    /*** Serial printfs must begin from here ***/
    /*******************************************/
    /*******************************************/

    /* register Bugdisp hook */
    PfCmdRegister("fwk", "Driver Framework", &DrvFrameworkDebug);
    
    if( IODRIVER_TABLE_SIZE == 0 ) {
	    PRINT( "No Device Drivers To Load" );
        return (1);
    }

    while( index < IODRIVER_TABLE_SIZE ) {
        iodriver = &IODriverTable[index];
        iodriver->Driver = iodriver->Initialize();
        if( !(iodriver->Driver) ) {
            PRINTCOPY( "%s Driver Initialization Failed", iodriver->Name );
            index++;
            continue;
        }
        PRINTCOPY( "%s Driver Initialized", iodriver->Name );
        index++;
    }

    /* set the framework init flag */
    FrameworkInit = TRUE;

    return (1);
}

/**
 * Returns the framework initialization status 
 *
 * @param void
 *
 * @return True - Framework initialization completed
 *         False - Framework initialization not completed yet
 */
BOOL DrvFrameworkInitialized(void)
{
    return FrameworkInit;
}

/**
 * Requests a handle from the driver framework for a specific device driver.
 *
 * @param drvId The corresponding numeric code of the requested device driver.
 *
 * @return A pointer to the driver interface data structure instance.
 *
 * Note: Consider revising algorithm: hashing; mapping
 *
 */
Driver_t *DrvGetHandle(IODRV_ID drvId)
{
    IODriver_t  *iodriver;
    int        index = 0;

    if( !FrameworkInit ) {
        /* wrong driver framework state */
        return NULL;
	}	

    while( index < IODRIVER_TABLE_SIZE ) {
        iodriver = &IODriverTable[index];
        if( iodriver->Id == drvId ) {
            /* found the driver */
            return iodriver->Driver;
        }
        index++;
    }

    /* if we got here, the driver has not been found */
    return NULL;
}

/**
 * Retrieves a pointer to the driver's status information memory location.
 *
 * @param driver A pointer to the driver interface data structure instance.
 *
 * @return A void pointer to the driver's status information
 *
 */
void *DrvGetStatus(Driver_t *driver)
{
    if( driver && driver->GetStatus ) {

        /* call high-level specific driver
         * GetStatus interface.
         */
        return (void *)(driver->GetStatus(driver));

    }

    /* if we got here, either the driver interface
     * can not be located or the device driver has
     * not implemented the GetStatus interface.
     */
    return NULL;
}

/**
 * Retrieves a pointer to the driver's statistical information memory location.
 *
 * @param driver A pointer to the driver interface data structure instance.
 *
 * @return A void pointer to the driver's statistical information.
 *
 */
void *DrvGetStats(Driver_t *driver)
{
    if( driver && driver->GetStats ) {

        /* call high-level specific driver
         * GetStats interface.
         */
        return (void *)(driver->GetStats(driver));

    }

    /* if we got here, either the driver interface
     * can not be located or the device driver has
     * not implemented the GetStats interface.
     */
    return NULL;
}

/**
 * Retrieves a pointer to the driver's configuration data memory location.
 *
 * @param driver A pointer to the driver interface data structure instance.
 *
 * @return A void pointer to the driver's configuration data.
 *
 */
void *DrvGetConfig(Driver_t *driver)
{
    if( driver && driver->GetConfig ) {

        /* call high-level specific driver
         * GetConfig interface.
         */
        return (void *)(driver->GetConfig(driver));

    }

    /* if we got here, either the driver interface
     * can not be located or the device driver has
     * not implemented the GetConfig interface.
     */
    return NULL;
}

/**
 * Performs initialization of the driver and activation of the underlying hardware
 * device and/or controller for Platform Software use.
 *
 * @param driver A pointer to the driver interface data structure instance.
 * @param flag A placeholder for future use..
 *
 * @return Positive integer if the call is successful. Otherwise, negative integer.
 *
 */
int DrvEnable(Driver_t *driver, int flag)
{
    if( driver && driver->Enable ) {

        /* call high-level specific driver
         * Enable interface.
         */
        return driver->Enable(driver, flag);

    }

    /* if we got here, either the driver interface
     * can not be located or the device driver has
     * not implemented the Enable interface.
     */
    return (-1);
}

/**
 * Performs deactivation of the driver and the underlying hardware device and/or 
 * controller.
 *
 * @param driver A pointer to the driver interface data structure instance.
 * @param normal An integer flag referring to either a normal disable or quick disable.
 *
 * @return Positive integer if the call is successful. Otherwise, negative integer.
 *
 */
int DrvDisable(Driver_t *driver, int normal)
{
    if( driver && driver->Disable ) {

        /* call high-level specific driver
         * Disable interface.
         */
        return driver->Disable(driver, normal);

    }

    /* if we got here, either the driver interface
     * can not be located or the device driver has
     * not implemented the Disable interface.
     */
    return (-1);
}

/**
 * Provides an entry point to invoke driver-specific functions which are not, in any
 * way, can not be performed by any functions defined in the driver interface
 * data structure.
 *
 * @param driver A pointer to the driver interface data structure instance.
 * @param control The numeric code of the command/request the user wish to perform.
 * @param ... Variable parameter list for the driver specific command/request.
 *
 * @return Positive integer if the call is successful. Otherwise, a negative integer.
 *
 */
int DrvControl(Driver_t *driver, int control, ...)
{
    va_list args;

    if( driver && driver->Control ) {

        /* initialize variable list.
         */
        VA_START( args, control );

        /* call high-level specific driver
         * Control interface.
         */
        return driver->Control(driver, control, args);

    }

    /* if we got here, either the driver interface
     * can not be located or the device driver has
     * not implemented the Control interface.
     */
    return (-1);
}

/**
 * Resets back to zero all the contents of the statistical data structure associated
 * with the device driver.
 *
 * @param driver A pointer to the driver interface data structure instance.
 * @param length The length in bytes of the entire statistics data structure.
 *
 * @return Positive integer if the call is successful. Otherwise, a negative integer.
 *
 */
int DrvResetStats(Driver_t *driver, DWORD length)
{
    void *stats;

    /* get a handle to the statistics
     * data structure
     */
    stats = DrvGetStats( driver );

    if( stats ) {

        /* reinitialize structure to zero */
        memset( stats, 0x00, length ); 

        /* return successfully */
        return (1);

    }

    /* if we got here, either the driver interface
     * can not be located or the device driver has
     * not implemented the GetStats interface.
     */
    return (-1);
}

/**
 * Prints out a formatted string of characters to the console. The printing of the
 * string depends on the current Driver Framework debug level. 
 *
 * @param flag   - The bitmasked representation of a control flag.
 * @param level  - The debug level by which the string is printed.
 * @param format - The formatted string to be printed.
 * @param ...    - The variable list of items to be printed.
 *
 * @return none
 */
int vsprintf(char *, const char *, va_list); 
void DrvPrint(DBG_FLG flag, DBG_LVL level, const char *format, ...)
{
    va_list args;
    char    buffer[256];
    DWORD   length;

    /* make sure the serial debugger exists.
     */
    if( DebugDrv ) {

        /* process messages based on debug level */
        if( (DebugLevel == level) || (DebugLevel == DBG_LVL_ALL) ) {

            /* initialize variable list.
            */
            VA_START(args, format);  

            /* format the print buffer.
             */
            length = vsprintf( buffer, format, args );

            /* do a console write.
            */
            DrvControl( DebugDrv, DRIVER_PORT_WRITE, buffer, length );
        }
    }

    return; 
}

/**
 * Increments the internal debug level of the Driver Framework.
 *
 * @param void
 *
 * @return none
 */
void DrvIncDebugLevel(void)
{
    if( DebugLevel < DBG_LVL_ALL ) {
        /* increment debug level */
        DebugLevel++;
    }
}

/**
 * Decrements the internal debug level of the Driver Framework.
 *
 * @param void
 *
 * @return none
 */
void DrvDecDebugLevel(void)
{
    if( DebugLevel > DBG_LVL_INFO ) {
        /* decrement debug level */
        DebugLevel--;
    }
}

/**
 * DrvFrameworkDebug
 *
 * Provides BugDisp debugging capabilitilies.
 *
 * @param cmd  - The BugDisp command specific to this module.
 * @param arg1 - The first parameter to the BugDisp command.
 * @param arg2 - The second parameter to the BugDisp command.
 *
 * @return None.
 *
 */
int sprintf(char *, const char *, ...); 
void DrvFrameworkDebug(char *cmd, DWORD arg1, DWORD arg2)
{
    const RE_MetricsDataStruct *metrics = &MetricsData;
    IODriver_t                 *iodriver;
    int                        index = 0;
    char                       buffer[256];

    if( !strcmp(cmd, "info") ) {

        PRINT( "**********************************" );
        PRINT( "*** Driver Framework Subsystem ***" );
        PRINT( "**********************************" );
        PRINT( " " );
        PRINT( "Core Information:" );
        PRINTCOPY( "   Build Date:   %s", FrameworkBuildDate ); 
        PRINTCOPY( "   Build Time:   %s", FrameworkBuildTime ); 
        PRINTCOPY( "   Build User:   %s", FrameworkBuildUser ); 
        PRINT( " " );
        PRINT( "Device Driver Information:" );
        while( index < IODRIVER_TABLE_SIZE ) {
            iodriver = &IODriverTable[index];
            sprintf(buffer, "   %s [%02d]", iodriver->Name, iodriver->Id );
            PRINTCOPY( "%s", buffer );
            index++;
        }
        PRINT( " " );
        PRINT( "Nessus Library Information" );
        PRINTN( "   OS Version:        0x%08x", metrics->OSVersion ); 
        PRINTN( "   Hardware ID:       0x%08x", metrics->HardwareID ); 
        PRINTN( "   Metrics Version:   0x%08x", metrics->Version ); 
        PRINTN( "   FS Code Version:   0x%08x", metrics->FileSystemVersion ); 
        PRINTN( "   FAT Code Version:  0x%08x", metrics->FatFSVersion ); 
        PRINTCOPY( "   Package Version:   %s", metrics->PackageVersionString ); 
        PRINTCOPY( "   Build Date:        %s", metrics->BuildDate ); 
        PRINTCOPY( "   Build Time:        %s", metrics->BuildTime ); 
        PRINTCOPY( "   Build User:        %s", metrics->BuildUserName ); 
        PRINTCOPY( "   Device String:     %s", metrics->DeviceString ); 
        PRINT( " " );
        PRINT( "**********************************" );
        PRINT( "**********************************" );
        PRINT( "**********************************" );

    }

    return;
}
