/*****************************************************************************
 * Filename:    i2c_drvfwk.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     May 14th, 2010
 *
 * Description: This file implements the driver framework interface
 *              to the I2C driver.
 *
 *****************************************************************************/
 /* standard library header files */
#include <string.h>

/* driver header files */
#include "rei.h"
#include "framework.h"
#include "driver.h"
#include "i2c_internal.h"
 
/* ---------------------------------------------------------------------------
 * Defines and data variables
 * --------------------------------------------------------------------------- */
/* Debug Defines */
#define SRCGROUP GROUP_PLT_DRV
#define SRCFILE  FILE_I2C_FWK

I2CDriver_t I2cDrv;

/* ---------------------------------------------------------------------------
 * Function forward declaration
 * --------------------------------------------------------------------------- */
static void DrvFwkI2cDebug(char * cmd, DWORD arg1, DWORD arg2);

/* ---------------------------------------------------------------------------
 * Function implementation
 * --------------------------------------------------------------------------- */
/*****************************************************************************
 * Function:    DrvI2cInit 
 * Description: Drv Framework API - driver initialization
 * Parameters:  void
 * Return:      Pointer to variable I2cDrv.
 *
 *****************************************************************************/
Driver_t * DrvFwkI2cInit(void)
{    
    PRINT("Initializing I2C driver...");

    /* Register Bugdisp hook */
	PfCmdRegister("i2c", "I2C Driver", &DrvFwkI2cDebug);    
    
    /* setup the driver interface functions */
    /* Note: the driver framework APIs for I2C haven't been implemented yet; 
       Apps can directly use the APIs defines in driver/include/i2c.h instead. */
    I2cDrv.drvApi.GetStatus     = 0;
    I2cDrv.drvApi.GetStats      = 0;
    I2cDrv.drvApi.GetConfig     = 0;
    I2cDrv.drvApi.Enable        = 0;
    I2cDrv.drvApi.Disable       = 0;
    I2cDrv.drvApi.Control       = 0;
    
   	I2cInit();
    
    return (Driver_t *) &I2cDrv;
}

/*****************************************************************************
 * Function:    DrvFwkI2cDebug 
 * Description: Driver local debugging and testing commands
 * Parameters:  ...
 * Return:      void
 *
 *****************************************************************************/
void DrvFwkI2cDebug(char * cmd, DWORD arg1, DWORD arg2)
{
    static I2C_ADDR addr;
    static I2C_BUS bus;
    
    if(strcmp(cmd, "addr") == 0)
    {
        addr = (I2C_ADDR) arg1;
        PRINT2N("I2C driver: set I2C address to %d (0x%x)", addr, addr);
    }
    else if(strcmp(cmd, "bus") == 0)
    {
        bus = (I2C_BUS) arg1;
        PRINTN("I2C driver: set I2C bus to %d", bus);
    }
    else if(strcmp(cmd, "show") == 0)
    {
        I2cShow();
    }
    if(strcmp(cmd, "en") == 0)
    {
        BOOL ret;
        
        ret = I2cEnable(bus);
        PRINT2N("I2C driver: enable I2C controller - %d, return code %d", bus, ret);
    }    
    else if(strcmp(cmd, "rd") == 0)
    {
        BYTE val;
        int ret;
        
        ret = I2cRead(bus, addr, &val, sizeof(val));
        PRINT3N("I2C driver read byte: value %d(0x%x), return code %d", val, val, ret);
    }
    else if(strcmp(cmd, "wr") == 0)
    {
        BYTE val;
        int ret;
        
        ret = I2cWrite(bus, addr, &val, sizeof(val));
        PRINT3N("I2C driver write byte: value %d(0x%x), return code %d", val, val, ret);        
    }
    else if(strcmp(cmd, "reg_rd") == 0)
    {
        BYTE reg = arg1;
        BYTE val;
        int ret;

        ret = I2cWriteRead(bus, addr, &reg, sizeof(reg), addr, &val, sizeof(val));
        PRINT4N("I2C driver register 0x%02x read value %d(0x%x), return code %d", reg, val, val, ret);
    }
    else if(strcmp(cmd, "reg_wr") == 0)
    {
        BYTE reg = arg1;
        BYTE val = arg2;
        int ret;

        ret = I2cWriteWrite(bus, addr, &reg, sizeof(reg), &val, sizeof(val));
        PRINT4N("I2C driver register 0x%02x write value %d(0x%x), return code %d", reg, val, val, ret);
    }
    else if(strcmp(cmd, "help") == 0)    
    {
        PRINT("----------------------------------------------------------------------");
        PRINT("I2C Driver help");
        PRINT("  x pf.i2c.addr X    : Set I2C address");
        PRINT("  x pf.i2c.bus X     : Set I2C bus");
        PRINT("  x pf.i2c.show      : Show current I2C test settings");
        PRINT("  x pf.i2c.rd        : I2C byte read");
        PRINT("  x pf.i2c.wr        : I2C byte write");
        PRINT("  x pf.i2c.reg_rd    : I2C register read");
        PRINT("  x pf.i2c.reg_wr    : I2C register write");
        PRINT("  x pf.i2c.en        : Enable I2C controller");
    }
}
