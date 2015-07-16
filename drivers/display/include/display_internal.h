/*****************************************************************************
 * Filename:    display_internal.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Victor Chen
 *
 * Created:    August 27, 2010
 *
 * Description: This header file defines the display subsystem public interfaces and core data structures used by 
 *                  display driver internal usage.
 *
 *****************************************************************************/
#ifndef __DISPLAY_INTERNAL_H_
#define __DISPLAY_INTERNAL_H_

#include "display.h"
#include "lcdmod.h"
#include "parallel.h"
#include "dsi.h"
#include "hdmi.h"

/* ---------------------------------------------------------------------------
 * Macro Definitions
 * --------------------------------------------------------------------------- */
 
#define DISP_MAX_XSIZE 1024
#define DISP_MAX_YSIZE 1024
#define DISP_BPP_DEFAULT 3
#define DISP_MAX_PANELS 0x02        /* Support upto 2 LCD panels on a running DSS */ 

#define LcdMainModule   (DisplayDriver.lcdModule[0])
#define LcdSubModule    (DisplayDriver.lcdModule[1])

 /* ---------------------------------------------------------------------------
  * Data structure definitions
  * --------------------------------------------------------------------------- */
  
 /* Output Path of pixel pipeline */
 typedef enum 
 {
	 OUTPATH_UNKNOWN = 0,
	 OUTPATH_DSI1,
	 OUTPATH_DSI2,
	 OUTPATH_HDMI,
	 OUTPATH_PARALLEL_CMD,
	 OUTPATH_PARALLEL_VIDEO
 } DispPipelineOutPath_t;
 
/* Display Controller */
typedef struct 
{
    /* global mutual exclusion  */
    RE_CountingSemaphore    dispLock;      
    BYTE                    cmdFifo[1024]; /* Command FIFO */
	BOOL                    dmaWrite;      /* DMA or CPU write */
	
    /* Todo : Add other MMP3 or TavorTD related details here*/
    RE_CountingSemaphore    waitForCmdInt;                         /* Command Interrupt */
    RE_CountingSemaphore    waitForReadInt;                        /* Read Interrupt */
} DispCtrl_t;

/**
   * DispDrv   - Display Driver structure is the shared structure by different controllers (display/parallel, DSI, HDMI) 
   *							  
  */
typedef struct
{
    /* Driver handle */
    Driver_t drvApi;
    
    /* Driver Info - confgiurations, status and statistics */
    DisplayCfg_t     config;
    DisplayStatus_t  status;
    DisplayStats_t   stats;

    /* Controllers */
    DispCtrl_t   dispcCtl;
    DsiCtrl_t    dsiCtl;
    HdmiCtrl_t   hdmiCtl; 

    /* Maximum 2 Lcd Panels supported by current design */
    LcdModule_t lcdModule[DISP_MAX_PANELS];  
} DispDrv_t;

/* Shared info by different DSS controllers */
extern DispDrv_t DisplayDriver;

/* Global APIs */
/***************************************************************************
 * Function:     DispInit  
 * Description:  the entry point of driver framework when initializing display driver
 * Return:        DispDrv_t*                  
 *              
 * Note:                 
 **************************************************************************/
DispDrv_t* DispInit(void);
/***************************************************************************
 * Function:     DispEnable  
 * Description:  Enable controllers based on configurations.
 *                  
 *              
 * Note:                 
 **************************************************************************/
BOOL DispEnable(DispDrv_t *pDisplay);

/***************************************************************************
 * Function:     DispDisable  
 * Description:  Disable All the controllers
 *                  
 *              
 * Note:                 
 **************************************************************************/
BOOL DispDisable(DispDrv_t *pDisplay);

/***************************************************************************
 * Function:     DispGetOutpath  
 * Description:  Get output path of Dispaly Subsystem based on dispalyId info
 *                  
 *              
 * Note:                 
 **************************************************************************/
DispPipelineOutPath_t DispGetOutpath(const DisplayId_t displayId);

/* Dump Registers */
void DispDumpRegs(DWORD catergory, DWORD arg);

#endif //__MMP3_DISPLAY_H_

