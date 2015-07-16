/*****************************************************************************
 * Filename:    rei_minios_gpio.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      
 *
 * Created:     Feb, 2010
 *
 * Description: GPIO functions and internal data structures. This file should
 *              be kept generic between processors and platforms. 
 *
 *              Portions of this file were taken from Nessus tavorpv2_gpio_def.h, 
 *              tavor_gpio.c
 *
 ****************************************************************************/
 
#include "rei.h"
#include "framework.h"
#include "rei_critical.h"
#include "rei_minios_gpio.h" 
#include "gpio_processor.h"

#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE  FILE_REI_GPIO
 
RE_PadLatchState RE_GpioLatchStates[ NUMBER_OF_GPIOS ] = INITIAL_LATCH_STATES;
static RE_PadInterruptType RE_GpioInterruptStates[ NUMBER_OF_GPIOS ] = INITIAL_INTERRUPT_STATES;
RE_Line RE_GPIO_REVERSE_MAPPINGS[ NUMBER_OF_GPIOS ] = GPIO_REVERSE_MAPPING;


/* Start Predeclarations --------------------------------------------------------- */
void RE_PadConfigure( RE_Line line, const RE_PadConfiguration *config, const RE_DeviceSpecificPadConfiguration *devSpecificConfig );
void RE_PadSet( RE_Line line, RE_PadAttribute attribute );
void RE_PadClear( RE_Line line, RE_PadAttribute attribute );
BOOL RE_PadRead( RE_Line line, RE_PadAttribute attribute );
void RE_PadSetMode( RE_Line line, DWORD mode );
void RE_PadInterruptConfigure( RE_Line line, RE_PadInterruptType interruptType );
void RE_PadRegisterIntHandler( RE_Line line, IRQHandler *function );
void RE_PadInterruptEnable( RE_Line line );
void RE_PadInterruptDisable( RE_Line line );
void RE_PadInterruptClear( RE_Line line );
static void RE_DeviceSpecificPadConfigure( RE_Line line, const RE_DeviceSpecificPadConfiguration *devSpecificConfig );
static void RE_SetInterruptState( RE_Line line, RE_PadInterruptType interruptType );
RE_PadInterruptType RE_GetInterruptState( RE_Line line );
static void RE_PadConfigureDrive (RE_Line line, RE_PadDriveStrengthSlewRate drive);
DWORD RE_MFPR_REG32( RE_Line line );
#if defined ( PROCESSOR_FAMILY_MMP )
static void RE_RegisterGpioIntHandler(DWORD line_, IRQHandler * handler_);
#endif


/* End Predeclarations --------------------------------------------------------- */


// See Nessus tavor_gpio.c for details
 
