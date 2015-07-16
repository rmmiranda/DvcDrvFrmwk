/*****************************************************************************
* Filename:    display_test.c
*
* Copyright 2009, Research In Motion Ltd
*
* Author:      Victor Chen
*
* Created:     Aug 10th, 2010
*
* Description: test code for display driver
*
*
*****************************************************************************/
#include "string.h"
#include "display_internal.h"

/* ===========================================================================
 * Data Structure and Macros 
 * ===========================================================================*/
#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE FILE_DISP_IF

#define DispCtrlConfig  (DisplayDriver.config.dispcConfig)

typedef enum
{
    COLOR_BLACK = 0,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_CYAN,
	COLOR_YELLOW,
	COLOR_MAGENTA,
	COLOR_WHITE
} DispColor_t;	

static DispSurface_t testSurface;
static BYTE frameBuffer[480*360*3];

static void dispInitFrameBuffer(DWORD bpp, DispColor_t color);

 /***************************************************************************
  * Function:       DispCopyBmpToSurface
  * Description:  Copy a bitmap file style data to the a frame buffer 
  *              
  * Note:         
  **************************************************************************/
BOOL DispCopyBmpToSurface(BitMap *bitmap, DispSurface_t* pSurface)
{
    return TRUE;
}    

/***************************************************************************
 * Function:	   dispTest
 * Description:  Test cases for display driver
 *				
 * Note:		 
 **************************************************************************/
static void dispTest(DWORD catergory, DWORD arg)
{
   BYTE buf[255];
   DWORD i = 0;
   Driver_t *displayDrv;
   DisplayCfg_t *pDispCtl;
   DispcCfg_t *pDispController;

    /* Enable Backlight */
    displayDrv = DrvGetHandle(IODRV_ID_DISPLAY);
    if (displayDrv == NULL)
    {
	    PRINT("Failed to enable backlight.");
	    return;
    }	
	pDispCtl = DrvGetConfig(displayDrv);
	if (pDispCtl == NULL)
		return;
	pDispController = &(pDispCtl->dispcConfig);
	
    switch (catergory)
    {
        case 0x01:
			PRINT("Sending one frame of pixels to parallel Smart Panel: Optis TMD");
			pDispController->enable = TRUE;
			pDispController->pipelineConfig = PIPELINE_DBI;
			pDispController->lcdMainModId = arg;                  /* Mirage Sharp TMD panel */
			if (displayDrv->Enable(displayDrv, 0))
			//if (!DispEnable(&DisplayDriver))
			{
				PRINT("Enable display controller failed!");
				return;
			}
			break;
		case 0x02:
			PRINTN("Sending one frame of pixels to parallel Smart Panel with color: %d", arg);
			dispInitFrameBuffer(2, (DispColor_t)arg);
			testSurface.valid       = TRUE;
			testSurface.displayId   = DISPLAY_ID_0;
			testSurface.surfaceId   = PRIMARY_SURFACE;
			testSurface.pixelFormat = PIXEL_FMT_RGB565;
			testSurface.pRGBAddr    = (DWORD)(&frameBuffer[0]);
			testSurface.width       = 480;
			testSurface.height      = 360;
			testSurface.xPos        = 0;
			testSurface.yPos        = 0;
			i = 0;
			DispShowSurface(&testSurface);
            //DispReadLcdPixels(DISPLAY_ID_0, &buf[0], 1);
            while ( i < 20) 
            {
                PRINT2N("buf[%d]=0x%02x ",i,buf[i]); i++;
            
            }
            break;  	
		default:
			break;
    }
	return;
}

/***************************************************************************
 * Function:	   dispInitFrameBuffer
 * Description:  Initialize the frame buffer to be the spicifed color
 *				
 * Note:		 
 **************************************************************************/
