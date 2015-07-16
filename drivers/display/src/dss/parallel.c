/*****************************************************************************
 * Filename:    parallel.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Victor Chen
 *
 * Created:     Aug 16, 2010
 *
 * Description: Display driver for parallel based LCD (DBI or DPI) implementation. Based on Orinoco board.
 *                  
 *****************************************************************************/

#include "display_internal.h"
#include "display_reg.h"
#include "gpio_platform.h"
//#include "pmic.h"

#define SRCGROUP GROUP_PLT_DRV
#define SRCFILE  FILE_DISP_HAL

#if !defined( LCD_DEFAULT_FUNC )
#define LCD_DEFAULT_FUNC        1
#endif

/* ---------------------------------------------------------------------------
 * Defines and data variables
 * --------------------------------------------------------------------------- */
#define DispController  (DisplayDriver.dispcCtl)
#define DispCtrlStatus  (DisplayDriver.status.dispcStatus)
#define LcdMainModule   (DisplayDriver.lcdModule[0])
#define LcdSubModule    (DisplayDriver.lcdModule[1])

#define NUM_LCD_GPIOS_DBI8   12

#define LCD_PARALLEL_IF_DED_WR_CLK_MHZ      312 // MHz
#define LCD_PARALLEL_IF_DED_RD_CLK_MHZ      26  // MHz

#define LCD_PARALELLEL_IF_AHB_TIMING    0xffff0211 // Read/Write timings

/* Command Type for smart panel: copy from TavorTD display driver on Nessus */
typedef enum 
{
    LCD_CMD_COMMAND_WRITE       = 0x01,
	LCD_CMD_COMMAND_READ		= 0x02,
    LCD_CMD_DATA_READ           = 0x82,
    LCD_CMD_DATA_WRITE          = 0x81,
} LcdCmdType;

typedef struct 
{
    BYTE data;    /*LSB. TavorTD supports 8, 16/18 bits data but the interface we are using is only 8 bits. */
    BYTE command; /*MSB. From LCD_CMD_xxxx below */
}  LcdCtrlCmd_t;


/* List of GPIO pins that we will be using for the interface */
static RE_Line lcdDbi8GpioList[ NUM_LCD_GPIOS_DBI8 ];

/* Configuration for the GPIO pins for the LCD Panel */
static const RE_PadConfiguration LcdPadConfiguration = 
{
    LCD_DEFAULT_FUNC,           /* altFunctionMode */
    PAD_DIRECTION_OUTPUT,       /* initialDirection */
    FALSE,                      /* initialLevel */
    FALSE,                      /* pullUpEnable */
    FALSE,                      /* pullDownEnable */
    PAD_INTERRUPT_NONE,         /* interruptType */
    NULL,                       /* interruptHandler */
    FALSE,                      /* interruptsInitiallyEnabled */
    FALSE                       /* interruptDebounceEnabled */
};

/* Configuration for the GPIO pins for the LCD Panel */
static const RE_PadConfiguration LcdPadConfiguration_data = 
{
    LCD_DEFAULT_FUNC,                          /* altFunctionMode */
    PAD_DIRECTION_OUTPUT,       /* initialDirection */
    FALSE,                      /* initialLevel */
    FALSE,                      /* pullUpEnable */
    TRUE,                      /* pullDownEnable */
    PAD_INTERRUPT_NONE,         /* interruptType */
    NULL,                       /* interruptHandler */
    FALSE,                      /* interruptsInitiallyEnabled */
    FALSE                       /* interruptDebounceEnabled */
};


static const RE_DeviceSpecificPadConfiguration lcdGpioDetail_SleepHi =
{
    DEFAULT_STRENGTH_SLEW,      /* default drive */
    SLEEP_SEL_DEFAULT,          /* sleepSel */
    SLEEP_DATA_HIGH,            /* sleepData */
    SLEEP_DIRECTION_OUTPUT,     /* sleepDirection */
    PULL_SEL_DEFAULT,           /* pullSel */
    USE_RDH_DEFAULT,            /* useRdh */
    EDGE_DETECT_DEFAULT,        /* edgeDetectEnable */
    FALSE,                      /* detectRisingEdge */
    FALSE                       /* detectFallingEdge */
};

