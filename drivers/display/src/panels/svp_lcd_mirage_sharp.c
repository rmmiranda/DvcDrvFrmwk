/*****************************************************************************
* Filename:    svp_lcd_mirage_sharp.c
*
* Copyright 2009, Research In Motion Ltd
*
* Author:      Victor Chen
*
* Created:     Aug 10th, 2010
*
* Description: SVP file for Mirage Sharp LCD Module with DBI interface
*
*
*****************************************************************************/

#include "lcdmod.h"
#include "parallel.h"
#include "gpio_platform.h"

/* ===========================================================================
 * Data Structure and Macros 
 * ===========================================================================*/
#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE  FILE_DISP_HAL

#define LCD_MIRAGE_SHARP_WIDTH  480
#define LCD_MIRAGE_SHARP_HEIGHT 360

#define LCD_CMD_READ_ID 0x06E0
#define LCD_MIRAGE_SHARP_ID 0x7F21

typedef struct 
{
    WORD reg;      /* LSB */
	WORD data;           
}tmdRegValue_t;

static const tmdRegValue_t tmdPowerOnData[] =
{
    /* Sharp Password */
    {0x0BE, 0xC329},
	
    /* Display Control */
    {0x001, 0x0000},
	{0x002, 0x0100},
	{0x070, 0x0001},
	{0x00D, 0x0002},
	{0x606, 0x0000},
	{0x007, 0x0103},
	{0x008, 0x0202},
	{0x400, 0x00B3},
    {0x401, 0x0001},
	{0x402, 0x0000},
	{0x403, 0x0167},
		
	//Panel Interface Control
	{0x010, 0x005C},	// default refresh rate: 59.7Hz
	{0x011, 0xB004},
	{0x012, 0xAA07},
	{0x013, 0x000C},
	{0x014, 0x0015},
	{0x015, 0x0004},
	{0x016, 0x0000},
	{0x017, 0xB004},
	{0x018, 0x0000},
	{0x030, 0xFFFF},
	{0x031, 0x0003},
	{0x032, 0x0000},
	{0x033, 0x0000},
	{0x090, 0x0000},
	{0x091, 0x0003},
	{0x0E0, 0x0001},
	{0x0E1, 0x0031},
		
	//Power Control
	{0x100, 0x1030},
	{0x101, 0x2622},
	{0x102, 0x5C1B},
	{0x103, 0x001C},
	{0x105, 0x0154},
	{0x106, 0x050F},
	{0x107, 0x8010},
		
	//Power Sequencer Control
	{0x111, 0x6540},
	{0x112, 0xCF00},
	{0x113, 0x2010},
	{0x114, 0xAA4A},
	{0x115, 0xAA8A},
	{0x116, 0xA866},
	{0x117, 0x00DA},
	{0x118, 0x22AA },
	{0x119, 0x6B26},
	{0x11A, 0x0002},
	{0x120, 0x0F1F},
		
	//Gamma Control
	{0x300, 0x060C},
	{0x301, 0x0101},
	{0x302, 0x0707},
	{0x303, 0x0101},
	{0x304, 0x0301},
	{0x305, 0x0000},
	{0x306, 0x0735},
	{0x307, 0x080E},
	{0x308, 0x0001},
	{0x309, 0x0707},
	{0x30A, 0x0101},
	{0x30B, 0x0301},
	{0x30C, 0x0000},
	{0x30D, 0x063B},
	
	{0x003, 0x0230},	  //RAM Write Control
	{0x005, 0x0000},
	{0x006, 0x0000},
	{0x200, 0x0000},	 //??? KW: R200 and R210 Should be 0x0000
	{0x201, 0x0000},
	{0x210, 0x0000},	 //Sets up the window size, may not be needed since duplicated elsewhere
	{0x211, 0x01DF},	 // 480
	{0x212, 0x0000},
	{0x213, 0x0167}	     // 360
};

/* ===========================================================================
 * LCD SVP Interface Functions
 * ===========================================================================*/

