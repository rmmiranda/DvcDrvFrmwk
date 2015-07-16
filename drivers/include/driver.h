/*****************************************************************************
 * Filename:    driver.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     February 2009
 *
 * Description: Contains definition of the driver 
 *              interface data structure.
 *
 *****************************************************************************/

#ifndef __DRIVER_H__
#define __DRIVER_H__

typedef enum {
    DRIVER_CLOCK_ON,
    DRIVER_CLOCK_OFF,
    DRIVER_PORT_CONFIGURE,
    DRIVER_PORT_WRITE,
    DRIVER_PORT_READ,
    DRIVER_SPECIFIC_CONTROL,
} DRIVER_CONTROL;

typedef enum {
    DRIVER_UNINIT,
    DRIVER_DISABLED,
    DRIVER_ENABLED,
    DRIVER_BUSY,
    DRIVER_ERROR,
} DRIVER_STATE;

typedef enum {
    DRIVER_NO_ERROR = 0,
    DRIVER_INVALID_HW_PORT,
    DRIVER_NO_INSTANCE,
    DRIVER_UNINIT_HW_PORT,
    DRIVER_UNKNOWN_ERROR,
} DRIVER_ERROR_CODE;

typedef void * va_list;

#define VA_ALIGN(X)     ((sizeof(X)+sizeof(unsigned int)-1) & ~(sizeof(unsigned int)-1))
#define VA_START(ap,A)  (va_list)((ap) = (((char *)&(A)) + VA_ALIGN(A)))
#define VA_ARG(ap,T)    (*((T *)((char *)((ap)=(va_list)((char *)(ap)+VA_ALIGN(T))) - VA_ALIGN(T))))

