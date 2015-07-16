/*****************************************************************************
 * Filename:    display.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Victor Chen
 *
 * Created:     July 16, 2010
 *
 * Description: Display driver implementation on TavorTD or MMP2/MMP3. Mostly implements dispaly controller
 *                  related code. (Paralle, DSI and HDMI implemented in seperately files)
 *
 *****************************************************************************/

#include <string.h>
#include "display_internal.h"
#include "display_reg.h"

/* ---------------------------------------------------------------------------
 * Defines and data variables
 * --------------------------------------------------------------------------- */
#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE FILE_DISP_DEV

#define DispCtrlConfig  (DisplayDriver.config.dispcConfig)
#define DispCtrlStatus  (DisplayDriver.status.dispcStatus)
#define DispCtrlStatis  (DisplayDriver.status.dispcConfig)
#define DispController  (DisplayDriver.dispcCtl)
#define LcdMainModule   (DisplayDriver.lcdModule[0])
#define LcdSubModule    (DisplayDriver.lcdModule[1])

/* TavorTD/MMP3 Pipeline Group */
typedef enum 
{
    PIPELINE_GROUP_LCD0 = 0x00,
    PIPELINE_GROUP_TV,
    PIPELINE_GROUP_LCD1,
    PIPELINE_GROUP_MAX
} DispPipelineGroup_t;

/* TavorTD/MMP3 Pipeline Configuration */
typedef struct 
{
    BOOL enabled;
    DispPipelineGroup_t index;
    DispPipelineOutPath_t destPath;
}DispcPipelineCfg_t;


/* ---------------------------------------------------------------------------
 * Global variables
 * --------------------------------------------------------------------------- */
DispDrv_t  DisplayDriver;                      /* Display Driver instance of DSS */

/* ---------------------------------------------------------------------------
 * Local Fuctions
 * --------------------------------------------------------------------------- */
static BOOL dispSetLcdTiming(LcdModule_t *lcdModule);
	
/* Ioctls implementations - clear screen, set background color etc. The implementation of these APIs are depended on DSS hardware  
static BOOL dispClearScreen(DisplayId_t displayId);
static BOOL dispSetBgColor(DisplayId_t displayId, BYTE color);
static BOOL dispSetRotation(DisplayId_t displayId, DispRotAngle_t angle);
*/

/* Interrrupt service routine */
static void dispcCtrlIsr(void);

/* ---------------------------------------------------------------------------
 * Local static variables
 * --------------------------------------------------------------------------- */
/**
  * Predefined pixel pipeline settings on TavorTD or MMP3 - Each group has graphics, video pipelines. Graphics pipeline is used
  * for primary surface (backplane) of RGB only format pixels; video pipeline is used for overlay surface of RGB or YUV format pixels. 
  */
static const DispcPipelineCfg_t dispcPixelPipeline [PIPELINE_CNT][PIPELINE_GROUP_MAX] =     /* Pixel Pipeline Configurations */
{
   
   /* DBI parallell only(Smart Panel) config */
   {
       {TRUE,  PIPELINE_GROUP_LCD0, OUTPATH_PARALLEL_CMD},                     /*  group LCD0 */
       {FALSE, PIPELINE_GROUP_TV, OUTPATH_UNKNOWN},                            /*  group TV */
       {FALSE, PIPELINE_GROUP_LCD1, OUTPATH_UNKNOWN},                          /*  group LCD1 */
   },
   /* DSI1 only config*/
   {
       {TRUE, PIPELINE_GROUP_LCD0, OUTPATH_DSI1}, 
       {FALSE, PIPELINE_GROUP_TV, OUTPATH_UNKNOWN},                    
       {FALSE, PIPELINE_GROUP_LCD1, OUTPATH_UNKNOWN},                
   },
   /* DSI1 + DSI2 config */
   {
       {TRUE, PIPELINE_GROUP_LCD0, OUTPATH_DSI1}, 
       {FALSE, PIPELINE_GROUP_TV, OUTPATH_UNKNOWN},                    
       {TRUE, PIPELINE_GROUP_LCD1, OUTPATH_DSI2},    
   },
   /* DBI + DSI1 config */
   {
       {TRUE, PIPELINE_GROUP_LCD0, OUTPATH_PARALLEL_CMD}, 
       {FALSE, PIPELINE_GROUP_TV, OUTPATH_UNKNOWN},                    
       {TRUE, PIPELINE_GROUP_LCD1, OUTPATH_DSI1},    
   },
   /* HDMI only */
   {
       {FALSE, PIPELINE_GROUP_LCD0, OUTPATH_UNKNOWN},             
       {TRUE, PIPELINE_GROUP_TV, OUTPATH_HDMI},
       {FALSE, PIPELINE_GROUP_LCD1, OUTPATH_UNKNOWN},             
   },
   /* DSI1 + HDMI */
   {
       {TRUE, PIPELINE_GROUP_LCD0, OUTPATH_DSI1},             
       {TRUE, PIPELINE_GROUP_TV, OUTPATH_HDMI},
       {FALSE, PIPELINE_GROUP_LCD1, OUTPATH_UNKNOWN},             
   },
   /* DPI */
   {
       {TRUE, PIPELINE_GROUP_LCD0, OUTPATH_PARALLEL_VIDEO},            
       {FALSE, PIPELINE_GROUP_TV, OUTPATH_UNKNOWN},
       {FALSE, PIPELINE_GROUP_LCD1, OUTPATH_UNKNOWN},             
   },
};


