/*****************************************************************************
 * Filename:    i2c.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Mikhail Zakharov
 *
 * Created:     March 2010
 *
 * Description: Implementation of the I2C driver
 *
 *****************************************************************************/
#include "rei.h"
#include "framework.h"
#include "gpio_platform.h"
#include "i2c_internal.h"
#include "i2c_reg.h"

/* Debug Defines */
#define SRCGROUP GROUP_PLT_DRV
#define SRCFILE  FILE_I2C

#define I2C_GENERIC_ERROR 	-1

/* ---------------------------------------------------------------------------
 * Data structure types
 * --------------------------------------------------------------------------- */
typedef enum {
	I2C_OP_WRITE,
	I2C_OP_READ,
	I2C_OP_START
} I2cOperation;

typedef struct {
	I2cOperation op;
	I2C_ADDR address;
	BYTE *buffer;
	WORD len;
} I2cDescriptor;

/* ---------------------------------------------------------------------------
 * Constants and static data variables
 * --------------------------------------------------------------------------- */
#define I2C_TIMEOUT_MS  100

I2c_Controller_t i2cCtrl[I2C_DEV_MAX];

/* ---------------------------------------------------------------------------
 * Functions
 * --------------------------------------------------------------------------- */
/*****************************************************************************
 * Function:    I2cEnable
 * Description: Configures Pads, initialization registers
 * Parameters:
 * Return:
 *****************************************************************************/
BOOL I2cEnable(I2C_BUS bus)
{
    I2c_Controller_t * I2c_dev;

	if (bus < 0 || bus >= I2C_DEV_MAX)
	{
		WARN("Unknown I2C Controller ID");
		return FALSE;
	}
    
    I2c_dev = &(i2cCtrl[bus]);
    
	RE_RimLockSemaphore(&I2c_dev->Sem);
	if (!I2c_dev->Enabled)
	{
		const RE_PadConfiguration I2cPadConfig = {
				0, // altFunctionMode
				PAD_DIRECTION_INPUT, // initialDirection
				FALSE, // initialLevel
				TRUE, // pullUpEnable
				FALSE, // pullDownEnable
				PAD_INTERRUPT_NONE, // interruptType
				NULL, // interruptHandler
				FALSE, // interruptsInitiallyEnabled
				FALSE // interruptDebounceEnabled
				};
        
		// Configure the SCL pad
		RE_PadConfigure(I2c_dev->PadConfig.Scl, &I2cPadConfig, NULL );
		RE_PadSetMode(I2c_dev->PadConfig.Scl, I2c_dev->PadConfig.AltFnScl);

		// Configure the SDA pad
		RE_PadConfigure(I2c_dev->PadConfig.Sda, &I2cPadConfig, NULL );
		RE_PadSetMode(I2c_dev->PadConfig.Sda, I2c_dev->PadConfig.AltFnSda);

		// Enable the I2C clock
		RE_ClockEnable(I2c_dev->Clock);

		// Reset the I2C module
		WriteRegDword(TWSI_CR(I2c_dev->Base), TWSI_CR_UR );
		WriteRegDword(TWSI_SR(I2c_dev->Base), TWSI_SR_CLEAR );

		// Enable the I2C module
		WriteRegDword(TWSI_CR(I2c_dev->Base), TWSI_CR_ENABLE);
        WriteRegDword(TWSI_LCR(I2c_dev->Base), ILCR_DEFAULT);

		I2c_dev->Enabled = TRUE;
	}
    
	RE_RimUnlockSemaphore(&I2c_dev->Sem);
	PRINTN ("I2C Device %d initialized", bus);
	return TRUE;
}

/**
 * Performs a START or REPEATED_START transaction on the I2C bus
 *
 * @param address 7-bit device address on the I2C bus.
 * @param rw A bit indicating whether this is a write (0) or read (1) transaction
 *
 * @return 1 on success, 0 on any failure
 */