static const RE_DeviceSpecificPadConfiguration lcdGpioDetail_SleepLow = 
{
    DEFAULT_STRENGTH_SLEW,      /* default drive */
    SLEEP_SEL_DEFAULT,          /* sleepSel */
    SLEEP_DATA_LOW,             /* sleepData */
    SLEEP_DIRECTION_OUTPUT,     /* sleepDirection */
    PULL_SEL_DEFAULT,           /* pullSel */
    USE_RDH_DEFAULT,            /* useRdh */
    EDGE_DETECT_DEFAULT,        /* edgeDetectEnable */
    FALSE,                      /* detectRisingEdge */
    FALSE                       /* detectFallingEdge */
};

static const RE_DeviceSpecificPadConfiguration lcdGpioDetail_SleepLow_data = {
    DEFAULT_STRENGTH_SLEW,      // default drive
    SLEEP_SEL_DEFAULT,          // sleepSel
    SLEEP_DATA_LOW,             // sleepData
    SLEEP_DIRECTION_OUTPUT,     // sleepDirection
    PULL_SEL_MFPR,              // pullSel
    USE_RDH_DEFAULT,            // useRdh
    EDGE_DETECT_DEFAULT,        // edgeDetectEnable
    FALSE,                      // detectRisingEdge
    FALSE                       // detectFallingEdge
};


typedef DWORD DispcRegInitValue_t[2];

static DispcRegInitValue_t dispcParallelRegInitValue[] = {
    // Zero out the TV Video Frame Registers
    // These registers are supposed to be 0 at
    // reset and writing to them may cause problems
    // because they may not actually exist on some
    // versions of the processor.
    // Commented out for now, enable if there is
    // unexplained corruption.
    { LCD_DMA_START_ADDR_Y_REG(0), 0x00000000 },
    { LCD_DMA_START_ADDR_Y_REG(1), 0x00000000 },
    { LCD_DMA_START_ADDR_U_REG(0), 0x00000000 },
    { LCD_DMA_START_ADDR_U_REG(1), 0x00000000 },
    { LCD_DMA_START_ADDR_V_REG(0), 0x00000000 },
    { LCD_DMA_START_ADDR_V_REG(1), 0x00000000 },
    { LCD_DMA_START_ADDR_C_REG(0), 0x00000000 },
    { LCD_DMA_START_ADDR_C_REG(1), 0x00000000 },
    { LCD_DMA_PITCH_YC_REG, 0x00000000 },
    { LCD_DMA_PITCH_UV_REG, 0x00000000 },
    { LCD_DMA_OVSA_HPXL_VLN_REG, 0x00000000 },
    { LCD_DMA_HPXL_VLN_REG, 0x00000000 },
    { LCD_DMAZM_HPXL_VLN_REG, 0x00000000 },
    { LCD_GRA_START_ADDR_REG(0), 0x00000000 },
    { LCD_GRA_PITCH_REG, 0x00000000 },
    { LCD_GRA_OVSA_HPXL_VLN_REG, 0x00000000 },
    { LCD_GRA_HPXL_VLN_REG, 0x00000000 },
    { LCD_GRAZM_HPXL_VLN_REG, 0x00000000 },
    { LCD_PN_V_H_TOTAL_REG, 0x00000000 },
    { LCD_PN_V_H_ACTIVE_REG, 0x00000000 },
    { LCD_PN_H_PORCH_REG, 0x00000000 },
    { LCD_PN_V_PORCH_REG, 0x00000000 },
    { LCD_PN_CTRL_REG(0),
        CFG_PN_NOBLENDING & ~CFG_PN_GAMMA_ENA & ~CFG_PN_CSBH_ENA 
        & ~CFG_PN_PALETTE_ENA & ~CFG_ARBFAST_ENA
        & ~CFG_HWC_ENA | CFG_DMA_FORMAT_RGB565 
        | CFG_GRA_FORMAT_RGB565                                                       /* Default to RGB565 */
        & ~CFG_GRA_FTOGGLE & ~CFG_GRA_HSMOOTH & ~CFG_GRA_TSTMODE 
        & ~CFG_GRA_SWAPRB & ~CFG_GRA_SWAPUV & ~CFG_GRA_SWAPYU 
        & ~CFP_YUV2RGB_GRA & ~CFG_GRA_ENA & ~CFG_DMA_FTOGGLE & ~CFG_DMA_HSMOOTH
        & ~CFG_GRA_TSTMODE & ~CFG_DMA_SWAPRB & ~CFG_DMA_SWAPUV & ~CFG_DMA_SWAPYU
        & ~CFP_YUV2RGB_DMA & ~CFG_DMA_ENA 
    },
    { LCD_PN_CTRL_REG(1), CFG_FRAME_TRIG | CFG_PN_VSYNC_MODE_DMA_FRAME_TRIG |  CFG_PXLCMD },

    // SPU_IRQ_ENA - 0x00000000 (turn off ALL IRQs)
    { LCD_SPU_IRQ_ENA_REG, 0x00000000 },

    // LCD_TOP_CTRL 
    // TODO: May need to revisit this.  This is setting DMA bursts to 64 bytes,
    // it is setting upauto detect for swapping of panel paths (?), no I/O pads
    // are being inverted (which is probably for dumb panel/TV anyway), MIPI 
    // DSI1 or CMU Input Color mode is being set to 24 bit RGB888.
    // TV DMA objects are going to TV (which we don't use at present anyway) and
    // Panel DMA objects go to the panel (CFG_ALL2PNORTV_PNANDTV).
    { LCD_TOP_CTRL_REG, CFG_ALL2PNORTV_PNANDTV },

    // LCD_AFA_ALL2ONE - 0x00000000
    // Not enabling any zooming, vertical smoothing or changing alpha blending
    // layer ordering (since we aren't doing any video alpha blending).
    { LCD_AFA_ALL2ONE_REG, 0x00000000 },

    { LCD_PN_IOPAD_CONTROL_REG, 0x00000008 }
};