/* ---------------------------------------------------------------------------
 * Function implementation
 * --------------------------------------------------------------------------- */
 
/***************************************************************************/
DispDrv_t* DispInit()
{
    DWORD cnt = 0;
   
   /* Reset DispDrv and initialize its member variables, LCD panels */
    memset(&DisplayDriver, 0, sizeof(DisplayDriver));
    while (cnt < DISP_MAX_PANELS)
    {
        DisplayDriver.lcdModule[cnt].displayId = (cnt == 0? DISPLAY_ID_0 : DISPLAY_ID_1);
        DisplayDriver.lcdModule[cnt].valid = FALSE;
        DisplayDriver.lcdModule[cnt].lcdModuleId = 0;
        cnt++;
    }
    DispCtrlStatus.state = DISP_STATE_UNINITIALIZED;
    DispCtrlConfig.enable = FALSE;
    /* Initialize DSI, HDMI controllers */
    if (!DispParallelInit())
        return NULL;
    if (!DsiInit())
        return NULL;    
    if (!HdmiInit())
        return NULL;
    /* Setup mutual semaphore, ISR etc */
    RE_RimInitCountingSemaphore( &DispController.dispLock, 1, 1);
    RE_RimInitCountingSemaphore( &DispController.waitForCmdInt, 1, 1);                            /* AHB Slave Port write*/
    RE_RimInitCountingSemaphore( &DispController.waitForReadInt, 1, 1);                            /* AHB Slave Port Read */
	
    RE_RegisterIntHandler( HWIRQ_DISPLAY_CTRL, ( IRQHandler * )&dispcCtrlIsr );
    EnableInterrupt( HWIRQ_DISPLAY_CTRL );

	/* Turn on LCD functional clock */
    WriteRegDword( PMUA_LCD_CLK_RES_CTRL_REG, PMUA_LCD_PST_CKEN          |         
                                              PMUA_LCD_ALL_RST_N         |    
                                              PMUA_LCD_ALL_CLKS_EN       |    
                                              PMUA_LCD_CLK_SEL_312MHZ );          
    WriteRegDwordOr( PMUA_CCIC_CLK_RES_CTRL_REG, PMUA_CCIC_AXI_RST_N     |    
                                                 PMUA_CCIC_AXICLK_EN );           /* why turn on camera clock here? */

    DispCtrlStatus.state = DISP_STATE_DISABLED;                                   /* update state */
    return (&DisplayDriver);
}