/***************************************************************************
 * Function:      svp_lcdOptisTmdGetConfig
 * Description:  API to retrieve OPTIS TMD panel timing and other configurations  
 *              
 * Note:          Derived from Nessus SVP file of OPTIS-TMD
 **************************************************************************/
BOOL svp_lcdSharpMirageGetConfig(LcdModule_t *pLcdConfig)
{
    pLcdConfig->type = LCD_TYPE_MIPI_DBI_8BIT;
    pLcdConfig->mode = LCD_MODE_CMD;
    pLcdConfig->pixelFormat = PIXEL_FMT_RGB565;
    pLcdConfig->xres = LCD_MIRAGE_SHARP_WIDTH;
    pLcdConfig->yres = LCD_MIRAGE_SHARP_HEIGHT;
    pLcdConfig->lcdTimings.command.reserved[0] = 0x8e56;    /* RXL_RXH_TXL_TXH */
    pLcdConfig->lcdTimings.command.reserved[1] = 0x0000;    /* BFW_EFW */
    pLcdConfig->lcdTimings.command.reserved[2] = 0x0000;    /* HSW_PCD */
    pLcdConfig->lcdTimings.command.reserved[3] = 0x0000;    /* WRITE_PCD */
        
    return TRUE;
}

/***************************************************************************
 * Function:      svp_lcdOptisTmdInit
 * Description:  Initialize OPTIS TMD panel by sending required command sequence  
 *              
 * Note:          Derived from Nessus SVP file of OPTIS-TMD
 **************************************************************************/
BOOL svp_lcdSharpMirageInit(LcdModule_t *pLcdConfig)
{
    WORD ret;
	WORD cnt = 0;

    /* Wake up LCD from sleep */
	DispParallelWriteCmd(pLcdConfig, 0xFFFF);
    RE_RimSleep(1);
	DispParallelWriteCmd(pLcdConfig, 0xFFFF);
	DispParallelWriteCmd(pLcdConfig, 0xFFFF);
    RE_RimSleep(5);
	DispParallelWriteCmd(pLcdConfig, 0x0000);
	DispParallelWriteCmd(pLcdConfig, 0x0000);

    /* Generate RESET Pulse: low 10ms-> high  */
	RE_PadClear(LCD_RST_N, PAD_INPUT_ENABLE );
    RE_PadClear(LCD_RST_N, PAD_OUTPUT);
	RE_PadSet(LCD_RST_N, PAD_OUTPUT_ENABLE );
    RE_RimSleep(10);
	RE_PadClear(LCD_RST_N, PAD_INPUT_ENABLE );
    RE_PadSet(LCD_RST_N, PAD_OUTPUT);    
	RE_PadSet(LCD_RST_N, PAD_OUTPUT_ENABLE );

    RE_RimSleep(100);

    /* Write command 0x0000 */
	DispParallelWriteCmd(pLcdConfig, 0x0000);
	DispParallelWriteCmd(pLcdConfig, 0x0000);
	DispParallelWriteCmd(pLcdConfig, 0x0000);
	DispParallelWriteCmd(pLcdConfig, 0x0000);
    RE_RimSleep(100);

    /* ToDo: Copy from Nessus SVP for OPTIS_TMD. Should replace magic number with definitions */
    DispParallelWriteCmd(pLcdConfig, 0x06E0);    
    ret = DispParallelReadData(pLcdConfig);
    PRINTN("svp_lcdShartMirageInit: ret = 0x%08x",ret);

    /* 0x7F21 = Sharp, 0x7F22 TMD mirage, 0x7F24 SEC Mirage  */
    if ( ret == LCD_MIRAGE_SHARP_ID)

    {
        while (cnt < (NELEMENTS(tmdPowerOnData)))
        {
            DispParallelWriteCmd(pLcdConfig, tmdPowerOnData[cnt].reg);
            DispParallelWriteData(pLcdConfig, tmdPowerOnData[cnt].data);
			cnt++;
        }	
    }
    else
    {
        WARN("Unsupported LCD Module - not Mirage Sharp!");
        return FALSE;
    }	

    /* Turn on the display glass (IC already on) */
	DispParallelWriteCmd(pLcdConfig, 0x0000);
	DispParallelWriteCmd(pLcdConfig, 0x0000);
	DispParallelWriteCmd(pLcdConfig, 0x111);
	DispParallelWriteData(pLcdConfig, 0x6541);
    RE_RimSleep(100);
    /* Reg 0x120 = 0x0E0F */	
	DispParallelWriteCmd(pLcdConfig, 0x120);
	DispParallelWriteData(pLcdConfig, 0x0E0F);
	
    /* Set up window to write or read */
	DispParallelWriteCmd(pLcdConfig, 0x0000);
	DispParallelWriteCmd(pLcdConfig, 0x0000);
	DispParallelWriteCmd(pLcdConfig, 0x210);
	DispParallelWriteData(pLcdConfig, 0x00);               // HSA 
	DispParallelWriteCmd(pLcdConfig, 0x211);
	DispParallelWriteData(pLcdConfig, pLcdConfig->xres);   // HEA

	DispParallelWriteCmd(pLcdConfig, 0x212);
	DispParallelWriteData(pLcdConfig, 0x00);               // VSA
	DispParallelWriteCmd(pLcdConfig, 0x213);
	DispParallelWriteData(pLcdConfig, pLcdConfig->yres);   // VEA 

	DispParallelWriteCmd(pLcdConfig, 0x200);
	DispParallelWriteData(pLcdConfig, 0x00);               // HSA 
	DispParallelWriteCmd(pLcdConfig, 0x201);
	DispParallelWriteData(pLcdConfig, 0x00);               // HEA

    return TRUE;
}