void RE_PadConfigure( RE_Line line, const RE_PadConfiguration *config,
                   const RE_DeviceSpecificPadConfiguration *devSpecificConfig )
{
    static const RE_DeviceSpecificPadConfiguration ForceRDHSpecificConfig = {
        DEFAULT_STRENGTH_SLEW,      // drive
        SLEEP_SEL_DEFAULT,          // sleepSel
        SLEEP_DATA_DEFAULT,         // sleepData
        SLEEP_DIRECTION_DEFAULT,    // sleepDirection
        PULL_SEL_DEFAULT,           // pullSel
        USE_RDH_DEFAULT,            // useRdh
        EDGE_DETECT_DEFAULT,        // edgeDetectEnable
        FALSE,                      // detectRisingEdge
        FALSE                       // detectRisingEdge
    };
    if( IS_PAD( line ) ) {
        if( config != NULL ) {
            if( config->pullUpEnable ) {
                RE_PadClear( line, PAD_PULL_DOWN );
                RE_PadSet( line, PAD_PULL_UP );
            } else if( config->pullDownEnable ) {
                RE_PadClear( line, PAD_PULL_UP );
                RE_PadSet( line, PAD_PULL_DOWN );
            } else {
                RE_PadClear( line, PAD_PULL_UP );
                RE_PadClear( line, PAD_PULL_DOWN );
            }
            if( config->initialDirection == PAD_DIRECTION_OUTPUT ) {
                ASSERT("Can't configure line as both output and GPIRQ!",
                                    config->interruptType == PAD_INTERRUPT_NONE );
                if( config->initialLevel ) {
                    RE_PadSet( line, PAD_OUTPUT );
                } else {
                    RE_PadClear( line, PAD_OUTPUT );
                }
                RE_PadSet( line, PAD_OUTPUT_ENABLE );
                RE_PadSetMode( line, config->altFunctionMode );
            } else {
                if( config->interruptType == PAD_INTERRUPT_NONE ) {
                    RE_PadSet( line, PAD_INPUT_ENABLE );
                    RE_PadSetMode( line, config->altFunctionMode );
                    RE_PadInterruptConfigure( line, PAD_INTERRUPT_NONE );
                } else {
                    RE_PadInterruptDisable( line );
                    RE_PadSet( line, PAD_INPUT_ENABLE );
                    RE_PadSetMode( line, config->altFunctionMode );
                    RE_PadInterruptConfigure( line, config->interruptType );
                    // Configure interrupt debounce
                    if( config->interruptDebounceEnabled ) {
                        WARN( "Interrupt Debounce Not Available" );
                    }
                    if( config->interruptHandler != NULL ) {
                        RE_PadRegisterIntHandler( line, config->interruptHandler );
                    }
                    RE_PadInterruptClear( line );
                    if( config->interruptsInitiallyEnabled ) {
                        RE_PadInterruptEnable( line );
                    }
                }
            }
        }
        if( devSpecificConfig == NULL ) {
            // Force the setting of the RDH bit
            devSpecificConfig = &ForceRDHSpecificConfig;
        }
        RE_DeviceSpecificPadConfigure( line, devSpecificConfig );
    } else {
        WARNN("Invalid PadConfigure line %d", line);
    }
}



void RE_PadSet( RE_Line line, RE_PadAttribute attribute )
{
    // If we are configuring GPIO properties (not just pad properties),
    // we must specify a valid GPIO and a valid GPIO source
    if( ( !IS_GPIO(line) ) &&
        (PAD_OUTPUT_ENABLE == attribute ||
        PAD_INPUT_ENABLE  == attribute ||
        PAD_OUTPUT        == attribute) ) {
            return;
    }

    if( IS_PAD( line ) ) {
        
        DWORD idx = GPIO_NUM( line );
        DWORD mask = ( (DWORD)1 << (idx & 0x1F) );
        DWORD mfpr = RE_MFPR_REG32( line );
        RE_RimEnterCriticalSection();

        switch( attribute ) {
            case PAD_OUTPUT_ENABLE:
                WriteRegDword( GSDR_REG32( idx ), mask );
                WriteRegDwordAnd( mfpr, ~MFPR_SLEEP_INPUT_EN );
                // dummy read required to complete MFPR operation
                ReadDummyRegDword( mfpr );
                break;
            case PAD_INPUT_ENABLE:
                WriteRegDword( GCDR_REG32( idx ), mask );
                WriteRegDwordOr( mfpr, MFPR_SLEEP_INPUT_EN );
                // dummy read required to complete MFPR operation
                ReadDummyRegDword( mfpr );
                break;
            case PAD_OUTPUT:
                WriteRegDword( GPSR_REG32( idx ), mask );
                WriteRegDwordOr( mfpr, MFPR_SLEEP_DATA );
                // dummy read required to complete MFPR operation
                ReadDummyRegDword( mfpr );
                break;
            case PAD_OUTPUT_SLEEP_OFF:
                WriteRegDword( GPSR_REG32( idx ), mask );
                WriteRegDwordAnd( mfpr, ~MFPR_SLEEP_DATA );
                // dummy read required to complete MFPR operation
                ReadDummyRegDword( mfpr );
                break;
            case PAD_PULL_UP:
                WriteRegDwordOr( mfpr, ( MFPR_PULL_SEL | MFPR_PULLUP_EN ) );
                // dummy read required to complete MFPR operation
                ReadDummyRegDword( mfpr );
                break;
            case PAD_PULL_DOWN:
                WriteRegDwordOr( mfpr, ( MFPR_PULL_SEL | MFPR_PULLDOWN_EN ) );
                // dummy read required to complete MFPR operation
                ReadDummyRegDword( mfpr );
                break;
            default:
                // TODO
                break;
        }
        RE_RimLeaveCriticalSection();
    } else {
        WARN2N( "Invalid: PadSet( %d, %d )", line, attribute );
    }

}