/*****************************************************************************/
/* Note - Bugdisp User Interface only update pipeline configurations */
BOOL DispEnable(DispDrv_t *pDisplay)
{
    DispPipelineCfg_t pipelineIndex; 
    const DispcPipelineCfg_t *pipelineCfg;
	const LcdModSvpFuncs_t * svpFuncs;

    if (pDisplay != &DisplayDriver)
        return FALSE;
    if (! (DispCtrlConfig.enable))
        return FALSE;

    /* Configure pixel pipelines -> enable output path controller -> Enable Display controller */
    pipelineIndex = DispCtrlConfig.pipelineConfig; 
    if (pipelineIndex >= PIPELINE_CNT)
        return FALSE;                                                

    /* At least one LCD panel should be connected */    
    LcdMainModule.valid = TRUE;
    LcdMainModule.lcdModuleId = DispCtrlConfig.lcdMainModId;

    switch (pipelineIndex)
    {
        /* Parallel LCD */
        case PIPELINE_DPI:
            return FALSE;                                             /* Dumb LCD panel not supported now */
            break;
        case PIPELINE_DBI:
            pipelineCfg = &(dispcPixelPipeline[pipelineIndex][0]);    /* Group LCD0 */
            if (!pipelineCfg->enabled)                                       
                return FALSE;
            
            /* configure Smart panel GPIO settings, reset etc */ 
            if (!DispParallelEnable()) 
                return FALSE;
            /* Configure Timing based on retrieved LCD timing info */
            if (LcdMainModule.valid)
                dispSetLcdTiming(&LcdMainModule);

			/* Initialize the connected LCD panel */
			svpFuncs = svp_lcdGetFuncs(&LcdMainModule);
			if (svpFuncs == NULL)
				return FALSE;
			if (!svpFuncs->svpInit(&LcdMainModule))
				return FALSE;
            break;
        /* DSI */    
        case PIPELINE_DSI1:
            /* Configure DSI controller */
            DsiEnable();
            
            break;
        case PIPELINE_DSI1_DSI2:
            LcdSubModule.valid = TRUE;
            LcdSubModule.lcdModuleId = DispCtrlConfig.lcdSubModId;
            /* Configure DSI controller */
            DsiEnable();
            break;
        case PIPELINE_HDMI:
            pipelineCfg = &(dispcPixelPipeline[pipelineIndex][1]);    /* TV group */
            HdmiEnable(); 
            break;
        default:
            break;
    }
    /* TODO: Enable Display Controller */
    /* Update state (ready to send or read pixels) */
    DispCtrlStatus.state = DISP_STATE_IDLE;

    return TRUE;
}

/*****************************************************************************/
BOOL DispDisable(DispDrv_t *pDisplay)
{
    DispPipelineCfg_t pipelineIndex; 

    if (DispCtrlStatus.state == DISP_STATE_DISABLED)
        return TRUE;
    while (DispCtrlStatus.state != DISP_STATE_IDLE) ;

    pipelineIndex = DispCtrlConfig.pipelineConfig; 
    if (pipelineIndex >= PIPELINE_CNT)
        return FALSE;                                                 /* Wrong configuration of pipeline */

    switch (pipelineIndex)
    {
        /* Parallel LCD */
        case PIPELINE_DBI:
        case PIPELINE_DPI:
            if (!DispParallelDisable()) 
                return FALSE;
            break;
        /* DSI */    
        case PIPELINE_DSI1:
        case PIPELINE_DSI1_DSI2:
            if (!DsiDisable())
                return FALSE;
            break;
        case PIPELINE_HDMI:
            if (!HdmiDisable())
                return FALSE;
            break;
        default:
            break;
    }
    
    /* TODO: Disable Display Controller */
    /* Update state */
    DispCtrlStatus.state = DISP_STATE_DISABLED;
    
    return TRUE;
}