void svp_lcdSharpMirageWriteStart(LcdModule_t *pLcdConfig)
{
    PRINT("svp_lcdSharpMirageWriteStart");

    /* Set start address and issue write command */
	DispParallelWriteCmd(pLcdConfig, 0x200);
	DispParallelWriteData(pLcdConfig, 0x00);               // HSA 
	DispParallelWriteCmd(pLcdConfig, 0x201);
	DispParallelWriteData(pLcdConfig, 0x00);               // HEA
	DispParallelWriteCmd(pLcdConfig, 0x202);
}

void svp_lcdSharpMirageWriteEnd(LcdModule_t *pLcdConfig)
{
    DispParallelWriteCmd(pLcdConfig, 0x200);
}

void svp_lcdSharpMirageReadStart(LcdModule_t *pLcdConfig)
{
    /* Set start address and issue read command */
	DispParallelWriteCmd(pLcdConfig, 0x200);
	DispParallelWriteData(pLcdConfig, 0x00);               // HSA 
	DispParallelWriteCmd(pLcdConfig, 0x201);
	DispParallelWriteData(pLcdConfig, 0x00);               // HEA
    DispParallelWriteCmd(pLcdConfig, 0x202);
}

void svp_lcdSharpMirageReadEnd(LcdModule_t *pLcdConfig)
{
    return;
}

/***************************************************************************
 * Function:      svp_lcdOptisTmdIoctl
 * Description:   Ioctl of  OPTIS TMD panel  
 *              
 * Note:          Derived from Nessus SVP file of OPTIS-TMD
 **************************************************************************/
BOOL svp_lcdSharpMirageIoctl(LcdModule_t *pLcdConfig, int cmd, void *para)
{
    switch (cmd)
    {
    /* TODO: implement ioctls on the fly
        //case LCD_IOCTL_SET_ROTATE:
        //    DispGetOutpath(displayId);
        //    break;
        //case LCD_IOCTL_GET_ROTATE:
        //    break;
        //case LCD_IOCTL_SET_MIRROR:
        //    break;
        //case LCD_IOCTL_GET_MIRROR:
        //        break;
        //case LCD_IOCTL_ENABLE_TE:
            // Call dcsWrite for tear on or off
        //    break;
        //case LCD_IOCTL_VEDIOMODE_EN:
        //   break;
    */
    }
    return TRUE;
}

