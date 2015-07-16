/*****************************************************************************
* Filename:    dispif_drvfwk.c
*
* Copyright 2009, Research In Motion Ltd
*
* Author:      Victor Chen
*
* Created:     April 2009
*
* Description: Display Driver for Driver Framwork Interface 
*              
*
*****************************************************************************/

#include <string.h>

#include "rei.h"
#include "framework.h"
#include "driver.h"
#include "display_internal.h"

/* ---------------------------------------------------------------------------
 * Defines and data variables
 * --------------------------------------------------------------------------- */
#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE  FILE_DISP_FWK

DispDrv_t *pDispDrv = NULL;

extern void DrvDispBugDispHandle(char * cmd, DWORD catergory, DWORD arg);

/* return a pointer to the driver status */
static void *DrvDispGetStatus(Driver_t *drvPtr)
{
    /* Call actual display driver API */
    return (void *) &(((DispDrv_t *)drvPtr)->status);
}

/* return a pointer to the driver statistics */
static void *DrvDispGetStats(Driver_t *drvPtr)
{
   return (void *) &(((DispDrv_t *)drvPtr)->stats);
}

/* return a pointer to the driver configuration */
static void *DrvDispGetConfig(Driver_t *drvPtr)
{
    /* Call actual display driver API */
    return (void *) &(((DispDrv_t *)drvPtr)->config);
}

/* Enable Display Driver whever any configuration changed */
static int DrvDispEnable(Driver_t *drvPtr, int unused)
{
    /* Call generic display driver implementation */
    DispEnable((DispDrv_t *)drvPtr);
    return 0;
}

static int DrvDispDisable(Driver_t *drvPtr, int unused)
{
    /* Call generic display driver API */
	DispDisable((DispDrv_t *)drvPtr);
    return 0;
}

static int DrvDispControl(Driver_t *drvPtr, int cmd, va_list args)
{
    int rc = DRIVER_UNKNOWN_ERROR; 
    
    switch (cmd)
    {
		case DISP_CMD_CLEAR_LCD:
			break;
		case DISP_CMD_SET_BACKGROUND_COLOR:
			break;
		case DISP_CMD_SET_ROTATION:
			break;
		case DISP_CMD_CLEAR_STATIS:
			break;
		case DISP_CMD_STOP_TRANSMISSION:
			break;
        case DISP_CMD_DUMP_REGS:
			break;
		case DISP_CMD_DSI_SETVC:
		    /*dsiSetVc(vc, displayId);	 */
			break;
        default:
            break;
    }
    return rc;
}

/* Driver Initialization - the only entry point for Driver Framework */
Driver_t * DrvFwkDispInit(void)
{
    Driver_t *pDriver = NULL;   
    pDispDrv = DispInit();

    /* Fill in function pointers for Driver_t */
    if (pDispDrv != NULL)
    {
        pDriver               = (Driver_t *) &(pDispDrv->drvApi);
        pDriver->GetStatus    = DrvDispGetStatus;
        pDriver->GetConfig    = DrvDispGetConfig;
        pDriver->GetStats     = DrvDispGetStats;
        pDriver->Control      = DrvDispControl;
        pDriver->Enable       = DrvDispEnable; /* call DispEnable directly */
        pDriver->Disable      = DrvDispDisable;
        PRINT("DrvFwkDispInit succeed!");
        /* Register Bugdisp hook */
        PfCmdRegister("disp", "Display Driver", &DrvDispBugDispHandle);
    }
    else
    {
        PRINT("DrvFwkDispInit failed!");
    }    
    return pDriver;    
}
