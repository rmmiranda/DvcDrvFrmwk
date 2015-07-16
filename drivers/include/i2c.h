/*****************************************************************************
 * Filename:    i2c.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Mikhail Zakharov
 *
 * Created:     March 16, 2010
 *
 * Description: This header file defines the Framework I2C public interface
 *
 *****************************************************************************/
#ifndef __I2C_H__
#define __I2C_H__

#include "rei.h"

#define I2C_GENERIC_ERROR 	-1

#define I2C_DEV(x)	((x)-1)

/* BaseBand I2C/TWSI controller list */
typedef int I2C_BUS;

/* type for I2C address specification */
typedef unsigned short I2C_ADDR;

/*****************************************************************************
 * Function:    I2cRead
 * Description: Receives data on the I2C bus
 * Parameters:
 * @param dev_id I2C Controller ID
 * @param address 7-bit device address on bus
 * @param *buffer Pointer to receive buffer
 * @param buffersize Number of bytes to receive
 * Return: 0 on success, -1 on any failure
 *****************************************************************************/
int I2cRead(I2C_BUS bus_id, I2C_ADDR address, BYTE *buffer, WORD length);

/*****************************************************************************
 * Function:    I2cWriteRead
 * Description: Writes and receives data on the I2C bus
 * Parameters:
 * @param wAddress 7-bit device address for writing to the bus
 * @param *wBuffer Pointer to write buffer
 * @param wSize wBuffer size
 * @param rAddress 7-bit device address for reading from the bus
 * @param *rBuffer Pointer to read buffer
 * @param rSize rBuffer size
 * Return: 0 on success, I2C_GENERIC_ERROR on any failure
 *****************************************************************************/
int I2cWriteRead(I2C_BUS bus_id, I2C_ADDR wAddress, BYTE *wBuffer, WORD wSize, I2C_ADDR rAddress, BYTE *rBuffer, WORD rSize);


/*****************************************************************************
 * Function:    I2cWrite
 * Description: Sends data on the I2C bus
 * Parameters:
 * @param dev_id I2C Controller ID
 * @param address 7-bit device address on bus
 * @param *buffer Pointer to send buffer
 * @param buffersize Number of bytes to send
 * Return: 0 on success, -1 on any failure
 *****************************************************************************/
int I2cWrite(I2C_BUS bus_id, I2C_ADDR address, BYTE *buffer, WORD length);

/*****************************************************************************
 * Function:    I2cWriteWrite
 * Description: Writes and receives data on the I2C bus
 * Parameters:
 * @param address 7-bit device address on bus
 * @param *wBuffer Pointer to write buffer
 * @param wSize wBuffer size
 * @param *rBuffer Pointer to read buffer
 * @param rSize rBuffer size
 * Return: 0 on success, I2C_GENERIC_ERROR on any failure
 *****************************************************************************/
int I2cWriteWrite(I2C_BUS bus_id, I2C_ADDR address, BYTE *wBuffer1, WORD wSize1, BYTE *wBuffer2, WORD wSize2 );


/*****************************************************************************
 * Function:    I2cEnable
 * Description: Enabel I2C controller
 * Parameters:
 * @param 
 *****************************************************************************/
BOOL I2cEnable(I2C_BUS bus);

#endif
