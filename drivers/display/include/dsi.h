/*****************************************************************************
 * Filename:    display_dsi.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Victor Chen
 *
 * Created:     July 16, 2010
 *
 * Description: This header file defines the Framework DSI public interface and data structures
 *
 *****************************************************************************/
#ifndef __DISPLAY_DSI_H__
#define __DISPLAY_DSI_H__

#include "mipi.h"

/* ---------------------------------------------------------------------------
 * Defines and data variables
 * --------------------------------------------------------------------------- */
#define DSI_CTL_NUM 0x02            /* 2 DSI controllers on DSS */
#define DSI_MAX_VC 0xFF             /* Virutal channel number */
#define DSI_MAX_PANELS 0x02         /* Maximum LCD panels per DSI controlelr */


typedef struct 
{
    /* ToDo: MMP3 specific */
    RE_Semaphore  dsiLock[DSI_CTL_NUM];

	/* command FIFO buffer and other control info */ 
} DsiCtrl_t;

/* 
  *APIs DSI driver should be implemented by each DSI driver 
  *
  */

/***************************************************************************
 * Function:      DsiInit
 * Description:  API to retrive DSS DSI interface configurations such as GPIOs and initialize any 
 *                   variables here.
 *
 * Note:          Derived from LcdCtrlInit() on Nessus TavorTD LCD driver
 **************************************************************************/
BOOL DsiInit(void);

/***************************************************************************
 * Function:      DsiEnable
 * Description:  API to initialize DSS DSI interface  
 *              
 * Note:          Derived from LcdCtrlInit() on Nessus TavorTD LCD driver
 **************************************************************************/
BOOL DsiEnable(void);

/***************************************************************************
 * Function:      DsiDisable
 * Description:  API to disable DSI interface   
 *              
 * Note:          Derived from LcdCtrlInit() on Nessus TavorTD LCD driver
 **************************************************************************/
BOOL DsiDisable(void);


/*****************************************************************************
 * Function:	DsiDcsWrite 
 * Description: Write DCS command to LCD panel
 * Parameters:	NONE
 * Return:		TRUE or FALSE.
 *
****************************************************************************/
BOOL DsiDcsWrite(BYTE *buf, DWORD len, DisplayId_t displayId);

/*****************************************************************************
 * Function:    DsiDcsRead 
 * Description: Write DCS command to read data back from LCD panel
 * Parameters:  NONE
 * Return:      TRUE or FALSE.
 *
****************************************************************************/
BOOL DsiDcsRead(const MipiDcsCmd_t cmd, BYTE *buf, const DWORD len, DisplayId_t displayId);

/*****************************************************************************
 * Function:     DsiRawWrite 
 * Description:  Write raw data to LCD panel
 * Parameters:  NONE
 * Return:        TRUE or FALSE.
 *
****************************************************************************/
BOOL DsiRawWrite(BYTE *buf, const DWORD len, DisplayId_t displayId);

/*****************************************************************************
 * Function:    DsiSendVideoPacket 
 * Description: What DSI controller to do when display driver send one frame to LCD panel
 * Parameters:  NONE
 * Return:      TRUE or FALSE.
 *
****************************************************************************/
BOOL DsiSendVideoPacket(DisplayId_t displayId);

/*****************************************************************************
 * Function:    DispDsiConfigTiming 
 * Description: Configure DSI D-PHY timings based on configurations
 * Parameters: NONE
 * Return:       TRUE or FALSE.
 *
****************************************************************************/
BOOL DispDsiConfigTiming(LcdModule_t *lcdModule);

#endif //__DISPLAY_DSI_H__