static BOOL I2cDoStart(DWORD Base_Adrr, I2C_ADDR address, BOOL rw, BOOL useInterrupts)
{
	BOOL result = 1;
	int counter;

	WriteRegDword( TWSI_DBR(Base_Adrr), address | rw );

	WriteRegDwordOr( TWSI_CR(Base_Adrr), TWSI_CR_START );
	WriteRegDwordAnd( TWSI_CR(Base_Adrr), ~TWSI_CR_STOP );

	// use polling only for now
	{
		WriteRegDwordOr( TWSI_CR(Base_Adrr), TWSI_CR_TB );
		for (counter = 0; (ReadRegDword( TWSI_CR(Base_Adrr) ) & TWSI_CR_TB) != 0; counter++)
		{
			RE_HwDelay(RE_MICROSECOND);
			if (counter > I2C_TIMEOUT_MS * 1000)
			{
				WARN("i2cDoStart Timed Out");
				result = 0;
				break;
			}
		}
	}

	// clear i2c transmitter empty flag
	WriteRegDwordOr( TWSI_SR(Base_Adrr), TWSI_SR_ITE );

	return (result);
}

/**
 * Performs a READ operation on the I2C bus
 * Must be preceded by an i2cDoStart function.
 *
 * @param buf_ptr pointer to a buffer into which to insert the read data
 * @param len number of bytes to read from the device
 * @param stop boolean indicating whether or not to end the read transaction with a STOP
 *
 * @return 1 on success, 0 on any failure
 */
static BOOL I2cDoRead(DWORD Base_Adrr, BYTE *buf_ptr, WORD len, BOOL stop, BOOL useInterrupts)
{
	BOOL result = 1;
	DWORD i;
	int counter;

	for (i = 0; i < (len); i++)
	{
		WriteRegDwordAnd( TWSI_CR(Base_Adrr), ~TWSI_CR_START );
		if (i == (len) - 1)
		{

			WriteRegDwordOr( TWSI_CR(Base_Adrr), TWSI_CR_ACKNACK );
			if (stop)
			{

				WriteRegDwordOr( TWSI_CR(Base_Adrr), TWSI_CR_STOP );
			}
			else
			{
				WriteRegDwordAnd( TWSI_CR(Base_Adrr), ~TWSI_CR_STOP );
			}
		}
		else
		{
			WriteRegDwordAnd( TWSI_CR(Base_Adrr), ~( TWSI_CR_STOP | TWSI_CR_ACKNACK ) );
		}

		// use polling only for now
		{
			WriteRegDwordOr( TWSI_CR(Base_Adrr), TWSI_CR_TB );
			for (counter = 0; (ReadRegDword( TWSI_CR(Base_Adrr) ) & TWSI_CR_TB) != 0; counter++)
			{
				RE_HwDelay(RE_MICROSECOND);
				if (counter > I2C_TIMEOUT_MS * 1000)
				{
					WARN("i2cDoRead Timed Out");
					result = 0;
					break;
				}
			}
		}

		// grab/store the byte we just finished reading
		((BYTE *) buf_ptr)[i] = ReadRegByte( TWSI_DBR(Base_Adrr) );

		if (result == 0)
			break;
	}
	return (result);
}

/**
 * Performs a WRITE operation on the I2C bus
 * Must be preceded by an i2cDoStart function.
 *
 * @param buf_ptr pointer to a buffer from which to obtain the write data
 * @param len number of bytes to write to the device
 * @param stop boolean indicating whether or not to end the write transaction with a STOP
 *
 * @return 1 on success, 0 on any failure
 */
