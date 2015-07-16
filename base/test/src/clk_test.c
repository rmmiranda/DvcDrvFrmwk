/*****************************************************************************
 * Filename:    clk_test.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:     Daniel Hadi 
 *
 * Created:     March 17, 2010
 *
 * Description: REI CLK bugdisp functions
 *
 *****************************************************************************/
#include "rei.h"
#include "framework.h"
#include <string.h>

#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE  FILE_REI_CLK

void clkBugdispCmd (char * cmd, DWORD arg1, DWORD arg2) {
    
    
    if(strcmp(cmd, "enable") == 0)
    {
        PRINTN("Enabling RE_PeripheralClock (%d)", arg1);
        RE_ClockEnable((RE_PeripheralClock) arg1);
    }
    else if(strcmp(cmd, "disable") == 0)
    {
        PRINTN("Disabling RE_PeripheralClock (%d)", arg1);
        RE_ClockDisable((RE_PeripheralClock) arg1);
        
    }
    else if(strcmp(cmd, "list") == 0)
    {
        RE_DumpClockList();
    }
    else {
        PRINT("***************************************************************");
        PRINT("REI CLOCKS Bugdisp Commands:");
        PRINT("");
        PRINT("pf.clk.enable <peripheral clk>");
        PRINT("pf.clk.disable <peripheral clk>");
        PRINT("pf.clk.list                              Dump list of clocknames and enum value");

        PRINT("***************************************************************");

    }
    
    return;
    
}
