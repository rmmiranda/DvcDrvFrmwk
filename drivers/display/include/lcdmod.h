/*****************************************************************************
 * Filename:    lcdmodule.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Victor Chen
 *
 * Created:     July 16, 2010
 *
 * Description: This header file defines LCD peripheral Modules and SVP APIs 
 *
 *****************************************************************************/
#ifndef __LCDMODULE_H__
#define __LCDMODULE_H__

#include "display.h"

/**
 * LcdModuleType_t    - Define LCD Module Interface type based on MIPI spec
 *                            
 */

typedef enum 
{
    LCD_TYPE_NONE = 0,
    LCD_TYPE_MIPI_DSI = 1,   // LCD module with DSI interface (video/cmd mode) 
    LCD_TYPE_MIPI_DPI,       // LCD module with Parallel interface for Dumb panel (DPI - Video mode; DBI - Command mode)
    LCD_TYPE_MIPI_DBI_8BIT,  // DBI-8, 9 or 16 bit mode for smart panel 
    LCD_TYPE_MIPI_DBI_9BIT,
    LCD_TYPE_MIPI_DBI_16BIT,
    LCD_TYPE_MIPI_DBI_18BIT,
    LCD_TYPE_DSI_BRG,         // DSI bridge device
    LCD_TYPE_HDMI_TV,
    LCD_TYPE_UNKNOWN
} LcdModuleType_t;

/**
 * LcdModuleMode_t    - Define LCD Module working mode. Video mode means dumb panle and display controller has to refresh
 *                               LCD panel with specified refresh rate. Command mode means smart panel and the panle has its internal
 *                               frame buffer so that it can refresh itself.
 *                            
 */
typedef enum
{
    LCD_MODE_NONE = 0,
    LCD_MODE_VIDEO,
    LCD_MODE_CMD
}LcdModuleMode_t;

/* Define of LCD Module */
typedef struct LcdModule
{
    BOOL valid;
    BYTE lcdModuleId;
    DisplayId_t displayId;
    LcdModuleType_t type;
    LcdModuleMode_t mode;
    pixelFmt_t pixelFormat;
	/* X, Y resolutions */
    WORD xres;   
    WORD yres;
    /* Timings for video or command mode */
    union 
    {
        struct  
        {
            WORD refresh;      /* refresh reate (frames per second)*/
            WORD pclk;         /* Pixel Clock */
            WORD hsw;          /* Horizontal Sync Width */
            WORD hbp;          /* Horizontal Back Porch */  
            WORD hfp;          /* Horizontal Front Porch */
            WORD vsw;          /* Vertical Sync Pulse Width */
			WORD vfp;          /* Vertical Front Porch */
            WORD vbp;          /*Vertical Back Porch */
            WORD hsync;        /* hsync and vsync */
            WORD vsync;
       }video;
       struct  
       {
           WORD reserved[10];   /* reserved for command mode timings */
       }command;
    }lcdTimings;
     /* Reserved for output PATH such as DSI has extra timing requirement: Virutal channel; Maximu Speed; data lanes; tLpx;tHsPrep;tHsZero;tHsTrail;tHsExit;tTaGo;tClZero;tClTrail;tTaGet;tWakeup; */     
	 WORD dsiReserved[16]; 
}LcdModule_t;

/*
 * LCD Module SVP Functions - The Display Driver will use SVP functions to retrieve LCD module 
 * settings, initialize LCD DriverIC and other ioctls can be run on the fly.
 */
typedef struct LcdModSvpFuncs
{
    const BYTE lcdModId;
    BOOL (*svpInit)(LcdModule_t *lcdModule);
    void (*svpWriteStart)(LcdModule_t *lcdModule);                     /* command to be sent before writing */
    void (*svpWriteEnd)(LcdModule_t *lcdModule);
    void (*svpReadStart)(LcdModule_t *lcdModule);
    void (*svpReadEnd)(LcdModule_t *lcdModule);
    BOOL (*svpIoctl)(LcdModule_t *lcdModule, int cmd, void *param);
    BOOL (*svpGetSettings)(LcdModule_t *lcdModule);
}LcdModSvpFuncs_t;


/***************************************************************************
 * Function:      svp_lcdGetFuncs
 * Description:  API to return back function pointers based on the speicified LCD module ID
 *              
 * Note:          The display driver will use returned fucntion pointers to initialize LCD module, send ioctls etc.  
 **************************************************************************/
const LcdModSvpFuncs_t *svp_lcdGetFuncs(LcdModule_t *lcdModule);

#endif