/*****************************************************************************/
BOOL DispShowSurface(DispSurface_t* pSurface)
{
    DispPipelineCfg_t pipelineCfgIndex;
    const DispcPipelineCfg_t *pipeline;
    DWORD regVal0, regVal1 = 0;
    BOOL rc = FALSE;

    /*ToDo: Take semaphore to lock DSS */
    RE_RimLockCountingSemaphore( &DispController.dispLock);
    if (pSurface == NULL)
        goto cleanUp;

    /* wait for  idle state */
    while(DispCtrlStatus.state != DISP_STATE_IDLE) ;
    
    /* Get the pipeline configuration Index */
    pipelineCfgIndex = DispCtrlConfig.pipelineConfig;
    pipeline = &(dispcPixelPipeline[pipelineCfgIndex][0]);

    if (!pipeline->enabled)
        goto cleanUp;


    /* Using pixel pipeline based on primary or overlay surface and pipeline configruations */
    if (pSurface->surfaceId == PRIMARY_SURFACE)
    {
        /* Graphics pipeline */
        switch (pipelineCfgIndex) 
        {
            case PIPELINE_DBI:
                /* DMA path config: Panel path DMA goes to panel , TV path goest to GV */				
				WriteRegDword( LCD_TOP_CTRL_REG, CFG_ALL2PNORTV_PNANDTV );
				
				/* Stop former transmission */
                WriteRegDwordAnd(LCD_PN_CTRL_REG(1), ~CFG_FRAME_TRIG);                         
                WriteRegDwordOr(LCD_PN_CTRL_REG(0), CFG_GRA_ENA);

				/* Inform LCD module about pixel transmission before sending */
                DispParallelSendVideoPacket(pSurface->displayId);

				/* Write 8-bit bus from low byte to high byte */
				regVal0 = (CFG_SMPN_ENA | CFG_SLV_ENA | CFG_SWAP_BYTES); 
                switch (pSurface->pixelFormat)
                {
                    case PIXEL_FMT_RGB565:
						regVal0 |= CFG_SMPN_MODE_RGB565;
                        regVal1 |= CFG_GRA_FORMAT_RGB565;
                        break;
                    case PIXEL_FMT_RGB888P:
						regVal0 |= CFG_SMPN_MODE_RGB888;
                        regVal1 |= CFG_GRA_FORMAT_RGB888_PACKED;
                        break;
                    case PIXEL_FMT_RGB888UP:
						regVal0 |= CFG_SMPN_MODE_RGB888;
                        regVal1 |= CFG_GRA_FORMAT_RGB888_UNPACKED;
                        break;
                    case PIXEL_FMT_YUV422P:
                        regVal1 |= CFG_GRA_FORMAT_YUV422_PACKED;
                        break;
                    default:
                        break;
                }
                /* Config SMPN_CTRL to enable controll, AHB port */
				WriteRegDwordOr( LCD_SMPN_CTRL_REG, regVal0 );
				
                /* Set DMA start address and config pixel pipeline */
                WriteRegDword(LCD_PN_SEPXLCNT_REG, (pSurface->height << 16) | pSurface->width); 
                WriteRegDword(LCD_PN_CTRL_REG(0),0);                            /* Stop DMA */
                WriteRegDword(LCD_PN_CTRL_REG(1),0);
                WriteRegDword(LCD_PN_V_H_TOTAL_REG,0);                          /* only for Dumb Panel */
                WriteRegDword( LCD_GRA_START_ADDR_REG(0), pSurface->pRGBAddr);  /* Start address */

                if (pSurface->pixelFormat == PIXEL_FMT_RGB565)
					WriteRegDword(LCD_GRA_PITCH_REG, pSurface->width >> 1); 	/* Pitch in bytes */
                else if (pSurface->pixelFormat == PIXEL_FMT_RGB888P)
					WriteRegDword(LCD_GRA_PITCH_REG, pSurface->width >> 1); 	/* Pitch in bytes */
                else 
                {
                    PRINT("DispShowSurface: Unsupported source pixel format for graphics pipeline!");
                    return FALSE;
                }

                /* Configure Size: Primary surface has no scaling */                
                WriteRegDword(LCD_DMA_OVSA_HPXL_VLN_REG,0);                                            /* always frame0 */
                WriteRegDword(LCD_GRA_HPXL_VLN_REG, pSurface->height<<16 | pSurface->width);           /* source size before zooming */
                WriteRegDword(LCD_GRAZM_HPXL_VLN_REG,pSurface->height<<16 | pSurface->width);          /* Size after zooming */
                WriteRegDword(LCD_PN_V_H_ACTIVE_REG, (pSurface->height << 16) | pSurface->width);      /* Active Size */   
        
                /* configure the expected input format and trigger DMA */
                regVal1  = regVal1 | ((~CFG_GRA_FORMAT_MASK) & (ReadRegDword(LCD_PN_CTRL_REG(0)))); 
                regVal1 |= CFG_GRA_ENA;
				WriteRegDwordOr(LCD_SPU_IRQ_ENA_REG, TWC_FRAME_DONE_IRQ_ENA 
					                                 | GRA_FRAME_IRQ0_ENA | GRA_FRAME_IRQ1_ENA); 	  /* Enable IRQ */
                WriteRegDwordOr(LCD_PN_CTRL_REG(0), regVal1);                                         /* Graphic transfer enable */
                WriteRegDwordOr(LCD_PN_CTRL_REG(1), CFG_FRAME_TRIG | CFG_PN_VSYNC_MODE_DMA_FRAME_TRIG | CFG_PXLCMD);                
                break;
            case PIPELINE_DSI1:
                /* Set DMA start address and config pixel pipeline */
                WriteRegDword( LCD_GRA_START_ADDR_REG(0), pSurface->pRGBAddr); 
                DsiSendVideoPacket(pSurface->displayId);
                break;
            case PIPELINE_DSI1_DSI2:
                break;
            case PIPELINE_HDMI:
                break;
            default:
                break;
        }
    }   

    /* TODO: 
          1. For overlay surface, also get and configure postion info, determine if scaling is needed 
          2. configure the pipeline
          3. Trigger DMA to fetch one frame of pixels from system memory
     */
    else if (pSurface->surfaceId == OVERLAY_SURFACE_1)
    {
        /* Video pipeline */
        switch (pSurface->pixelFormat)
        {
            case PIXEL_FMT_RGB565:
                regVal1 |= CFG_GRA_FORMAT_RGB565;
                break;
            case PIXEL_FMT_RGB888P:
                regVal1 |= CFG_GRA_FORMAT_RGB888_PACKED;
                break;
            case PIXEL_FMT_RGB888UP:
                regVal1 |= CFG_GRA_FORMAT_RGB888_UNPACKED;
                break;
            case PIXEL_FMT_YUV422P:
                regVal1 |= CFG_GRA_FORMAT_YUV422_PACKED;          /* Color Conversion needed */
                break;
            default:
                break;
        }
        /* Graphics pipeline */
        switch (pipelineCfgIndex) 
        {
            case PIPELINE_DBI:
                break;
            case PIPELINE_DSI1:
                break;
            default:
                break;
        }
    }
    else
        goto cleanUp;
        
    /* Update state (ISR should change the state after finishing sending) */
    DispCtrlStatus.state = DISP_STATE_SENDING;
    rc = TRUE;
cleanUp:
    RE_RimUnlockCountingSemaphore( &DispController.dispLock);
    return rc;
}


