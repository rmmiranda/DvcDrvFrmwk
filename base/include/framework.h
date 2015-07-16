/*****************************************************************************
 * Filename:    framework.h
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 2008
 *
 * Description: Contains definitions of the platform software driver framework
 *              data structures, enumerated types and macros.
 *
 *****************************************************************************/

#ifndef __FRAMEWORK_H__
#define __FRAMEWORK_H__

#include "rei.h"
#include "framework_utilities.h"
#include "driver.h"

typedef enum {
    IODRV_ID_SERIAL,
    IODRV_ID_CAMERA,
    IODRV_ID_DISPLAY,
    IODRV_ID_I2C,
} IODRV_ID;

typedef enum {
    DBG_LVL_INFO = 0,
    DBG_LVL_WARN,
    DBG_LVL_ERROR,
    DBG_LVL_ALL,
} DBG_LVL;

typedef unsigned int DBG_FLG;

/**
 * References a generic console command function declaration.
 *
 * @param argc - The number of command arguments.
 * @param argv - The array of strings containg the command arguments.
 *
 * @return If successful, returns 0. Otherwise, returns negative value.
 *
 */
typedef int (*DrvCommandFunc)(int argc, char *argv[]);

/**
 * Prepares the entire driver framework for platform software use.
 *
 * @param void
 *
 * @return Positive integer if the call is successful. Otherwise, negative integer.
 *
 */
int DrvFrameworkInit(void);

/**
 * Requests a handle from the driver framework for a specific device driver.
 *
 * @param drvId - The corresponding numeric code of the requested device driver.
 *
 * @return A pointer to the driver interface data structure instance.
 *
 */
Driver_t *DrvGetHandle(IODRV_ID drvId);

/**
 * Returns the framework initialization status 
 *
 * @param void
 *
 * @return True - Framework initialization completed
 *         False - Framework initialization not completed yet
 */
BOOL DrvFrameworkInitialized(void);
 
/**
 * Retrieves a pointer to the driver's status information memory location.
 *
 * @param driver A pointer to the driver interface data structure instance.
 *
 * @return A void pointer to the driver's status information
 *
 */
void *DrvGetStatus(Driver_t *driver);

/**
 * Retrieves a pointer to the driver's statistical information memory location.
 *
 * @param driver A pointer to the driver interface data structure instance.
 *
 * @return A void pointer to the driver's statistical information.
 *
 */
void *DrvGetStats(Driver_t *driver);

/**
 * Retrieves a pointer to the driver's configuration data memory location.
 *
 * @param driver A pointer to the driver interface data structure instance.
 *
 * @return A void pointer to the driver's configuration data.
 *
 */
void *DrvGetConfig(Driver_t *driver);

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
int DrvEnable(Driver_t *driver, int flag);

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
int DrvDisable(Driver_t *driver, int normal);

/**
 * Provides an entry point to invoke driver-specific functions which are not, in any
 * way, can not be performed by any functions defined in the driver interface data
 * structure.
 *
 * @param driver A pointer to the driver interface data structure instance.
 * @param control The numeric code of the command/request the user wish to perform.
 * @param ... Variable parameter list for the driver specific command/request.
 *
 * @return Positive integer if the call is successful. Otherwise, a negative integer.
 *
 */
int DrvControl(Driver_t *driver, int control, ...);

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
int DrvResetStats(Driver_t *driver, DWORD length);

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
void DrvPrint(DBG_FLG flag, DBG_LVL level, const char *format, ...);
  
/**
 * Increments the internal debug level of the Driver Framework.
 *
 * @param void
 *
 * @return none
 */
void DrvIncDebugLevel(void);

/**
 * Decrements the internal debug level of the Driver Framework.
 *
 * @param void
 *
 * @return none
 */
void DrvDecDebugLevel(void);

#endif // __FRAMEWORK_H__