void RE_PadClear( RE_Line line, RE_PadAttribute attribute )
{
    // If we are configuring GPIO properties (not just pad properties),
    // we must specify a valid GPIO and a valid GPIO source
    if( ( !IS_GPIO(line) ) &&
        (PAD_OUTPUT_ENABLE == attribute ||
        PAD_INPUT_ENABLE  == attribute ||
        PAD_OUTPUT        == attribute) ) {
            return;
    }

    if( IS_PAD( line ) ) {
        DWORD idx = GPIO_NUM( line );
        DWORD mask = ( (DWORD)1 << (idx & 0x1F) );
        DWORD mfpr = RE_MFPR_REG32( line );
        DWORD temp;
        
        RE_RimEnterCriticalSection();

        switch( attribute ) {
            case PAD_OUTPUT_ENABLE:
                // clearing the output is the same as setting the input
                WriteRegDword( GCDR_REG32( idx ), mask );
                WriteRegDwordOr( mfpr, MFPR_SLEEP_INPUT_EN );
                // dummy read required to complete MFPR operation
                ReadDummyRegDword( mfpr );
                break;
            case PAD_INPUT_ENABLE:
                // clearing the input is the same as setting the output
                WriteRegDword( GSDR_REG32( idx ), mask );
                WriteRegDwordAnd( mfpr, ~MFPR_SLEEP_INPUT_EN );
                // dummy read required to complete MFPR operation
                ReadDummyRegDword( mfpr );
                break;
            case PAD_OUTPUT_SLEEP_OFF:
            case PAD_OUTPUT:
                WriteRegDword( GPCR_REG32( idx ), mask );
                WriteRegDwordAnd( mfpr, ~MFPR_SLEEP_DATA );
                // dummy read required to complete MFPR operation
                ReadDummyRegDword( mfpr );
                break;
            case PAD_PULL_UP:
                temp = ReadRegDword( mfpr );
                temp |= MFPR_PULL_SEL;
                temp &= ~MFPR_PULLUP_EN;
                WriteRegDword( mfpr, temp );
                // dummy read required to complete MFPR operation
                ReadDummyRegDword( mfpr );
                break;
            case PAD_PULL_DOWN:
                temp = ReadRegDword( mfpr );
                temp |= MFPR_PULL_SEL;
                temp &= ~MFPR_PULLDOWN_EN;
                WriteRegDword( mfpr, temp );
                // dummy read required to complete MFPR operation
                ReadDummyRegDword( mfpr );
                break;
            default:
                // TODO
                break;
        }
        RE_RimLeaveCriticalSection();

    } else {
        WARN2N( "Invalid: PadClear( %d, %d )", line, attribute );
    }
}

BOOL RE_PadRead( RE_Line line, RE_PadAttribute attribute )
{
    DWORD value = 0;

    // If we are reading GPIO properties (not just pad properties),
    // we must specify a valid GPIO and a valid GPIO source
    if( !IS_GPIO(line) &&
        (PAD_PULL_UP != attribute &&
        PAD_PULL_DOWN  != attribute) ) {
            return FALSE;
    }

    if( IS_PAD( line ) ) {
        
        DWORD idx = GPIO_NUM( line );
        DWORD mask = ( (DWORD)1 << (idx & 0x1F) );
        RE_RimEnterCriticalSection();

        switch( attribute ) {
            case PAD_INPUT:
            case PAD_OUTPUT:
                value = ( ReadRegDword( GPLR_REG32( idx ) ) ) & mask;
                break;
            case PAD_OUTPUT_ENABLE:
                value = ( ReadRegDword( GPDR_REG32( idx ) ) ) & mask;
                break;
            case PAD_INPUT_ENABLE:
                value = ( ~ReadRegDword( GPDR_REG32( idx ) ) ) & mask;
                break;
            case PAD_PULL_UP:
                value = ReadRegDword( RE_MFPR_REG32( line ) ) & ( MFPR_PULL_SEL | MFPR_PULLUP_EN );
                value = ( value >> 14 ) & ( value >> 15 );
                break;
            case PAD_PULL_DOWN:
                value = ReadRegDword( RE_MFPR_REG32( line ) ) & ( MFPR_PULL_SEL | MFPR_PULLDOWN_EN );
                value = ( value >> 13 ) & ( value >> 15 );
                break;
            // Note: RE_PadRead will only return true if the parameter being read is the most
            // recently latched edge.  So if a rising edge occurs, and is immediately followed
            // by a falling edge, RE_PadRead( GPIO_XX, PAD_RISING_EDGE_LATCH ) will return FALSE.
            // Note also that reading a GPIO's latch state will clear that state.
            case PAD_RISING_EDGE_LATCH:
                if( RE_GpioLatchStates[ idx ] == PAD_LATCH_RISING_EDGE ) {
                    RE_GpioLatchStates[ idx ] = PAD_LATCH_NONE;
                    value  = 1;
                }
                break;
            case PAD_FALLING_EDGE_LATCH:
                if( RE_GpioLatchStates[ idx ] == PAD_LATCH_FALLING_EDGE ) {
                    RE_GpioLatchStates[ idx ] = PAD_LATCH_NONE;
                    value  = 1;
                }
                break;
            default:
                // TODO
                break;
        }
        RE_RimLeaveCriticalSection();

    } else {
        WARN2N( "Invalid: RE_PadRead( %d, %d )", line, attribute );
    }
    return ( value != 0 );
}


