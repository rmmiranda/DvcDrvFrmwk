/*****************************************************************************
 * Filename:    camif_tavorpv2.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Mar 9th, 2009
 *
 * Description: This file implements the HW specific code for camera driver on PV2.
 *
 *****************************************************************************/
/* include header files */
/* include header files */
#include "camif_internal.h"
#include "camif_i2c.h"

#define HS2I2C_TIMEOUT_MS  100
#define SLAVE_ADDR  0x3
#define I2C_ICR_HS_ENABLE (  I2C_ICR_FM | I2C_ICR_IUE | I2C_ICR_SCLEA )

#define SRCGROUP GROUP_PLT_DRV
#define SRCFILE  FILE_CAM_I2C

#define HS2I2C_SCL                     GPIO_79
#define HS2I2C_SDA                     GPIO_80

#define PV2_ISP_I2C_ADDR               0x20

static Semaphore HsI2cSemaphore;

static const PadConfiguration Hs2I2cPadConfig = {
    2,                          // altFunctionMode
    PAD_DIRECTION_INPUT,        // initialDirection
    FALSE,                      // initialLevel
    FALSE,                      // pullUpEnable
    FALSE,                      // pullDownEnable
    PAD_INTERRUPT_NONE,         // interruptType
    NULL,                       // interruptHandler
    FALSE,                      // interruptsInitiallyEnabled
    FALSE                       // interruptDebounceEnabled
};


/**
 * Initialize the I2C peripheral
 *
 */
void Hs2I2cInit(void)
{
    // initially configure I2C_SDA on the bus
    PadConfigure( HS2I2C_SDA, &Hs2I2cPadConfig, NULL );
    PadConfigure( HS2I2C_SCL, &Hs2I2cPadConfig, NULL );

    RimInitSemaphore( &HsI2cSemaphore );
    // init semaphore to 0 to block on first lock
    //RimInitCountingSemaphore( &HsI2cCommSemaphore, 0, 1 );

    /* Enable I2C clock */
    //WriteRegDword(HS2I2C_ILCR_REG32, HS2I2C_ILCR_VALUE);
    ClockEnable( CLOCK_HSI2C );

    // Configure the I2C module
    WriteRegDword( HS2I2C_ISAR_REG32, SLAVE_ADDR );    // slave addr
    WriteRegDword( HS2I2C_ICR_REG32,  I2C_ICR_HS_ENABLE );

    ClockDisable( CLOCK_HSI2C );
}

/**
 * Performs a START or REPEATED_START transaction on the I2C bus
 *
 * @param address 7-bit device address on the I2C bus.
 * @param rw A bit indicating whether this is a write (0) or read (1) transaction
 *
 * @return 1 on success, 0 on any failure
 */