typedef struct driver_t {

    /* GetStatus is a pointer to a function that retrieves a
     * pointer to the driver's status information memory lo-
     * cation. It is significant to note that this pointer
     * must be treated as a read-only pointer.
     *
     * Status information shall be encapsulated on a data 
     * structure as defined by the high-level (hardware in-
     * dependent) driver. Such information includes, but are
     * not limited to:
     *
     *      - driver state / mode
     *      - boolean flags
     *
     * The function shall accept one input parameter to deter-
     * mine which certain driver instance from which status
     * information will be retrieved.
     *
     * The function shall return a void pointer, which when
     * type-casted accordingly, reveals the high-level dri-
     * ver specific status information data structure.
     * 
     * The function shall be implemented specific to a high
     * level driver and the code shall reside in xxxDriver.c
     * file (eq: gpsDriver.c).
     */
    void * (*GetStatus)(struct driver_t *);

    /* GetStats is a pointer to a function that retrieves a
     * pointer to the driver's statistical information me-
     * mory location. It is significant to note that this
     * pointer must be treated as a read-only pointer.
     *
     * Statistical information shall be encapsulated on a 
     * data structure as defined by the high-level (hardware
     * independent) driver. Such information includes, but
     * are not limited to:
     *
     *      - counters (users, bytes transferred, errors,
     *        interrupts, etc)
     *
     * The function shall accept one input parameter to deter-
     * mine which certain driver instance from which statis-
     * tical information will be retrieved.
     *
     * The function shall return a void pointer, which when
     * type-casted accordingly, reveals the high-level dri-
     * ver specific statistical information data structure.
     * 
     * The function shall be implemented specific to a high
     * level driver and the code shall reside in xxxDriver.c
     * file (eq: gpsDriver.c).
     */
    void * (*GetStats)(struct driver_t *);

    /* GetConfig is a pointer to a function that retrieves
     * a pointer to the driver's configuration data memory
     * location. Usually, this function is called to per-
     * form modification of the configuration data.
     *
     * Configuration data shall be encapsulated on a data 
     * structure as defined by the high-level (hardware in-
     * dependent) driver. Such data includes, but are not
     * limited to:
     * 
     *      - peripheral clock
     *      - GPIO settings (line, drive strength, etc)
     *      - interrupt vector
     *      - LDO (power) group
     *      - DMA settings (channels, descriptors, etc)
     *      - NV record number
     *      - I2C address
     *
     * The function shall accept one input parameter to deter-
     * mine which certain driver instance from which configu-
     * ration data will be retrieved.
     *
     * The function shall return a void pointer, which when
     * type-casted accordingly, reveals the high-level dri-
     * ver specific configuration data structure.
     * 
     * The function shall be implemented specific to a high
     * level driver and the code shall reside in xxxDriver.c
     * file (eq: gpsDriver.c).
     */
    void *(*GetConfig)(struct driver_t *);

    /* Enable is a pointer to a function that performs ini-
     * tialization of the driver and activation of the un-
     * derlying hardware device and/or controller for Plat-
     * form Software use. A few examples of what the func-
     * tion shall do are:
     *
     *      - Configure pad settings and enable GPIO
     *      - Enable peripheral clock
     *      - Configure and start DMA
     *      - Turn on power for the LDO group
     *      - Start hardware device
     *      - Enable device interrupts
     *
     * The function shall accept one input parameter which is
     * a placeholder for future use.
     *
     * The function implementation shall reside in <xxx>Dri-
     * ver.c file (eq: gpsDriver.c). The function shall also
     * include low level calls to a device specific <xxx>De- 
     * vice.c file (eq: gpsDevice.c) specifically for device
     * register accesses, as well as calls to Nessus to reuse
     * existing Nessus control functions (for pad, peripheral
     * clock, system DMA, LDO, interrupts, etc).  
     */
    int (*Enable)(struct driver_t *, int);

    /* Disable is a pointer to a function that performs deac-
     * tivation of the driver and the underlying hardware de-
     * vice and/or controller. A few examples of what the func-
     * tion shall do are:
     *
     *      - Disable pad and GPIO
     *      - Disable peripheral clock
     *      - Stop DMA
     *      - Turn off power for the LDO group
     *      - Stop hardware device
     *      - Disable device interrupts
     *
     * The function shall accept one input parameter to deter-
     * mine if the deactivation process should proceed the nor-
     * mal way or should proceed the quick way.
     *
     * The function implementation shall reside in <xxx>Dri-
     * ver.c file (eq: gpsDriver.c). The function shall also
     * include low level calls to a device specific <xxx>De- 
     * vice.c file (eq: gpsDevice.c) specifically for device
     * register accesses, as well as calls to Nessus to reuse
     * existing Nessus control functions (for pad, peripheral
     * clock, system DMA, LDO, interrupts, etc).  
     */
    int (*Disable)(struct driver_t *, int);

    /* Control is a pointer to a function that provides an entry
     * point to invoke driver-specific functions which are not, in
     * any way, can not be performed by any of the functions de-
     * fined in this driver interface data structure. This basical-
     * ly mimics the functionality of IOCTL in the POSIX program-
     * ming model. 
     *
     * The function shall accept an input parameter to determine
     * specifically the request/command ID the user wish to perform.
     * A variable number of in-out parameter is also passed as argu-
     * ments for the driver specific command/request.
     *
     * The function implementation shall reside in <xxx>Driver.c
     * file (eq: gpsDriver.c). The function shall also include low
     * level calls to a device specific <xxx>Device.c file (eq:
     * gpsDevice.c) specifically for device register accesses, as
     * well as calls to Nessus to reuse existing Nessus control
     * functions (for pad, peripheral clock, system DMA, LDO, inter-
     * rupts, etc).  
     */
    int (*Control)(struct driver_t *, int, va_list args);

    /* PollEvents is a pointer to a function that performs
     * polling and handling of device driver software and
     * hardware events.
     *
     * The function shall be impelemented specific to a high
     * level driver and the code shall reside in xxxDriver.c
     * file (eq: gpsDriver.c). The function shall also include
     * low level calls to a device specific <xxx>Device.c file
     * (eq: gpsDevice.c) specifically for device register ac-
     * cesses.
     */
    void (*PollEvents)(struct driver_t *);

} Driver_t;


#endif // __DRIVER_H__