void RE_PadSetMode( RE_Line line, DWORD mode )
{
    // Check if this line is to be set as a GPIO
    if( mode == GPIO_FUNCTION ) {
        mode = 0x0; // No look-up required on this processor
    }

    if( IS_PAD( line ) ) {
        DWORD mfpr;
        DWORD temp;

        RE_RimEnterCriticalSection();

        mfpr = RE_MFPR_REG32( line );
        temp = ReadRegDword( mfpr );
        temp = ( temp & ( ~0x7 ) ) | (mode & 0x7);
        //PRINTN("Writing To Address: %x", mfpr);
        WriteRegDword( mfpr, temp );
        // dummy read required to complete MFPR operation
        ReadDummyRegDword( mfpr );

        RE_RimLeaveCriticalSection();
    } else {
        WARN2N( "Invalid: RE_PadSetMode( %d, %d )", line, mode );
    }
}


void RE_PadInterruptConfigure( RE_Line line, RE_PadInterruptType interruptType )
{
    if( IS_GPIO( line ) ) {
        DWORD idx = GPIO_NUM( line );
        DWORD mask = ( (DWORD)1 << (idx & 0x1F) );
        DWORD risingEdgeEnabled;
        DWORD fallingEdgeEnabled;
        RE_RimEnterCriticalSection();
        RE_SetInterruptState( line, interruptType );
        // If the interrupt is already enabled, change it to be the current interrupt type
        risingEdgeEnabled = ReadRegDword( GRER_REG32( idx ) ) & mask;
        fallingEdgeEnabled = ReadRegDword( GFER_REG32( idx ) ) & mask;
        if( risingEdgeEnabled || fallingEdgeEnabled ) {
            RE_PadInterruptEnable( line );
        }
        RE_RimLeaveCriticalSection();
    } else {
        WARN2N( "Invalid: RE_PadInterruptConfigure( %d, %d )", line, interruptType );
    }
}

void RE_PadRegisterIntHandler( RE_Line line, IRQHandler *function )
{
    if( IS_GPIO(line) ) {
        DWORD idx = GPIO_NUM( line );
#if defined( PROCESSOR_FAMILY_MMP )
        RE_RegisterGpioIntHandler( idx, function );
#else
        RegisterIntHandler( GPIO_IRQ, idx, function );
#endif
    } else {
        WARN2N("Invalid RE_Line: RE_PadRegisterIntHandler( %d, 0x%x )", line, (DWORD)function);
    }
}