/* Move an surface to a specified position */
BOOL DispMoveSurface(DispSurface_t* pSurface)
{
    /* ToDo: Take semaphore to lock DSS */
    RE_RimLockCountingSemaphore( &DispController.dispLock);

    while (DispCtrlStatus.state != DISP_STATE_IDLE) ;
    /* ToDo: 
               1. Get pipeline Info based on displayId and overlayId
               2.  Set the postion of a surface
       */             

    DispCtrlStatus.state = DISP_STATE_SENDING;
    RE_RimUnlockCountingSemaphore( &DispController.dispLock);
    return TRUE;
}

/****************************************************************************/
BOOL DispReadLcdPixels(DisplayId_t displayId, BYTE *buf, DWORD pixels)
{
    DispPipelineOutPath_t outPath;
    DWORD ret = 0;
	
    /* ToDo: Take semaphore to lock DSS */
    RE_RimLockCountingSemaphore( &DispController.dispLock);

    while (DispCtrlStatus.state != DISP_STATE_IDLE) ;

    outPath = DispGetOutpath(displayId);
    switch (outPath)
    {
        case OUTPATH_PARALLEL_CMD:
            ret = DispParallelReadVideoPacket(displayId, buf, pixels);
			PRINTN("Return: 0x%x pixels",ret);
            break;
        case OUTPATH_DSI1:
            break;
        case OUTPATH_DSI2:
            break;
        case OUTPATH_HDMI:
            break;
        default:
            break;
    }

    /* ToDo: Unlock semaphore to free DSS */
    RE_RimUnlockCountingSemaphore( &DispController.dispLock);

    return (TRUE);
}


/*****************************************************************************/
/* Search timings based on LCD perepheral module */
static BOOL dispSetLcdTiming(LcdModule_t *lcdModule)
{
    if (lcdModule->type == LCD_TYPE_MIPI_DBI_8BIT)
        DispParallelConfigTiming(lcdModule);        /* set parallel LCD timings */
    else if (lcdModule->type == LCD_TYPE_MIPI_DSI)
        DispDsiConfigTiming(lcdModule);
    else
        return FALSE;
    return TRUE;
}


