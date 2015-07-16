/*****************************************************************************
 * Filename:    gpio_bugdispcmds.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:     Daniel Hadi 
 *
 * Created:     March 2, 2010
 *
 * Description: REI GPIO bugdisp functions
 *
 *****************************************************************************/
 
#include "rei.h"
#include "framework.h"
#include "gpio_platform.h"
#include <string.h>

#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE  FILE_TST_GPIO

#define INVALID_LINE 0xFFFFFFFFu

RE_Line getGpio (DWORD idx) {
    RE_Line line;
    
#if defined( PROCESSOR_FAMILY_TAVOR )
    line = RE_GetPadDefinition( idx );
#else
    line = GPIO_0 + idx;
#endif
    if( !IS_GPIO(line) ) {
        line = INVALID_LINE;
    }
    
    return line;
}

 
static const RE_PadConfiguration gpioOutput = {
    0,                          // altFunctionMode: GPIO
    PAD_DIRECTION_OUTPUT,       // initialDirection
    FALSE,                      // initialLevel
    FALSE,                      // pullUpEnable
    FALSE,                      // pullDownEnable
    PAD_INTERRUPT_NONE,         // interruptType
    NULL,                       // interruptHandler
    FALSE,                      // interruptsInitiallyEnabled
    FALSE                       // interruptDebounceEnabled
};

static const RE_PadConfiguration gpioInput = {
    0,                          // altFunctionMode: GPIO
    PAD_DIRECTION_INPUT,       // initialDirection
    FALSE,                      // initialLevel
    FALSE,                      // pullUpEnable
    FALSE,                      // pullDownEnable
    PAD_INTERRUPT_NONE,         // interruptType
    NULL,                       // interruptHandler
    FALSE,                      // interruptsInitiallyEnabled
    FALSE                       // interruptDebounceEnabled
};
 
 
void gpioBugdispCmd (char * cmd, DWORD arg1, DWORD arg2) {
    
    RE_Line line;
    
    line = getGpio( arg1 );
    
    if ( line == INVALID_LINE ) {
        WARN ("! Invalid gpio line specified!");
        return;
    }
    
    if(strcmp(cmd, "setdir") == 0)
    {

        // if arg2==0 set output direction
        if( arg2 == 0 ){
            RE_PadConfigure( line, &gpioOutput,  NULL );
            PRINTN("GPIO_%d direction set to OUTPUT", arg1);
        }
        // if arg2==1 set input direction
        else if( arg2 == 1 ) {
            RE_PadConfigure( line, &gpioInput,  NULL );
            PRINTN("GPIO_%d direction set to INPUT", arg1);
        }
        else {
            PRINT("Invalid argument");
        }
    }
    else if (strcmp(cmd, "setlvl") == 0) 
    {
        
        // if arg2==0 set low
        if ( arg2 == 0 ){
            RE_PadClear(line, PAD_OUTPUT);
            PRINTN("GPIO_%d level set to 0", arg1);
        }
        // if arg2==1 set high
        else if ( arg2 == 1 ) {
            RE_PadSet(line, PAD_OUTPUT);
            PRINTN("GPIO_%d level set to 1", arg1);
            
        }
        else {
            PRINT("Invalid argument");
        }
        //RimSleep(5);  
        
    }
    else if (strcmp(cmd, "read") == 0) 
    {
        
        if( RE_PadRead(line, PAD_OUTPUT) ) {
            PRINTN("GPIO_%d value is 1", arg1);
        }
        else {
            PRINTN("GPIO_%d value is 0", arg1);
        }

    }
    else if (strcmp(cmd, "dump") == 0) 
    {
        PRINT ("GPIO dump Not yet implemented.");

    }
    else {
        PRINT("***************************************************************");
        PRINT("REI GPIO Bugdisp Commands:");
        PRINT("pf.gpio.setdir XX Y               XX = GPIO number");
        PRINT("                                  Y = 0 for output, 1 for input");
        PRINT("pf.gpio.setlvl XX Y               XX = GPIO number");
        PRINT("                                  Y = 0 for low, 1 for high");
        PRINT("pf.gpio.read XX                   XX = GPIO number ");
        PRINT("pf.gpio.dump                      dump status of all gpios (not implemented) ");
        PRINT("(Note: Should run setdir first to initially configure gpio pad.");
        PRINT("***************************************************************");


    }


    return;
 
 
 }
 