static WORD lcdWrClkMhz = LCD_PARALLEL_IF_DED_WR_CLK_MHZ;
static WORD lcdRdClkMhz = LCD_PARALLEL_IF_DED_RD_CLK_MHZ;

/* ---------------------------------------------------------------------------
 * Local static APIs
 * --------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------
 * Forward declare for DBI interface of smart panel
 * --------------------------------------------------------------------------- */
static BYTE dbiReadByte(BOOL  A0);
static void dbiWriteBytes(BOOL A0, BYTE *bytes, BYTE len);
static void setLcdGpioPins( RE_Line *lcdGpioList, DWORD gpioNum );


/* ---------------------------------------------------------------------------
 * Function implementation
 * --------------------------------------------------------------------------- */
BOOL DispParallelInit(void)
{
    /* Config LCD Pins: GPIO settings for DBI-8 smart panel */
    lcdDbi8GpioList[0]= LCD_RD_N;
    lcdDbi8GpioList[1]= LCD_A0;
    lcdDbi8GpioList[2]= LCD_WR_N;
    lcdDbi8GpioList[3]= LCD_CS_N;
    lcdDbi8GpioList[4]= LCD_D0;
    lcdDbi8GpioList[5]= LCD_D1;
    lcdDbi8GpioList[6]= LCD_D2;
    lcdDbi8GpioList[7]= LCD_D3;
    lcdDbi8GpioList[8]= LCD_D4;
    lcdDbi8GpioList[9]= LCD_D5;
    lcdDbi8GpioList[10]= LCD_D6;
    lcdDbi8GpioList[11]= LCD_D7;

    return (TRUE);
}

/***************************************************************************
 * Function:      DispParallelEnable
 * Description:  API to initialize DSS parallel interface  
 *              
 * Note:          Derived from LcdCtrlInit() on Nessus TavorTD LCD driver
 **************************************************************************/