/*****************************************************************************/
DispPipelineOutPath_t DispGetOutpath(const DisplayId_t displayId)
{
    DispPipelineOutPath_t  outPath;

    DispPipelineCfg_t pipelineIndex = DispCtrlConfig.pipelineConfig;
    switch (pipelineIndex)
    {
        case PIPELINE_DBI:
        case PIPELINE_DPI:    
            outPath = OUTPATH_PARALLEL_CMD;
            break;
        case PIPELINE_DSI1:
            outPath = OUTPATH_DSI1;
            break;
        case PIPELINE_DSI1_DSI2:
            if (displayId == DISPLAY_ID_0)
                 outPath = OUTPATH_DSI1;
            else
                 outPath = OUTPATH_DSI2;
            break;
        case PIPELINE_HDMI:
            outPath = OUTPATH_HDMI;
            break;
        case PIPELINE_DSI1_HDMI:
            if (displayId == DISPLAY_ID_0)
                outPath = OUTPATH_DSI1;
            else
                outPath = OUTPATH_HDMI;
            break;
        default:
            outPath = OUTPATH_UNKNOWN;
            break;

    }
    return (outPath);
}


/*****************************************************************************/
static void dispcCtrlIsr(void)
{
    DWORD tempIRQISR = ReadRegDword( LCD_SPU_IRQ_ISR_REG );
    DWORD tempIRQENA = ReadRegDword( LCD_SPU_IRQ_ENA_REG );
    DWORD clearMask = 0;

    PRINT2N( "LcdCtlIsr: Entry IRQISR: 0x%08X IRQENA: 0x%08X", tempIRQISR, tempIRQENA );

    /* TODO - copy from Nessus and need reformatting to current design of DSS
      // --------------------------------------------
      // Test for events and are they Masked?
      // --------------------------------------------
    
      // DMA_FRAME_IRQ0 - TODO: Assumes we are using Panel Path Video 
      // Frame 0 DMA for command transfers
     */
    if ( ( tempIRQISR & TWC_FRAME_DONE_IRQ_ENA ) 
            && ( tempIRQENA & TWC_FRAME_DONE_IRQ_ENA ) )
    {
        DispCtrlStatus.state = DISP_STATE_IDLE;
        clearMask |= ( TWC_FRAME_DONE_IRQ_ENA );
        WriteRegDwordAnd(LCD_SPU_IRQ_ENA_REG, ~TWC_FRAME_DONE_IRQ_ENA);               /* Turn off IRQ */
    }    
           
    if( tempIRQISR & SLV_IRQ_ENA )    
    { 
        clearMask |= ( SLV_IRQ_ENA );
        /* Turn off Slave Path Irq */
        WriteRegDwordAnd( LCD_SPU_IRQ_ENA_REG, ~ SLV_IRQ_ENA ); 
        RE_RimUnlockCountingSemaphore( &DispController.waitForCmdInt);               /* Release Semaphore */
	}
               
    if( ( tempIRQISR & GRA_FRAME_IRQ0_ENA ) 
        || ( tempIRQISR & GRA_FRAME_IRQ1_ENA ) ) 
    {
        PRINTN( "LcdCtlIsr: GRA_FRAME_IRQ0/1 fired! tempIRQISR - 0x%08X", tempIRQISR );

        DispCtrlStatus.state = DISP_STATE_IDLE;
        // This interrupt although it appears important doesn't really
        // mean a whole lot.  All it really means is that the DMA engine
        // is finished with the resiter settings that have been used to
        // set it up so it is ok to write to those registers again.  At
        // the time this interrupt is triggered it is NOT ok to either
        // turn off the DMA engine OR turn off the clocks to the LCD 
        // controller.    We could potentially use this interrupt to call
        // the LcdDmaTransferCallback but we would have to do some
        // thorough testing to make sure it is safe to program registers.
        clearMask |=  ( GRA_FRAME_IRQ0_ENA | GRA_FRAME_IRQ1_ENA );
   
        // Turn off Graphics Frame Done Irq's
        WriteRegDwordAnd( LCD_SPU_IRQ_ENA_REG, ~( GRA_FRAME_IRQ0_ENA | GRA_FRAME_IRQ1_ENA ) ); 
		DispCtrlStatus.state = DISP_STATE_IDLE;
    }
    
    if( !clearMask ) 
    {
        WARNN( "LcdCtlIsr: Unexpected IRQ (0x%8.8x) request", tempIRQISR );
    
        // Clear any pending IRQ's
        WriteRegDword( LCD_SPU_IRQ_ISR_REG, 0 );
        return;
    }
    
    // Clear the IRQ's we know about
    WriteRegDwordAnd( LCD_SPU_IRQ_ISR_REG, ~clearMask );

    return;
}