void RE_PadInterruptEnable( RE_Line line )
{
    if( IS_GPIO( line ) ) {
        DWORD idx = GPIO_NUM( line );
        DWORD mask = ( (DWORD)1 << (idx & 0x1F) );
        DWORD mfpr = RE_MFPR_REG32( line );
        DWORD temp;
        RE_PadInterruptType interruptType;
        RE_RimEnterCriticalSection();
        interruptType = RE_GetInterruptState( line );
        switch( interruptType ) {
            case PAD_INTERRUPT_RISING_EDGE:
                WriteRegDword( GSRER_REG32( idx ), mask );
                WriteRegDword( GCFER_REG32( idx ), mask );
                temp = ReadRegDword( mfpr );
                temp |= MFPR_EDGE_RISE_EN;
                temp &= ~( MFPR_EDGE_CLEAR | MFPR_EDGE_FALL_EN );
                WriteRegDword( mfpr, temp );
                // dummy read required to complete MFPR operation
                ReadDummyRegDword( mfpr );
                break;
            case PAD_INTERRUPT_FALLING_EDGE:
                WriteRegDwordOr( GSFER_REG32( idx ), mask );
                WriteRegDwordOr( GCRER_REG32( idx ), mask );
                temp = ReadRegDword( mfpr );
                temp |= MFPR_EDGE_FALL_EN;
                temp &= ~( MFPR_EDGE_CLEAR | MFPR_EDGE_RISE_EN );
                WriteRegDword( mfpr, temp );
                // dummy read required to complete MFPR operation
                ReadDummyRegDword( mfpr );
                break;
            case PAD_INTERRUPT_BOTH_EDGES:
                WriteRegDword( GSFER_REG32( idx ), mask );
                WriteRegDword( GSRER_REG32( idx ), mask );
                temp = ReadRegDword( mfpr );
                temp |= ( MFPR_EDGE_FALL_EN | MFPR_EDGE_RISE_EN );
                temp &= ~MFPR_EDGE_CLEAR;
                WriteRegDword( mfpr, temp );
                // dummy read required to complete MFPR operation
                ReadDummyRegDword( mfpr );
                break;
            case PAD_INTERRUPT_HIGH_LEVEL:
                // TODO: There are no level interrupts on Tavor!!!
                WARNN("level high interrupts not supported %d", idx);
                break;
            case PAD_INTERRUPT_LOW_LEVEL:
                // TODO: There are no level interrupts on Tavor!!!
                WARNN("level low interrupts not supported %d", idx);
                break;
            case PAD_INTERRUPT_NONE:
                WriteRegDword( GCFER_REG32( idx ), mask );
                WriteRegDword( GCRER_REG32( idx ), mask );
                WriteRegDwordOr( RE_MFPR_REG32( line ), MFPR_EDGE_CLEAR );
                // dummy read required to complete MFPR operation
                ReadDummyRegDword( mfpr );
                break;
            default:
                // TODO
                break;
        }
        RE_RimLeaveCriticalSection();
    } else {
        WARNN( "Invalid: RE_PadInterruptEnable( %d )", line );
    }

}

void RE_PadInterruptDisable( RE_Line line )
{
    if( IS_GPIO( line ) ) {
        DWORD idx = GPIO_NUM( line );
        DWORD mask = ( (DWORD)1 << (idx & 0x1F) );
        DWORD mfpr = RE_MFPR_REG32( line );
        RE_RimEnterCriticalSection();
        WriteRegDword( GCFER_REG32( idx ), mask );
        WriteRegDword( GCRER_REG32( idx ), mask );
        WriteRegDwordOr( mfpr, MFPR_EDGE_CLEAR );
        // dummy read required to complete MFPR operation
        ReadDummyRegDword( mfpr );
        RE_RimLeaveCriticalSection();
    } else {
        WARNN( "Invalid: RE_PadInterruptDisable( %d )", line );
    }
}

void RE_PadInterruptClear( RE_Line line )
{
    if( IS_GPIO( line ) || IS_PAD( line ) ) {
        DWORD mfpr = RE_MFPR_REG32( line );
        DWORD temp = ReadRegDword( mfpr );

        if( IS_GPIO( line ) ) {
            DWORD idx = GPIO_NUM( line );
            DWORD mask = ( (DWORD)1 << (idx & 0x1F) );
            WriteRegDword( GEDR_REG32( idx ), mask );
        }

        // Clearing and Disabling the edge detect is the same function
        // so Disable then Enable only if the edge detection was Enabled
        if( ( temp & MFPR_EDGE_CLEAR ) == 0 ) {
            temp |= MFPR_EDGE_CLEAR;
            WriteRegDword( mfpr, temp );
            // dummy read required to complete MFPR operation
            ReadDummyRegDword( mfpr );
            temp &= ~MFPR_EDGE_CLEAR;
            WriteRegDword( mfpr, temp );
            // dummy read required to complete MFPR operation
            ReadDummyRegDword( mfpr );
        }

    } else {
        WARNN( "Invalid: RE_PadInterruptClear( %d )", line );
    }
}