BOOL DispParallelEnable()
{
    LcdModule_t* pLcdModule = (LcdModule_t *)(&LcdMainModule);
	const LcdModSvpFuncs_t * svpFuncs;
    DWORD cnt = 0;
	DWORD regVal = 0;
	
    if (!pLcdModule->valid)
	    return FALSE;

    /* Retrieve LCD timing configuration from LCD SVP based on lcdMoudleId*/
    svpFuncs = svp_lcdGetFuncs(pLcdModule);
	if (svpFuncs == NULL)
		return FALSE;
	svpFuncs->svpGetSettings(pLcdModule);

    /*Turn on parallel lcocks; Set GPIO; Turn off DMA */
    if (pLcdModule->type == LCD_TYPE_MIPI_DBI_8BIT)               /* Currently only support smart Panel with 8-bits interface */
    {
        setLcdGpioPins(&lcdDbi8GpioList[0], NUM_LCD_GPIOS_DBI8);  /* Set GPIO confiurations for DBI-8 panel */
    }
	else if (pLcdModule->type == LCD_TYPE_MIPI_DPI)               /* Not supported on Orinoco */
		return FALSE;
	else
		return FALSE;

    /* Disable graphics and video DMA transfer:  */
    WriteRegDwordAnd( LCD_PN_CTRL_REG(0), ~( CFG_GRA_ENA | CFG_DMA_ENA ));
	/* Configure Display controller */
	for( cnt = 0; cnt < sizeof( dispcParallelRegInitValue ) / sizeof( DispcRegInitValue_t ); cnt++ ) 
	{
		PRINT2N( "Writing 0x%08X to register address 0x%08X", dispcParallelRegInitValue[cnt][1], dispcParallelRegInitValue[cnt][0] );
		WriteRegDword( dispcParallelRegInitValue[cnt][0], dispcParallelRegInitValue[cnt][1]);
	}

    /* Configure input/output pixel format. */
	regVal = ReadRegDword( LCD_PN_CTRL_REG(0) ) & CFG_GRA_FORMAT_MASK;;
	regVal |= CFG_GRA_FORMAT_RGB565;
	WriteRegDword( LCD_PN_CTRL_REG(0), regVal);

	regVal = ReadRegDword( LCD_SMPN_CTRL_REG);
	regVal &= ~CFG_SMPN_MODE_MASK;
	if (pLcdModule->pixelFormat == PIXEL_FMT_RGB888P)
	    WriteRegDword( LCD_SMPN_CTRL_REG, regVal | CFG_SMPN_MODE_RGB888);
	else if (pLcdModule->pixelFormat == PIXEL_FMT_RGB1555)
        WriteRegDword( LCD_SMPN_CTRL_REG, regVal | CFG_SMPN_MODE_RGB666);
	else
        WriteRegDword( LCD_SMPN_CTRL_REG, regVal | CFG_SMPN_MODE_RGB565);
	
	/* Enable All interrupts */
	WriteRegDword( LCD_SPU_IRQ_ENA_REG, ALL_IRQ_ENA);               

	/* Set background color */
	WriteRegDword(LCD_PN_BLANKCOLOR_REG, 0x00ff00f0); 
	
    return TRUE;
}

/***************************************************************************
 * Function:      DispParallelDisable
 * Description:  Disable parallel interface  
 *              
 * Note:          Derived from LcdCtrlInit() on Nessus TavorTD LCD driver
 **************************************************************************/
BOOL DispParallelDisable()
{
    /* Disable Display controller: Disable Graphics and Video DMA transfer */
    WriteRegDwordAnd( LCD_PN_CTRL_REG(0), ~( CFG_GRA_ENA | CFG_DMA_ENA ) );
	
    return TRUE;
}

/***************************************************************************
 * Function:      DispParallelConfigTiming
 * Description:  API to initialize DSS parallel interface  
 *              
 * Note:          Derived from LcdconfigureInterfaceTiming() on Nessus TavorTD LCD driver
 **************************************************************************/
