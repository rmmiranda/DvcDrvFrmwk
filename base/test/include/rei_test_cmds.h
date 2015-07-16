/*****************************************************************************
 * Filename:    rei_test_cmds.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:     Daniel Hadi 
 *
 * Created:     March 2, 2010
 *
 * Description: declaration of REI test commands for bugdisp.
 *
 *****************************************************************************/
 
#ifndef __REI_TEST_CMDS_H__
#define __REI_TEST_CMDS_H__

void clkBugdispCmd (char * cmd, DWORD arg1, DWORD arg2);
void gpioBugdispCmd (char * cmd, DWORD arg1, DWORD arg2);

#endif // __REI_TEST_CMDS_H__