static BOOL I2cDoWrite(DWORD Base_Adrr, BYTE *buf_ptr, WORD len, BOOL stop, BOOL useInterrupts)
{
	BOOL result = 1;
	DWORD i;
	int counter;

	for (i = 0; i < (len); i++)
	{
		WriteRegDword( TWSI_DBR(Base_Adrr), ( (BYTE *)buf_ptr )[i] );

		WriteRegDwordAnd( TWSI_CR(Base_Adrr), ~TWSI_CR_START );
		if ((stop) && i == (len) - 1)
		{
			WriteRegDwordOr( TWSI_CR(Base_Adrr), TWSI_CR_STOP );
		}
		else
		{
			WriteRegDwordAnd( TWSI_CR(Base_Adrr), ~TWSI_CR_STOP );
		}

		// if we arn't in reset or catfail configure interrupt
		{
			WriteRegDwordOr( TWSI_CR(Base_Adrr), TWSI_CR_TB );
			for (counter = 0; (ReadRegDword( TWSI_CR(Base_Adrr) ) & TWSI_CR_TB) != 0; counter++)
			{
				RE_HwDelay(RE_MICROSECOND);
				if (counter > I2C_TIMEOUT_MS * 1000)
				{
					WARN("i2cDoWrite Timed Out");
					result = 0;
					break;
				}
			}
		}

		if (result == 0)
			break;
	}
	return (result);
}

/**
 * Performs custom I2C operations
 *
 * @param *descriptors Pointer to descriptor chain
 * @param size Number of descriptors in chain
 *
 * @return 0 on success, I2C_GENERIC_ERROR on any failure
 *
 */
static int I2cRunDescriptors(I2C_BUS bus_id, I2cDescriptor *descriptors, WORD size)
{
	int result = 0;
	int i;
	BOOL useInterrupts = FALSE;
	DWORD Base_Addr;
    I2c_Controller_t * I2c_dev = &(i2cCtrl[bus_id]);
    
	if (size == 0)
	{
		return 0;
	}

	if (bus_id < 0 || bus_id >= I2C_DEV_MAX)
	{
		WARN("Unknown I2C Controller ID");
		return I2C_GENERIC_ERROR;
	}

	if (!I2c_dev->Enabled)
	{
		I2cEnable(bus_id);
	}
    
	Base_Addr = I2c_dev->Base;
	RE_RimLockSemaphore(&I2c_dev->Sem);

	for (i = 0; i < size; i++)
	{
		I2cDescriptor *desc = &descriptors[i];
		I2cDescriptor *next = &descriptors[i + 1];

		switch (desc->op)
		{
		case I2C_OP_START:
			if (!I2cDoStart(Base_Addr, desc->address, next->op, useInterrupts))
			{
				WARNN( "Bus timeout/invalid state error while addressing device %d", desc->address );
				result = I2C_GENERIC_ERROR;
				break;
			}

			// Check for a bus error (ie no ACK received)
			if (!useInterrupts && ReadRegDword( TWSI_SR(Base_Addr) ) & TWSI_SR_BED)
			{
				WARNN( "Bus error while addressing device %d", desc->address );
				WriteRegDword( TWSI_SR(Base_Addr), TWSI_SR_BED );
				result = I2C_GENERIC_ERROR;
				break;
			}
			break;
		case I2C_OP_WRITE:
			if (!I2cDoWrite(Base_Addr, desc->buffer, desc->len, i == size - 1, useInterrupts))
			{
				WARNN( "Bus timeout error while writing to device %d", desc->address );
				result = I2C_GENERIC_ERROR;
				break;
			}

			// Check for a bus error (ie no ACK received)
			if (!useInterrupts && ReadRegDword( TWSI_SR(Base_Addr) ) & TWSI_SR_BED)
			{
				WARNN( "Bus error while writing to device %d", desc->address );
				WriteRegDword( TWSI_SR(Base_Addr), TWSI_SR_BED );
				result = I2C_GENERIC_ERROR;
				break;
			}
			break;
		case I2C_OP_READ:
			if (!I2cDoRead(Base_Addr, desc->buffer, desc->len, i == size - 1, useInterrupts))
			{
				WARNN( "Bus timeout error while reading from device %d", desc->address );
				result = I2C_GENERIC_ERROR;
				break;
			}
			break;
		default:
			WARN( "Unknown i2c operation" );
		}
		if (result == I2C_GENERIC_ERROR)
		{
			break;
		}
	}

	//wait for the stop condition to complete
	i = 0;
	while ((ReadRegDword( TWSI_SR(Base_Addr) ) & TWSI_SR_UB) != 0)
	{
		HwDelay(RE_MICROSECOND);
		i++;
		if (i > (I2C_TIMEOUT_MS * 1000))
		{
			WARN( "i2c wait for stop timed out" );
			result = I2C_GENERIC_ERROR;
			break;
		}
	}
	RE_RimUnlockSemaphore(&I2c_dev->Sem);

	return result;
}
/*****************************************************************************
 * Function:    I2cRead
 * Description: Receives data on the I2C bus
 * Parameters:
 * @param address 7-bit device address on bus
 * @param *buffer Pointer to receive buffer
 * @param buffersize Number of bytes to receive
 * Return: 0 on success, I2C_GENERIC_ERROR on any failure
 *****************************************************************************/
