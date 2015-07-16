/*****************************************************************************
 * Filename:    i2c_internal.h
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
#ifndef __I2C_INTERNAL_H__
#define __I2C_INTERNAL_H__

#include "rei.h"
#include "driver.h"
#include "i2c.h"

/* I2C pad config structure */
typedef struct {
	RE_Line Scl;
	RE_Line Sda;
	BYTE AltFnScl;
	BYTE AltFnSda;
} I2cPadConfig_t;

/* I2C controller driver structure */
typedef struct {
	BOOL Enabled;
	DWORD Base;         /*Base address register for I2C transactions */
	RE_PeripheralClock Clock;
    RE_Semaphore Sem;
	I2cPadConfig_t PadConfig;
} I2c_Controller_t;

/* I2C driver structure */
typedef struct {
    /* I2C driver public info   */
    Driver_t drvApi;
    /* I2C driver private data members */
} I2CDriver_t;

/*****************************************************************************
 * Function:    HSI2cInit
 * Description: I2C initialization routine
 * Parameters:
 * @param dev_id I2C Controller ID
 * Return:      TRUE if succeeds.
 *
 *****************************************************************************/
BOOL I2cInit(void);

void I2cShow(void);
#endif