static void RE_DeviceSpecificPadConfigure( RE_Line line, const RE_DeviceSpecificPadConfiguration *devSpecificConfig )
{
    DWORD mfpr;
    RE_PadConfigureDrive( line, devSpecificConfig->drive );    // Configure drive strength
    mfpr = ReadRegDword( RE_MFPR_REG32( line ) );
    if( devSpecificConfig->sleepSel != SLEEP_SEL_DEFAULT ) {
        if( devSpecificConfig->sleepSel == SLEEP_SEL_COMMS ) {
            mfpr |= MFPR_SLEEP_SEL;
        } else {
            mfpr &= ~MFPR_SLEEP_SEL;
        }
    }
    // Force the RDH bit as the default
    if( devSpecificConfig->useRdh != USE_RDH_DEFAULT ) {
    	if( devSpecificConfig->useRdh == USE_RDH_ENABLE ) {
    		mfpr |= MFPR_USE_RDH;
    	} else {
    		mfpr &= ~MFPR_USE_RDH;
    	}
    }
    if( devSpecificConfig->sleepData != SLEEP_DATA_DEFAULT ) {
        if( devSpecificConfig->sleepData == SLEEP_DATA_HIGH ) {
            mfpr |= MFPR_SLEEP_DATA;
        } else {
            mfpr &= ~MFPR_SLEEP_DATA;
        }
    }
    if( devSpecificConfig->sleepDirection != SLEEP_DIRECTION_DEFAULT ) {
        if( devSpecificConfig->sleepDirection == SLEEP_DIRECTION_INPUT ) {
            mfpr |= MFPR_SLEEP_INPUT_EN;
        } else {
            mfpr &= ~MFPR_SLEEP_INPUT_EN;
        }
    }
    if( devSpecificConfig->pullSel != PULL_SEL_DEFAULT ) {
        if( devSpecificConfig->pullSel == PULL_SEL_MFPR ) {
            mfpr |= MFPR_PULL_SEL;
        } else {
            mfpr &= ~MFPR_PULL_SEL;
        }
    }

    if( devSpecificConfig->edgeDetectEnable != EDGE_DETECT_DEFAULT ) {
        if( devSpecificConfig->edgeDetectEnable == EDGE_DETECT_ENABLE ) {
            mfpr &= ~( MFPR_EDGE_CLEAR | MFPR_EDGE_FALL_EN | MFPR_EDGE_RISE_EN );
            if( devSpecificConfig->detectFallingEdge ) {
                mfpr |= MFPR_EDGE_FALL_EN;
            }
            if( devSpecificConfig->detectRisingEdge ) {
                mfpr |= MFPR_EDGE_RISE_EN;
            }
        } else {
            mfpr |= MFPR_EDGE_CLEAR;
        }
    }

    WriteRegDword( RE_MFPR_REG32( line ), mfpr );
    // dummy read required to complete MFPR operation
    ReadDummyRegDword( RE_MFPR_REG32( line ) );
}


static void RE_SetInterruptState( RE_Line line, RE_PadInterruptType interruptType )
{
    if( IS_GPIO( line ) ) {
        DWORD idx = GPIO_NUM( line );
        RE_GpioInterruptStates[ idx ] = interruptType;
    } else {
        WARN("Cannot save interrupt state of Non-GPIO!");
    }
}

RE_PadInterruptType RE_GetInterruptState( RE_Line line )
{
    RE_PadInterruptType retVal = PAD_INTERRUPT_NONE;
    if( IS_GPIO( line ) ) {
        DWORD idx = GPIO_NUM( line );
        retVal = RE_GpioInterruptStates[ idx ];
    } else {
        WARN("Cannot restore interrupt state of Non-GPIO!");
    }
    return retVal;
}