static BOOL HsI2cDoStart( i2cAddress address, BOOL rw, BOOL useInterrupts )
{
    BOOL result = 1;
    int counter;

    WriteRegDword( HS2I2C_IDBR_REG32, address | rw );

    WriteRegDwordOr( HS2I2C_ICR_REG32, I2C_ICR_START );
    WriteRegDwordAnd( HS2I2C_ICR_REG32, ~I2C_ICR_STOP );

    // use polling only for now
    {
        WriteRegDwordOr( HS2I2C_ICR_REG32, I2C_ICR_TB );
        for( counter = 0 ; ( ReadRegDword( HS2I2C_ICR_REG32 ) & I2C_ICR_TB ) != 0 ; counter++ ){
            HwDelay(RE_MICROSECOND);
            if( counter > HS2I2C_TIMEOUT_MS*1000 ){
                WARN("i2cDoStart Timed Out");
                result = 0;
                break;
            }
        }
    }

    return( result );
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
static BOOL HsI2cDoRead( BYTE *buf_ptr, WORD len, BOOL stop, BOOL useInterrupts )
{
    BOOL result = 1;
    DWORD i;
    int counter;

    for ( i = 0; i < ( len ); i++ ) {
        WriteRegDwordAnd( HS2I2C_ICR_REG32, ~I2C_ICR_START );
        if ( i == ( len ) - 1 ) {

            WriteRegDwordOr( HS2I2C_ICR_REG32, I2C_ICR_ACKNACK );
            if ( stop ) {

                WriteRegDwordOr( HS2I2C_ICR_REG32, I2C_ICR_STOP );
            } else {
                WriteRegDwordAnd( HS2I2C_ICR_REG32, ~I2C_ICR_STOP );
            }
        } else {
            WriteRegDwordAnd( HS2I2C_ICR_REG32, ~( I2C_ICR_STOP | I2C_ICR_ACKNACK ) );
        }

        // use polling only for now
        {
            WriteRegDwordOr( HS2I2C_ICR_REG32, I2C_ICR_TB );
            for( counter = 0 ; ( ReadRegDword( HS2I2C_ICR_REG32 ) & I2C_ICR_TB ) != 0 ; counter++ ){
                HwDelay(RE_MICROSECOND);
                if( counter > HS2I2C_TIMEOUT_MS*1000 ){
                    WARN("i2cDoRead Timed Out");
                    result = 0;
                    break;
                }
            }
        }

        // grab/store the byte we just finished reading
        ( (BYTE *)buf_ptr )[i] = ReadRegByte( HS2I2C_IDBR_REG32 );

        if( result == 0 )
            break;
    }
    return( result );
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
static BOOL HsI2cDoWrite( BYTE *buf_ptr, WORD len, BOOL stop, BOOL useInterrupts )
{
    BOOL result = 1;
    DWORD i;
    int counter;

    for ( i = 0; i < ( len ); i++ ) {
        WriteRegDword( HS2I2C_IDBR_REG32, ( (BYTE *)buf_ptr )[i] );

        WriteRegDwordAnd( HS2I2C_ICR_REG32, ~I2C_ICR_START );
        if ( ( stop ) && i == ( len ) - 1 ) {
            WriteRegDwordOr( HS2I2C_ICR_REG32, I2C_ICR_STOP );
        } else {
            WriteRegDwordAnd( HS2I2C_ICR_REG32, ~I2C_ICR_STOP );
        }

        // if we arn't in reset or catfail configure interrupt
        {
            WriteRegDwordOr( HS2I2C_ICR_REG32, I2C_ICR_TB );
            for( counter = 0 ; ( ReadRegDword( HS2I2C_ICR_REG32 ) & I2C_ICR_TB ) != 0 ; counter++ ){
                HwDelay(RE_MICROSECOND);
                if( counter > HS2I2C_TIMEOUT_MS*1000 ){
                    WARN("i2cDoWrite Timed Out");
                    result = 0;
                    break;
                }
            }
        }

        if( result == 0 )
            break;
    }
    return( result );
}

/**
 * Performs custom I2C operations
 *
 * @param address 7-bit device address on bus
 * @param *descriptors Pointer to descriptor chain
 * @param size Number of descriptors in chain
 *
 * @return 0 on success, -1 on any failure
 *
 */
static int HsI2cRunDescriptors( i2cAddress address, I2cDescriptor *descriptors, WORD size )
{
    int result = 0;
    int i;
    BOOL useInterrupts = FALSE;

    if ( size == 0 ) {
        return 0;
    }

    if ( address == ( SLAVE_ADDR << 1 ) ) {
        WARN( "Cannot access our own I2C address" );
        return -1;
    }
    
    //RimLockSemaphore( &HsI2cSemaphore );

    ClockEnable( CLOCK_HSI2C );

    for( i=0; i<size; i++ ) {
        I2cDescriptor *desc = &descriptors[i];
        I2cDescriptor *next = &descriptors[i+1];

        switch( desc->op ) {
            case I2C_OP_START:
                if( !HsI2cDoStart( address, next->op, useInterrupts ) ) {
                    WARNN( "Bus timeout/invalid state error while addressing device %(i2cAddressTag)", address );
                    result = -1;
                    break;
                }

                // Check for a bus error (ie no ACK received)
                if( !useInterrupts && ReadRegDword( HS2I2C_ISR_REG32 ) & I2C_ISR_BED ) {
                    WARNN( "Bus error while addressing device %(i2cAddressTag)", address );
                    WriteRegDword( HS2I2C_ISR_REG32, I2C_ISR_BED );
                    result = -1;
                    break;
                }
                break;
            case I2C_OP_WRITE:
                if( !HsI2cDoWrite( desc->buffer, desc->len, i == size-1, useInterrupts ) ) {
                    WARNN( "Bus timeout error while writing to device %(i2cAddressTag)", address );
                    result = -1;
                    break;
                }

                // Check for a bus error (ie no ACK received)
                if( !useInterrupts && ReadRegDword( HS2I2C_ISR_REG32 ) & I2C_ISR_BED ) {
                    WARNN( "Bus error while writing to device %(i2cAddressTag)", address );
                    WriteRegDword( HS2I2C_ISR_REG32, I2C_ISR_BED );
                    result = -1;
                    break;
                }
                break;
            case I2C_OP_READ:
                if( !HsI2cDoRead( desc->buffer, desc->len, i == size-1, useInterrupts ) ) {
                    WARNN( "Bus timeout error while reading from device %(i2cAddressTag)", address );
                    result = -1;
                    break;
                }
                break;
            default:
                WARN( "Unknown i2c operation" );
        }
        if( result == -1 ) {
            break;
        }
    }

    // before turning off the clock, wait for the stop condition to complete
    i = 0;
    while( ( ReadRegDword( HS2I2C_ISR_REG32 ) & I2C_ISR_UB ) != 0 ) {
        HwDelay( RE_MICROSECOND );
        i++;
        if( i > ( HS2I2C_TIMEOUT_MS * 1000 ) ){
            WARN( "i2c wait for stop timed out" );
            result = -1;
            break;
        }
    }

    ClockDisable( CLOCK_HSI2C );
    //RimUnlockSemaphore( &HsI2cSemaphore );

    return result;
}

/**
 * Receives data on the I2C bus
 *
 * @param address 7-bit device address on bus
 * @param *buffer Pointer to receive buffer
 * @param buffersize Number of bytes to receive
 *
 * @return 0 on success, -1 on any failure
 *
 */
int HsI2cRead( i2cAddress address, BYTE *buffer, WORD length )
{
    I2cDescriptor descriptors[2];

    // clear possibly erroneous RW bit
    address = address & I2C_ADDRESS_MASK;

    descriptors[0].op     = I2C_OP_START;
    descriptors[0].buffer = NULL;
    descriptors[0].len    = 0;
    descriptors[1].op     = I2C_OP_READ;
    descriptors[1].buffer = buffer;
    descriptors[1].len    = length;

    return HsI2cRunDescriptors( address, descriptors, 2 );
}

/**
 * Sends data on the I2C bus
 *
 * @param address 7-bit device address on bus
 * @param *buffer Pointer to send buffer
 * @param buffersize Number of bytes to send
 *
 * @return 0 on success, -1 on any failure
 *
 */
int HsI2cWrite( i2cAddress address, BYTE *buffer, WORD length )
{
    I2cDescriptor descriptors[2];

    // clear possibly erroneous RW bit
    address = address & I2C_ADDRESS_MASK;

    descriptors[0].op     = I2C_OP_START;
    descriptors[0].buffer = NULL;
    descriptors[0].len    = 0;
    descriptors[1].op     = I2C_OP_WRITE;
    descriptors[1].buffer = buffer;
    descriptors[1].len    = length;

    return HsI2cRunDescriptors( address, descriptors, 2 );
}

int HsI2cWriteRead( i2cAddress address, BYTE *wBuffer, WORD wSize, BYTE *rBuffer, WORD rSize )
{
    I2cDescriptor descriptors[4];

    // clear possibly erroneous RW bit
    address = address & I2C_ADDRESS_MASK;

    descriptors[0].op     = I2C_OP_START;
    descriptors[0].buffer = NULL;
    descriptors[0].len    = 0;
    descriptors[1].op     = I2C_OP_WRITE;
    descriptors[1].buffer = wBuffer;
    descriptors[1].len    = wSize;
    descriptors[2].op     = I2C_OP_START;
    descriptors[2].buffer = NULL;
    descriptors[2].len    = 0;
    descriptors[3].op     = I2C_OP_READ;
    descriptors[3].buffer = rBuffer;
    descriptors[3].len    = rSize;

    return HsI2cRunDescriptors( address, descriptors, 4 );
}

int HsI2cWriteWrite( i2cAddress address, BYTE *wBuffer1, WORD wSize1, BYTE *wBuffer2, WORD wSize2 )
{
    I2cDescriptor descriptors[3];

    // clear possibly erroneous RW bit
    address = address & I2C_ADDRESS_MASK;

    descriptors[0].op     = I2C_OP_START;
    descriptors[0].buffer = NULL;
    descriptors[0].len    = 0;
    descriptors[1].op     = I2C_OP_WRITE;
    descriptors[1].buffer = wBuffer1;
    descriptors[1].len    = wSize1;
    descriptors[2].op     = I2C_OP_WRITE;
    descriptors[2].buffer = wBuffer2;
    descriptors[2].len    = wSize2;

    return HsI2cRunDescriptors( address, descriptors, 3 );
}

/***************************************************************************
 * Function:    CamSensorRegRead(BYTE reg)
 * Description: Returns the value read from a specified camera register
 * Params:      reg - the register to read out
 * Return Val:  the value from the register
 **************************************************************************/
BYTE CamSensorReadReg(WORD reg)
{
    BYTE result = 0;
    BYTE buffer[2];
    WORD size;
    int width = 16;

    if ( width == 16 ) {
        buffer[0] = reg >> 8;
        buffer[1] = reg & 0x00ff;
        size = 2;
    } else {
        buffer[0] = reg;
        size = 1;
    }

    if( HsI2cWriteRead( PV2_ISP_I2C_ADDR, buffer, size, &result, 1 ) != -1 ) {
        //PRINT2N( "I2c Reg (RD): %04X = %02X", reg, result );
        return result;
    } else {
        WARN2N( "I2c Reg (RD-FAILED): %04X = %02X", reg, result );
        return -1;
    }    
}

/***************************************************************************
 * Function:    CamSensorWriteReg(WORD reg, BYTE value)
 * Description: Writes a byte to a register on the camera over I2C
 * Params:      reg - the register to write to
 *              value - the new value of the register
 **************************************************************************/
int CamSensorWriteReg(WORD reg, BYTE value)
{
    BYTE buffer[3];
    WORD size;
    int width = 16;

    if ( width == 16 ) {
        buffer[0] = reg >> 8;
        buffer[1] = reg & 0x00ff;
        buffer[2] = value;
        size = 3;
    }
    else {
        buffer[0] = reg;
        buffer[1] = value;
        size = 2;
    }

    if( HsI2cWrite( PV2_ISP_I2C_ADDR, buffer, size ) != -1 ) {
        //PRINT2N( "I2c Reg (WR): %04X = %02X", reg, value );
        return 0;        
    } else {
        WARN2N( "I2c Reg (WR-FAILED): %04X = %02X", reg, value );
        return -1;
    }
}

/***************************************************************************
 * Function:    CamSensorWaitStatus(BYTE reg)
 * Description: Returns the value read from a specified camera register
 * Params:      reg - the register to read out
 * Return Val:  the value from the register
 **************************************************************************/
 // we give up after 6 (10ms x 600) seconds of polling
 #define CAM_SENSOR2_POLL_MAX_RETRY     600
 #define CAM_SENSOR2_POLL_REG_DELAY     1000 /* micro-seconds*/
BOOL CamSensorWaitStatus( WORD reg, BYTE value ) {
    int count;
    const int MaxRetries = CAM_SENSOR2_POLL_MAX_RETRY; // make this part of the function call

    HwDelay(CAM_SENSOR2_POLL_REG_DELAY * RE_MICROSECOND); // Must give Sensor time after a write before reading status
    PRINT2N ("Waiting for reg %08X to equal %08X", reg, value);

    for ( count = 0; count < MaxRetries; count ++ ) {
        int val = CamSensorReadReg(reg);
        if ( val == -1 ) {
            WARN ("No response from ISP.");
        } else if( ( val != value ) &&  (count == (MaxRetries-1)) ) {
            WARNN ("Incorrect response from ISP: %d", val);
        } else if ( val == value ) {
            break;
        }
        //HwDelay (CAM_SENSOR2_POLL_REG_DELAY * RE_MICROSECOND);
        RimSleep(10);
    }

    if ( count >= MaxRetries ) {
        WARNN("Chip failed to come back alive! Reg %08X", reg);
    }

    return count < MaxRetries;
}

/***************************************************************************
 * Function:    CamSensorWriteAuto( DWORD args, ... )
 * Description: Function to read the auto increment bytes and send them over I2C
 * Params:      args - The 1st argument is the number of bytes to write
 *              ...  - The 2nd argument is the register address
 *              ...  - The next arguments are the bytes to write
 **************************************************************************/
void CamSensorWriteAuto( DWORD args, ... ) {
    WORD reg, index, size;
    BYTE buffer[100];
    int i;
    int result;
    va_list current;
    int width = 16;

    VA_START(current,args); 
  
    reg = VA_ARG(current,int);
  
    if ( width == 16 ) {
        buffer[0] = reg >> 8;
        buffer[1] = reg & 0x00ff;
        index = 2;
    }
    else {
        buffer[0] = reg;
        index = 1;
    }

    size = index + args;
    for ( i = index ; i < size ; i++ ) {
        buffer[i]=VA_ARG(current,int);
    }
    result = HsI2cWrite( PV2_ISP_I2C_ADDR, buffer, size );
    if( result != -1 ) {
        //PRINT4N( "I2c Reg (WR): %04X = %02X,...,%02X (%d bytes)", reg, buffer[index], buffer[size-1], args );
    } else {
        WARN4N( "I2c Reg (WR-FAILED): %04X = %02X,...,%02X (%d bytes)", reg, buffer[index], buffer[size-1], args );
    }
}