static void dispInitFrameBuffer(DWORD bpp, DispColor_t color)
{
    DWORD i = 0;
    DWORD width = 480;
	DWORD height= 360;
    BYTE byte[4];

	switch (color)
	{
		case COLOR_BLACK: 		   /* black */
			byte[0] = 0x00;
			byte[1] = 0x00;
			if (bpp == 3)
			    byte[2] = 0x00;
			break;
		case COLOR_RED: 		   /* red */
			if (bpp == 3)
			{
				byte[0] = 0x00;
				byte[1] = 0x00;
			    byte[2] = 0xff;
			}
			else if (bpp == 2)
			{
			    byte[1] = 0xf8;
			    byte[0] = 0x00;
			}
			break;
		case COLOR_GREEN:          /* Green */ 
			if (bpp == 3)
			{
				byte[0] = 0x00;
				byte[1] = 0xff;
			    byte[2] = 0x00;
			}
			else if (bpp == 2)
			{
			    byte[1] = 0x07;
			    byte[0] = 0xE0;
			}
			break;
		case COLOR_BLUE: 	       /* Blue */ 
			if (bpp == 3)
			{
				byte[0] = 0xff;
				byte[1] = 0x00;
				byte[2] = 0x00;
			}
			else if (bpp == 2)
			{
				byte[1] = 0x00;
				byte[0] = 0x1f;
			}
			break;
		case COLOR_CYAN: 	       /* Cyan */ 
			if (bpp == 3)
			{
				byte[0] = 0xff;
				byte[1] = 0xff;
				byte[2] = 0x00;
			}
			else if (bpp == 2)
			{
				byte[1] = 0x07;
				byte[0] = 0xff;
			}
			break;
		case COLOR_YELLOW: 	       /* Yellow */ 
			if (bpp == 3)
			{
				byte[0] = 0x00;
				byte[1] = 0xff;
				byte[2] = 0xff;
			}
			else if (bpp == 2)
			{
				byte[1] = 0xff;
				byte[0] = 0xe0;
			}
			break;
		case COLOR_MAGENTA: 	   /* Magenta */ 
			if (bpp == 3)
			{
				byte[0] = 0xff;
				byte[1] = 0x00;
				byte[2] = 0xff;
			}
			else if (bpp == 2)
			{
				byte[1] = 0xf8;
				byte[0] = 0x1f;
			}
			break;
		case COLOR_WHITE: 	   /* White */ 
			if (bpp == 3)
			{
				byte[0] = 0xff;
				byte[1] = 0xff;
				byte[2] = 0xff;
			}
			else if (bpp == 2)
			{
				byte[1] = 0xff;
				byte[0] = 0xff;
			}
			break;
        default:
			break;
	}
	
    while ( i < (width*height/bpp))
   	{
	    frameBuffer[i] = byte[0];
	    frameBuffer[i+1] = byte[1];
		if (bpp == 3)
		{
	        frameBuffer[i+2] = byte[2];
		    i++;
		}
		i = i + 2;
    }
    return;	
}


/***************************************************************************
 * Function:	   DrvDispBugDispHandle
 * Description:  BugDisp handler for display driver 
 *				
 * Note:		 
 **************************************************************************/
void DrvDispBugDispHandle(char * cmd, DWORD catergory, DWORD arg)
{
    /* TODO - Add BugDisp command here
      */
    if(strcmp(cmd, "dump") == 0)
    {
         /* category - dump registers (0 - 3) or test cases (4) */
         /* arg - test case number */
         PRINT("Display Driver : Dump DSI/Display Registers");
         DispDumpRegs(catergory, arg);
    }
    if(strcmp(cmd, "test") == 0)
    {
         /* category - dump registers (0 - 3) or test cases (4) */
         /* arg - test case number */
         PRINT("Display Driver Test Cases:");
         dispTest(catergory, arg);
    }
    else if(strcmp(cmd, "help") == 0)
    {
        /* Show bugdisp commands for camera driver */
        PRINT("****************************************");
        PRINT("Display Platform Driver Bugdisp Commands:");
        PRINT("[1]pf.disp.dump    1  - Dump All Display subsytem Related Registers");
        PRINT("[1]pf.disp.dump    2  - Dump Display controller Related Registers");
        PRINT("[2]pf.disp.test    1 0xxx - Test display driver with only one Parallel Smart Panel (Panle ID: 0xxx)");
        PRINT("[2]pf.disp.test    2 0xxx - Test display driver with one DSI Smart Panel (Panel ID: 0xxx)");
    } 
}
