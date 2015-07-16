/*****************************************************************************
 * Filename:    rei.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:     Daniel Hadi 
 *
 * Created:     March 17, 2010
 *
 * Description: Contains general REI related functions such as init routine
 *
 *****************************************************************************/

#include "rei.h"
#include "framework.h"


void ReiInit (void) {

    /* Register REI test debug commands to bugdisp */
    PfCmdRegister("gpio", "REI GPIO", &gpioBugdispCmd);
    PfCmdRegister("clk", "REI CLK", &clkBugdispCmd);
    
    RE_GpioInit();
}