/* ===========================================================================
 * Debug functions
 * ===========================================================================*/
/*****************************************************************************
 * Function:     DispDumpRegs 
 * Description:  Dump display controller and DSI controller reigsters
 * Parameters:
 * Return:        None
 *
 *****************************************************************************/
void DispDumpRegs(DWORD catergory, DWORD arg)
{
    /* Dump all display controller registers */
    if (catergory == 1)
    {
        PRINT( " " );
        PRINT("================= Display Controller Registers =================");
        PRINT2N( "PMUA_LCD_CLK_RES_CTRL(0x%08X)     =  0x%8.8x ", PMUA_LCD_CLK_RES_CTRL_REG, ReadRegDword( PMUA_LCD_CLK_RES_CTRL_REG ) );
        PRINT2N( "PMUA_CCIC_CLK_RES_CTRL(0x%08X)    =  0x%8.8x ", PMUA_CCIC_CLK_RES_CTRL_REG, ReadRegDword( PMUA_CCIC_CLK_RES_CTRL_REG ) );
        PRINT2N( "PMUA_DMA_CLK_RES_CTRL(0x%08X)     =  0x%8.8x ", PMUA_DMA_CLK_RES_CTRL_REG, ReadRegDword( PMUA_DMA_CLK_RES_CTRL_REG ) );
        PRINT2N( "LCD_TOP_CTRL(0x%08X)              =  0x%8.8x ", LCD_TOP_CTRL_REG, ReadRegDword( LCD_TOP_CTRL_REG ) );
        PRINT2N( "LCD_PN_CTRL0(0x%08X)              =  0x%8.8x ", LCD_PN_CTRL_REG(0), ReadRegDword( LCD_PN_CTRL_REG(0) ) );
        PRINT2N( "LCD_PN_CTRL1(0x%08X)              =  0x%8.8x ", LCD_PN_CTRL_REG(1), ReadRegDword( LCD_PN_CTRL_REG(1) ) );
        PRINT2N( "LCD_PN_IOPAD_CONTROL(0x%08X)      =  0x%8.8x ", LCD_PN_IOPAD_CONTROL_REG, ReadRegDword( LCD_PN_IOPAD_CONTROL_REG ) );
        PRINT2N( "LCD_SMPN_CTRL(0x%08X)             =  0x%8.8x ", LCD_SMPN_CTRL_REG, ReadRegDword( LCD_SMPN_CTRL_REG ) );
        PRINT( " " );

        PRINT2N( "LCD_SPU_IRQ_ENA_REG(0x%08X)       =  0x%8.8x ", LCD_SPU_IRQ_ENA_REG, ReadRegDword( LCD_SPU_IRQ_ENA_REG ) );
        PRINT2N( "LCD_SPU_IRQ_ISR_REG(0x%08X)       =  0x%8.8x ", LCD_SPU_IRQ_ISR_REG, ReadRegDword( LCD_SPU_IRQ_ISR_REG ) );
        PRINT( " " );

        PRINT2N( "LCD_DMA_START_ADDR_Y0(0x%08X)     =  0x%8.8x ", LCD_DMA_START_ADDR_Y_REG(0), ReadRegDword( LCD_DMA_START_ADDR_Y_REG(0) ) );
        PRINT2N( "LCD_DMA_START_ADDR_U0(0x%08X)     =  0x%8.8x ", LCD_DMA_START_ADDR_U_REG(0), ReadRegDword( LCD_DMA_START_ADDR_U_REG(0) ) );
        PRINT2N( "LCD_DMA_START_ADDR_V0(0x%08X)     =  0x%8.8x ", LCD_DMA_START_ADDR_V_REG(0), ReadRegDword( LCD_DMA_START_ADDR_V_REG(0) ) );
        PRINT2N( "LCD_DMA_START_ADDR_C0(0x%08X)     =  0x%8.8x ", LCD_DMA_START_ADDR_C_REG(0), ReadRegDword( LCD_DMA_START_ADDR_C_REG(0) ) );
        PRINT2N( "LCD_DMA_PITCH_YC(0x%08X)          =  0x%8.8x ", LCD_DMA_PITCH_YC_REG, ReadRegDword( LCD_DMA_PITCH_YC_REG ) );
        PRINT2N( "LCD_DMA_PITCH_UV(0x%08X)          =  0x%8.8x ", LCD_DMA_PITCH_UV_REG, ReadRegDword( LCD_DMA_PITCH_UV_REG ) );
        PRINT2N( "LCD_DMA_OVSA_HPXL_VLN(0x%08X)     =  0x%8.8x ", LCD_DMA_OVSA_HPXL_VLN_REG, ReadRegDword( LCD_DMA_OVSA_HPXL_VLN_REG ) );
        PRINT2N( "LCD_DMAZM_HPXL_VLN(0x%08X)        =  0x%8.8x ", LCD_DMAZM_HPXL_VLN_REG, ReadRegDword( LCD_DMAZM_HPXL_VLN_REG ) );
        PRINT( " " );

        PRINT2N( "LCD_GRA_START_ADDR_0(0x%08X)      =  0x%8.8x ", LCD_GRA_START_ADDR_REG(0), ReadRegDword( LCD_GRA_START_ADDR_REG(0) ) );
        PRINT2N( "LCD_GRA_PITCH(0x%08X)             =  0x%8.8x ", LCD_GRA_PITCH_REG, ReadRegDword( LCD_GRA_PITCH_REG ) );
        PRINT2N( "LCD_GRA_OVSA_HPXL_VLN(0x%08X)     =  0x%8.8x ", LCD_GRA_OVSA_HPXL_VLN_REG, ReadRegDword( LCD_GRA_OVSA_HPXL_VLN_REG ) );
        PRINT2N( "LCD_GRAZM_HPXL_VLN(0x%08X)        =  0x%8.8x ", LCD_GRAZM_HPXL_VLN_REG, ReadRegDword( LCD_GRAZM_HPXL_VLN_REG ) );
        PRINT2N( "LCD_GRA_CUTHPXL(0x%08X)           =  0x%8.8x ", LCD_GRA_CUTHPXL_REG, ReadRegDword( LCD_GRA_CUTHPXL_REG ) );
        PRINT2N( "LCD_TVGGRAVLD_HLEN_REG(0x%08X)    =  0x%8.8X", LCD_TVGGRAVLD_HLEN_REG, ReadRegDword( LCD_TVGGRAVLD_HLEN_REG ) );
        PRINT( " " );

        PRINT2N( "LCD_PN_V_H_TOTAL(0x%08X)          =  0x%8.8x ", LCD_PN_V_H_TOTAL_REG, ReadRegDword( LCD_PN_V_H_TOTAL_REG ) );
        PRINT2N( "LCD_PN_V_H_ACTIVE(0x%08X)         =  0x%8.8x ", LCD_PN_V_H_ACTIVE_REG, ReadRegDword( LCD_PN_V_H_ACTIVE_REG ) );
        PRINT( " " );

        PRINT2N( "LCD_ISA_RXDATA(0x%08X)            =  0x%8.8x ", LCD_ISA_RXDATA_REG, ReadRegDword( LCD_ISA_RXDATA_REG ) );
        PRINT2N( "LCD_SLV_PORT(0x%08X)              =  0x%8.8x ", LCD_SLV_PORT_REG, ReadRegDword( LCD_SLV_PORT_REG ) );
        PRINT( " " );

        PRINT2N( "LCD_SCLK_DIV(0x%08X)              =  0x%8.8x ", LCD_SCLK_DIV_REG, ReadRegDword( LCD_SCLK_DIV_REG ) );
        PRINT2N( "LCD_TCLK_DIV(0x%08X)              =  0x%8.8x ", LCD_TCLK_DIV_REG, ReadRegDword( LCD_TCLK_DIV_REG ) );

        PRINT2N( "LCD_SLV_DBG(0x%08X)               =  0x%8.8x ", LCD_SLV_DBG_REG, ReadRegDword( LCD_SLV_DBG_REG ) );
        PRINT2N( "LCD_FRAME_CNT(0x%08X)             =  0x%8.8x ", LCD_FRAME_CNT_REG, ReadRegDword( LCD_FRAME_CNT_REG ) );

        PRINT( " " );
    }
    else if (catergory == 2)
    {
        /* DSI registers */
    }
    else
        return;
}
 