int I2cRead(I2C_BUS bus_id, I2C_ADDR address, BYTE *buffer, WORD length)
{
	I2cDescriptor descriptors[2];
	if (bus_id < 0 || bus_id >= I2C_DEV_MAX)
	{
		WARN("Unknown I2C Controller ID");
		return I2C_GENERIC_ERROR;
	}
    
	// clear possibly erroneous RW bit
	address = address & I2C_ADDRESS_MASK;

	descriptors[0].op = I2C_OP_START;
	descriptors[0].address = address;
	descriptors[0].buffer = NULL;
	descriptors[0].len = 0;
	descriptors[1].op = I2C_OP_READ;
	descriptors[1].address = address;
	descriptors[1].buffer = buffer;
	descriptors[1].len = length;

	return I2cRunDescriptors(bus_id, descriptors, 2);
}

/*****************************************************************************
 * Function:    I2cWrite
 * Description: Sends data on the I2C bus
 * Parameters:
 * @param address 7-bit device address on bus
 * @param *buffer Pointer to send buffer
 * @param buffersize Number of bytes to send
 * Return: 0 on success, I2C_GENERIC_ERROR on any failure
 *****************************************************************************/
int I2cWrite(I2C_BUS bus_id, I2C_ADDR address, BYTE *buffer, WORD length)
{
	I2cDescriptor descriptors[2];
	if (bus_id < 0 || bus_id >= I2C_DEV_MAX)
	{
		WARN("Unknown I2C Controller ID");
		return I2C_GENERIC_ERROR;
	}
    
	// clear possibly erroneous RW bit
	address = address & I2C_ADDRESS_MASK;

	descriptors[0].op = I2C_OP_START;
	descriptors[0].address = address;
	descriptors[0].buffer = NULL;
	descriptors[0].len = 0;
	descriptors[1].op = I2C_OP_WRITE;
	descriptors[1].address = address;
	descriptors[1].buffer = buffer;
	descriptors[1].len = length;

	return I2cRunDescriptors(bus_id, descriptors, 2);
}
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
int I2cWriteRead(I2C_BUS bus_id, I2C_ADDR wAddress, BYTE *wBuffer, WORD wSize, I2C_ADDR rAddress, BYTE *rBuffer, WORD rSize)
{
	I2cDescriptor descriptors[4];
	if (bus_id < 0 || bus_id >= I2C_DEV_MAX)
	{
		WARN("Unknown I2C Controller ID");
		return I2C_GENERIC_ERROR;
	}
    
	// clear possibly erroneous RW bit
	wAddress = wAddress & I2C_ADDRESS_MASK;
	rAddress = rAddress & I2C_ADDRESS_MASK;

	descriptors[0].op = I2C_OP_START;
	descriptors[0].address = wAddress;
	descriptors[0].buffer = NULL;
	descriptors[0].len = 0;
	descriptors[1].op = I2C_OP_WRITE;
	descriptors[1].address = wAddress;
	descriptors[1].buffer = wBuffer;
	descriptors[1].len = wSize;
	descriptors[2].op = I2C_OP_START;
	descriptors[2].address = rAddress;
	descriptors[2].buffer = NULL;
	descriptors[2].len = 0;
	descriptors[3].op = I2C_OP_READ;
	descriptors[3].address = rAddress;
	descriptors[3].buffer = rBuffer;
	descriptors[3].len = rSize;

	return I2cRunDescriptors(bus_id, descriptors, 4);
}

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
int I2cWriteWrite(I2C_BUS bus_id, I2C_ADDR address, BYTE *wBuffer1, WORD wSize1, BYTE *wBuffer2, WORD wSize2 )
{
	I2cDescriptor descriptors[3];
	if (bus_id < 0 || bus_id >= I2C_DEV_MAX)
	{
		WARN("Unknown I2C Controller ID");
		return I2C_GENERIC_ERROR;
	}
        
    // clear possibly erroneous RW bit
    address = address & I2C_ADDRESS_MASK;

    descriptors[0].op = I2C_OP_START;
	descriptors[0].address = address;
    descriptors[0].buffer = NULL;
    descriptors[0].len = 0;
    descriptors[1].op = I2C_OP_WRITE;
	descriptors[1].address = address;
    descriptors[1].buffer = wBuffer1;
    descriptors[1].len = wSize1;
    descriptors[2].op = I2C_OP_WRITE;
	descriptors[2].address = address;
    descriptors[2].buffer = wBuffer2;
    descriptors[2].len = wSize2;

	return I2cRunDescriptors(bus_id, descriptors, 3);
}