void DispParallelConfigTiming(LcdModule_t* lcdModule)
{
    BYTE wrClkDiv, rdClkDiv;
    DWORD cmdTiming;
	DWORD regVal;
	
    if (!lcdModule->valid)
	    return;
	
    /* ToDo: Calculate clock divisors; Disable pixel clocks; Apply read/write timings, clock divisors and re-enable the pixel clocks */
    if (lcdModule->type != LCD_TYPE_MIPI_DBI_8BIT)
		return;
 
    wrClkDiv = ((LCD_DISPLAY_CLK1_NOM_MHZ << 1) - 1 ) / lcdWrClkMhz;
    rdClkDiv = ((LCD_DISPLAY_CLK2_NOM_MHZ << 1) - 1 ) / lcdRdClkMhz;
	cmdTiming = (lcdModule->lcdTimings.command.reserved[0]) << 16; 

    WriteRegDword( LCD_SCLK_DIV_REG, LCD_xCLK_DISABLE );
    WriteRegDword( LCD_TCLK_DIV_REG, LCD_xCLK_DISABLE );

    /* Apply read/write timings, clock divisors and re-enable the pixel clocks */
    PRINT3N("LcdConfigureInterfaceTiming: Applying wrClkDiv=%d, rdClkDiv=%d, "
            "SMPN_CTRL=0x%08X", wrClkDiv, rdClkDiv,  cmdTiming | CFG_SMPN_ENA | CFG_SLV_ENA );

    /* TODO: fix the ORing of 0x1110000 */
	regVal = ReadRegDword(LCD_SMPN_CTRL_REG);
	regVal = regVal & (~LCD_SMTN_CTLR_RXTX_MASK);
	regVal |= (CFG_SMPN_ENA | CFG_SLV_ENA | cmdTiming);

	WriteRegDword( LCD_SMPN_CTRL_REG, regVal);

    if( wrClkDiv > 1 ) {
        WriteRegDword( LCD_SCLK_DIV_REG, (LCD_xCLK_DIV_xCLK_SOURCE_SELECT_LCD_DISP_CLK1 | LCD_xCLK_INT_DIV( wrClkDiv )) );
    } else {
        WriteRegDword( LCD_SCLK_DIV_REG, LCD_xCLK_DIV_xCLK_SOURCE_SELECT_LCD_DISP_CLK1 | LCD_xCLK_INT_DIV( 1 ) );
    }
	WriteRegDword( LCD_SCLK_DIV_REG, 0x40000001);     // Copy from BBU code, set panel path pixel clock to be 312/8 = 39Mhz 
    WriteRegDword( LCD_TCLK_DIV_REG, (LCD_xCLK_DIV_xCLK_SOURCE_SELECT_LCD_DISP_CLK1 | LCD_xCLK_INT_DIV( rdClkDiv )) );

}

/***************************************************************************
 * Function:      DispParallelSendVideoPacket
 * Description:  When sending pixels to external, this API should be called to write required command
 *                   sequence to external LCD module
 *              
 * Note:          The command sequence is related to LCD module type
 **************************************************************************/
void DispParallelSendVideoPacket(DisplayId_t displayId)
{
	const LcdModSvpFuncs_t * svpFuncs;
    LcdModule_t* pLcdModule;
	if (displayId == LcdMainModule.displayId) 
		pLcdModule = (LcdModule_t *)(&LcdMainModule);
	else
		pLcdModule = (LcdModule_t *)(&LcdSubModule);

    /* Initialize the connected LCD panel */
    svpFuncs = svp_lcdGetFuncs(pLcdModule);
    if (svpFuncs != NULL)
        svpFuncs->svpWriteStart(pLcdModule);
}

/* Read pixels back from specifced LCD mdoule */
DWORD DispParallelReadVideoPacket(DisplayId_t displayId, BYTE *buf, DWORD pixels)
{
	/* Read one pixel */
	BOOL A0 = TRUE;
	DWORD cnt = 0;
    LcdModule_t* lcdModule;
	const LcdModSvpFuncs_t * svpFuncs;
	DWORD totalBytes;

	if (displayId == LcdMainModule.displayId) 
		lcdModule = (LcdModule_t *)(&LcdMainModule);
	else
		lcdModule = (LcdModule_t *)(&LcdSubModule);
	
	/* Initialize the connected LCD panel */
	svpFuncs = svp_lcdGetFuncs(lcdModule);

	if ((lcdModule->type == LCD_TYPE_MIPI_DBI_8BIT) || (lcdModule->type == LCD_TYPE_MIPI_DBI_16BIT))
	{
		if (svpFuncs != NULL)
			svpFuncs->svpReadStart(lcdModule);

		totalBytes = pixels * ((lcdModule->pixelFormat == PIXEL_FMT_RGB888P)? 3 : 2);
		while (totalBytes-- > 0)
		{
		   buf[cnt] = dbiReadByte(A0);
		   cnt++;
		}
	}
	else 
		return 0;
	
	return pixels;
}


