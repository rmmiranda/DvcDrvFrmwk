/*****************************************************************************
 * Filename:    rei_minios_clocks.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Dan Hadi
 *
 * Created:     February 2010
 *
 * Description: This file defines the REI(runtime environment interface) for 
 *              clock control.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *
 ****************************************************************************/

#include "rei.h"
#include "rei_minios_clocks.h"
#include "clocks.h"
#include "framework.h"

#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE  FILE_REI_CLK


typedef struct PeripheralClockInfoTag {
   RE_PeripheralClock clock;
   char clockName [16];
   DWORD clkAddrOffset;
} PeripheralClockInfo;

// **************************************************************************
// APB Clock register address lookup table.
// RE_PERIPHERAL_CLOCK_MAPPING acts as a lookup table based on the
// clock enum defined by RE_PeripheralClock. It contains clock enum,
// clock name, and clock address offset.
// **************************************************************************
static PeripheralClockInfo ClkLookupTable [NUM_PERIPHERAL_CLOCKS] = RE_PERIPHERAL_CLOCK_MAPPING;


// **************************************************************************
// RE_PeripheralClock and RE_PERIPHERAL_CLOCK_MAPPING have a corresponding order
// and mapping. RE_PERIPHERAL_CLOCK_MAPPING acts as a lookup table base on the
// clock index defined by RE_PeripheralClock.
// **************************************************************************
PeripheralClockInfo GetPeripheralClock (RE_PeripheralClock peripheral){

    int i;
    PeripheralClockInfo tempClk;
    
    // search the lookup table for the matching enum...
    for (i=0; i < NUM_PERIPHERAL_CLOCKS; i++) {
        tempClk = ClkLookupTable[i];
        if ( tempClk.clock == peripheral ) {
            return tempClk;
        }
    
    }
    
    PRINT ("ERROR: INVALID RE_PERIPHERAL_CLOCK!");
    return tempClk;
}


void RE_ClockEnable( RE_PeripheralClock peripheral ){
    
    DWORD clkAddr;
    PeripheralClockInfo clkInfo;
    
    //Should check and make sure main clock source is running

    
    // determine peripheral clock control register address
    if ( peripheral >= NUM_PERIPHERAL_CLOCKS ) {
        PRINT ("ERROR: INVALID RE_PERIPHERAL_CLOCK!");
        return;
    }
    clkInfo = GetPeripheralClock (peripheral);
    clkAddr = clkInfo.clkAddrOffset;
    
    // set the functional clock freq to default (?)
    WriteRegDword( clkAddr, APB_CLOCK_FNCLKSEL_DEFAULT );

    // enable both functional and bus clocks
    WriteRegDwordOr( clkAddr, (APB_CLOCK_FNCLK_ON | APB_CLOCK_APBCLK_ON) );
    
    // reset the clocks
    WriteRegDwordAnd( clkAddr, ~APB_CLOCK_RESET );
    
    // delay for 3 clock periods of the slowest frequency.
    // Since this should be generic for all the peripheral clocks, the slowest 
    // clk freq is 32kHz. So wait 3 * 32kHz = 93.75 us = 93570 ns
    
    // NOTE: No clean method of adding the delay right now. Developers
    // should be aware that this should be addressed in the future.
    
    return;
}

void RE_ClockDisable( RE_PeripheralClock peripheral ){

    DWORD clkAddr;
    PeripheralClockInfo clkInfo;
    
    // determine peripheral clock control register address
    if ( peripheral >= NUM_PERIPHERAL_CLOCKS ) {
        PRINT ("ERROR: INVALID RE_PERIPHERAL_CLOCK!");
        return;
    }
    clkInfo = GetPeripheralClock (peripheral);
    clkAddr = clkInfo.clkAddrOffset;

    // disable first the functional clock
    WriteRegDwordAnd( clkAddr, ~APB_CLOCK_FNCLK_ON );

    // delay for 3 clock periods of the slowest frequency.
    // Since this should be generic for all the peripheral clocks, the slowest 
    // clk freq is 32kHz. So wait 3 * 32kHz = 93.75 us = 93570 ns

    // NOTE: No clean method of adding the delay right now. Developers
    // should be aware that this should be addressed in the future.

    // then disable the bus clock 
    WriteRegDwordAnd( clkAddr, ~APB_CLOCK_APBCLK_ON );

    // reset the clocks
    WriteRegDwordOr( clkAddr, APB_CLOCK_RESET );
    
    return;

}


void RE_DumpClockList ( void ) {

    int i;
    PRINT("#\t\tClock Name");
    PRINT("----------------------------------");
    for (i=0; i < NUM_PERIPHERAL_CLOCKS; i++) {
        PRINT2N ("%d\t\t%s", ClkLookupTable[i].clock, ClkLookupTable[i].clockName); 

    }
    PRINT("----------------------------------");

}