/*****************************************************************************
 * Function:    I2cInit
 * Description: I2C initialization routine
 * Parameters:  void
 * Return:      TRUE if succeeds.
 *
 *****************************************************************************/
BOOL I2cInit(void)
{
    I2c_Controller_t * dev;
    I2C_BUS bus_id;
    I2cPadConfig_t I2cPadConfig[] = {
    	I2C_PAD_CONFIG,
    };
    
	for(bus_id = 0; bus_id < I2C_DEV_MAX; bus_id++)
	{
		dev = &(i2cCtrl[bus_id]);
    
		if (dev->Enabled)
		{
			WARNN("I2C controller %d already initialized", bus_id);
			continue;
		}

		dev->Enabled = FALSE;
		dev->Base = TWSI_I2C_BASE(bus_id);
		dev->Clock = TWSI_I2C_CLOCK(bus_id);
		RE_RimInitSemaphore(&(dev->Sem));
		dev->PadConfig.Scl = I2cPadConfig[bus_id].Scl;
		dev->PadConfig.Sda = I2cPadConfig[bus_id].Sda;
		dev->PadConfig.AltFnScl = I2cPadConfig[bus_id].AltFnScl;
		dev->PadConfig.AltFnSda = I2cPadConfig[bus_id].AltFnSda;

		PRINTN("I2C controller %d initialized", bus_id);
	}
    
	return TRUE;
}

/*****************************************************************************
 * Function:    I2cShow
 * Description: Dump I2C register values
 * Parameters:  void
 * Return:      void
 *
 *****************************************************************************/
void I2cShow(void)
{
	DWORD reg;
	I2C_BUS	 bus;
	I2c_Controller_t * dev;

	for(bus = 0; bus < I2C_DEV_MAX; bus++)
	{
		dev = &(i2cCtrl[bus]);

		PRINTN("I2C Controller %d:", bus);

		reg = TWSI_DBR(dev->Base);
		PRINT2N("   DBR[0x%08x]              =   0x%08x", reg, ReadRegDword(reg));

		reg = TWSI_CR(dev->Base);
		PRINT2N("   CR [0x%08x]              =   0x%08x", reg, ReadRegDword(reg));

		reg = TWSI_SR(dev->Base);
		PRINT2N("   SR [0x%08x]              =   0x%08x", reg, ReadRegDword(reg));

		reg = TWSI_SAR(dev->Base);
		PRINT2N("   SAR [0x%08x]             =   0x%08x", reg, ReadRegDword(reg));

		reg = TWSI_LCR(dev->Base);
		PRINT2N("   LCR[0x%08x]              =   0x%08x", reg, ReadRegDword(reg));
	}
}

