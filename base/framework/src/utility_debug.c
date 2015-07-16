/*****************************************************************************
 * Filedriver:    utility_debug.c
 *
 * Copyright 2005, Research In Motion Ltd
 *
 * Author:      Geoff Mather and Bin Lu
 *
 * Created:     January 2009
 *
 * Description: Bugdisp entry point for the framework.
 *****************************************************************************/

/* standard library header files */
#include <string.h>

/* application header files */
#include  "rei.h"
#include  "framework.h"

#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE  FILE_FWK_BUGDISP

#define PfCmdListSize 30
#define VfCmdListSize 10

//Provides a list of commands that are registered.
typedef struct {
    const char      *driver;
    const char      *help;
	PfCmdFunction func;
} CmdListEntryType;

static CmdListEntryType PfCmdList[PfCmdListSize];
static CmdListEntryType VfCmdList[VfCmdListSize];

//The current count of registered commands.
static WORD pfCmdCount = 0;
static WORD vfCmdCount = 0;

/**
 * This searches the registered commands for a specific command based on the params.
 *
 * @param const char *dvr: The driver name.
 * @param WORD *index: This will return with the index of the command found if func returns TRUE. 
 *
 * @return TRUE if the call is successful. Otherwise, returns FALSE.
 *
 */
BOOL CmdLookup(CmdListEntryType * cmdList, int cmdCount, const char *dvr, WORD *index)
{
    WORD i;

    for(i = 0; i < cmdCount; ++i) {
        if(strcmp(dvr, cmdList[i].driver) == 0) {
            if(index) *index = i;
            return TRUE;
        }
    }

    return FALSE;
}

void DumpCmdList(CmdListEntryType * cmdList, int cmdCount)
{
    WORD i;
    for(i = 0; i < cmdCount; ++i) 
    {
            PRINTN("Command Index - %d", i);
            PRINTCOPY("  driver: %s", cmdList[i].driver);
            PRINTCOPY("  help(%s)", cmdList[i].help);
    }    
}

/**
 * This is the Platform Software entry point in Bugdisp.
 *
 * @param const char *cmd: The command string sent from bugdisp.
 * @param DWORD arg1, arg2: 
 *
 * @return void
 *
 */
void PlatformBugdispEntry(const char *cmd, DWORD arg1, DWORD arg2){   
	char tmp[12];
	char *driver = 0;
    char *action = 0;
	WORD index;
	
	//Strip the pf and the first '.'
	strcpy(tmp, cmd+3);
	
	//Print help when no command is entered.
	if ( strcmp( tmp, "" ) == 0 ){
		PRINT("Invalid driver name.");
		return;
	}
	
	//Parse the command for the various parts.
	driver = strtok ( tmp, "." );
	action = strtok( NULL, " ");
	
	//If there is no action, print help for commands with same driver.
	if ( action == NULL ) {
		PRINT("Invalid action name.");
		return;
	}
	
	//Verify the command entered is registered and call the function it holds.
	if( cmd[0] == 'p' && cmd[1] == 'f')
    {
        if(CmdLookup(PfCmdList, pfCmdCount, driver, &index) == TRUE )
		    PfCmdList[index].func(action, arg1, arg2);
	    else 
		    PRINT("Incorrect driver command. Please enter 'x pf' for help.");
	}

	if( cmd[0] == 'v' && cmd[1] == 'f')
    {
        if(CmdLookup(VfCmdList, vfCmdCount, driver, &index) == TRUE )
		    VfCmdList[index].func(action, arg1, arg2);
	    else 
        {
            PRINTN("Total number of VF commands is - %d", vfCmdCount);
            DumpCmdList(VfCmdList, vfCmdCount);
        }
	}    
}


/**
 * Register a command so that it can be recognized and handled by the driver.
 *
 * @param const char *driver: The commands driver name.
 * @param const char *help: Help associated with the command.
 * @param PfCmdFunction func: A pointer to a function that will execute the given action on the driver . 
 *
 * @return void.
 *
 */
void PfCmdRegister(
    const char *driver, 
    const char *help,
	PfCmdFunction func)
{ 
    if(pfCmdCount >= PfCmdListSize) {
        ASSERTFC("Command limit has been reached");
    }
    if(driver == NULL || strlen(driver) == 0) {
        ASSERTFC("Command must have a driver");
    }
	
    if(CmdLookup(PfCmdList, pfCmdCount, driver, NULL)) {
        ASSERTFC("Command already registered with this driver");
    }

    PfCmdList[pfCmdCount].driver = driver;
    PfCmdList[pfCmdCount].help = help;
	PfCmdList[pfCmdCount].func = func;
    pfCmdCount++;
}

/**
 * Register a command so that it can be recognized and handled by the application and verification code.
 *
 * @param const char *test: The test application name.
 * @param const char *help: Help associated with the command.
 * @param PfCmdFunction func: A pointer to a function that will execute the given action on the application . 
 *
 * @return void.
 *
 */
void VfCmdRegister(
    const char *test, 
    const char *help,
	PfCmdFunction func)
{ 
    if(vfCmdCount >= VfCmdListSize) {
        ASSERTFC("Command limit has been reached");
    }
    if(test == NULL || strlen(test) == 0) {
        ASSERTFC("Command must have a driver");
    }
	
    if(CmdLookup(VfCmdList, vfCmdCount, test, NULL)) {
        ASSERTFC("Command already registered with this driver");
    }

    VfCmdList[vfCmdCount].driver = test;
    VfCmdList[vfCmdCount].help = help;
	VfCmdList[vfCmdCount].func = func;    
    vfCmdCount++;    
}

