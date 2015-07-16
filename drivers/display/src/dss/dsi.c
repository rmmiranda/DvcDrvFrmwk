/*****************************************************************************
 * Filename:    dsi.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Victor Chen
 *
 * Created:     July 16, 2010
 *
 * Description: Platform specific DSI driver implementation
 *
 *****************************************************************************/

#include <string.h>
#include "display_internal.h"
#include "display_reg.h"

/* ---------------------------------------------------------------------------
 * Defines and data variables
 * --------------------------------------------------------------------------- */
#define DsiCtrlConfig   (DisplayDriver.config.dsiConfig)
#define DsiCtrlStatus   (DisplayDriver.status.dsiStatus)
#define DsiCtrlStatis   (DisplayDriver.stats.dsiStats)
#define DsiController   (DisplayDriver.dsiCtl)

/* ---------------------------------------------------------------------------
 * Function forward declaration
 * --------------------------------------------------------------------------- */
static BYTE dsiGetVc(DisplayId_t displayId);
static BOOL dsiSetVc(BYTE vc, DisplayId_t displayId);

/* ---------------------------------------------------------------------------
 * Function implementation
 * --------------------------------------------------------------------------- */

/* 
  * Driver Framework APIs for DSI  
  *
  */

/*****************************************************************************/
BOOL DsiInit(void)
{
    /* Initialize DSI controller */ 
    memset(&DsiController, 0, sizeof(DsiController));
    DsiCtrlStatus[0].dsiCtlState= DSI_STATE_UNINITIALIZED;
    DsiCtrlStatus[1].dsiCtlState = DSI_STATE_UNINITIALIZED;
    
    /* TODO: Register ISR, set DMA etc
      //RegisterIntHandler( HWIRQ_MIPI_DSI, ( IRQHandler * )&DsiCtlIsr );
     */
	RE_RimInitSemaphore( &DsiController.dsiLock[0]);
	RE_RimInitSemaphore( &DsiController.dsiLock[1]);

    /* Update state */
    DsiCtrlStatus[0].dsiCtlState = DSI_STATE_DISABLED;
    DsiCtrlStatus[1].dsiCtlState = DSI_STATE_DISABLED;
	
    return TRUE;
}


/******************************************************************************/
BOOL DsiEnable()
{
    /* TODO */
    return TRUE;
}

/******************************************************************************/
BOOL DsiDisable()
{
    return TRUE;
}

/******************************************************************************/
int DsiControl(int cmd, void *param)
{
    switch (cmd)
    {
        case DISP_CMD_DSI_SETVC:
			dsiSetVc(0, DISPLAY_ID_0);      /* TODO: Debugging only , should be changed later */
			break;
		default:
			break;
   	}		
    	
    return (DRIVER_UNKNOWN_ERROR);
}

/*****************************************************************************/
BOOL DsiSendVideoPacket(DisplayId_t displayId)
{
    return TRUE;
}

/*****************************************************************************/
BOOL DsiDcsWrite(BYTE *buf, DWORD len, DisplayId_t displayId)
{
    /* Get Virtual channel and DSI controller ID */
    DispPipelineOutPath_t dsiId = DispGetOutpath(displayId);
    BYTE vc = dsiGetVc(displayId);	
	

    return TRUE;

}

/******************************************************************************/
BOOL DsiDcsRead(const MipiDcsCmd_t cmd, BYTE *buf, const DWORD len, DisplayId_t displayId)
{
    BYTE vcChannel = dsiGetVc(displayId);
    BYTE buffer[8];

	buffer[0] = MIPI_DSI_DT_DCS_READ_NO_PARAMETER;

    /* ToDo */
	if (len == 1)
	{
        buffer[0] = MIPI_DSI_DT_DCS_READ_NO_PARAMETER;
	    buffer[1] = cmd;
        DsiDcsWrite(&buffer[0], 2, displayId);
    }	
	else if ( len == 2)
       ; 
    else
	   ; 
	return TRUE;

}

/******************************************************************************/
BOOL DsiRawWrite(BYTE *buf, const DWORD len, DisplayId_t displayId)
{
    return TRUE;
}	

/******************************************************************************/
BOOL DispDsiConfigTiming(LcdModule_t *lcdModule)
{
    /* ToDo */
    return TRUE;
}

/*****************************************************************************
 * Function:    dsiGetVc 
 * Description: From displayId to get virtual number (displayId->manufactureId->call svp)
 * Parameters: NONE
 * Return:       virtual channel number.
 *
****************************************************************************/
static BYTE dsiGetVc(DisplayId_t displayId)
{
    BYTE vc = 0;
    LcdModule_t* pLcdModule;

    if (displayId == DISPLAY_ID_0)
        pLcdModule = (&LcdMainModule);
    else 
        pLcdModule = (&LcdSubModule);

    if ((pLcdModule->valid) && (pLcdModule->displayId == displayId))
	     vc = pLcdModule->dsiReserved[0];

    return (vc);	
}

/*****************************************************************************
 * Function:    dsiSetVc 
 * Description: Set virtual number for a specified LCD panel 
 * Parameters: NONE
 * Return:       TRUE or FALSE.
 *
****************************************************************************/
static BOOL dsiSetVc(BYTE vc, DisplayId_t displayId)
{
	BOOL rc = FALSE;
    LcdModule_t* pLcdModule;

    if (displayId == DISPLAY_ID_0)
        pLcdModule = (&LcdMainModule);
    else 
        pLcdModule = (&LcdSubModule);

    if (pLcdModule->valid)
    {
	    pLcdModule->dsiReserved[0] = vc;
        rc = TRUE;
    }
    return (rc);	
}