void DispParallelWriteData(LcdModule_t *lcdModule, WORD data)
{
    BOOL A0 = TRUE;
	BYTE bytes[2];

    /* According to LCD module bus width */
    bytes[0] = (BYTE) (data >> 8);
    bytes[1] = (BYTE) data;
    dbiWriteBytes(A0, bytes, (BYTE) 2);
}

void DispParallelWriteCmd(LcdModule_t *lcdModule, WORD cmd)
{
    BOOL A0 = FALSE;
	BYTE bytes[2];

    if ((lcdModule->type == LCD_TYPE_MIPI_DBI_8BIT) || (lcdModule->type == LCD_TYPE_MIPI_DBI_16BIT))
    {
		bytes[0] = (BYTE) (cmd >> 8);
		bytes[1] = (BYTE) cmd;
		dbiWriteBytes(A0, bytes, (BYTE) 2);
    }	
}

/* Read data back */
DWORD DispParallelReadData(LcdModule_t *lcdModule)
{
    BOOL A0 = TRUE;
	DWORD data = 0;
	
    /* According to LCD module bus width */
    if ((lcdModule->type == LCD_TYPE_MIPI_DBI_8BIT) || (lcdModule->type == LCD_TYPE_MIPI_DBI_16BIT))
    {
        data = dbiReadByte(A0);
		data <<= 8;
		data |= dbiReadByte(A0);
        if (lcdModule->pixelFormat == PIXEL_FMT_RGB888P)    /* 3 bytes if RGB888 packed */
        {
			data <<= 8;
		    data |= dbiReadByte(A0);
        }	
    }	
	else
		return (DWORD) NULL;

	return data;
}

/* Read command data back */
DWORD DispParallelReadCmd(LcdModule_t *lcdModule)
{
    BOOL A0 = FALSE;
    /* According to LCD module bus width */
    if ((lcdModule->type == LCD_TYPE_MIPI_DBI_8BIT) || (lcdModule->type == LCD_TYPE_MIPI_DBI_16BIT))
		return dbiReadByte(A0);
	else
		return (DWORD) NULL;
}

/* Read One Byte from LCD panel */
static BYTE dbiReadByte(BOOL  A0)
{
	DWORD tempRegister;
	DWORD tempRXDATA;
	DWORD regVal = 0;

    RE_RimLockCountingSemaphore( &DispController.waitForReadInt);

	/* Enable Slave Path so we can actually write/read data via the slave path */
	WriteRegDwordOr( LCD_SMPN_CTRL_REG, CFG_SLV_ENA );
	
	/* Setup the Panel READ FIFO Descriptor */
	if( A0 ) {
		tempRegister = LCD_CMD_DATA_READ << 8; 
	} else {
		tempRegister = LCD_CMD_COMMAND_READ << 8;
	}

	PRINTN( "dbiReadByte: pre-read value of ISA_RXDATA: %08X",
			 ReadRegDword( LCD_ISA_RXDATA_REG ) );

    /* Back up timings */
    regVal = ReadRegDword(LCD_SMPN_CTRL_REG);
    /* Set timing */
    WriteRegDword( LCD_SMPN_CTRL_REG, LCD_PARALELLEL_IF_AHB_TIMING);
    WriteRegDword( LCD_PN_CTRL_REG(0),  0x00000000 );    /* Disable DMA */

	/* turn on the Slave Port IRQ */
	WriteRegDwordOr( LCD_SPU_IRQ_ENA_REG, ( ALL_IRQ_ENA & ~VSYNC_IRQ_ENA ) );
	WriteRegDword( LCD_SLV_PORT_REG, tempRegister );

    RE_RimSleep(1);                                               /* Delay before read */
	
    /* Retrieve the RXDATA and read the byte */
	tempRXDATA = ( ReadRegDword( LCD_ISA_RXDATA_REG ) & 0xFF ); 
	
	PRINTN( "dbiReadByte: 0x%2.2x", ISA_RXDATA_GET_DATA( tempRXDATA ) );

    /* restore original timings */
    WriteRegDword(LCD_SMPN_CTRL_REG, regVal);

    /* Release semaphore */
    RE_RimUnlockCountingSemaphore( &DispController.waitForReadInt);
	
	return ISA_RXDATA_GET_DATA( tempRXDATA );
}
	