static void RE_PadConfigureDrive (RE_Line line, RE_PadDriveStrengthSlewRate drive)
{
   DWORD mfpr = ReadRegDword( RE_MFPR_REG32( line ) );
   if( drive != DEFAULT_STRENGTH_SLEW ) {
#if defined( PROCESSOR_TAVOR )
        mfpr &= ~( 7 << 10 );
        switch( drive ) {
            case FAST_1_MA_STRENGTH_SLEW:
                break;
            case FAST_2_MA_STRENGTH_SLEW:
                mfpr |= ( 1 << 10 );
                break;
            case FAST_3_MA_STRENGTH_SLEW:
                mfpr |= ( 2 << 10 );
                break;
            case FAST_4_MA_STRENGTH_SLEW:
                mfpr |= ( 3 << 10 );
                break;
            case SLOW_6_MA_STRENGTH_SLEW:
                mfpr |= ( 4 << 10 );
                break;
            case FAST_6_MA_STRENGTH_SLEW:
                mfpr |= ( 5 << 10 );
                break;
            case SLOW_10_MA_STRENGTH_SLEW:
                mfpr |= ( 6 << 10 );
                break;
            case FAST_10_MA_STRENGTH_SLEW:
                mfpr |= ( 7 << 10 );
                break;
            default:
                WARN( "Unknown drive strength / slew rate parameter!" );
                break;
        }
#else
        mfpr &= ~( 3 << 11 );
        switch( drive ) {
            case PAD_DRIVE_SLOW:
                break;
            case PAD_DRIVE_MEDIUM:
                // See Electrical, Mechanical & Thermal Specifications (EMTS) for more info
                mfpr |= ( 2 << 11 );
                break;
            case PAD_DRIVE_FAST:
                mfpr |= ( 3 << 11 );
                break;
            default:
                WARN( "Unknown drive strength / slew rate parameter!" );
                break;
        }
#endif
        WriteRegDword( RE_MFPR_REG32( line ), mfpr );
        // dummy read required to complete MFPR operation
        ReadDummyRegDword( RE_MFPR_REG32( line ) );
    }
}

#if defined ( PROCESSOR_FAMILY_MMP )

IRQHandler * RE_GPIOHdlr[ NUMBER_OF_GPIOS ] = { NULL, };

static void RE_RegisterGpioIntHandler(DWORD line_, IRQHandler * handler_)
{
    RE_RimEnterCriticalSection();
    RE_GPIOHdlr[ line_ ] = handler_;
    RE_RimLeaveCriticalSection();
}

void GPIOIsr( void )
{
    RE_Line source;
    RE_Line line;
    DWORD gedr;

    line = 0;
    while( IS_GPIO(line) ) {
        gedr = ReadRegDword( GEDR_REG32( line ) );
        if( gedr ) {
        	PRINT( "GPIO interrupt occurred." );
            source = 0;
    		gedr = gedr >> 1;
        	while( gedr ) {
        		source++;
        		gedr = gedr >> 1;
        	}
            ( RE_GPIOHdlr[ source ] )();
            return;
        }
        line += 32;
    }

    WARN( "No GPIO interrupt encountered." );

    return;
}

/*---------------------------------------------------------------------------*/
#endif  // defined ( PROCESSOR_FAMILY_MMP )


RE_Line RE_GetPadDefinition( DWORD gpio ) {
    return RE_GPIO_REVERSE_MAPPINGS[ gpio ];
}


#if defined( PROCESSOR_TAVOR_PV2 ) || defined( PROCESSOR_TAVOR_MG ) || defined( PROCESSOR_NEVO )
DWORD RE_MFPR_REG32( RE_Line line )
{
    DWORD offset = 0x0;
    
    DWORD bso = GPIO_BSO( line );
    
    offset = mfpr_lookuptable[bso - 66];    // BSO index starts at 66
    
    return (MFPR_BASE + offset);
}

#elif defined( PROCESSOR_MMP2 )
DWORD RE_MFPR_REG32( RE_Line line )
{
    DWORD offset = 0x0;

    offset = mfpr_lookuptable[ line ];  
    
    return (MFPR_BASE + offset);
}
#else
#error Undefined processor!
#endif

#if defined( PROCESSOR_MMP2 )
void RE_GpioInit (void){
    RE_ClockEnable( CLOCK_GPIO );
    RE_RegisterIntHandler( HWIRQ_GPIO, GPIOIsr );
}
#else
void RE_GpioInit (void){
    
}
#endif