/* Write one or 2 bytes to AHB Slave path data port */
static void dbiWriteBytes(BOOL A0, BYTE *bytes, BYTE len)
{
    /* TODO - batch mode to send cmd sequence  */
    BYTE writeCmd = A0 ? LCD_CMD_DATA_WRITE : LCD_CMD_COMMAND_WRITE;
	DWORD regVal0 = 0;
    DWORD regVal1 = 0;
	
    if( len != 1 && len != 2 ) 
    {
        WARNN( "writeBytes: numBytes=%u (it must be either 1 or 2)", len);
        return;
	}

    // RE_RimLockSemaphore( &DispController.waitForCmdInt);
    RE_RimLockCountingSemaphore( &DispController.waitForCmdInt);

    /* Back up timings */
    regVal1 = ReadRegDword(LCD_SMPN_CTRL_REG);

    /* Set timings for writing */
    WriteRegDword( LCD_SMPN_CTRL_REG, LCD_PARALELLEL_IF_AHB_TIMING);
    WriteRegDword( LCD_PN_CTRL_REG(0),  0x00000000 );    /* Disable DMA */
		
    /* Compose command (1 or 2) and write for AHB Slave path data port */
    regVal0 = (DWORD)((writeCmd << 8) | bytes[0]);
    if (len == 2)
    {
		regVal0 = (DWORD)(((writeCmd << 8 | bytes[1]) << 16) | regVal0);
    }	

	/* Use AHB path to send command */
	PRINTN("Write 0x%08x to AHB slave port and wait interrupt",regVal0);

    /* Enable SLV interrupt and write data to SLV */
    WriteRegDwordOr( LCD_SPU_IRQ_ENA_REG, SLV_IRQ_ENA); 
    WriteRegDword( LCD_SLV_PORT_REG, regVal0);           /* Write to AHB slave port and trig interrupt */

    /* restore original timings */
    WriteRegDword(LCD_SMPN_CTRL_REG, regVal1);
	return;
}

static void setLcdGpioPins( RE_Line *lcdGpioList, DWORD gpioNum )
{
    DWORD count;
    const RE_DeviceSpecificPadConfiguration *dspc;
	const RE_PadConfiguration *padConfig;
//    Driver_t *pmicDrv;

    /* Loop through the lists, and call PadConfigure */
    for( count = 0; count < gpioNum; count++ ) 
	{
	    padConfig = &LcdPadConfiguration;
        if( lcdGpioList[count] == LCD_RD_N  ||
            lcdGpioList[count] == LCD_WR_N  ||
            lcdGpioList[count] == LCD_CS_N ) 
        {
           /**
                    * Drive strength is specified in config_gpio.xml.  The only difference
                    * is whether the line is pulled HI or LOW when
                    * the processor is asleep.
                    */
            dspc = &lcdGpioDetail_SleepHi;
        } 
		else 
		{
            if(( lcdGpioList[count] == LCD_D0 ) 
            || ( lcdGpioList[count] == LCD_D1 )
            || ( lcdGpioList[count] == LCD_D2 )
            || ( lcdGpioList[count] == LCD_D3 )
            || ( lcdGpioList[count] == LCD_D4 )
            || ( lcdGpioList[count] == LCD_D5 )
            || ( lcdGpioList[count] == LCD_D6 )
            || ( lcdGpioList[count] == LCD_D7 ) ) {
                dspc = &lcdGpioDetail_SleepLow_data;
                padConfig = &LcdPadConfiguration_data;
            } else {
                dspc = &lcdGpioDetail_SleepLow;
            }		
            dspc = &lcdGpioDetail_SleepLow;
        }
        PRINT2N( "Configuring LCD gpio line %d to %p.", 
                 GPIO_NUM(lcdGpioList[count]), (BDPARM)dspc );
        RE_PadConfigure( lcdGpioList[count], padConfig, dspc );
    }

#if 0
    /* Enable Backlight */
    pmicDrv = DrvGetHandle(IODRV_ID_PMIC);
    if (pmicDrv == NULL)
    {
	    PRINT("Failed to enable backlight.");
	    return;
    }	
    PmicInit();
    PortofinoInit();
    pmicDrv->Control(pmicDrv, PMIC_CMD_ENABLE_BK_LIGHT, NULL);
#endif
}

