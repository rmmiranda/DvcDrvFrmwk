/*****************************************************************************
* Filename:    dispif_tavorpv2.c
*
* Copyright 2009, Research In Motion Ltd
*
* Author:      Victor Chen
*
* Created:     April 13th, 2009
*
* Description: This file implements the HAL layer (HW specific code) for 
*              DSI/Display controller driver on Tavor PV2.
*
*
*****************************************************************************/
/* include header files */

#include "dispif_internal.h"

/* Register Definitions */
#include "dispif_tavorpv2.h"
#include "disphal_tavorpv2.h"


/* ===========================================================================
 * Data Structure and Macros 
 * ===========================================================================*/

#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE FILE_DISP_DEV

/* MVED DMA leverages System DMA macros: requires redefinition of the DMA base address */
#undef DMA_BASE
#define DMA_BASE    0x56100000

/* the main DSI/Display driver data structure */
static Tavor2DispDrv Tavor2DisplayDriver;

/* Including mode table to support multiple LCD panels or DSI bridges in one system */
static lcdMode tavorpv2_lcdModeTable[] =
{
#include "tavorpv2_lcd_settings.dat"
};

static BOOL lcdEnableFlag = FALSE;

static BOOL lcdStopFlag = FALSE;

/* DSI Command FIFO Buffer */
static __align(4) WORD dsiCmdFifoBuffer[DSI_CMD_FIFO_BUF_LEN];

// DMA descriptor
static __align(16) DWORD dispDmaDescriptor[4];     // For LCD Ch0  DMA
static __align(16) RE_DmaDescriptor dsiDmaDescriptor[TAVORPV2_DSI_CTL];

// Frame Buffer used internally by the driver
__align(32) BYTE dispFrameBuffer[TAVORPV2_LCD_DEF_BPP * TAVORPV2_LCD_MAX_HEIGHT * TAVORPV2_LCD_MAX_WIDTH];


/* ===========================================================================
 * Global and Local Functions
 * ===========================================================================*/

/* The only exported global function under Driver Framework */
DispDrv *drvLvlLcdInit(void);

/* Local functions */
static int tavorpv2_drvLcdEnable(DispDrv *pDisplayDriver);
static int tavorpv2_drvLcdDisable(DispDrv *pDisplayDriver);
static int tavorpv2_drvLcdWriteCmd(DispDrv *pDisplayDriver, DWORD cmd, void *param);
static int tavorpv2_drvLcdReadData(DispDrv *pDisplayDriver, void *param);
static int tavorpv2_drvLcdWriteData(DispDrv *pDisplayDriver, void *param);
static void tavorpv2_drvLcdDebug(DispDrv *pDisplayDriver, DWORD catergory, DWORD arg);
static BOOL tavorpv2_drvLcdSetMode(DispDrv *pDisplayDriver);
static BOOL tavorpv2_drvDsiSetMode(DispDrv *pDisplayDriver, lcdMode *pMode, DWORD dsiId);
static void tavorpv2_drvDispSetMode(DispDrv *pDisplayDriver);

// Config DSI D-PHY timings
static void tavorpv2_drvLcdConfigInterfaceTiming(DispDrv *pDisplayDriver, DWORD regA, DWORD regB, DWORD regC, BYTE displayId);

//Nessus APIs
static void tavorpv2_drvLcdCopyToLcd(DispDrv *pDisplayDriver,DWORD src, DWORD high, DWORD wide, DWORD displayWidth, BYTE displayId );
// Optional APIs for driver framework
static void tavorpv2_drvLcdIsr(DispDrv *pDisplayDriver);
static void tavorpv2_drvLcdIntEvtWait(DispDrv *pDisplayDriver);

/* DSI/Display BugDisp commands implementation*/
static BOOL tavorpv2_drvLcdTest(DispDrv *pDisplayDriver, DWORD testMode);

// speicific LCD panel/DSI briges initialization
static BOOL tavorpv2_desLm2550Init(DispDrv *pDisplayDriver, DWORD dsiId, DWORD powerMode);
static BOOL tavorpv2_dcsTpoLcdInit(DispDrv *pDisplayDriver, DWORD dsiId, DWORD panelId);
static BOOL tavorpv2_dcsProteusLcdInit(DispDrv *pDisplayDriver, DWORD dsiId, DWORD panelId);

// DSI command FIFO APIs
static  BOOL tavorpv2_dsiWriteCmd(DispDrv *pDisplayDriver, DWORD dsiId, DWORD src, DWORD length);
//Internal 128 entries DSI CMD FIFO Buffer APIs
static BOOL tavorpv2_dsiAddCmd2Buffer(DispDrv *pDisplayDriver, DWORD dsiId, BYTE dsiCommand, BYTE dsiData, BYTE dsiVc, BYTE loopFlag);
static BOOL tavorpv2_dsiResetCmdFifoBuf(DispDrv *pDisplayDriver, DWORD dsiId);
static BOOL tavorpv2_dsiSendCmdSeq(DispDrv *pDisplayDriver, DWORD dsiId);

// Fill in DSI command FIFO by DMA method
static BOOL tavorpv2_dsiSendCmdSeqWithDma(DispDrv *pDisplayDriver, DWORD dsiId);
// Trig a "int proc" intrrupt to start executing DSI command
static void tavorpv2_dsiTrigIntProc(DispDrv *pDisplayDriver, DWORD dsiId);

/* Interrupt Handler */
static void tavorpv2_dsi0Int(void);
static void tavorpv2_dsi1Int(void);
static void tavorpv2_dispInt(void);
static void tavorpv2_dsi0DmaInt(void);
static void tavorpv2_dsi1DmaInt(void);

static void tavorpv2_dsiInterruptHandler(DispDrv *pDisplayDriver, DWORD dsiId);

// Send Pixels to LCD with/without going through DSI Command FIFO
static BOOL tavorpv2_dispCopyToLcdFromDsi(DispDrv *pDisplayDriver,DWORD src, DWORD high, DWORD wide, DWORD displayWidth, BYTE displayId);

//Software WorkAround for DCS long packet write by using DSI controller cmd FIFO 
static BOOL tavorpv2_dispCopyToLcdFromDsiWka(DispDrv *pDisplayDriver,DWORD src, DWORD high, DWORD wide, BYTE displayId);
// Write pixels by going trhough Display controlelr pixel pipeline
static BOOL tavorpv2_dispCopyToLcd(DispDrv *pDisplayDriver,DWORD src, DWORD high, DWORD wide, DWORD iterators, BYTE displayId);
//display controller pixel pipeline (stage 1 to write pixels)
static BOOL tavorpv2_dispSendFrame(DispDrv *pDisplayDriver, DWORD src, DWORD high, DWORD wide, BYTE displayId);
//"DCS long write" for dsi cmd fifo (stage 2 to write pixels)
static BOOL tavorpv2_dsiSendFrame(DispDrv *pDisplayDriver, DWORD high, DWORD wide, BYTE displayId, BOOL loopFlag);

// Initialize Frame Buffer with Color bars
static void tavorpv2_dispInitFrmBuf(DispDrv *pDisplayDriver);

// Get DSI ID based on dispalyId
static DWORD tavorpv2_dispGetDsiId(DispDrv *pDisplayDriver, DWORD displayId);
// Get Mode based on DSI clock and panelId
static DWORD tavorpv2_dispModeFind(DispDrv *pDisplayDriver, BYTE dsiLinkClock, BYTE panelId);

// Clear DSI statistics
static void tavorpv2_dsiResetStatis(DispDrv *pDisplayDriver, DWORD dsiId);

//DSI Event APIs for returning back data from smart LCD panel
static void tavorpv2_dsiSetEvent(DispDrv *pDisplayDriver, DWORD dsiId, DWORD eventFlag);
static void tavorpv2_dsiClearEvent(DispDrv *pDisplayDriver,DWORD dsiId, DWORD eventFlag);
static BOOL tavorpv2_dsiWaitForEvent(DispDrv *pDisplayDriver, DWORD dsiId, DWORD eventFlag);

/* Externs */
extern RE_DmaDescriptor DmaDispDesp[];
//static __align(16) DmaDescriptor DmaDispDesp[1];


/*****************************************************************************
 * Function:    drvLvlLcdInit 
 * Description: Drv Framework API - driver initialization. This is the only exposed fuction as Display Driver's
 *                                               Entry Point
 * Parameters:  void
 * Return:      Pointer to variable DispDrv.
 *
 *****************************************************************************/
DispDrv *drvLvlLcdInit()
{
    WORD i = 0;
    DispDrv *pDisplayDriver = (DispDrv *) (&Tavor2DisplayDriver);
    lcdDsiConfig *dsiConfig = NULL;
    lcdDsiStatus *dsiStatus = NULL;
    //DWORD frameBufferSize = 0;

    /* Check if already initializated */
    if (pDisplayDriver->lcdCtlFlag == lcdCtrlIntialized)
        goto cleanUp;

    /* Buffer for DSI Command FIFO */
    Tavor2DisplayDriver.dsiCmdFifoBuf = (&dsiCmdFifoBuffer[0]); 
    // Pointer for current position 
    Tavor2DisplayDriver.dsiCmdFifoBufIndex= 0;
    // Pointer for head position 
    Tavor2DisplayDriver.dsiCmdFifoBufHead = 0;

    /* Allocate system resources - DMA channel, DAM descriptor,*/
    // DMA descriptors for Fetch Unit
    Tavor2DisplayDriver.pDispDmaDescriptor = (DWORD *) (&dispDmaDescriptor[0]);

    /* MVED DMA settings : DMA channel with appropriate DMA request is used to fill DSI command FIFO */
    // Setup the DMA channel ISR's
    RE_RegisterIntHandler( HWIRQ_MVED_DMA, tavorpv2_dsi0DmaInt );
    RE_RegisterIntHandler( HWIRQ_MVED, tavorpv2_dsi0DmaInt );

    // Enable MVED clock (move to other place)
    RE_ClockEnable( (RE_PeripheralClock) 27 ); /* sync value with CLOCK_MVED */

    // Setup MVED DMA channels
    Tavor2DisplayDriver.dsiDmaChannel[0] = MVED_DMA_CHANNEL_0; 
    Tavor2DisplayDriver.dsiDmaChannel[1] = MVED_DMA_CHANNEL_1; 

    // Setup MVED DMA requests: Req2 for DSI0 and Req3 for DSI1
    Tavor2DisplayDriver.dsiDmaRequest[0] = DMA_MVED_2;
    Tavor2DisplayDriver.dsiDmaRequest[1] = DMA_MVED_3;

    // Setup MVED DMA descriptors
    Tavor2DisplayDriver.pDsiDmaDesp[0] = &dsiDmaDescriptor[0]; 
    Tavor2DisplayDriver.pDsiDmaDesp[1] = &dsiDmaDescriptor[1]; 

    // Clear the DMA channels
    DMA_CLEAR_CHANNEL( Tavor2DisplayDriver.dsiDmaChannel[0] );
    DMA_CLEAR_CHANNEL( Tavor2DisplayDriver.dsiDmaChannel[1] );

    // Map request to DMA channels
    DMA_MAP_SOURCE( Tavor2DisplayDriver.dsiDmaRequest[0], Tavor2DisplayDriver.dsiDmaChannel[0] );
    DMA_MAP_SOURCE( Tavor2DisplayDriver.dsiDmaRequest[1], Tavor2DisplayDriver.dsiDmaChannel[1] );

    // Enable MVED and MVED_DMA interrupts
    RE_EnableInterrupt( HWIRQ_MVED_DMA );
    RE_EnableInterrupt( HWIRQ_MVED );

    // initialize DMA counting Semaphore
    RE_RimInitCountingSemaphore(&(Tavor2DisplayDriver.dsiDmaLock[0]), 0, 1);
    RE_RimInitCountingSemaphore(&(Tavor2DisplayDriver.dsiDmaLock[1]), 0, 1);

    /* Initialize all Tavor2DisplayDriver data area to default values*/
    // DSI config
    for (i = 0; i < TAVORPV2_DSI_CTL; i ++)
    {
        dsiConfig =(lcdDsiConfig *) (&(pDisplayDriver->config.dsiConfig[i]));
        dsiStatus=(lcdDsiStatus*) (&(pDisplayDriver->status.dsiStatus[i]));

        // Disable DSI contrller by default
        dsiConfig->status = DSI_DISABLED;     

        // Set to 2 active data lanes by default (Lemur only supports 2 data lanes)
        dsiConfig->activeLanes = 2; 
        // Set DSI transmission mode to be keep in High-Speed mode between packets (multiple packets per transmission)
        dsiConfig->powerMode = DSI_POWERMODE_HS_NORIHS;
        // DSI test mode : continously running without stop
        dsiConfig->errControl = DSI_ERR_CONT;
        // Default Bit Clock and panelId
        dsiConfig->dsiLinkClock = DSI_LINK_CLK_312M;                      
        dsiConfig->panelId = 0x01;

        // Connected LCD panels
        dsiConfig->panels = 0;
        dsiConfig->pMode = NULL;

        // Initialize dsiStatus/dsiStats (staticstics)
        dsiStatus->enabled = FALSE;
        tavorpv2_dsiResetStatis(pDisplayDriver, i);

		if (i == 1) {
			dsiConfig->dsiLinkClock = DSI_LINK_CLK_156M;					  
			dsiConfig->panelId = 0x01;
		}			
        
    }

    /* Set function pointers required for display driver */
    // Driver Framework
    pDisplayDriver->drvLcdEnable = tavorpv2_drvLcdEnable;
    pDisplayDriver->drvLcdDisable =tavorpv2_drvLcdDisable;
    pDisplayDriver->drvLcdDebug = tavorpv2_drvLcdDebug;
    // Methods to send command/pixels
    pDisplayDriver->drvLcdWriteCmd = tavorpv2_drvLcdWriteCmd;
    pDisplayDriver->drvLcdWriteData =tavorpv2_drvLcdWriteData;
    pDisplayDriver->drvLcdReadData =tavorpv2_drvLcdReadData;

    pDisplayDriver->drvLcdIsr = tavorpv2_drvLcdIsr;
    pDisplayDriver->drvLcdTest =tavorpv2_drvLcdTest;

    pDisplayDriver->drvLcdIntEvtWait = tavorpv2_drvLcdIntEvtWait;
    // Configure DSI/Display controllers based on Mode settings 
    pDisplayDriver->drvLcdSetMode = tavorpv2_drvLcdSetMode;

    // Nessus Interface APIs
    pDisplayDriver->nessusCopyToLcd = tavorpv2_drvLcdCopyToLcd;
    pDisplayDriver->nessusLcdConfigInterfaceTiming= tavorpv2_drvLcdConfigInterfaceTiming;

    /* Retrieve LCD modes that PV2 supported */
    pDisplayDriver->pModeList = (lcdMode *) (&tavorpv2_lcdModeTable[0]);

    /* Initialize Display Controller configurations, states, staticstics */
    // Windows IDs used for baseplane and overlay
    Tavor2DisplayDriver.dispBasePlane = DISP_CTL_BASE_PLANE;
    Tavor2DisplayDriver.dispBasePlaneDmaCh = DISP_CTL_DMA_CH00;
    Tavor2DisplayDriver.dispOverlay= DISP_CTL_OVERLAY1;
    Tavor2DisplayDriver.dispOverlayDmaCh = DISP_CTL_DMA_CH01;
    // DMA channel for Base plane         
    pDisplayDriver->config.dispConfig.dispBpChanel0 = 0;
    pDisplayDriver->config.dispConfig.dispBpChanel1 = 1;
    //Overlay DMA channel (Channel 4 can be used for both RGB and YUV pixel format)
    pDisplayDriver->config.dispConfig.dispOlChanel0 = 4;
    pDisplayDriver->config.dispConfig.dispOlChanel1 = 5;
    pDisplayDriver->config.dispConfig.dispOlChanel2 = 6;

    /* Initialize frame buffer and pointers */
    Tavor2DisplayDriver.pDispFrameBuffer = (WORD *) (&dispFrameBuffer[0]);
    Tavor2DisplayDriver.pDispFrmPtr = 0xffffffff;
    // Fill in pixels for a color bar
    tavorpv2_dispInitFrmBuf(pDisplayDriver);

    // Intialize "read back" buffer and length
	Tavor2DisplayDriver.lcdRdData = (BYTE *) NULL;
    Tavor2DisplayDriver.lcdRdLen = 0;

	/* register interrupt handler, but disable interrupt */
    // DSI0 
    RegisterIntHandler( HWIRQ_DSI0, tavorpv2_dsi0Int);
    // DSI1
    RegisterIntHandler( HWIRQ_DSI1, tavorpv2_dsi1Int);

    // Primary Global Display Controller
    RegisterIntHandler( HWIRQ_DISP1, tavorpv2_dispInt);

	// Enable Display and DSI controllers functional/interface Clocks before any other settings
	tavorpv2_lcdSetFIClock(pDisplayDriver->config.dsiConfig[0].dsiLinkClock,pDisplayDriver->config.dsiConfig[1].dsiLinkClock);
	// Config GPIOs for any external LCD panel reset, backlight etc
	tavorpv2_lcdSetGpios();
    
    /* Set intialize successfully flag */
    pDisplayDriver->lcdCtlFlag = lcdCtrlIntialized;
    return (pDisplayDriver);

cleanUp:
    // If failed return NULL
    return (NULL);

}

/*****************************************************************************
 * Function:    tavorpv2_dsi0Int 
 * Description: Wrapped Interrupt Hanlder for DSI Controller 0
 * Parameters:  void
 * Return:      void.
 *
 *****************************************************************************/
static void tavorpv2_dsi0Int()
{
    DispDrv *pDisplayDriver = (DispDrv *) (&Tavor2DisplayDriver);

    tavorpv2_dsiInterruptHandler(pDisplayDriver, 0);
}

/*****************************************************************************
 * Function:    tavorpv2_dsi1Int 
 * Description: Wrapped Interrupt Hanlder for DSI Controller 1
 * Parameters:  void
 * Return:      void.
 *
 *****************************************************************************/
static void tavorpv2_dsi1Int()
{
    DispDrv *pDisplayDriver = (DispDrv *) (&Tavor2DisplayDriver);

    tavorpv2_dsiInterruptHandler(pDisplayDriver, 1);
}

static void tavorpv2_dispInt()
{
    DispDrv *pDisplayDriver = (DispDrv *) (&Tavor2DisplayDriver);

    tavorpv2_drvLcdIsr(pDisplayDriver);
}

/*****************************************************************************
 * Function:    tavorpv2_dsiInterruptHandler 
 * Description: Actual Interrupt Hanlder for DSI Controller
 * Parameters: DispDrv*, DSI ID
 * Return:       void.
 *
 *****************************************************************************/
//Delete the first parameter (DispDrv *)
static void tavorpv2_dsiInterruptHandler(DispDrv *pDisplayDriver, DWORD dsiId)
{
    DWORD regVal0 = 0;
    DWORD regVal1 = 0;
    DWORD rtnVal = 0;
    BOOL errorFlag = FALSE;
    lcdMode *pMode = pDisplayDriver->config.dsiConfig[dsiId].pMode;
    
    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    // Check if it's valid DSI ID
    if ( (dsiId >= TAVORPV2_DSI_CTL)) 
        return;

    // Set dsiId here for posting message
    pTavor2DisplayDriver->dispMsg.Data[0] = dsiId;

    // Read Insterrupt Status Registers
    regVal0 = ReadRegDword(DSI_DxINST0(dsiId));
    regVal1 = ReadRegDword(DSI_DxINST1(dsiId));
    
    // Debug Message
    PRINT5N("DSI%d interrupt happended with DxINST0(0x%08x)= 0x%08x and DxINST1(0x%08x)= 0x%08x", dsiId, DSI_DxINST0(dsiId), regVal0,
              DSI_DxINST1(dsiId),regVal1);

    // Update staticstics before any interrupt handling process
    // Smart panel Break interrupt
    if (regVal0 & BREAK_INT_STS)
        pDisplayDriver->statistics.dsiStats[dsiId].dsiBreak++;

    // Mask the SP_UNDER_RUN interrupt if it happened
     if (regVal0 & SP_UNDRUN_INT_STS)
     {
         WriteRegDwordAnd(DSI_DxSCR0(dsiId), ~DSI_DxSCR0_SP_UNDRUN_INT_EN);
         pDisplayDriver->statistics.dsiStats[dsiId].dsiSpUnderRun++;
     }

    // If SP_GEN_INT_STS happened
    if (regVal0 & SP_GEN_INT_STS)
    {
#if !defined(TAVORPV2_HWVER_MG1)    

        // a. Mask the SP_UNDER_RUN interrupt
        WriteRegDwordAnd(DSI_DxSCR0(dsiId), ~DSI_DxSCR0_SP_UNDRUN_INT_EN);
 
        // b. Send ULPS enter command  if it is enabled- set DxSCR1_ULPS
        if ( (pDisplayDriver->config.dsiConfig[dsiId].powerMode) & DSI_POWERMODE_ENTER_ULPS)
            tavorpv2_dsiCtlSetPowerMode(dsiId,DSI_POWERMODE_ENTER_ULPS);

        // c. For Zx clear DxINST1.21 
        WriteRegDwordOr(DSI_DxINST1(dsiId), (1<<21));

        // Software workaround if panelId is TPO
        if (pMode->panelId == 0x00)
        {
            // Start sending pixels if needed by software workaround (eDISCO is using hardware wordaround instead)
            if (pTavor2DisplayDriver->pDispFrameBuffer != ((WORD *) NULL))
            {
                tavorpv2_dispCopyToLcdFromDsiWka(pDisplayDriver,
					                             (DWORD )pTavor2DisplayDriver->pDispFrameBuffer,
					                             pMode->height,
					                             pMode->width,
					                             0);
            }
        }
#endif
        // Writing next 128 commands in DSI cmd FIFO Buffer 
        tavorpv2_dsiSendCmdSeq(pDisplayDriver,dsiId);

        // Sending frame by pixel pipeline (hardware workaround)
        if (pDisplayDriver->lcdData.repeatTimes > 0)
        {
			pDisplayDriver->lcdData.repeatTimes--;
            tavorpv2_dsiSendFrame(pDisplayDriver, 
                                  pDisplayDriver->lcdData.height, 
                                  pDisplayDriver->lcdData.width,
                                  pDisplayDriver->lcdData.lcdPanelId,
                                  FALSE);
                                  
        }    

        // Update staticstics
        pDisplayDriver->statistics.dsiStats[dsiId].dsiSpGen++;

    }

    // Smart panel Read interrupt
    if (regVal0 & SP_RD_INT_STS)
    {
        // Smart Panel Read Interrupt, data is returned back from DSI_DxPRSR
        // Update returned data 
        rtnVal = ReadRegDword(DSI_DxPRSR(dsiId));
		if (pTavor2DisplayDriver->lcdRdData != (BYTE *)NULL)
		{
            *(pTavor2DisplayDriver->lcdRdData) = (BYTE) (rtnVal & 0x00ff);
			pTavor2DisplayDriver->lcdRdData++;
            *(pTavor2DisplayDriver->lcdRdData) = (BYTE) ((rtnVal & 0xff0000) >> 16);
			pTavor2DisplayDriver->lcdRdData++;
		}
	    pDisplayDriver->statistics.dsiStats[dsiId].dsiSpRd++;
	   
	    PRINTN("Interrupt: Smart Panel Read (return)= 0x%04x", ((rtnVal & 0xff0000) >> 8) |rtnVal & 0x00ff);

		// Execute next command from DSI command FIFO
		WriteRegDwordOr(DSI_DxPRSR(dsiId), DSI_DxPRSR_CON_NT | DSI_DxPRSR_ST_OK );
	   
        // Read next 2 bytes by sending command
        if (pTavor2DisplayDriver->lcdRdLen > 0) 
	    { 
	   	   pTavor2DisplayDriver->lcdRdLen = (pTavor2DisplayDriver->lcdRdLen > 2) ? (pTavor2DisplayDriver->lcdRdLen - 2) : 0;
		   if (pTavor2DisplayDriver->lcdRdLen > 0) 
		   {
	           tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);	   
	           tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_READ_NO_PARAMETER, 0, 0);
	           tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DCS_CMD_READ_MEMORY_CONTINUE, 0, 0);
	           tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0, 0, 0);
	           // Issue BTA
	           tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_READ_FRAME_BUFFER, 0x00, 0, 0);
	           tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_NO_OPERATION, 0x42, 0x00, 0x00);
	           // Trig "int proc"
	           tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00);
		   } 
           else
	   	      // Post "Read Finished" message to verification/test code
              ;
        } 

         // Execute next command from DSI command FIFO
         // WriteRegDwordOr(DSI_DxPRSR(dsiId), DSI_DxPRSR_CON_NT | DSI_DxPRSR_ST_OK );
 
    }

    // Start Of Transmission
    if (regVal0 & DISP_SOF_INT_STS)
        pDisplayDriver->statistics.dsiStats[dsiId].dsiDispSof++;

    // End Of Transmission
    if (regVal0 & DISP_EOF_INT_STS)
    {
        pDisplayDriver->statistics.dsiStats[dsiId].dsiDispEof++;
        pDisplayDriver->statistics.dsiStats[dsiId].dsiTransBytes += (pMode->height) * (pMode->width) * (pMode->colorDepth >> 3);
		// Post message to test code to indicate the transmission is finished
        if (lcdStopFlag == TRUE)
        {
            // Reset pointers to stop traffic when controller is enabled
            pDisplayDriver->lcdData.repeatTimes = 0;
            pTavor2DisplayDriver->pDispFrmPtr = 0xFFFFFFFF;
            pTavor2DisplayDriver->pDispFrameBuffer = (WORD *)NULL;
            pTavor2DisplayDriver->lcdRdLen = 0;

			//pTavor2DisplayDriver->dispMsg.Event = (DWORD) PLATFORM_MSG_DISP_DRV;
			//pTavor2DisplayDriver->dispMsg.SubMsg = (DWORD) LCD_MSG_DSI_EOT;
			//RE_RimPostMessage( PLATFORM_THREAD_ID_APP0, &(pTavor2DisplayDriver->dispMsg));
        }	
    }    

    // Display underrun interrupt
    if (regVal0 & DISP_URUN_INT_STS)
        pDisplayDriver->statistics.dsiStats[dsiId].dsiDispUnderRun++;
    
    // Low Power High contention on lane L3
    if (regVal1 & LP1_L3)
        pDisplayDriver->statistics.dsiStats[dsiId].dsiLp1L3++;

    // Low Power Low Contention on Lane L3
    if (regVal1 & LP0_L3)
        pDisplayDriver->statistics.dsiStats[dsiId].dsiLp0L3++;

    // Trigger Code in response to BTA 
    if (regVal1 & TRIG_ST)
    {
        rtnVal = ReadRegDword(DSI_DxTRIG(dsiId));
        rtnVal = (rtnVal & 0xFF000000);
        // Clear interrupt
        PRINTN("Interrupt: trigger in code = 0x%02x", rtnVal >> 24);
        pDisplayDriver->statistics.dsiStats[dsiId].dsiTrig++;
    }

    // Acknowledge with Error report in response to BTA
    if (regVal1 & ACK_ERR_ST)
    {
        rtnVal = ReadRegDword(DSI_DxADAT(dsiId));
        pTavor2DisplayDriver->dsiAckData[dsiId]= (WORD) (rtnVal & 0x0000FFFF);

        PRINTN("DSI Interrupt: Acknowledge with Error = 0x%08x", pTavor2DisplayDriver->dsiAckData[dsiId]);
        tavorpv2_dsiSetEvent(pDisplayDriver, dsiId, dsiEvtFlagAckErr);
        pDisplayDriver->statistics.dsiStats[dsiId].dsiAckErr++;

        if (!(pTavor2DisplayDriver->dsiAckData[dsiId] & DSI_PACKET_ERROR_PROTOCOL_VIOLATION))
            errorFlag = TRUE;

        // Update Statistics from the received Errors
        // SoT Error
        if (pTavor2DisplayDriver->dsiAckData[dsiId] & DSI_PACKET_ERROR_SOT)
            pDisplayDriver->statistics.dsiStats[dsiId].dsiRtnAckSotErr++;
        // SoT Sync Error
        if (pTavor2DisplayDriver->dsiAckData[dsiId] & DSI_PACKET_ERROR_SOT_SYNC)
            pDisplayDriver->statistics.dsiStats[dsiId].dsiRtnAckSotSyncErr++;
        // EoT Sync Error
        if (pTavor2DisplayDriver->dsiAckData[dsiId] & DSI_PACKET_ERROR_EOT_SYNC)
            pDisplayDriver->statistics.dsiStats[dsiId].dsiRtnAckEotSyncErr++;
        //  Escape Mode Entry Command Error
        if (pTavor2DisplayDriver->dsiAckData[dsiId] & DSI_PACKET_ERROR_ESCAPE)
            pDisplayDriver->statistics.dsiStats[dsiId].dsiRtnEscModEntryCmdErr++;
        // Low-Power Transmit Sync Error
        if (pTavor2DisplayDriver->dsiAckData[dsiId] & DSI_PACKET_ERROR_LP_TRANSMIT_SYNC)
            pDisplayDriver->statistics.dsiStats[dsiId].dsiRtnLpTransSyncErr++;
        // HS Receive Timoutout Error
        if (pTavor2DisplayDriver->dsiAckData[dsiId] & DSI_PACKET_ERROR_HS_RECEIVE_TIMEOUT)
            pDisplayDriver->statistics.dsiStats[dsiId].dsiRtnHsRcvTimeoutErr++;
        //  Flase Control Error
        if (pTavor2DisplayDriver->dsiAckData[dsiId] & DSI_PACKET_ERROR_FALSE_CONTROL)
            pDisplayDriver->statistics.dsiStats[dsiId].dsiRtnFalseCtlErr++;
        // ECC Error single-bit
        if (pTavor2DisplayDriver->dsiAckData[dsiId] & DSI_PACKET_ERROR_ECC_1BIT)
            pDisplayDriver->statistics.dsiStats[dsiId].dsiRtnEcc1BitErr++;
        // ECC Error multi-bit
        if (pTavor2DisplayDriver->dsiAckData[dsiId] & DSI_PACKET_ERROR_ECC_MBIT)
            pDisplayDriver->statistics.dsiStats[dsiId].dsiRtnEccBitsErr++;
        // Checksum Error
        if (pTavor2DisplayDriver->dsiAckData[dsiId] & DSI_PACKET_ERROR_CHECKSUM)
            pDisplayDriver->statistics.dsiStats[dsiId].dsiRtnChecksumErr++;
        //  DSI Data Type Not Recognized
        if (pTavor2DisplayDriver->dsiAckData[dsiId] & DSI_PACKET_ERROR_DATA_UNKNOWN)
            pDisplayDriver->statistics.dsiStats[dsiId].dsiRtnDataTypeErr++;
        //   DSI VC ID Invalid 
        if (pTavor2DisplayDriver->dsiAckData[dsiId] & DSI_PACKET_ERROR_VCID_INVALID)
            pDisplayDriver->statistics.dsiStats[dsiId].dsiRtnVcIdErr++;
        //   Invalid Transmission Length 
        if (pTavor2DisplayDriver->dsiAckData[dsiId] & DSI_PACKET_ERROR_LENGTH_INVALID)
            pDisplayDriver->statistics.dsiStats[dsiId].dsiRtnTransLenErr++;
        //   DSI Protocol Violation
        if (pTavor2DisplayDriver->dsiAckData[dsiId] & DSI_PACKET_ERROR_PROTOCOL_VIOLATION)
            pDisplayDriver->statistics.dsiStats[dsiId].dsiRtnDsiProErr++;

    }

    // False Controll  
    if (regVal1 & FALSE_CTL_ERR)
    {
        // Check if Manual BTA is needed for PV2 Z0/Z1 BTA workaround
        if ( pTavor2DisplayDriver->dsiBtaSt[dsiId] == btaIssued)
            tavorpv2_dsiIssueBTA(dsiId,TRUE);
            
        //PRINT("False Control Interrupt: Check if BTA workaround is needed.");

        pDisplayDriver->statistics.dsiStats[dsiId].dsiFalseCtlErr++;

    }

    // VSYNC Start Of Transmission
    if (regVal1 & SOF_ST)
        pDisplayDriver->statistics.dsiStats[dsiId].dsiSoF++;

    // Start Of Lane
    if (regVal1 & SOL_ST)
        pDisplayDriver->statistics.dsiStats[dsiId].dsiSoL++;

    // If error happens and if test mode is not continous running mode, post a message to test code
    if ( errorFlag)
    {
        // Post message to Test code if it's not in continous running mode
        pTavor2DisplayDriver->dispMsg.Event = (DWORD) PLATFORM_MSG_DISP_DRV;
        pTavor2DisplayDriver->dispMsg.SubMsg = (DWORD) LCD_MSG_DSI_ERR;
        RE_RimPostMessage( PLATFORM_THREAD_ID_APP0, &(pTavor2DisplayDriver->dispMsg));
    }

    // Clear all interrupts and return
    //WriteRegDword(DSI_DxINST0(dsiId), 0x000000FF);
    //WriteRegDword(DSI_DxINST1(dsiId), 0xFFFFFFFF);
    WriteRegDword(DSI_DxINST0(dsiId), regVal0);
    WriteRegDword(DSI_DxINST1(dsiId), regVal1);

    return;

}


/*****************************************************************************
 * Function:    tavorpv2_drvLcdIsr 
 * Description: Actual Interrupt Hanlder for LCD Controller
 * Parameters:  DispDrv*
 * Return:      void
 *
 *****************************************************************************/
static void tavorpv2_drvLcdIsr(DispDrv *pDisplayDriver)
{
    DWORD regVal0 = 0;

    PRINT("DISP controller interrupts happened");

    // Disable all interrupts first
    // WriteRegDword(DSI_DxINEN(dsiId), 0x00000000);
    
    regVal0 = ReadRegDword(LCD_CTL_INT_STS);
    if (regVal0 & 0x07)
    {
        PRINT2N("LCD controller interrupt happended with Global Interup Status Reg(0x%08x)= 0x%08x ", LCD_CTL_INT_STS, regVal0);
        WriteRegDwordOr(LCD_CTL_INT_STS, 0x00000007);
    }

    regVal0 = ReadRegDword(LCD_FETCH_INT_STS0);
    if (regVal0 & 0x007F007F)
    {
        PRINT2N("LCD controller interrupt happended with Fectch Unit0(0x%08x)= 0x%08x ", LCD_FETCH_INT_STS0, regVal0);
        WriteRegDwordOr(LCD_FETCH_INT_STS0, 0x007F007F);
    }
    
    regVal0 = ReadRegDword(LCD_FETCH_INT_STS1);
    if (regVal0 & 0x007F007F)
    {
        PRINT2N("LCD controller interrupt happended with Fectch Unit0(0x%08x)= 0x%08x ", LCD_FETCH_INT_STS1, regVal0);
        WriteRegDwordOr(LCD_FETCH_INT_STS1, 0x007F007F);
    }    

    return;
}

/*****************************************************************************
 * Function:    tavorpv2_dsiDmaHandler 
 * Description: Interrupt Hanlder for DSI MVED DMA interrupt
 * Parameters:  DispDrv*
 * Return:      void
 *
 *****************************************************************************/
static void tavorpv2_dsiDmaHandler( DWORD dmaChannel )
{
    DWORD dcsr = ReadRegDword(DMA_DCSR_REG32(dmaChannel));

    DMA_DISABLE_CHANNEL( dmaChannel );
    while( DMA_IS_CHANNEL_ACTIVE(dmaChannel) );

    if( dcsr & DMA_DCSR_MASK_ENDINTR ) {
        // DMA transfer completed
        WriteRegDwordOr( DMA_DCSR_REG32(dmaChannel), DMA_DCSR_MASK_ENDINTR );
    }

    if( dcsr & DMA_DCSR_MASK_STOPINTR ) {
        // DMA transfer stopped
        DMA_DISABLE_STOPINTR( dmaChannel );
    }

    if( dcsr & DMA_DCSR_MASK_RASINTR ) {
        // DMA request after channel stopped
        WriteRegDwordOr( DMA_DCSR_REG32(dmaChannel), DMA_DCSR_MASK_RASINTR );
    }

    if( dcsr & DMA_DCSR_MASK_STARTINTR ) {
        // DMA descriptor loaded
        WriteRegDwordOr( DMA_DCSR_REG32(dmaChannel), DMA_DCSR_MASK_STARTINTR );
    }

    if( dcsr & DMA_DCSR_MASK_BUSERRINTR ) {
        // Bus error
        WriteRegDwordOr( DMA_DCSR_REG32(dmaChannel), DMA_DCSR_MASK_BUSERRINTR );
    }

    PRINT2N( "MVED DMA Channel %d Interrupt Cleared (DCSR: 0x%08x)", dmaChannel, dcsr );
}

static void tavorpv2_dsi0DmaInt(void)
{
    tavorpv2_dsiDmaHandler( MVED_DMA_CHANNEL_0 );

    // Release semaphore
    RE_RimUnlockCountingSemaphore(&(Tavor2DisplayDriver.dsiDmaLock[0]));
}

static void tavorpv2_dsi1DmaInt(void)
{
    tavorpv2_dsiDmaHandler( MVED_DMA_CHANNEL_1 );

    // Release semaphore
    RE_RimUnlockCountingSemaphore(&(Tavor2DisplayDriver.dsiDmaLock[1]));
}


// Event
static void tavorpv2_dsiSetEvent(DispDrv *pDisplayDriver, DWORD dsiId, DWORD eventFlag)
{
    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    // Valid dsiId
    if ( dsiId > 1) return;
    // Valid Flag
    if ( eventFlag & (eventFlag -1)) return;

    pTavor2DisplayDriver->dsiEventFlags[dsiId] |= eventFlag;

}

static void tavorpv2_dsiClearEvent(DispDrv *pDisplayDriver,DWORD dsiId, DWORD eventFlag)
{
    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    // Valid dsiId
    if ( dsiId > 1) return;
    // Valid Flag
    if ( eventFlag & (eventFlag -1)) return;

    pTavor2DisplayDriver->dsiEventFlags[dsiId] &= ~eventFlag;

}

static BOOL tavorpv2_dsiWaitForEvent(DispDrv *pDisplayDriver, DWORD dsiId, DWORD eventFlag)
{
    DWORD regTimeOut = 0;
    
    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    // Valid dsiId
    if ( dsiId > 1) return FALSE;
    // Valid Flag
    if ( eventFlag & (eventFlag -1)) return FALSE;

    // Wait for the event until timeout
    do
    {
        if ( (pTavor2DisplayDriver->dsiEventFlags[dsiId]) & eventFlag)
            break;
        
        RE_HwDelay(RE_MICROSECOND);

        regTimeOut += RE_MICROSECOND;
    
    } while (regTimeOut <= 2000*TAVORPV2_REG_TIMEOUT);
    
    if (regTimeOut > 2000*TAVORPV2_REG_TIMEOUT) 
    {
        WARN("tavorpv2_dsiWaitForEvent: time Out");
        return FALSE;
    } 
    else
        return TRUE;
}


/**************************************************************************************
* Function:    tavorpv2_drvLcdSetMode 
* Description: Configure DSI/Display Controllers based on settings on corresponding saved config structure
*                     
* Parameters: DispDrv*
* Return:       TRUE/FALSE
**************************************************************************************/

static BOOL tavorpv2_drvLcdSetMode(DispDrv *pDisplayDriver)
{
    lcdDsiConfig *dsiConfig = NULL;
    DWORD i;
    DWORD modeIx;

    // Set DSI controller configurations
    for (i = 0; i< TAVORPV2_DSI_CTL; i++)
    {
        dsiConfig =(lcdDsiConfig *) (&(pDisplayDriver->config.dsiConfig[i]));

        if ( dsiConfig->status == DSI_ENABLED)
        {
            // Find the lcdMode based on DSI clock and panel Id
            modeIx = tavorpv2_dispModeFind(pDisplayDriver, dsiConfig->dsiLinkClock, dsiConfig->panelId);
            PRINTN("modeIx = 0x%08x",modeIx);
            if (modeIx != 0xFF)
            {
                dsiConfig->pMode = (lcdMode *) (pDisplayDriver->pModeList + modeIx);
                PRINT3N("tavorpv2_drvDsiSetMode: colorDepth=0x%08x, panelId=0x%08x, LPX=0x%08x", dsiConfig->pMode->colorDepth,
                         dsiConfig->pMode->panelId, dsiConfig->pMode->tLPX);
                tavorpv2_drvDsiSetMode(pDisplayDriver, dsiConfig->pMode, i);
            }    
        }    
    }    

    // Set Display Controller configurations
    tavorpv2_drvDispSetMode(pDisplayDriver);

    return TRUE;
    
}



// SetMode will do all hardware-level settings based on configurations and at last enable corresponding controller
static BOOL  tavorpv2_drvDsiSetMode(DispDrv *pDisplayDriver, lcdMode *pMode, DWORD dsiId)
{
    BOOL rc = FALSE;
    lcdDsiConfig *dsiConfig = NULL;
    DWORD dsiDxPhyTim0, dsiDxPhyTim1, dsiDxPhyTim2;

    dsiConfig =(lcdDsiConfig *) (&(pDisplayDriver->config.dsiConfig[dsiId]));

    // Check if invalid parameters
    if ( (dsiId >= TAVORPV2_DSI_CTL) || (pMode == NULL))
        goto cleanUp;

    // Check Enable/Disable status
    if (dsiConfig->status == DSI_ENABLED)
    {
        // Enable DSI Corresponding Mixer
        tavorpv2_dispCtlMixerEnable(dsiId,TRUE);
        
        // Set Convert ID
         if (dsiId == 0)
            tavorpv2_dispSetMixConvId(dsiId, MIXER_CONV_ID_S_CONV0);
        else
            tavorpv2_dispSetMixConvId(dsiId, MIXER_CONV_ID_S_CONV1);

        // Configure D-PHY Timings
        dsiDxPhyTim0 = (pMode->tLPX << 24) | (pMode->tHSPREP << 16)
                      | (pMode->tHSZERO << 8) | (pMode->tHSTRAIL);
        dsiDxPhyTim1 = (pMode->tHSEXIT << 24) | (pMode->tTAGO << 16)
                      | (pMode->tCLZERO << 8) | (pMode->tCLTRAIL);
        dsiDxPhyTim2 = (pMode->tTAGET << 16)  | (pMode->tWAKEUP);
        tavorpv2_dispConfigureInterfaceTiming(dsiId, dsiDxPhyTim0, dsiDxPhyTim1, dsiDxPhyTim2);

        //Also change DSI interface clock (BIT_CLOCK)
        tavorpv2_dsiSetIntClock(dsiId, dsiConfig->dsiLinkClock);

        // Minimal initialization for DSI controller (follow Marvell workaround of "Smart panel underflow")
        tavorpv2_lcdInitDsi(dsiId);

        // Enable DSI
        tavorpv2_dsiCtlEnable(dsiId,DSI_CTL_ENABLE);
            
        // Set Power Mode         
        tavorpv2_dsiCtlSetPowerMode(dsiId, dsiConfig->powerMode);     

        // Set Active Lanes
        tavorpv2_dsiCtlSetActiveLane(dsiId, dsiConfig->activeLanes);

        // Set VC mapping based on number of LCD panels
        tavorpv2_dsiCtlSetVcMap(dsiId, dsiConfig->panels);
    }
    else
        // Not enabled - disable it
        tavorpv2_dsiCtlEnable(dsiId, DSI_CTL_DISABLE);

     rc = TRUE;
cleanUp:
    return rc;
}


/* SetMode for Display Controller */
static void  tavorpv2_drvDispSetMode(DispDrv *pDisplayDriver)
{
    //Set Gamma Correction to default value
    tavorpv2_dispCtlSetDefaultGamma();    
    return;        
}    

/**************************************************************************************
* Function:    tavorpv2_drvLvlLcdEnable 
* Description: Enable DSI/LCD controller - including SetMode to configure DSI/LCD registers
*                     
* Parameters: DispDrv*
* Return:       DRIVER_NO_ERROR
**************************************************************************************/
static int tavorpv2_drvLcdEnable(DispDrv *pDisplayDriver)
{
    // Enable interrupts
    EnableInterrupt( HWIRQ_DSI0 );
    EnableInterrupt( HWIRQ_DSI1 );
    // EnableInterrupt( HWIRQ_DISP1 );

    // Call SetMode for register settings (SetMode will config and enable controller)
    tavorpv2_drvLcdSetMode(pDisplayDriver); 

    // 2 DSI controllers can be enabled respectively
    if (pDisplayDriver->config.dsiConfig[0].status == DSI_ENABLED)
        pDisplayDriver->status.dsiStatus[0].enabled  = TRUE;
    if (pDisplayDriver->config.dsiConfig[1].status == DSI_ENABLED)
        pDisplayDriver->status.dsiStatus[1].enabled  = TRUE;

    pDisplayDriver->status.dispStatus.enabled = TRUE;

    return (DRIVER_NO_ERROR);
}    


/**************************************************************************************
* Function:    tavorpv2_drvLvlLcdDisable 
* Description: Disable DSI/LCD controller 
*                     
* Parameters: DispDrv*
* Return:       DRIVER_NO_ERROR
**************************************************************************************/
static int tavorpv2_drvLcdDisable(DispDrv *pDisplayDriver)
{
    // Set stop flag
	lcdStopFlag = TRUE;

    // Disable DSI interrupt
    DisableInterrupt(HWIRQ_DSI0);
	DisableInterrupt(HWIRQ_DSI1);

    //Disable DSI controller    
    tavorpv2_dsiCtlEnable(0, DSI_CTL_DISABLE);
    tavorpv2_dsiCtlEnable(1, DSI_CTL_DISABLE);

    // Disable LCD controller :Use Quick Disable instead of normal disable
    tavorpv2_dispCtlEnable(DISP_CTL_QDISABLE);

    // Set Disable Flag for both DSI controllers
    pDisplayDriver->status.dsiStatus[0].enabled  = FALSE;
    pDisplayDriver->status.dsiStatus[1].enabled  = FALSE;
    pDisplayDriver->status.dispStatus.enabled = FALSE;

    return (DRIVER_NO_ERROR);    
}    

/**************************************************************************************
* Function:    tavorpv2_drvLcdWriteCmd 
* Description: Dispatch and execute command from test code 
*                     
* Parameters: DispDrv*, command, parameters followed command
* Return:       DRIVER_NO_ERROR or ERRORs
**************************************************************************************/
static int tavorpv2_drvLcdWriteCmd(DispDrv *pDisplayDriver, DWORD cmd, void *param)
{
    int rc = DRIVER_INVALID_HW_PORT;

    Data4Lcd_t *dsiCmds = NULL;
    Cmd4Dsi_t *dsiCmd = NULL;
    
    if ( cmd == DISP_CNTRL_WRITE_DSI)
        // Not implemented yet
        dsiCmds = (Data4Lcd_t *)param;
    else
        //convert param to structrue
        dsiCmd = (Cmd4Dsi_t *) param; 

    switch (cmd)
    {
        // Reset DSI command FIFO - empty FIFO
        case DISP_CNTRL_DSI_RESET_CMD_FIFO:
            if (tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver,dsiCmd->dsiId))
                rc = DRIVER_NO_ERROR;
            break;
        // Write one DSI comamnd entry to DSI command FIFO
        case DISP_CNTRL_DSI_WRITE_ONE_CMD:
            if (tavorpv2_dsiAddCmd2Buffer(pDisplayDriver,dsiCmd->dsiId, dsiCmd->lcdCmd, dsiCmd->dsiData, dsiCmd->vcMap, dsiCmd->loopFlag))
                rc = DRIVER_NO_ERROR;
            break;
        // Execute all commands in DSI command FIFO
        case DISP_CNTRL_DSI_EXECUTE_CMD_FIFO:
            // End of command sequence: always try to trigger a DxINST0[SP_GEN_INT_STS] interrupt
            if (tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiCmd->dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00))
                if (tavorpv2_dsiSendCmdSeq(pDisplayDriver,dsiCmd->dsiId))
                    rc = DRIVER_NO_ERROR;
            break;
        case DISP_CNTRL_WRITE_DSI:
            // Convert dsiCmds->lcdPanelId to dsiId here
            tavorpv2_dsiWriteCmd(pDisplayDriver, dsiCmds->lcdPanelId, dsiCmds->dataAddr, dsiCmds->length);    
            break;
        case DISP_DUMP_REGS:
            // Dump All LCD/DSI Registers
            tavorpv2_lcdRegsDump(0);
            break;

        case DISP_CNTRL_DSI_ENTER_ULPS:
            // Enter ULPS mode
            // pDisplayDriver->config.dsiConfig[dsiCmd->dsiId].powerMode = DSI_POWERMODE_ENTER_ULPS;      
            tavorpv2_dsiCtlSetPowerMode(dsiCmd->dsiId, DSI_POWERMODE_ENTER_ULPS);
            break;

        case DISP_CNTRL_DSI_EXIT_ULPS:
            // Exit ULPS
            // pDisplayDriver->config.dsiConfig[dsiCmd->dsiId].powerMode = DSI_POWERMODE_EXIT_ULPS;      
            tavorpv2_dsiCtlSetPowerMode(dsiCmd->dsiId, DSI_POWERMODE_EXIT_ULPS);
            break;

        case DISP_CNTRL_DSI_RESET_STATISTICS:
            // Reset Statistics
            tavorpv2_dsiResetStatis(pDisplayDriver, dsiCmd->dsiId);
            break;
        case DISP_CNTRL_DSI_INIT_LCD_PANEL:
			PRINT("Init DISP_CNTRL_DSI_INIT_LCD_PANEL");
			if (dsiCmd->dsiData == 0x00)
                tavorpv2_dcsTpoLcdInit(pDisplayDriver, dsiCmd->dsiId, dsiCmd->dsiData);         
			else if (dsiCmd->dsiData == 0x02)
				tavorpv2_dcsProteusLcdInit(pDisplayDriver,dsiCmd->dsiId, dsiCmd->dsiData);
            break;	

       case DISP_CNTRL_DSI_STOP_TRAFFIC:
	   	    // Stop traffic on DSI interfaces
	   	    lcdStopFlag = TRUE;
	   	    break;

        default:
            break;

    }

    return (rc);
}


/**************************************************************************************
* Function:    tavorpv2_drvLcdReadData 
* Description:  Read LCD panel' s internal frame buffer or registers
*                     
* Parameters: DispDrv*, Data4Lcd_t *
* Return:       DRIVER_NO_ERROR or ERRORs
**************************************************************************************/

static int tavorpv2_drvLcdReadData(DispDrv *pDisplayDriver, void *param)
{
    // Check if interrupt happened
    Data4Lcd_t *lcdData = (Data4Lcd_t *)param;
    DWORD dsiId;
    DWORD rc = DRIVER_UNKNOWN_ERROR;

    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    // Find DSI controller ID by displayId
    dsiId = tavorpv2_dispGetDsiId(pDisplayDriver, lcdData->lcdPanelId);
    if (dsiId >= TAVORPV2_DSI_CTL)
        goto cleanUp;

	pTavor2DisplayDriver->lcdRdData = (BYTE *)lcdData->dataAddr;
    pTavor2DisplayDriver->lcdRdLen = lcdData->length;

    PRINT2N("tavorpv2_drvLcdReadData: lcdData=0x%08x, lcdRdLen=0x%08x",pTavor2DisplayDriver->lcdRdData, pTavor2DisplayDriver->lcdRdLen);
	// Trig "Read Start"
	tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId); 	
	tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_READ_NO_PARAMETER, 0, 0);
	tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DCS_CMD_READ_MEMORY_CONTINUE, 0, 0);
	tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0, 0, 0);
	// Issue BTA
	tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_READ_FRAME_BUFFER, 0x00, 0, 0);
	tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_NO_OPERATION, 0x42, 0x00, 0x00);
	// Trig "int proc"
	tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00);

cleanUp:
    return (rc);
}


/**************************************************************************************
* Function:    tavorpv2_drvLcdWriteData 
* Description: Write & display pixels to the specified LCD panel 
*                     
* Parameters: DispDrv*, Data4Lcd_t *
* Return:       DRIVER_NO_ERROR or ERRORs
**************************************************************************************/
static int tavorpv2_drvLcdWriteData(DispDrv *pDisplayDriver, void *param)
{

    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;
    DWORD dsiId = 0xFF;

    //parse param to frame buffer address, length, fetch channel ,lcdPanelId, 
    Data4Lcd_t *lcdData = (Data4Lcd_t *) param;
    lcdMode *pMode = NULL;
    
    // Find DSI number according to panelId
    dsiId = tavorpv2_dispGetDsiId(pDisplayDriver, lcdData->lcdPanelId);

    if (dsiId == 0xFF)
        return (DRIVER_INVALID_HW_PORT);

    // Send one frame of pixels to LCD panel
    if (lcdData->dataType == DISP_DATA_PIXELS)
    {
        pTavor2DisplayDriver->pDispFrameBuffer = (WORD *)(lcdData->dataAddr);

        PRINT4N("DISP_DATA_PIXELS:  lcdData->dataAddr=%08X height=%08X width=%08X lcdPanelId=%08X",
                 lcdData->dataAddr, lcdData->height, lcdData->width, lcdData->lcdPanelId);        

        // Call actual API to send pixels
        pTavor2DisplayDriver->pDispFrameBuffer = (WORD *)(lcdData->dataAddr);
        if (pTavor2DisplayDriver->pDispFrameBuffer == NULL)
            pTavor2DisplayDriver->pDispFrameBuffer = (WORD *)(&dispFrameBuffer[0]);
        // Always go through pixel pipeline
        tavorpv2_dispCopyToLcd(pDisplayDriver,
                               (DWORD )(pTavor2DisplayDriver->pDispFrameBuffer),
                               lcdData->height, lcdData->width, 
                               lcdData->repeatTimes, lcdData->lcdPanelId);
        // Overlay feature is not supported now
        if (0)
        {
            WARN("Overlay feature currently is not supported if not backplane surface!");
        }
    }

    // Issue a manual BTA
    if (lcdData->dataType == DISP_DATA_DSI_BTA)
    {
        if (! tavorpv2_dsiIssueBTA(dsiId, TRUE))
            return (DRIVER_INVALID_HW_PORT);
    }

    // Test case for color bar or frame buffer
    if (lcdData->dataType == DISP_TEST_COLOR_BAR)
    {
        //Initialize color bar
        pMode = pDisplayDriver->config.dsiConfig[lcdData->lcdPanelId].pMode;
        // Call actual API to send pixels
        pTavor2DisplayDriver->pDispFrameBuffer = (WORD *)(lcdData->dataAddr);
        if (pTavor2DisplayDriver->pDispFrameBuffer == NULL)
        {
            pTavor2DisplayDriver->pDispFrameBuffer = (WORD *)(&dispFrameBuffer[0]);
            tavorpv2_dispInitFrmBuf(pDisplayDriver);
        }    

#if !defined(TAVORPV2_HWVER_MG1)
        // TPO panelId is 0x00, use software workaround
        if (pMode->panelId == 0x00)
        {
            // Using DSI Cmd FIFO to send pixels for TPO -Reset pointer and trig "int proc" to send pixels
            pTavor2DisplayDriver->pDispFrmPtr = 0x00;
            tavorpv2_dsiTrigIntProc(pDisplayDriver, dsiId);
        }
        else
#endif

        // Send pixels to the panel
        tavorpv2_dispCopyToLcd(pDisplayDriver,
                               (DWORD )(pTavor2DisplayDriver->pDispFrameBuffer),
                               pMode->height,pMode->width, 
                               0, lcdData->lcdPanelId);
    }
    
    return (DRIVER_NO_ERROR);
}

static void tavorpv2_drvLcdCopyToLcd(DispDrv *pDisplayDriver,DWORD src, DWORD high, DWORD wide, DWORD displayWidth, BYTE displayId )
{
    //Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;
    DWORD dsiId = 0xFF;
    lcdMode *pMode = NULL;

    // Find DSI number according to panelId
    dsiId = tavorpv2_dispGetDsiId(pDisplayDriver, displayId);

    if (dsiId == 0xFF)
        return;

    pMode = pDisplayDriver->config.dsiConfig[dsiId].pMode;

    if (pMode == NULL)
        return;

#if !defined(TAVORPV2_HWVER_MG1)
    // Software workaround is used if panelId is 0x00
    if (pMode->panelId == 0x00)
    {
        // We have to use software workaround to send pixels from DSI command FIFO until B0

        // Reset Frame Buffer's pointer
        pTavor2DisplayDriver->pDispFrmPtr = 0x00;
        pTavor2DisplayDriver->pDispFrameBuffer = (WORD *)src;
        if (!tavorpv2_dispCopyToLcdFromDsiWka(pDisplayDriver, src, high, wide, displayId))
        {
            PRINT("tavorpv2_drvLvlLcdCopyToLcd: failed from DSI cmd FIFO!");    
        }
    }
    else
#endif

    {
        // Using pixel pipeline
        if (!tavorpv2_dispCopyToLcd(pDisplayDriver, src, high, wide, 0, displayId))
        {
            PRINT("tavorpv2_drvLvlLcdCopyToLcd: failed from pixel pipeline!");    
        }
    }
    return;
}


/**************************************************************************************
* Function:    tavorpv2_drvLvlLcdDebug 
* Description: BugDisp cmd handler - dump registers, run test cases etc  
*                     
* Parameters: DispDrv*, command catergory, parameters followed command
* Return:       void
**************************************************************************************/
static void tavorpv2_drvLcdDebug(DispDrv *pDisplayDriver, DWORD catergory, DWORD arg)
{
    /* catergory: 
                    0 - dump all LCD/DSIs/Clock controller registers
                    1 - dump all Clock controller registers
                    2 - dump all DSI controller registers
                    3 - dump all LCD controller registers
                    4 - run a specific test case
                    others - dump a specific register
      */ 

    // Dump All DSI Registers here
    PRINT("================= TavorPV2 DSI/Display Module Debug =================");

    if ( catergory < 4)
    {
        tavorpv2_lcdRegsDump(catergory);
    }

    // Run Test case for traffic generation here
    else if (catergory == 4)
    {
        PRINT("================= TavorPV2 Debug: Traffic Generation Pattern Test =================");
        tavorpv2_drvLcdTest(pDisplayDriver, arg);    
    }

        
    //Dump some registers value with specified physical address
    else
    {
        DWORD i = 0;
        for (i = 0; i < arg; i++)
        {
            PRINT2N("                       Register(0x%08X)           = %08X", catergory + i*4, ReadRegDword(catergory + i*4));
        }
    }
}



static void tavorpv2_drvLcdIntEvtWait(DispDrv *pDisplayDriver)
{
    return;
}


/* LM2550 Intitialization */
static BOOL tavorpv2_desLm2550Init(DispDrv *pDisplayDriver, DWORD dsiId, DWORD powerMode)
{
    // Any specific reason to set VC = 0x03?
    BYTE vcId = 0x03;
    BYTE loopFlag = 0x00;
    //DWORD regVal = 0;

    // LM2550: to access LM2550 registers, DSI controller should be put under low-power mode
    tavorpv2_dsiCtlSetPowerMode(dsiId, DSI_POWERMODE_LPDT);     

    // Empty DSI coommand FIFO
    tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);    

    // 1. Config register VC0_CFG_CPU : 16bit CPU interface, Enable DCS translate for DCS long write, CPU interface Master Mode    
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, vcId, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, LM2550_VC0_CFG_CPU, 0, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, LM2550_CPU_MODE_16BIT | LM2550_DCS_ENABLE | LM2550_TARGET_CPU_MASTER, 0, loopFlag);

    // 2. Config CPU_CFG : CPU interface Clock setting
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, vcId, loopFlag);
    // Configure CPU Interface Clock based on DSI clock : CPU interface clock = dsiClock/(2*div) , temperary set to 10M
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, LM2550_CPU_CFG,  0, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, LM2550_CPU_CFG_CPUDIV(0xFH), 0, loopFlag);
    
    // Need Dummy R/D insert?

    // 3. Config TE_CFG (Suppose  ThorIC is rising edge)
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, vcId, loopFlag);
    // Enable all TEs as Rising Effective
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, LM2550_TE_CFG, 0, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, LM2550_TE_CFG_TE0_RISING | LM2550_TE_CFG_TE1_RISING | LM2550_TE_CFG_TE2_RISING, 0, loopFlag);

    // 4. Config DSI_CFGA, by default capture all errors
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, vcId, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE,  LM2550_DSI_CFGA, 0, loopFlag);

    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE,  \
        LM2550_DSI_CFGA_SOT | LM2550_DSI_CFGA_SOT_SYNC \
        | LM2550_DSI_CFGA_EOT_SYNC | LM2550_DSI_CFGA_ESC_ENTRY \
        | LM2550_DSI_CFGA_LPTX_SYNC | LM2550_DSI_CFGA_HSRX_TO \
        | LM2550_DSI_CFGA_CONTROL, \
        0, loopFlag);

    // 5. Config DSI_CFGB
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, vcId, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, LM2550_DSI_CFGC, 0, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE,  \
        LM2550_DSI_CFGB_ECC_SINGLE | LM2550_DSI_CFGB_ECC_MULTI \
        | LM2550_DSI_CFGB_CHECKSUM | LM2550_DSI_CFGB_BAD_DT \
        | LM2550_DSI_CFGB_BAD_VC | LM2550_DSI_CFGB_BAD_LEN \
        | LM2550_DSI_CFGB_PROTOCOL, \
        0, loopFlag);


    // 6. Config DSI_CFGC - config lanes
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, vcId, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, LM2550_DSI_CFGC, 0, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE,  \
        LM2550_DSI_CFGC_LP0 | LM2550_DSI_CFGC_LP1 \
        | LM2550_DSI_CFGC_HOLD | LM2550_DSI_CFGC_IGN_HSCMD \
        | LM2550_DSI_CFGC_SYNC_SHAPE | LM2550_DSI_CFGC_LANES(3) , \
        0, loopFlag);

    // 7. Config DSI_CFGD - timeout settings, keep reset vaule

    // 8. Config DSI_CFGE - LPDIVO / LPDIVS clock, keep default 

    // 9. Config DSI_CFGF - SETTLE/LPTX_TO keep default

    // 10. Config DSI_CFGG - VC enable, keep default settings to enable all VC0 - VC3
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, vcId, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE,  LM2550_DSI_CFGG, 0, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE,  \
        LM2550_DSI_CFGG_VC0_EN | LM2550_DSI_CFGG_VC1_EN \
        | LM2550_DSI_CFGG_VC2_EN | LM2550_DSI_CFGG_VC3_EN,  \
        0, loopFlag);

    // 11. Config ERR_CMD to clear all errors statistics
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, vcId, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, LM2550_ERR_CMD, 0, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE,  \
        LM2550_ERR_CMD_CLR_CNT | LM2550_ERR_CMD_CLR_PROT | LM2550_ERR_CMD_CLR_CONTEND, \
        0, loopFlag);

    // 12. Config CFGVALID to make configurations effective, enable CPU interface clock
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, vcId, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, LM2550_CFGVALID, 0, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE,  \
        LM2550_CFGVALID_CFG_VALID | LM2550_CFGVALID_CFG_CLK_EN , \
        0, loopFlag);


    //Verify initialization by reading back LM2550 Registers
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_READ_1_PARAMETER, vcId, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, LM2550_VC0_CFG_CPU, 0, loopFlag);
    tavorpv2_dsiSendCmdSeq(pDisplayDriver, dsiId);
    tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);
    tavorpv2_drvLcdTest(pDisplayDriver, 0xf4);

    // Disable DSI/Display controller    
    tavorpv2_drvLcdTest(pDisplayDriver, 0xf1);

    // Bus Turn Around  
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_READ_STATUS, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x00, 0x00, 0x00); 
    
    // Interrupt Processor 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x00, 0x00, 0x00); 

    // Disable DSI/Display
    tavorpv2_drvLcdTest(pDisplayDriver, 0xf1);
    
    tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);    
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_READ_1_PARAMETER, vcId, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, LM2550_VC1_CFG_CPU, 0, loopFlag);

    // Bus Turn Around  
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_READ_STATUS, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x00, 0x00, 0x00); 
    // Interrupt Processor 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x00, 0x00, 0x00); 

    tavorpv2_dsiSendCmdSeq(pDisplayDriver, dsiId);


    // Disable DSI/Display
    tavorpv2_drvLcdTest(pDisplayDriver, 0xf1);
    
    tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);    
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_READ_1_PARAMETER, vcId, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, LM2550_VC2_CFG_CPU, 0, loopFlag);

    // Bus Turn Around  
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_READ_STATUS, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x00, 0x00, 0x00); 

    // Interrupt Processor 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x00, 0x00, 0x00); 

    tavorpv2_dsiSendCmdSeq(pDisplayDriver, dsiId);


    // Disable DSI/Display
    tavorpv2_drvLcdTest(pDisplayDriver, 0xf1);

    tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);    
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_READ_1_PARAMETER, vcId, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, LM2550_CPU_CFG, 0, loopFlag);

    // Bus Turn Around  
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_READ_STATUS, 0, 0, 0);
    // Interrupt Processor 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 

    tavorpv2_dsiSendCmdSeq(pDisplayDriver, dsiId);


    // Disable DSI/Display
    tavorpv2_drvLcdDisable(pDisplayDriver);
    
    tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);    
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_READ_1_PARAMETER, vcId, loopFlag);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, LM2550_DSI_CFGA, 0, loopFlag);
    tavorpv2_dsiSendCmdSeq(pDisplayDriver, dsiId);

    // Bus Turn Around  
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_READ_STATUS, 0, 0, 0);
    // Interrupt Processor 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 

    tavorpv2_dsiSendCmdSeq(pDisplayDriver, dsiId);

    return TRUE;
}    


/* Initialize TPO DSI Smart Panel */
static BOOL tavorpv2_dcsTpoLcdInit(DispDrv *pDisplayDriver, DWORD dsiId, DWORD panelId)
{
    BYTE width,height;
    WORD colorDepth, regVal;
    BOOL rc = FALSE;
    lcdMode *pMode = NULL;
    
    if (dsiId >= TAVORPV2_DSI_CTL) 
    {
        WARN("tavorpv2_dcsSmartLcdInit: Only 2 DSI controllers are supported on PV2!");
        goto cleanUp;
    }    

    PRINT("tavorpv2_dcsSmartLcdInit: Initialize Lemur TPO panel with DCS command!");

    pMode = pDisplayDriver->config.dsiConfig[dsiId].pMode;

    // Get the Required LCD Panel Color Depth
    colorDepth = pMode->colorDepth;
    
    //Force to be RGB24?
    if ((colorDepth != 24) && (colorDepth != 16))
    {
        PRINT("tavorpv2_dcsSmartLcdInit: TPO LCD panel only supports RGB24 and RGB16 format!");
        goto cleanUp;
    }
    
    // Always set DSI controller active lanes to be 2 for Lemur/TPO 
#if !defined(TAVORPV2_HWVER_MG1)
    // Force to 1 active data lane as a workaround for Z1
    if (pDisplayDriver->config.dsiConfig[dsiId].powerMode == DSI_POWERMODE_LPDT)
        tavorpv2_dsiCtlSetActiveLane(dsiId, 0x01);      
#endif

    // Enable DSI controller if not enabled before sending BTA/Reset
    tavorpv2_dsiCtlEnable(dsiId, DSI_CTL_ENABLE);

    // Reset Comand FIFO buffer
    tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     

    // Set NOP delay to a minimum value  for delay ms level purpose (1 cycle  = 3.2 ns for 312Mhz)
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_SET_DLY_MULT, 0x00, 0x00, 0x00);

    //Send "Soft Reset" and delay 120 ms before any other command 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_SHORT_WRITE_NO_PARAMETER, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SOFT_RESET, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    // Insert Interrupt proc to execute command
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
    // Delay 120ms for soft reset
    RE_HwDelay(120*RE_MILLISECOND);

    // Reset Comand FIFO buffer
    tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     

    //Step 1: Force TPO be "Sleep Out" and Delay 5ms 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_SHORT_WRITE_NO_PARAMETER, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_EXIT_SLEEP_MODE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    // Insert Interrupt proc to execute command
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
    // Delay 5ms for sleep out
    RE_HwDelay(5*RE_MILLISECOND);

    // Reset Comand FIFO buffer
    tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     

    //Step 2: Turn On Peripheral DSI command
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_SHORT_WRITE_NO_PARAMETER, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_DISPLAY_ON, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    
    //Step 3: set brightness to be maximum
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_SHORT_WRITE_1_PARAMETER, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_WRITE_BRIGHTNESS, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xff, 0, 0);
    
    //Step 4: Enter into normal mode
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_SHORT_WRITE_NO_PARAMETER, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_ENTER_NORMAL_MODE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    // Insert Interrupt proc to execute command
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
    // Delay 5ms
    RE_HwDelay(5*RE_MILLISECOND);

    // Reset Comand FIFO buffer
    tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     
 
    //Step 5: Set Maximum Return Packet Size : 2
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_SET_MAX_RETURN_PACKET_SIZE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x02, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);

    //Step 6: Set Pixel Format to match with Panel (RGB24 by default)
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_SHORT_WRITE_1_PARAMETER, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_PIXEL_FORMAT, 0, 0);
    if (colorDepth == 16)
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x05, 0, 0);
    else if (colorDepth == 18)
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x06, 0, 0);
    else if (colorDepth == 24)
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x07, 0, 0);
    else
    {
        WARN("tavorpv2_dcsSmartLcdInit: Unsupported LCD panel color depth. Only RGB 16/18/24 are supported!");
        goto cleanUp;
    }        
    

    /*WVGA landscape mode - 864*480*24 */
    //Step 7: DCS Long Write to set start/end of Column Address
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x05, 0, 0); // Length LS byte
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_COLUMN_ADDRESS, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0); // Start column address (MSB)
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    regVal = pMode->width - 1; 
    width = (BYTE)(( regVal & 0xFF00) >> 8);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, width, 0, 0); // End column address (MSB)
    width =(BYTE) (regVal & 0x00FF);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, width, 0, 0); // LSB
        
    //Step 8: DCS Long Write to set start/end Page Address
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x05, 0, 0); // Length LS byte
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_PAGE_ADDRESS, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0); // Start Page address (MSB)
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    regVal = pMode->height - 1; 
    height = (BYTE)(( regVal & 0xFF00) >> 8);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, height, 0, 0); // End Page address (MSB)
    height =(BYTE) (regVal & 0x00FF);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, height, 0, 0); // LSB

    // Set CTRL to force backlight on
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_SHORT_WRITE_1_PARAMETER, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_WRITE_CTRL, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x04, 0, 0);

    // Get CTRL (Samsung special command)
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_READ_NO_PARAMETER, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_READ_CTRL, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
        
    // "Read Status" causes BTA
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_READ_STATUS, 0x00, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x10, 0x00, 0x00);

    // Insert Interrupt proc for the last command
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 

    // Initialization Finisehd
    rc = TRUE;
cleanUp:
    return rc;
}

/* Sending a bunch of DSI commands by one shot */
static  BOOL tavorpv2_dsiWriteCmd(DispDrv *pDisplayDriver, DWORD dsiId, DWORD src, DWORD length)
{
    WORD i = 0;
    WORD *src16 = (WORD *) src;     
    //Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    if (dsiId >= TAVORPV2_DSI_CTL) 
    {
        WARN("tavorpv2_dsiWriteCmd: Only 2 DSI controllers are supported!");
        return FALSE;
    }    


    if ((length == 0) || (src == 0 )) return FALSE;

    // length is based on 16-bit (DSI command entries
    tavorpv2_dsiStart(dsiId);
    while (length > 0)
    {        
        tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);

        for ( i = 0; i < DSI_CMD_FIFO_LEN; i++)
        {
            if (length == 0 ) 
                break;
            else 
                length--;
            tavorpv2_dsiWriteCmd2Fifo(dsiId, *((WORD *)src16) & 0xFFFF);
            src16++;   
        }
    }

    return TRUE;
}

#if defined(PV2_HW_VER_B0)

/* Send Command Sequence from internal Command FIFO Buffer*/
static BOOL tavorpv2_dsiResetCmdFifoBuf(DispDrv *pDisplayDriver, DWORD dsiId)
{
    WORD i = 0;    
    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    if (dsiId >= TAVORPV2_DSI_CTL) 
    {
        WARN("tavorpv2_dsiResetCmdFifoBuf: tavorpv2_dsiResetCmdFifoBuf: Only 2 DSI controllers are supported!");
        return FALSE;
    }    
    
    // Reset DSI CMD FIFO Buffer    
    pTavor2DisplayDriver->dsiCmdIndex[dsiId] = 0;
    // Fill in with NOP, no data, no VC, no loopFlag
    for (i = 0; i < DSI_CMD_FIFO_LEN; i++)
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_NO_OPERATION, 0, 0, 0);

    pTavor2DisplayDriver->dsiCmdIndex[dsiId] = 0;

    return TRUE;    
}


/* Setup Command Sequence for DSI command FIFO */
static BOOL tavorpv2_dsiAddCmd2Buffer(DispDrv *pDisplayDriver, DWORD dsiId, BYTE dsiCommand, BYTE dsiData, BYTE dsiVc, BYTE loopFlag)
{
    // Adjust to a ring buffer with front/back pointer?
    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;
    DWORD dsiCmdIndex = pTavor2DisplayDriver->dsiCmdIndex[dsiId];

    if (dsiId >= TAVORPV2_DSI_CTL) 
    {
        WARN("tavorpv2_dsiAddCmd2Buffer: Only 2 DSI controllers are supported!");
        return FALSE;
    }    


    if (dsiCmdIndex >= DSI_CMD_FIFO_LEN) 
    {
        WARN("tavorpv2_dsiAddCmd2Buffer: DSI command FIFO overflow. Please reset the FIFO first!");
        return FALSE;
    }    

    //clear the buffer
    pTavor2DisplayDriver->dsiCmdFifo[dsiId][dsiCmdIndex] = 0;

    // Virutal Channle always comes with dsiData. dsiCommand has no VC concetp. Marvell datasheet is wrong here.
    dsiData = dsiData | ((dsiVc & 0x03) << 6);
    
    pTavor2DisplayDriver->dsiCmdFifo[dsiId][dsiCmdIndex] = ( (dsiData & 0xFF)
        | ((dsiCommand & 0xFF) << 8)  
        | ((loopFlag & 0x01)<< 13));
    
    pTavor2DisplayDriver->dsiCmdIndex[dsiId]++;
	PRINTN("tavorpv2_dsiAddCmd2Buffer: dsiCmdIndex=0x%08x",pTavor2DisplayDriver->dsiCmdIndex[dsiId]);
    return TRUE;

}    


/* Send Command Sequence from internal Command FIFO Buffer*/
static BOOL tavorpv2_dsiSendCmdSeq(DispDrv *pDisplayDriver, DWORD dsiId)
{
    BOOL rc = FALSE;
    WORD i = 0;
    DWORD regValue = 0;

    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    if (dsiId >= TAVORPV2_DSI_CTL) 
    {
        WARN("tavorpv2_dsiSendCmdSeq: Only 2 DSI controllers are supported!");
        return FALSE;
    }    

    // Next step DMA will be used to transfer DSI command to CMD FIFO for acceleration here    
    for (i = 0; i < pTavor2DisplayDriver->dsiCmdIndex[dsiId]; i= i+2)
    {
        regValue = (((DWORD)pTavor2DisplayDriver->dsiCmdFifo[dsiId][i])  
            | ((DWORD)pTavor2DisplayDriver->dsiCmdFifo[dsiId][i + 1] << 16));   

        PRINT2N("                Write command (0x%08x) to DSI(%d) Command FIFO (DSI_DxCFIF)", regValue, dsiId);
        tavorpv2_dsiWriteCmd2Fifo(dsiId, regValue);
    }

    rc = TRUE;
    return rc;    

}

#else

static BOOL tavorpv2_dsiResetCmdFifoBuf(DispDrv *pDisplayDriver, DWORD dsiId)
{
    WORD i = 0;    
    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    if (dsiId >= TAVORPV2_DSI_CTL) 
    {
        WARN("tavorpv2_dsiResetCmdFifoBufWithIntProc: Only 2 DSI controllers are supported!");
        return FALSE;
    }    
    
    // Reset DSI CMD FIFO Buffer    
    pTavor2DisplayDriver->dsiCmdFifoBufIndex = 0;
    pTavor2DisplayDriver->dsiCmdFifoBufHead = 0;


    // Fill in with NOP, no data, no VC, no loopFlag
    for (i = 0; i < DSI_CMD_FIFO_BUF_LEN; i++)
        pTavor2DisplayDriver->dsiCmdFifoBuf[i] = ((LCD_CMD_NO_OPERATION & 0xFF) << 8);  

    return TRUE;    
}


/* Setup Command Sequence for DSI command FIFO: if it hits the bottom of FIFO, add a processor interrupt there and return*/
static BOOL tavorpv2_dsiAddCmd2Buffer(DispDrv *pDisplayDriver, DWORD dsiId, BYTE dsiCommand, BYTE dsiData, BYTE dsiVc, BYTE loopFlag)
{
    BOOL rc = FALSE;
    
    // Adjust head/back pointer if it's needed
    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    DWORD dsiCmdFifoBufHead = pTavor2DisplayDriver->dsiCmdFifoBufHead;
    DWORD dsiCmdFifoBufIndex = pTavor2DisplayDriver->dsiCmdFifoBufIndex;
    DWORD tmp,i = 0;

    if (dsiId >= TAVORPV2_DSI_CTL) 
    {
        WARN("tavorpv2_dsiAddCmd2Buffer: Only 2 DSI controllers are supported!");
        return rc;
    }    

    // Reset pointer if head hits bottom
    if (dsiCmdFifoBufHead == DSI_CMD_FIFO_BUF_LEN)
        dsiCmdFifoBufHead = 0;

    pTavor2DisplayDriver->dsiCmdFifoBufHead = dsiCmdFifoBufHead;

    // Buffer full situation, return writh error
    tmp = dsiCmdFifoBufHead + 1;
    if ( (tmp == DSI_CMD_FIFO_BUF_LEN)
          || (tmp == dsiCmdFifoBufIndex))
        
    {
        // Hit the bottom of the FIFO Buffer
        if (tmp == DSI_CMD_FIFO_BUF_LEN)
        {
            // insert "int proc"
            pTavor2DisplayDriver->dsiCmdFifoBuf[tmp - 1] = ( (0x42 & 0xFF) | ((LCD_CMD_INTERRUPT_THE_PROCESSOR & 0xFF) << 8));  
            // head pointer move one step
            pTavor2DisplayDriver->dsiCmdFifoBufHead++;
        }
        
        WARN("tavorpv2_dsiAddCmd2Buffer: DSI command FIFO buffer full. Please clear the buffer first!");
        // if buffer is full, start sending
         // Write command seq to FIFO
        tavorpv2_dsiSendCmdSeq(pDisplayDriver,dsiId);
        //Enable LCD controller to trig  "int proc" interrupt
        tavorpv2_dsiStart(dsiId);
        
        return rc;
    }    


    // If it's "int proc" command, it means the end of comand, insert "nop" and put "int proc" to the end
    if ( (dsiCommand == LCD_CMD_INTERRUPT_THE_PROCESSOR))
    {

            tmp = (dsiCmdFifoBufHead + 1)% DSI_CMD_FIFO_LEN;
            // Fill in "nop" between dsiCmdFifoBufHead and dsCmdFifoBufHead + 127 - i
            for (i = dsiCmdFifoBufHead; i < dsiCmdFifoBufHead + DSI_CMD_FIFO_LEN - tmp; i++)
            {
                pTavor2DisplayDriver->dsiCmdFifoBuf[i] = ( (0x00 & 0xFF) | ((LCD_CMD_NO_OPERATION & 0xFF) << 8)); 
                pTavor2DisplayDriver->dsiCmdFifoBufHead++;
            }    
            // Fill in "int proc" in dsiCmdFifoBufHead + 128 - i
            //PRINT2N("tavorpv2_dsiAddCmd2Buffer: Insert NoP & int proc head= (%08x) index= (%08x)", pTavor2DisplayDriver->dsiCmdFifoBufHead,
            //                                                   pTavor2DisplayDriver->dsiCmdFifoBufIndex);
            
            // Insert "int proc" 
            pTavor2DisplayDriver->dsiCmdFifoBuf[i] = ( (0x42 & 0xFF) | ((LCD_CMD_INTERRUPT_THE_PROCESSOR & 0xFF) << 8));  
            // head pointer move one step
            pTavor2DisplayDriver->dsiCmdFifoBufHead++;

            // Write command seq to FIFO
            tavorpv2_dsiSendCmdSeq(pDisplayDriver,dsiId);
            // Enable LCD controller to trig  "int proc" interrupt
            tavorpv2_dsiStart(dsiId);
    
            rc = TRUE;
            return (rc);

    }


    // Append an "int proc" command every 127 normal command
    if ( (dsiCmdFifoBufHead > 0) && (((dsiCmdFifoBufHead + 1)% DSI_CMD_FIFO_LEN) == 0))
    {
        PRINT2N("tavorpv2_dsiAddCmd2Buffer: Inser INT PROC head= (%08x) index= (%08x)", pTavor2DisplayDriver->dsiCmdFifoBufHead,
                                                           pTavor2DisplayDriver->dsiCmdFifoBufIndex);

        // Insert "int proc" 
        pTavor2DisplayDriver->dsiCmdFifoBuf[dsiCmdFifoBufHead] = ( (0x42 & 0xFF)    | ((LCD_CMD_INTERRUPT_THE_PROCESSOR & 0xFF) << 8));  
        // head pointer move one step
        pTavor2DisplayDriver->dsiCmdFifoBufHead++;
            
    }

    // Check buffer full state again
    dsiCmdFifoBufHead = pTavor2DisplayDriver->dsiCmdFifoBufHead;
    tmp = (dsiCmdFifoBufHead == DSI_CMD_FIFO_BUF_LEN) ? 0 : dsiCmdFifoBufHead + 1;
    if ( (dsiCmdFifoBufHead > DSI_CMD_FIFO_BUF_LEN)
          || (tmp == dsiCmdFifoBufIndex))
        
    {
        rc = FALSE;
        WARN("tavorpv2_dsiAddCmd2Buffer: DSI command FIFO buffer full!");
        return rc;
    }    

    
    //Reset head pointer if it reaches the end
    if (pTavor2DisplayDriver->dsiCmdFifoBufHead >= DSI_CMD_FIFO_BUF_LEN)
        pTavor2DisplayDriver->dsiCmdFifoBufHead = 0;

    // Update head pointer
    dsiCmdFifoBufHead = pTavor2DisplayDriver->dsiCmdFifoBufHead;

    // Normal Situation: fill the command in DSI command FIFO buffer
    //clear the buffer
    pTavor2DisplayDriver->dsiCmdFifoBuf[dsiCmdFifoBufHead] = 0;
    
    // Virutal Channle always comes with dsiData. dsiCommand has no VC concetp. Marvell datasheet is wrong here.
    dsiData = dsiData | ((dsiVc & 0x03) << 6);
    
    pTavor2DisplayDriver->dsiCmdFifoBuf[dsiCmdFifoBufHead] = ( (dsiData & 0xFF)
        | ((dsiCommand & 0xFF) << 8)  
        | ((loopFlag & 0x01)<< 13));

    //PRINTN("tavorpv2_dsiAddCmd2Buffe: Add command (0x%04x)!", pTavor2DisplayDriver->dsiCmdFifoBuf[dsiCmdFifoBufHead]);
    pTavor2DisplayDriver->dsiCmdFifoBufHead++;

    // Reset pointer if head hits bottom
    if (pTavor2DisplayDriver->dsiCmdFifoBufHead >= DSI_CMD_FIFO_BUF_LEN)
        pTavor2DisplayDriver->dsiCmdFifoBufHead = 0;

    rc = TRUE;

    return rc;
}    


/* Send Command Sequence from internal Command FIFO Buffer wiht Int proc interrupt*/
static BOOL tavorpv2_dsiSendCmdSeq(DispDrv *pDisplayDriver, DWORD dsiId)
{
    BOOL rc = FALSE;
    WORD i = 0;
    DWORD regValue = 0;

    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;
    WORD bufIndex = pTavor2DisplayDriver->dsiCmdFifoBufIndex;
    WORD bufHead = pTavor2DisplayDriver->dsiCmdFifoBufHead;
    

    if (dsiId >= TAVORPV2_DSI_CTL) 
    {
        WARN("tavorpv2_dsiSendCmdSeqWithIntProc: Only 2 DSI controllers are supported!");
        return FALSE;
    }    

    // Check Empty CMD FIFO Buffer
    if (bufHead == bufIndex)
    {
        // PRINT("tavorpv2_dsiSendCmdSeqWithIntProc: DSI CMD FIFO buffer empty!");
        return TRUE;
    }    

    // Write 128 entries to DSI command FIFO each time
    while ( (i < DSI_CMD_FIFO_LEN)  && (bufIndex < bufHead))
    {
        // 2 commands each write
        // if bufIndex == bufHead, insert one "NoP"
        if (bufIndex == bufHead -1)
            regValue = (((DWORD)pTavor2DisplayDriver->dsiCmdFifoBuf[bufIndex])  
                | ((LCD_CMD_NO_OPERATION & 0xFF) << 24));   
        else    
            regValue = (((DWORD)pTavor2DisplayDriver->dsiCmdFifoBuf[bufIndex])  
                | ((DWORD)pTavor2DisplayDriver->dsiCmdFifoBuf[bufIndex + 1] << 16));   
        // step over 2 commands
        bufIndex = bufIndex + 2;

        //PRINT2N("                Write command (0x%08x) to DSI(%d) Command FIFO (DSI_DxCFIF)", regValue, dsiId);
        tavorpv2_dsiWriteCmd2Fifo(dsiId, regValue);
        i  = i + 2;
    } 

    // Update index
    if (bufIndex > bufHead)
        bufIndex = bufHead;

    // Reset pointer if head hits bottom
    if (bufIndex >= DSI_CMD_FIFO_BUF_LEN)
        bufIndex = 0;

    if (bufHead >= DSI_CMD_FIFO_BUF_LEN)
       {
        bufHead = 0;
         pTavor2DisplayDriver->dsiCmdFifoBufHead = bufHead;    
       }
    
    pTavor2DisplayDriver->dsiCmdFifoBufIndex = bufIndex;     

    rc = TRUE;
    return rc;    

}

#endif

// Trigger a "int proc" in order to fix artifacts on LCD panel that caused by software based DSI cmd FIFO operation 
static void tavorpv2_dsiTrigIntProc(DispDrv *pDisplayDriver, DWORD dsiId)
{
    WORD i = 0;
    DWORD regVal = 0;
    // First time is trigger by software - only try to trigger "int proc" interrupt
    PRINT("Fill in DSI cmd FIFO to trigger int proc!");
    regVal = 0x0A210A21;
    tavorpv2_dsiWriteCmd2Fifo(dsiId, regVal);
    regVal = 0x0A000A00;
    for (i = 0; i < DSI_CMD_FIFO_LEN - 2; i=i+2) {
        tavorpv2_dsiWriteCmd2Fifo(dsiId, regVal);
    }
    regVal = 0x0B000A42;
    tavorpv2_dsiWriteCmd2Fifo(dsiId, regVal);
    return;
}



/* Send Command Sequence from internal Command FIFO Buffer with a system DMA channel */
static BOOL tavorpv2_dsiSendCmdSeqWithDma(DispDrv *pDisplayDriver, DWORD dsiId)
{
    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    // sanity check
    if( dsiId >= TAVORPV2_DSI_CTL ) {
        WARN( "Only 2 DSI controllers are supported!" );
        return (FALSE);
    }

    // setup MVED DMA descriptor
    pTavor2DisplayDriver->pDsiDmaDesp[dsiId]->source = (DWORD) pTavor2DisplayDriver->dsiCmdFifo[dsiId]; 
    pTavor2DisplayDriver->pDsiDmaDesp[dsiId]->target = DSI_DxCFIF( dsiId ); 
    pTavor2DisplayDriver->pDsiDmaDesp[dsiId]->command = DMA_DCMD_MASK_INCSRCADDR|DMA_DCMD_MASK_FLOWTRG|DMA_DCMD_MASK_ENDIRQEN|DMA_DCMD_BURST_32_BYTES|DMA_DCMD_WIDTH_4_BYTE;
    pTavor2DisplayDriver->pDsiDmaDesp[dsiId]->descriptor = DMA_DDADR_MASK_STOP; 

    // configure length to DMA transfer
    if ((pTavor2DisplayDriver->dsiCmdIndex[dsiId]) & 0x01) {
        pTavor2DisplayDriver->pDsiDmaDesp[dsiId]->command += ( ((pTavor2DisplayDriver->dsiCmdIndex[dsiId]+1)>>1) & DMA_DCMD_MASK_LEN );
		//pTavor2DisplayDriver->pDsiDmaDesp[dsiId]->command += ( ((pTavor2DisplayDriver->dsiCmdFifoBufHead+1)>>1) & DMA_DCMD_MASK_LEN );
			

    } else {
        pTavor2DisplayDriver->pDsiDmaDesp[dsiId]->command += ( (pTavor2DisplayDriver->dsiCmdIndex[dsiId]>>1) & DMA_DCMD_MASK_LEN );
        //pTavor2DisplayDriver->pDsiDmaDesp[dsiId]->command += ( (pTavor2DisplayDriver->dsiCmdFifoBufHead>>1) & DMA_DCMD_MASK_LEN );

    }

    // write dirty data from cache to main memory and then set the data in cache invalid
    CacheExecute( DATA_CACHE, CACHE_CLEAN_AND_INVALIDATE, (DWORD)pTavor2DisplayDriver->pDsiDmaDesp[dsiId]->source, 32 );
    CacheExecute( DATA_CACHE, CACHE_CLEAN_AND_INVALIDATE, (DWORD)pTavor2DisplayDriver->pDsiDmaDesp[dsiId]->target, 32 );
    CacheExecute( DATA_CACHE, CACHE_CLEAN_AND_INVALIDATE, (DWORD)pTavor2DisplayDriver->pDsiDmaDesp[dsiId], (pTavor2DisplayDriver->pDsiDmaDesp[dsiId]->command & DMA_DCMD_MASK_LEN)+32 );

    // write the descriptor
    WriteRegDword( DMA_DESCRIPTOR_REG32(pTavor2DisplayDriver->dsiDmaChannel[dsiId]), (DWORD)pTavor2DisplayDriver->pDsiDmaDesp[dsiId] );

    // enable the DMA channel
    DMA_ENABLE_CHANNEL( pTavor2DisplayDriver->dsiDmaChannel[dsiId] );

    // Lock DMA semaphore
    RE_RimLockCountingSemaphore(&(pTavor2DisplayDriver->dsiDmaLock[dsiId]));

    PRINTN( "DMA channel %d transfer done", pTavor2DisplayDriver->dsiDmaChannel[dsiId] );

    return (TRUE); 
}


// Send Pixels to LCD through DSI Command FIFO: WorkAround from Marvell for writing several pixels per DCS long write command within one DSI command FIFO (128 command entries)
static BOOL tavorpv2_dispCopyToLcdFromDsiWka(DispDrv *pDisplayDriver,DWORD src, DWORD high, DWORD wide, BYTE displayId)
{
    DWORD i,j;

    // Pixels sent to LCD panel
    BYTE pixelLsb = 0;
    BYTE pixelMsb = 0;
    BYTE wkaPixels;    
    DWORD width;
    DWORD dsiId;
    DWORD frameSize;
    DWORD regVal;
    DWORD frmPtr;
    DWORD numOfDcsWrites;
    WORD regVal01, regVal02;
    WORD bpp;
    WORD *srcPtr;
    lcdMode *pMode;

    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    // Retrieve DSI controller ID by using displayId
    dsiId = tavorpv2_dispGetDsiId(pDisplayDriver, displayId);
    if (dsiId >= TAVORPV2_DSI_CTL) 
    {
        WARN("Copy2LcdFromDsi: Only 2 DSI controllers are supported!");
        return FALSE;
    }    

    // Bytes per pixel
    pMode = pDisplayDriver->config.dsiConfig[dsiId].pMode;
    bpp = (pMode->colorDepth) >> 3;
    // Only support RGB565 currently
    if ((bpp != 2) && (bpp != 3))
    {
        WARN("Copy2LcdFromDsi: Only RGB565/RGB888 is supported!");
        return FALSE;
    }

    // Pixels per trig = 60 (RGB16) or 40 (RGB24)
    wkaPixels = (DSI_CMD_FIFO_LEN - 8)/bpp;

    // Conert to RGB16 for convient calculation
    wkaPixels = (wkaPixels * bpp) / 2;

    if (wkaPixels > DSI_CMD_FIFO_LEN)
    {
        WARN("Copy2LcdFromDsi: Workrouand pixels too big!");
        return FALSE;
    }
     
    // number of DCS writes = (DSI command FIFO - ( Nop + int proc)) / ( every DCS write total length: pixels + 2 nop + 1 dcs long write + 2 length + 1 memory write)
    numOfDcsWrites = (DSI_CMD_FIFO_LEN - 2) / ( wkaPixels * 2 + 6);
    if ( numOfDcsWrites < 1)
    {
        WARN("Copy2LcdFromDsi: One dcs long write exceeds DSI cmd FIFO length!");
        return FALSE;
    }
    
    frmPtr = pTavor2DisplayDriver->pDispFrmPtr;
    
 
    // total pixels (counted as 2 bytes per unit)
    frameSize = (high * wide * bpp)/2;

    // Determine if all pixels in frame buffer are sent 
    if (frmPtr >= frameSize)
    {
        //Finished
        PRINT("All pixels have been sent!");
        return TRUE;
    }

    PRINT3N("Copy2LcdFromDsi: (0x%08x)pixels, frmPtr(0x%08x), DCS long writes(0x%04x)",frameSize, frmPtr, numOfDcsWrites);
    // Remaining UIs
    frameSize = (frameSize - frmPtr);

    // Pointer for current pixels to be sent
    srcPtr = (WORD *) src;

    // Start "DCS long write" to send pixels 
    for (i = 0 ; i < numOfDcsWrites; i++) 
    {
        //Adjust starting address of pixels to be sent
        srcPtr = srcPtr + frmPtr;
    
        //Step 1: Alwasy insert a "NoP" at the beginning for convient calculating of pixels/trig 
        regVal01 = ( (0x01 & 0xFF) | ((LCD_CMD_NO_OPERATION & 0xFF) << 8));
        regVal02 = ( (0x01 & 0xFF) | ((LCD_CMD_NO_OPERATION & 0xFF) << 8));
            
        regVal = (regVal01) | (regVal02 << 16); 
        tavorpv2_dsiWriteCmd2Fifo(dsiId, regVal);

        //Step 2: DCS long write command + Length of Pixels (LSB)
        pixelLsb = ( (frameSize < wkaPixels) ? frameSize : wkaPixels);
        pixelMsb = 0x00; 
        regVal01 = ( (DSI_DATA_DCS_LONG_WRITE & 0xFF) | ((LCD_CMD_COMMAND_WRITE & 0xFF) << 8));
        regVal02 = ( ((pixelLsb*2 + 1) & 0xFF) | ((LCD_CMD_COMMAND_WRITE & 0xFF) << 8));
        regVal = (regVal01) | (regVal02 << 16); 
        tavorpv2_dsiWriteCmd2Fifo(dsiId, regVal);

        //Step3:  Length of Pixels (MSB) + DCS write memory start/continue
        regVal01 = ( (pixelMsb & 0xFF) | ((LCD_CMD_COMMAND_WRITE & 0xFF) << 8));
        if ( frmPtr == 0)
            regVal02 = ( (DCS_CMD_WRITE_MEMORY_START & 0xFF) | ((LCD_CMD_COMMAND_WRITE & 0xFF) << 8));
        else
            regVal02 = ( (DCS_CMD_WRITE_MEMORY_CONTINUE& 0xFF) | ((LCD_CMD_COMMAND_WRITE & 0xFF) << 8));
        regVal = (regVal01) | (regVal02 << 16);   
        tavorpv2_dsiWriteCmd2Fifo(dsiId, regVal);
     
        // Step 4: Send pixels 
        // Fill in actual pixels to be sent
        width = pixelLsb;
        if (bpp == 2)
        {
            for (j = 0; j < width; j++)
            {
            pixelLsb = (BYTE) (0xff & (*(srcPtr + j)));
            pixelMsb = (BYTE) ((0xff00 & (*(srcPtr + j ))) >> 8); 
            regVal01 = ( (pixelMsb & 0xFF) | ((LCD_CMD_COMMAND_WRITE & 0xFF) << 8));
            regVal02 = ( (pixelLsb & 0xFF) | ((LCD_CMD_COMMAND_WRITE & 0xFF) << 8));
            regVal = (regVal01) | (regVal02 << 16);    
            tavorpv2_dsiWriteCmd2Fifo(dsiId, regVal);
            }
        }
        else if ( bpp == 3)
        {
            for (j = 0; j < width; j = j+3)
            {
            // Big-Endian(RGB) : R0G0
            pixelMsb = (BYTE) (0xff & (*(srcPtr + j + 1)));
            pixelLsb = (BYTE) ((0xff00 & (*(srcPtr + j ))) >> 8); 
            regVal01 = ( (pixelMsb & 0xFF) | ((LCD_CMD_COMMAND_WRITE & 0xFF) << 8));
            regVal02 = ( (pixelLsb & 0xFF) | ((LCD_CMD_COMMAND_WRITE & 0xFF) << 8));
            regVal = (regVal01) | (regVal02 << 16);    
            tavorpv2_dsiWriteCmd2Fifo(dsiId, regVal);
            // B0R1
            pixelMsb = (BYTE) (0xff & (*(srcPtr + j)));
            pixelLsb = (BYTE) ((0xff00 & (*(srcPtr + j +2))) >> 8); 
            regVal01 = ( (pixelMsb & 0xFF) | ((LCD_CMD_COMMAND_WRITE & 0xFF) << 8));
            regVal02 = ( (pixelLsb & 0xFF) | ((LCD_CMD_COMMAND_WRITE & 0xFF) << 8));
            regVal = (regVal01) | (regVal02 << 16);    
            tavorpv2_dsiWriteCmd2Fifo(dsiId, regVal);
            // G1B1
            pixelMsb = (BYTE) (0xff & (*(srcPtr + j + 2)));
            pixelLsb = (BYTE) ((0xff00 & (*(srcPtr + j +1))) >> 8); 
            regVal01 = ( (pixelMsb & 0xFF) | ((LCD_CMD_COMMAND_WRITE & 0xFF) << 8));
            regVal02 = ( (pixelLsb & 0xFF) | ((LCD_CMD_COMMAND_WRITE & 0xFF) << 8));
            regVal = (regVal01) | (regVal02 << 16);    
            tavorpv2_dsiWriteCmd2Fifo(dsiId, regVal);
            }
        }    
        
        // Update frmPtr
        frmPtr = frmPtr + width;

        // Remaining pixels in frame buffer
        frameSize = frameSize - width;
        
        // Check if it's the last DCS long write
        if (width != wkaPixels)
        {
            i++;
            break;
        }    
  
    } 

    // Step 6: Fill in "NoP" and "int proc" for the remaining DSI cmd FIFO
    // Fill in "Nop" for the remaining places and the last place "int proc" for DSI command FIFO
    i = DSI_CMD_FIFO_LEN -2 - (i*6 + 2*(frmPtr -pTavor2DisplayDriver->pDispFrmPtr));
    j = 0;
    while (j < i )
    {
        j = j + 2;
        regVal01 = ( (0x00 & 0xFF) | ((LCD_CMD_NO_OPERATION & 0xFF) << 8));
        regVal02 = ( (0x00 & 0xFF) | ((LCD_CMD_NO_OPERATION & 0xFF) << 8));
        regVal = (regVal01) | (regVal02 << 16);    
        tavorpv2_dsiWriteCmd2Fifo(dsiId, regVal);
    }    

    // Add "int proc" at the end of DSI cmd FIFO to trig interrupt 
    regVal01 = ( (0x01 & 0xFF) | ((LCD_CMD_NO_OPERATION & 0xFF) << 8));
    regVal02 = ( (0x00 & 0xFF) | ((LCD_CMD_INTERRUPT_THE_PROCESSOR & 0xFF) << 8));
    regVal = (regVal01) | (regVal02 << 16);    
    tavorpv2_dsiWriteCmd2Fifo(dsiId, regVal);

    // Update pointer of Frame Buffer
    PRINTN("Update pDispFrmPtr=0x%08x", frmPtr);
    pTavor2DisplayDriver->pDispFrmPtr = frmPtr;
    
    return TRUE;
}


// Send Pixels to LCD through DSI Command FIFO - DCS long write is used to tranmsit pixels for DSI DCS LCD panel (Maximum bytes for DCS long write to be sent is 0xffff)
// displayWidth : stride - pixels per line
static BOOL tavorpv2_dispCopyToLcdFromDsi(DispDrv *pDisplayDriver,DWORD src, DWORD high, DWORD wide, DWORD displayWidth, BYTE displayId)
{
    DWORD i,j;

    // Pixels sent to LCD panel
    BYTE pixelLsb = 0;
    BYTE pixelMsb = 0;
    DWORD width;
    WORD *srcPtr;
    DWORD dsiId = 0xff;
    DWORD frameSize, numOfDcsWrites;

    // Retrieve DSI controller ID by using displayId
    dsiId = tavorpv2_dispGetDsiId(pDisplayDriver, displayId);
    if (dsiId >= TAVORPV2_DSI_CTL) 
    {
        WARN("Copy2LcdFromDsi: Only 2 DSI controllers are supported!");
        return FALSE;
    }    

    // Only support RGB565 currently
    if ((pDisplayDriver->config.dsiConfig[dsiId].pMode)->colorDepth != 16)
    {
        WARN("Copy2LcdFromDsi: Only RGB565 is supported!");
        return FALSE;
    }

    // total pixels (suppose RGB565)
    frameSize = high * wide * 2;
    // Maximum supported frameSize (because of DSI underrun issue causes interrupt lost. PV2 B0 is supposed to fix this issue)
    PRINT3N("tavorpv2_dispCopyToLcdFromDsi: high (0x%08x) wide (0x%08x), 0total pixels (0x%08X) write to LCD.", high, wide, frameSize);

    if (!frameSize)
    {
        return FALSE;
    }
    // Number of DCS long writes to transfer all pixels
    numOfDcsWrites = 1 + (frameSize / (0xFFFF -1));
    
    // Reset Comand FIFO buffer
    tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     

    // Compose DCS command sequence to send Pixels (WorkAround from Marvell: only send 42 pixels one time)
    srcPtr = (WORD *) src;
    
    i = 0;
    while ( i < numOfDcsWrites)
    {
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0, 0);
    
        // Workaround from Marvell : only sending 42 pixels one time 
        if (frameSize > 0xFFFF -1)
        {
            pixelLsb = 0xFF;
            pixelMsb = 0xFF;
        }
        else
        {
            pixelLsb = (frameSize + 1) & 0xFF;
            pixelMsb = ((frameSize + 1) & 0xFF00) >> 8; 
        }
        // Set length based on bytes    
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, pixelLsb, 0, 0); // Length LS byte
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, pixelMsb, 0, 0);
        if ( i == 0)
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_WRITE_MEMORY_START, 0, 0);
        else
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_WRITE_MEMORY_CONTINUE, 0, 0);
    
        width = (pixelLsb + (pixelMsb << 0x08) - 1) >> 1;
        for (j = 0; j < width; j++)
        {
            pixelLsb = (BYTE) (0xff & (*(srcPtr + j)));
            pixelMsb = (BYTE) ((0xff00 & (*(srcPtr + j ))) >> 8); 
    
            // Pixel from Frame Buffer (always initialized to be color bar with RGB565 format). Becareful here to limit total commands less than 128.
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, pixelMsb, 0, 0); // Start column address (MSB)
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, pixelLsb, 0, 0); // Start column address (MSB)
        }
        
        // Determine finished or not
        srcPtr = srcPtr + width;
        i++;
    }

    // The final command "int proc" triggers filling DSI command FIFO
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0, 0, 0); // Start column address (MSB)
    
    return TRUE;
}


// Send Pixels to LCD by going through display controlelr pixpel pipeline
static BOOL tavorpv2_dispCopyToLcd(DispDrv *pDisplayDriver,DWORD src, DWORD high, DWORD wide, DWORD repeats, BYTE displayId)
{
    BOOL rc = FALSE;

    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    // Determine if high is a multiple of "4"
    if ( high == 0) 
        return rc;
    else if (high < 4) 
        high = 4;

    if(tavorpv2_dispSendFrame(pDisplayDriver, src, high, wide, displayId))
    {
        // reset "stop" flag    
	    lcdStopFlag = FALSE;
		
        // Update "sending frame" info
        pDisplayDriver->lcdData.repeatTimes = repeats;
        pDisplayDriver->lcdData.height= high;
        pDisplayDriver->lcdData.width= wide;
        pDisplayDriver->lcdData.lcdPanelId= displayId;
        pDisplayDriver->lcdData.dataAddr = src;        // Continously sending the same frame

        if (repeats == 0xffffffff)
        {
            // Clear Smart Panel Mode
            tavorpv2_dispCtlSetSpMode(pTavor2DisplayDriver->dispBasePlaneDmaCh,FALSE);
            pDisplayDriver->lcdData.repeatTimes = 0;
            tavorpv2_dsiSendFrame(pDisplayDriver, high, wide, displayId, TRUE);
        }    
        // Sending the frame with determined times
        else
        {
            // Set Display Controller to use Smart Panel Mode
            tavorpv2_dispCtlSetSpMode(pTavor2DisplayDriver->dispBasePlaneDmaCh,TRUE);
            tavorpv2_dsiSendFrame(pDisplayDriver, high, wide, displayId, FALSE);
        }	
			
    } 

    return rc;
}

// Send Pixels to LCD by display controller's pixel baseplane pipeline
static BOOL tavorpv2_dispSendFrame(DispDrv *pDisplayDriver, DWORD src, DWORD high, DWORD wide, BYTE displayId)
{
    BOOL rc = FALSE;
    DWORD dsiId;
    BYTE bpp;
    DispPixelFormat pixelFormat;
    lcdMode *pMode;

    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    PRINT2N("tavorpv2_dispSendFrame: high (0x%08x) wide (0x%08x): ", high, wide);

    // Retrieve DSI controller ID by using displayId
    dsiId = tavorpv2_dispGetDsiId(pDisplayDriver, displayId);
    if (dsiId >= TAVORPV2_DSI_CTL) 
    {
        WARN("Copy2LcdFromDsi: Only 2 DSI controllers are supported!");
        return rc;
    }    

    pMode = pDisplayDriver->config.dsiConfig[dsiId].pMode;
    
    // Bytes per pixel
    bpp = (pMode->colorDepth) >> 3;
    // Only support RGB565 currently
    if ((bpp != 2) && (bpp != 3))
    {
        WARN("CopyToLcd: Only RGB565/RGB888 are supported now!");
        return rc;
    }
    if (bpp ==2)
        pixelFormat = DISP_PIXELFRT_RGB16;
    else
        pixelFormat = DISP_PIXELFRT_RGB24;

    // Disable LCD controller only once
    if (lcdEnableFlag == FALSE)
        tavorpv2_dispCtlEnable(DISP_CTL_QDISABLE);
	
    // Flush Cache before setting DMA engine
    //CacheExecute(DATA_CACHE, CACHE_CLEAN_AND_INVALIDATE, src, high*wide*bpp);
    CacheExecute(DATA_CACHE, CACHE_CLEAN_AND_INVALIDATE, src, 864*480*3);
    
    // Create DMA descriptor for fetch unit
    pTavor2DisplayDriver->pDispDmaDescriptor[0] = (DWORD)(pTavor2DisplayDriver->pDispDmaDescriptor);
    pTavor2DisplayDriver->pDispDmaDescriptor[1] = src;

    //DMA channel for Base Plane
    pTavor2DisplayDriver->pDispDmaDescriptor[2] = pTavor2DisplayDriver->dispBasePlaneDmaCh; 
    pTavor2DisplayDriver->pDispDmaDescriptor[3] = high*wide*bpp;

    PRINT2N("tavorpv2_dispSendFrame: pDispFrameBuffer=(0x%08x) dispFrameBuffer=(0x%08x)", pTavor2DisplayDriver->pDispFrameBuffer,&(dispFrameBuffer[0]));

    //PRINT4N("tavorpv2_dispSendFrame: descp[0]=(0x%08x) descp[1]=(0x%08x) descp[2]=(0x%08x) descp[3]=(0x%08x)",
    //        pTavor2DisplayDriver->pDispDmaDescriptor[0],pTavor2DisplayDriver->pDispDmaDescriptor[1],pTavor2DisplayDriver->pDispDmaDescriptor[2],
    //        pTavor2DisplayDriver->pDispDmaDescriptor[3]);

    //Invalidate and clean cache before sending
    CacheExecute( DATA_CACHE, CACHE_CLEAN_AND_INVALIDATE, (DWORD)pTavor2DisplayDriver->pDispDmaDescriptor, 16 );

    tavorpv2_dispCtlSetFetchUnit((DispDmaChannel)pTavor2DisplayDriver->pDispDmaDescriptor[2], 
                                 (DWORD)(pTavor2DisplayDriver->pDispDmaDescriptor), 
                                 pixelFormat);

    //Set Window Unit configuration for base plane ,
    tavorpv2_dispCtlSetWindowUnit(pTavor2DisplayDriver->dispBasePlane, wide, high);
    
    //Set mixer ID according to DSI controller ID
    tavorpv2_dispCtlSetMixerUnit(dsiId, pMode->width, pMode->height, 0, 0, pTavor2DisplayDriver->dispBasePlane);

    // Should put in setMode
    tavorpv2_dispCtlSetDefaultGamma();        
    
    // Enable LCD controller
    if (lcdEnableFlag == FALSE)
    { 
	    PRINT("Send frame: Enable LCD controller here!");
        tavorpv2_dispCtlEnable(DISP_CTL_ENABLE);
	    // Set Enable flag
	    lcdEnableFlag = TRUE; 
	}

    // Start fetching a frame of image here
    tavorpv2_dispCtlStartFetch(pTavor2DisplayDriver->pDispDmaDescriptor[2], TRUE);
    rc = TRUE;
    
    return rc;
}

// Send one frame of pixels to LCD by pixel pipeline (Workaround without using "DCS LONG" command) 
static BOOL tavorpv2_dsiSendFrame(DispDrv *pDisplayDriver, DWORD high, DWORD wide, BYTE displayId, BOOL loopFlag)
{
    BOOL rc = FALSE;
    DWORD dsiId,numOfLineWrites,totalLen = 0;
    BYTE pixelMsb, pixelLsb, bpp;
    lcdMode *pMode;

    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    // Retrieve DSI controller ID by using displayId
    dsiId = tavorpv2_dispGetDsiId(pDisplayDriver, displayId);
    if (dsiId >= TAVORPV2_DSI_CTL) 
    {
        WARN("Copy2LcdFromDsi: Only 2 DSI controllers are supported!");
        goto cleanUp;
    }    

    pMode = pDisplayDriver->config.dsiConfig[dsiId].pMode;

    // Bytes per pixel
    bpp = (pMode->colorDepth) >> 3;
    // Only support RGB565/RGB888 right now
    if ((bpp != 2) && (bpp != 3))
    {
        WARN("CopyToLcd: Only RGB565/RGB888 are supported now!");
        goto cleanUp;
    }

    // Force Fetch unit to fetch one new frame
	tavorpv2_dispCtlStartFetch(pTavor2DisplayDriver->dispBasePlaneDmaCh, TRUE);

    // bytes per line on the screen
    totalLen = wide * bpp;
    // Less than 4 lines is a special case: write only one line of data
    if ( (high > 0) && (high <= 4))
       numOfLineWrites = 1;        
    else
        numOfLineWrites = high;
    if (numOfLineWrites == 0)
        goto cleanUp;

    // Enable DSI controller if not enabled before sending BTA/Reset
    tavorpv2_dsiCtlEnable(dsiId, DSI_CTL_ENABLE);

    // Reset DSI cmd FIFO
    tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     

    // Packet Length
    totalLen = totalLen + 1;

    // Start sending DSI/DCS command to setup Samsung DriverIC
    // PRINTN("tavorpv2_dsiSendFrame: numOfLineWrites = 0x%08X", numOfLineWrites);

    if (loopFlag)
        // Set "Start Label" for loop    
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_START_LABEL, 0x00, 0x00, 0x00);

#if !defined(TAVORPV2_HWVER_MG1)
    // Hardware workaround for "DCS long write" on PV2 Z1: DCS_SHORT_WRITE + WRITE_MEMORY_START for eDISCO board      
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_SHORT_WRITE_NO_PARAMETER, 0x00, 0x00);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DCS_CMD_WRITE_MEMORY_START, 0x00, 0x00);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x00, 0x00, 0x00);
    // Send twice if using 2 active lanes
    if (pDisplayDriver->config.dsiConfig[dsiId].activeLanes == 2)
    {
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_SHORT_WRITE_NO_PARAMETER, 0x00, 0x00);
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DCS_CMD_WRITE_MEMORY_START, 0x00, 0x00);
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x00, 0x00, 0x00);
    }
    
    // RGB packets with packed/loose pixel stream
    if (bpp == 2)
      tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_RGB_565_PACKET, 0, 0x00);
    else if (bpp == 3)
       tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_RGB_888_PACKET, 0, 0x00);
    pixelMsb = (BYTE) ((wide & 0xff00) >> 8);
    pixelLsb = (BYTE) (wide & 0xff);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, pixelLsb, 0, 0x00);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, pixelMsb, 0, 0x00);

    // Send one line from frame buffer 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_LINE_DATA_WRITE, 0, 0, 0x00); 

    numOfLineWrites = numOfLineWrites - 1;

    // Loop Buffer Settings
    if (numOfLineWrites > 0) 
    {
        // Flush/Invalidate Loop Buffer
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_FLUSH_LOOP_BUFFER, 0x00, 0x00, 0x00);

        // Start putting in LOOP
        // DCS_SHORT_WRITE + WRITE_MEMORY_CONTINUE       
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_SHORT_WRITE_NO_PARAMETER, 0x00, 0x01);
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DCS_CMD_WRITE_MEMORY_CONTINUE, 0x00, 0x01);
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x00, 0x00, 0x01);

        // RGB packets with packed/loose pixel stream
        if (bpp == 2)
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_RGB_565_PACKET, 0, 0x01);
        else if (bpp == 3)
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_RGB_888_PACKET, 0, 0x01);
        pixelMsb = (BYTE) ((wide & 0xff00) >> 8);
        pixelLsb = (BYTE) (wide & 0xff);
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, pixelLsb, 0, 0x01);
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, pixelMsb, 0, 0x01);
    }
#else
    // Config DSI output pixel format
    if (bpp == 3)
	    tavorpv2_dsiCtlSetVcFomat(dsiId, 0, DISP_PIXELFRT_RGB24);
	else
	    tavorpv2_dsiCtlSetVcFomat(dsiId, 0, DISP_PIXELFRT_RGB16);

	tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x40, 0x00, 0x00);
	tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x40, 0x00, 0x00);

    //Using DCS command to send all pixels: DCS LONG WRITE + DCS_WRITE_MEMORY_START
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0x00, 0x00);
	// One line of Pixels length by bytes: MSB/LSB
    pixelMsb = (BYTE) ((totalLen & 0xff00) >> 8);
    pixelLsb = (BYTE) (totalLen & 0xff);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, pixelLsb, 0x00, 0x00);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, pixelMsb, 0x00, 0x00);
    // Send "Write_Memory_Start"
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DCS_CMD_WRITE_MEMORY_START, 0x00, 0x00);
    // Send one line from frame buffer 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_LINE_DATA_WRITE, 0, 0, 0x00); 

    numOfLineWrites = numOfLineWrites - 1;
    // Loop Buffer Settings
    if (numOfLineWrites > 0) 
    {
        // DCS LONG WRITE + DCS_WRITE_MEMORY_CONTINUE
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_FLUSH_LOOP_BUFFER, 0x00, 0x00, 0x00);

        // Delay between DCS long write (Optional here)
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_SET_DLY_MULT, 0x02, 0x00, 0x00);
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x40, 0x00, 0x01);

        // Start putting "DCS long write + Write_Memory_Continue" into the LOOP
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0x00, 0x01);
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, pixelLsb, 0x00, 0x01);
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, pixelMsb, 0x00, 0x01);
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DCS_CMD_WRITE_MEMORY_CONTINUE, 0x00, 0x01);
    }
#endif

    // Execute loop buffer to fetch "n" line of pixels
    if (numOfLineWrites > 1) 
    {
        // Send one line from frame buffer by loop buffer
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_LINE_DATA_WRITE, 0, 0, 0x01); // LSB

        // "line data write" already execut twice
        numOfLineWrites = numOfLineWrites - 1;
        while (numOfLineWrites >= 0x100)
        {
            // execute loop buffer instruction (can't be nested with loop flag) for 10 times
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_EXECUTE_LOOP_BUFFER, 0xff, 0x00, 0x00);
            numOfLineWrites -= 0x100;
        }

        // The last few lines
        if (numOfLineWrites >= 1) ;
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_EXECUTE_LOOP_BUFFER, numOfLineWrites - 1, 0x00, 0x00);

        // Flush/Invalidate Loop Buffer
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_FLUSH_LOOP_BUFFER, 0x00, 0x00, 0x00);
    }

    // One Frame finished:  Send "end of transmission"
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_END_OF_TRANSMISSION_PACKET, 0x00, 0x00);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x0f, 0x00, 0x00);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x0f, 0x00, 0x00);

    // Set "Goto Lable" if loop mode is used to generate continous traffic
    if (loopFlag)
        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_GOTO_START, 0x00, 0x00, 0x00);

    // Add Nop 
	tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x40, 0x00, 0x00);

    // End of command sequence: always trying to trigger a DxINST0[SP_GEN_INT_STS] interrupt
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x10, 0x00, 0x00); 

    rc = TRUE;
    
cleanUp:
    return rc;
}


// DSI/Display Test code for DSI interface traffic generation
static BOOL tavorpv2_drvLcdTest(DispDrv *pDisplayDriver, DWORD testMode)
{
    static DWORD dsiId = 0x00;

    lcdDsiConfig *dsiConfig = NULL;

    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    DWORD i = 0;
    // Pixels sent to LCD panel
    WORD width = 0;
    WORD height=0;
    DWORD fbSrcAddr;
    BOOL rc = FALSE;
    lcdMode *pMode = NULL;

    // Supported Test Cases (0x01 - 0x30) /Configurations(0xD0 - 0xFF)
    switch (testMode) {
        // Traffice generated for DSI maximum throughput test with DSI Data Write command : patthern 0xAA-0x33-0xF0-0x7F-0x55-0xCC-0x0F
        case 0x01:
            PRINT("Continous DSI traffic test with Data Write (LP)");
            // Reset Comand FIFO buffer
            tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     
            // Flush Loop Buffer
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_FLUSH_LOOP_BUFFER, 0x00, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x00, 0x00, 0x00);

            //The following DSI command will be put in loop buffer ( loop buufer is 32 bytes long - upto 16 loop commands) for traffic generation

            // Keep "Start Lable" in command_0 filed
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_START_LABEL, 0x00, 0x00, 0x00);

            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x00, 0x00, 0x00);

            // Keep "Data Write" related command start in command_0 filed
            // RGB24
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_RGB_888_PACKET, 0x00, 0x01);
            // Length 0x0003
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x06, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0x00, 0x01);
            // Data Write start
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0xA9, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0xA9, 0xA9, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0xAA, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0xAA, 0xAA, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0x33, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0x33, 0x33, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0xF0, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0xF0, 0xF0, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0x7F, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0x7F, 0x7F, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0x55, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0x55, 0x55, 0x00, 0x00);
            // execute loop buffer instruction for 0xff times
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_EXECUTE_LOOP_BUFFER, 0xff, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x00, 0x00, 0x00);
            // Keep "Goto Lable" in command_1 filed
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_GOTO_START, 0x00, 0x00, 0x00);

            // End of command sequence: always try to trigger a DxINST0[SP_GEN_INT_STS] interrupt
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
            break;
        // Data write withoug Loop Buffer version
        case 0x02:
            PRINT("Continous DSI traffic test with Data Write");
            // Reset Comand FIFO buffer
            tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);       
            //Create Comand Sequence to generate traffic 
            // Flush Loop Buffer
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_FLUSH_LOOP_BUFFER, 0x00, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x00, 0x00, 0x00);
            
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_START_LABEL, 0x00, 0x00, 0x00);

            // Keep "Data Write" in command_0 filed
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x00, 0x00, 0x00);

            // Keep "Data Write" related command start in command_0 filed
            // RGB24
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_RGB_888_PACKET, 0x00, 0x00);
            // Length 0x0003
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x06, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0x00, 0x00);
            // Data Write start
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0xA9, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0xA9, 0xA9, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0xAA, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0xAA, 0xAA, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0x33, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0x33, 0x33, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0xF0, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0xF0, 0xF0, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0x7F, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0x7F, 0x7F, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0x55, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0x55, 0x55, 0x00, 0x00);
            
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x00, 0x00, 0x00);

            // NOP
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x0, 0x00, 0x00);
             // Keep "Goto Lable" in command_1 filed
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_GOTO_START, 0x00, 0x00, 0x00);

            // End of command sequence: always try to trigger a DxINST0[SP_GEN_INT_STS] interrupt (Never hit)
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
             break;
        // DSI short packet with DSI Data Generic Short Write Command
        case 0x03:
            PRINT("Continous DSI traffic test with Data Generic Short Write (LP)");
            tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     
            // Flush Loop Buffer
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_FLUSH_LOOP_BUFFER, 0x00, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_NO_OPERATION, 0x01, 0x00, 0x01);

            //The following DSI command will be put in loop buffer ( loop buufer is 32 bytes long - upto 16 loop commands) for traffic generation
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_START_LABEL, 0x00, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xAA, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xAA, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x33, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x33, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xF0, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xF0, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x7F, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x7F, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x55, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x55, 0x00, 0x01);
            // Nop
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_NO_OPERATION, 0x01, 0x00, 0x01);
            
            // end of loop buffer command
            // execute loop buffer instruction (can't be nested with loop flag) for 480 times
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_EXECUTE_LOOP_BUFFER, 0xFF, 0x00, 0x00);

            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_NO_OPERATION, 0x01, 0x00, 0x00);

            // Keep "Goto Lable" in command_1 filed
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_GOTO_START, 0x00, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x01, 0x00, 0x00);
            // End of command sequence: always try to trigger a DxINST0[SP_GEN_INT_STS] interrupt
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
            break;
        // DCS long write with Loop Buffer Version      (verified OK)
        case 0x04:
            PRINT("Continous DSI traffic test with DCS Long Write");
            //Test for long packet with low-power mode  
            // Reset Comand FIFO buffer
            tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     
            // Flush Loop Buffer
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_FLUSH_LOOP_BUFFER, 0x00, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_NO_OPERATION, 0x31, 0x00, 0x00);

            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_START_LABEL, 0x00, 0x00, 0x00);
            // test mode for any lane - 0xAA 0x33 0xF0 0x7F 0x55 0xCC 0x0F 0x80
            // DCS long write command
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0x00, 0x01);
            // Packet Length
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x05, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x00, 0x00, 0x01);
            // Packets
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x2c, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xA0, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xA1, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xA2, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xA3, 0x00, 0x01);

            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0x00, 0x01);
            // Packet Length
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x05, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x00, 0x00, 0x01);
            // Packets
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x2c, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xF0, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xF1, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xF2, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xF3, 0x00, 0x01);

            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_EXECUTE_LOOP_BUFFER, 0x0A, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_NO_OPERATION, 0x31, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_GOTO_START, 0x00, 0x00, 0x00);

            // End of command sequence: always try to trigger a DxINST0[SP_GEN_INT_STS] interrupt
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
            break;

        // DCS long write without using Loop Buffer  
        case 0x05:
            PRINT("DSI traffic test with 2 DCS Long Write (without using loop buffer)");

            // Reset Comand FIFO buffer
            tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);       

            // Flush Loop Buffer
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_FLUSH_LOOP_BUFFER, 0x00, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_NO_OPERATION, 0x31, 0x00, 0x00);
                
            //The following DSI command will be put in loop buffer ( loop buufer is 32 bytes long - upto 16 loop commands) for traffic generation
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_START_LABEL, 0x00, 0x00, 0x00);
            // test mode for any lane - 0xAA 0x33 0xF0 0x7F 0x55 0xCC 0x0F 0x80
            // DCS long write command
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0x00, 0x00);
            // Packet Length
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x05, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x00, 0x00, 0x00);
            // Packets
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x2c, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xA0, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xA1, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xA2, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xA3, 0x00, 0x00);
                
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0x00, 0x00);
            // Packet Length
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x05, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x00, 0x00, 0x00);
            // Packets
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0x2c, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xF0, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xF1, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xF2, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_COMMAND_WRITE, 0xF3, 0x00, 0x00);
                
            // Goto Lable                
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_GOTO_START, 0x00, 0x00, 0x00);

            // End of command sequence: always try to trigger a DxINST0[SP_GEN_INT_STS] interrupt
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId, LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
            break;

        //Test for long packet mixed with short packet
        case 0x06:
            PRINT("Continous DSI traffic test with DCS Long Write +  Data Generic Short Write");
            // Reset Comand FIFO buffer
            tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     

            // Set Delay Multiplier of NOP command ( 0x08 = multiply by 256)
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_SET_DLY_MULT, 0x08, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x60, 0x00, 0x00);
            
            // Flush Loop Buffer
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_FLUSH_LOOP_BUFFER, 0x00, 0x00, 0x00);

            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_TURN_ON_PERIPHERAL, 0, 0);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);

            //The following DSI command will be put in loop buffer ( loop buufer is 32 bytes long - upto 16 loop commands) for traffic generation
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_START_LABEL, 0x00, 0x00, 0x00);
            // test mode for any lane - 0xAA 0x33 0xF0 0x7F 0x55 0xCC 0x0F 0x80
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_FLUSH_LOOP_BUFFER, 0x00, 0x00, 0x00);

            // Loop Buffer Command
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0x00, 0x00); // DCS long write
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x0A, 0x00, 0x00); // Packet Length MSB
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0x00, 0x00); // Packet Length LSB
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_WRITE_MEMORY_START, 0x00, 0x00); // Write Memory Start
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA0, 0x00, 0x00); //Payload
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA1, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA2, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA3, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA4, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA5, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA6, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA7, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA8, 0x00, 0x00);

            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, 0x00, 0x00); // Short packet
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x22, 0x00, 0x00); // Short packet
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x11, 0x00, 0x00); // Short packet

            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x31, 0x00, 0x00); // Do nothing for 0x31 cycles

            // Keep "Goto Lable" in command_1 filed
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_GOTO_START, 0x00, 0x00, 0x00);

            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x31, 0x00, 0x00); // Do nothing for 0x31 cycles

            // End of command sequence: always try to trigger a DxINST0[SP_GEN_INT_STS] interrupt
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 

            break;

        // Same as 8 but only send one-shot packets without any loop (Verified OK)
        case 0x07:
            PRINT("Non-continous DSI traffic test with DCS Long Write");
            // Reset Comand FIFO buffer
            tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     
            // Set Delay Multiplier of NOP command ( 0x08 = multiply by 256)
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_SET_DLY_MULT, 0x08, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x60, 0x00, 0x00);
            
            // Flush Loop Buffer
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_FLUSH_LOOP_BUFFER, 0x00, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_TURN_ON_PERIPHERAL, 0, 0);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);

            // test mode for any lane - 0xAA 0x33 0xF0 0x7F 0x55 0xCC 0x0F 0x80
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0x00, 0x00); // DCS long write
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x0A, 0x00, 0x00); // Packet Length MSB
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0x00, 0x00); // Packet Length LSB
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_WRITE_MEMORY_START, 0x00, 0x00); // Write Memory Start
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA8, 0x00, 0x00); //Payload
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA7, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA6, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA5, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA4, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA3, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA2, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA1, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA0, 0x00, 0x00);

            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x31, 0x00, 0x00); // Do nothing for 0x31 cycles

            // End of command sequence: always try to trigger a DxINST0[SP_GEN_INT_STS] interrupt
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
            break;
        case 0x08:
            // Used to Verify LPDT split packet issue when 2 active lanes are used
            PRINT("Non-Continous DSI traffic test with DCS Long Write: 54 bytes");
            tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0x00, 0x00); // DCS long write
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x37, 0x00, 0x00); // Packet Length MSB
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0x00, 0x00); // Packet Length LSB
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_WRITE_MEMORY_START, 0x00, 0x00); // Write Memory Start
            for (i = 0; i < 3; i++) {
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA0, 0x00, 0x00); //Payload
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA1, 0x00, 0x00);
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA2, 0x00, 0x00);
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA3, 0x00, 0x00);
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA4, 0x00, 0x00);
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA5, 0x00, 0x00);
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA6, 0x00, 0x00);
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA7, 0x00, 0x00);
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA8, 0x00, 0x00);
            
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA0, 0x00, 0x00); //Payload
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA1, 0x00, 0x00);
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA2, 0x00, 0x00);
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA3, 0x00, 0x00);
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA4, 0x00, 0x00);
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA5, 0x00, 0x00);
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA6, 0x00, 0x00);
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA7, 0x00, 0x00);
                    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA8, 0x00, 0x00);
            }
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x31, 0x00, 0x00); // Do nothing for 0x31 cycles
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
            break;
        case 0x09:
            PRINT("Continous DSI traffic test with DCS Long Write: 54 bytes");
            // Reset Comand FIFO buffer
            tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     
                    
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_START_LABEL, 0x00, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0xf0, 0x00, 0x00); // Do nothing for 0x31 cycles
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0xf0, 0x00, 0x00); // Do nothing for 0x31 cycles
                    
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0x00, 0x00); // DCS long write
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x37, 0x00, 0x00); // Packet Length MSB
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0x00, 0x00); // Packet Length LSB
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_WRITE_MEMORY_START, 0x00, 0x00); // Write Memory Start
            for (i = 0; i < 3; i++) {
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA0, 0x00, 0x00); //Payload
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA1, 0x00, 0x00);
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA2, 0x00, 0x00);
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA3, 0x00, 0x00);
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA4, 0x00, 0x00);
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA5, 0x00, 0x00);
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA6, 0x00, 0x00);
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA7, 0x00, 0x00);
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA8, 0x00, 0x00);
                
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA0, 0x00, 0x00); //Payload
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA1, 0x00, 0x00);
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA2, 0x00, 0x00);
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA3, 0x00, 0x00);
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA4, 0x00, 0x00);
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA5, 0x00, 0x00);
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA6, 0x00, 0x00);
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA7, 0x00, 0x00);
                        tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA8, 0x00, 0x00);
            }
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0xf0, 0x00, 0x00); // Do nothing for 0x31 cycles
            //The following DSI command will be put in loop buffer ( loop buufer is 32 bytes long - upto 16 loop commands) for traffic generation
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_GOTO_START, 0x00, 0x00, 0x00);
            // End of command sequence: always try to trigger a DxINST0[SP_GEN_INT_STS] interrupt
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
            break;
        case 0x0A:
                PRINT("DSI traffic test with DCS-long-write: 7*18 bytes");
                tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0x00, 0x00); // DCS long write
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x7f, 0x00, 0x00); // Packet Length MSB
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0x00, 0x00); // Packet Length LSB
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_WRITE_MEMORY_START, 0x00, 0x00); // Write Memory Start
                // 126 bytes in total (42 *3)
                for (i = 0; i < 7; i++) {
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA0, 0x00, 0x00); //Payload
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA1, 0x00, 0x00);
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA2, 0x00, 0x00);
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA3, 0x00, 0x00);
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA4, 0x00, 0x00);
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA5, 0x00, 0x00);
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA6, 0x00, 0x00);
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA7, 0x00, 0x00);
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA8, 0x00, 0x00);
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA0, 0x00, 0x00); //Payload
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA1, 0x00, 0x00);
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA2, 0x00, 0x00);
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA3, 0x00, 0x00);
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA4, 0x00, 0x00);
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA5, 0x00, 0x00);
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA6, 0x00, 0x00);
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA7, 0x00, 0x00);
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA8, 0x00, 0x00);
                   }
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_NOP, 0x00, 0x00); // Do nothing for 0x31 cycles
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x31, 0x00, 0x00); // Do nothing for 0x31 cycles
                // End of command sequence: always try to trigger a DxINST0[SP_GEN_INT_STS] interrupt
                tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
                break;
        case 0x0B:
            PRINT("DSI traffic test with Data-Write and Virtual Channel (VC = 0x00, 0x01, 0x02)");
            // Reset Comand FIFO buffer
            tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     
            //Create Comand Sequence to generate traffic 
            // Flush Loop Buffer
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_FLUSH_LOOP_BUFFER, 0x00, 0x00, 0x00);
            
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_TURN_ON_PERIPHERAL, 0x01, 0);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0x00, 0);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0x00, 0);
            
            //The following DSI command will be put in loop buffer ( loop buufer is 32 bytes long - upto 16 loop commands) for traffic generation
            // Keep "Start Lable" in command_0 filed
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_START_LABEL, 0x00, 0x00, 0x00);
            
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x00, 0x00, 0x00);
            
            // Keep "Data Write" related command start in command_0 filed
            // RGB24
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_RGB_888_PACKET, 0x02, 0x01);
            // Length 0x0003
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x06, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0x00, 0x01);
            // Data Write start
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0xA9, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0xA9, 0xA9, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0xAA, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0xAA, 0xAA, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0x33, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0x33, 0x33, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0xF0, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0xF0, 0xF0, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0x7F, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0x7F, 0x7F, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_DATA_WRITE, 0x55, 0x00, 0x01);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  0x55, 0x55, 0x00, 0x00);
            // execute loop buffer instruction (can't be nested with loop flag) for 10 times
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_EXECUTE_LOOP_BUFFER, 0xff, 0x00, 0x00);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x00, 0x00, 0x00);
            // Keep "Goto Lable" in command_1 filed
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_GOTO_START, 0x00, 0x00, 0x00);
            // End of command sequence: always try to trigger a DxINST0[SP_GEN_INT_STS] interrupt
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
            break;
        // Traffic from Frame Buffer and Go through Display Controller Pixel Pipeline and send to the first LCD pannel
        case 0x10:
            //Test Lemur/TPO board with color bar (frame buffer filled with color bar RGB565 pixels)
            pMode = pDisplayDriver->config.dsiConfig[dsiId].pMode;
            width = pMode->width;
            height = pMode->height;
            pTavor2DisplayDriver->pDispFrameBuffer = (WORD *) (&dispFrameBuffer[0]);
            fbSrcAddr = (DWORD )(pTavor2DisplayDriver->pDispFrameBuffer);
            PRINT2N("Send one frame of pixels to LCD panel by using LCD CTL's pixel pipeline: width=0x%08x, height=0x%08x!", width = pMode->width,width = pMode->height);
            rc = tavorpv2_dispCopyToLcd(pDisplayDriver,fbSrcAddr,height,width,0,0);
            return rc;
            break;
        // Traffic from Frame Buffer and Go through Display Controller Pixel Pipeline and send to the first LCD pannel
        case 0x11:
            PRINT("Continue Sending one frame to LCD panel by using LCD CTL's pixel pipeline");
            //Test Lemur/TPO board with color bar (frame buffer filled with color bar RGB565 pixels)
            pMode = pDisplayDriver->config.dsiConfig[dsiId].pMode;
            width = pMode->width;
            height = pMode->height;
            //Set update flag
            tavorpv2_dispCtlStartFetch(0, TRUE);
            rc = tavorpv2_dsiSendFrame(pDisplayDriver,height,width, 0, FALSE);
            return rc;
            break;
        case 0x12:    
            // Initialize "frame buffer" to be color "red"
            PRINT("Initalize Frame Buffer with RBG24 red pixels.");
            for (i = 0; i < 864*480*3; i=i+3) {dispFrameBuffer[i] = 0x00;dispFrameBuffer[i+1] = 0x00;dispFrameBuffer[i+2] = 0xFF;}
            CacheExecute(DATA_CACHE, CACHE_CLEAN_AND_INVALIDATE, (DWORD) (&dispFrameBuffer[0]), 864*480*3);
            rc = TRUE; return rc;
            break;
        case 0x13:
            PRINT("Initalize Frame Buffer with color bar.");
            tavorpv2_dispInitFrmBuf(pDisplayDriver);
			rc = TRUE;
			return rc;
			break;
        case 0x14:
            // Test to display a image with different size from the Panel
            tavorpv2_dispCopyToLcd(pDisplayDriver,(DWORD )(&dispFrameBuffer[0]), 240, width, 500, 0);
            break;
        case 0x15:
            // Test to send 2 frames
            pMode = pDisplayDriver->config.dsiConfig[dsiId].pMode;
            width = pMode->width; height = pMode->height;
            tavorpv2_dispCopyToLcd(pDisplayDriver,(DWORD )(&dispFrameBuffer[0]), height, width, 1, 0);
            break;
        case 0x16:
            // Test to send 501 frames
            pMode = pDisplayDriver->config.dsiConfig[dsiId].pMode;
            width = pMode->width; height = pMode->height;
            tavorpv2_dispCopyToLcd(pDisplayDriver,(DWORD )(&dispFrameBuffer[0]), height, width, 500, 0);
            break;
        case 0x17:
            // send 500001 frames
            pMode = pDisplayDriver->config.dsiConfig[dsiId].pMode;
            width = pMode->width; height = pMode->height;
           tavorpv2_dispCopyToLcd(pDisplayDriver,(DWORD )(&dispFrameBuffer[0]), height, width, 500000, 0);
           break;
		case 0x18:
            //LOOP forever
            pMode = pDisplayDriver->config.dsiConfig[dsiId].pMode;
            width = pMode->width; height = pMode->height;
            tavorpv2_dispCopyToLcd(pDisplayDriver,(DWORD )(&dispFrameBuffer[0]), height, width, 0xffffffff, 0);
            break;
        case 0x19:
			PRINT2N("SoF=0x%8x, EoF=0x%8x",pDisplayDriver->statistics.dsiStats[dsiId].dsiDispSof,
				pDisplayDriver->statistics.dsiStats[dsiId].dsiDispEof);
			break;
		case 0x1A:
			PRINT("Reset staticstics: SoF/EoF");
			pDisplayDriver->statistics.dsiStats[dsiId].dsiDispSof = 0x00;
			pDisplayDriver->statistics.dsiStats[dsiId].dsiDispEof = 0x00;
			break;
        //Initialize Camel board with LM2550 Deserializer
        case 0x20:
            PRINT("DSI test: test Camel board with DES LM2550 ");
            rc = FALSE;
            dsiConfig =(lcdDsiConfig *) (&(pDisplayDriver->config.dsiConfig[dsiId]));
            if (tavorpv2_desLm2550Init(pDisplayDriver, 0, dsiConfig->powerMode))
                rc = TRUE;
            return rc;
            break;
        // Initialize Lemur board and TPO LCD panel
        case 0x21:
             rc = FALSE;
             PRINT("DSI test: initialize Lemur/TPO board!");
             if (tavorpv2_dcsTpoLcdInit(pDisplayDriver, dsiId, 0))
             {
                tavorpv2_dsiStart(dsiId);
                rc = TRUE;
             }
             return rc;
             break;
        // Send Pixels from frame buffer to TPO
        case 0x22:
            //Test Lemur/TPO board with color bar (frame buffer filled with color bar)
            PRINT("DSI test: Send pixels to the LCD panel from DSI cmd FIFO!");
            // tavorpv2_dispInitFrmBuf(pDisplayDriver);
            pTavor2DisplayDriver->pDispFrameBuffer = (WORD *) (&dispFrameBuffer[0]);
            // tavorpv2_dispInitFrmBuf(pDisplayDriver);
            pTavor2DisplayDriver->pDispFrmPtr = 0x00;
            // Trig interrupt to start sending
            tavorpv2_dsiTrigIntProc(pDisplayDriver, dsiId);
            // enable DSI/LCD controller
            tavorpv2_dsiStart(dsiId);
            rc = TRUE;
            return rc;
            break;
        // Reading the first 2 bytes from interal frame buffer on TPO
         case 0x23:
			 pMode = pDisplayDriver->config.dsiConfig[dsiId].pMode;
             PRINTN("DSI test: Start reading pixels from TPO, 0x%08x pixels each time!",pMode->width);
			 pTavor2DisplayDriver->lcdRdLen= pMode->width*3;
             // Reset Comand FIFO buffer
             tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     
             tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_READ_NO_PARAMETER, 0, 0);
             tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_READ_MEMORY_START, 0, 0);
             tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
             // Issue BTA
             tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_READ_FRAME_BUFFER, 0x00, 0, 0);
             tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x42, 0x00, 0x00);
             // Trig "int proc"
             tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00);
             tavorpv2_dsiStart(dsiId);
             rc = TRUE;
             return rc;
             break;
        // Continue reading Pixels from frame buffer to TPO
        case 0x24:
            PRINT("DSI test: Continue reading pixels from TPO!");
			pMode = pDisplayDriver->config.dsiConfig[dsiId].pMode;
			pTavor2DisplayDriver->lcdRdLen= pMode->width*3;
            // Reset Comand FIFO buffer
            tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,    LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_READ_NO_PARAMETER, 0, 0);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,    LCD_CMD_COMMAND_WRITE, DCS_CMD_READ_MEMORY_CONTINUE, 0, 0);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,    LCD_CMD_COMMAND_WRITE, 0, 0, 0);
            // Issue BTA
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,    LCD_CMD_READ_FRAME_BUFFER, 0x00, 0, 0);
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,    LCD_CMD_NO_OPERATION, 0x42, 0x00, 0x00);
            // Trig "int proc"
            tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,    LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00);
            tavorpv2_dsiStart(dsiId);
            rc = TRUE;
            return rc;
            break;
        // Reading the first 2 bytes from interal frame buffer on TPO
         case 0x25:
             PRINT("DSI test: Read pixel format back from the panel!");
             // Reset Comand FIFO buffer
             tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     
             // Get pixel format
             // Nop for the first 16 bits 
			 tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,	 LCD_CMD_NO_OPERATION, 0xff, 0x00, 0x00);
             tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,    LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_READ_NO_PARAMETER, 0, 0);    
             tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,   LCD_CMD_COMMAND_WRITE, DCS_CMD_GET_PIXEL_FORMAT, 0, 0);    
             tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,    LCD_CMD_COMMAND_WRITE, 0, 0, 0);            
             //"Read  Status" causes BTA     
             tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,    LCD_CMD_READ_STATUS, 0x00, 0, 0);    
             tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x10, 0x00, 0x00);

			 // Dummy Cycle
			 //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_NULL_PACKET, 0, 0);
			 //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x05, 0, 0);
			 //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
			 //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
			 //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
			 //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
			 //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
			 //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);

             // Trig "int proc"
             tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00);
             tavorpv2_dsiStart(dsiId);
             rc = TRUE;
             return rc;
             break;

		case 0x26:
			PRINT("DSI test: read device ID:!");
			// Reset Comand FIFO buffer
			tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId); 	
			// Set Maximum Return Packet Size : 2
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_SET_MAX_RETURN_PACKET_SIZE, 0, 0);
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x08, 0, 0);
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);

			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,	LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_READ_1_PARAMETER, 0, 0);
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,	LCD_CMD_COMMAND_WRITE, DCS_CMD_READ_DEV_CODE, 0, 0);
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,	LCD_CMD_COMMAND_WRITE, 0, 0, 0);
			// "Read Status" causes BTA
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,	LCD_CMD_READ_STATUS, 0x00, 0, 0);
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,	LCD_CMD_NO_OPERATION, 0x10, 0x00, 0x00);

			// Trig "int proc"
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00);
			tavorpv2_dsiStart(dsiId);
			rc = TRUE;
			return rc;
			break;

		// Initialize Lemur board and TPO LCD panel
		case 0x27:
			 rc = FALSE;
			 PRINT("DSI test: initialize Proteus board!");
			 if (tavorpv2_dcsProteusLcdInit(pDisplayDriver, dsiId, 0))
			 {
				tavorpv2_dsiStart(dsiId);
				rc = TRUE;
			 }
			 return rc;
			 break;

		case 0x28:
			PRINT("DSI test: Update frame buffer:!");
			// Reset Comand FIFO buffer
			tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId); 	

			// DCS Long Write to set start/end of Column Address
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0, 0);
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x05, 0, 0); // Length LS byte
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_COLUMN_ADDRESS, 0, 0);
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0); // Start column address (MSB)
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x01, 0, 0); // End column address (MSB)
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xDF, 0, 0); // LSB
			
			// DCS Long Write to set start/end Page Address
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0, 0);
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x05, 0, 0); // Length LS byte
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_PAGE_ADDRESS, 0, 0);
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0); // Start Page address (MSB)
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x01, 0, 0); // End Page address (MSB)
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xB7, 0, 0); // LSB
			
			// Set Address Mode
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_SHORT_WRITE_1_PARAMETER, 0, 0);
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_ADDRESS_MODE, 0, 0);
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);

			// Trig "int proc"
			tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00);
			tavorpv2_dsiStart(dsiId);
			rc = TRUE;
			return rc;
			break;

        //////// DSI/Display controller configurations from here ////////
        // Change DSI Controller configiruations to generate traffic
        case 0xA0:
             PRINT("DSI test: Set DSI Controller under test to be DSI 0");
             dsiId = 0;
             return TRUE;
             break;
        case 0xA1:
             PRINT("DSI test: Set DSI Controller under test to be DSI 1");
             dsiId = 1;
             return TRUE;
             break;
        case 0xA2:
             // Change Active Data Lanes
             PRINT("Set DSI link active lanes to be 1 !");
             tavorpv2_dsiCtlSetActiveLane(dsiId, 1);
             pDisplayDriver->config.dsiConfig[dsiId].activeLanes = 1;
             return TRUE;
             break;
        case 0xA3:
             // Change Active Data Lanes
             PRINT("Set DSI link active lanes to be 2 !");
             tavorpv2_dsiCtlSetActiveLane(dsiId, 2);
             pDisplayDriver->config.dsiConfig[dsiId].activeLanes = 2;
             return TRUE;
             break;
        case 0xA4:
             // Change Active Data Lanes
             PRINT("Set DSI link active lanes to be 3 !");
             tavorpv2_dsiCtlSetActiveLane(dsiId, 3);
             pDisplayDriver->config.dsiConfig[dsiId].activeLanes = 3;
             return TRUE;
             break;
        case 0xA5:
             // Set to use TPO
             PRINT("Set to use TPO panel !");
             dsiConfig =(lcdDsiConfig *) (&(pDisplayDriver->config.dsiConfig[dsiId]));
             dsiConfig->status = DSI_ENABLED;      
             dsiConfig->dsiLinkClock = DSI_LINK_CLK_312M;                      
             dsiConfig->panelId = 0x00;
             return TRUE;
             break;
        case 0xA6:
             // Set to use TPO
             PRINT("Set to use eDISCO panel !");
             dsiConfig =(lcdDsiConfig *) (&(pDisplayDriver->config.dsiConfig[dsiId]));
             dsiConfig->status = DSI_ENABLED;      
             dsiConfig->dsiLinkClock = DSI_LINK_CLK_156M;                      
             dsiConfig->panelId = 0x01;
             return TRUE;
             break;
        case 0xA7:
			  // Set to use Proteus
			  PRINT("Set to use Proteus panel !");
			  dsiConfig =(lcdDsiConfig *) (&(pDisplayDriver->config.dsiConfig[dsiId]));
			  dsiConfig->status = DSI_ENABLED;		
			  dsiConfig->dsiLinkClock = DSI_LINK_CLK_312M;						
			  dsiConfig->panelId = 0x02;
			  dsiConfig->activeLanes = 0x01;
			  return TRUE;
			  break;

        // LP-HP-LPDT test         
        case 0xB0:
             PRINT("Transmit DSI data in Low Power Mode!");
             pDisplayDriver->config.dsiConfig[dsiId].powerMode = DSI_POWERMODE_LPDT;
             tavorpv2_dsiCtlSetPowerMode(dsiId, DSI_POWERMODE_LPDT);      
             return TRUE;
             break;
        case 0xB1:
             //Transition back to High Speed (short packet mode) Mode to transfer data
             PRINT("Transmit DSI data in High Speed Mode and keep in LP between transmission!");
             //tavorpv2_dsiCtlSetPowerMode(dsiId, DSI_POWERMODE_HS);     
             tavorpv2_dsiCtlSetPowerMode(dsiId, DSI_POWERMODE_HS_NORIHS); 
             //pDisplayDriver->config.dsiConfig[dsiId].powerMode &= (~DSI_POWERMODE_LPDT);             
             pDisplayDriver->config.dsiConfig[dsiId].powerMode = DSI_POWERMODE_HS_NORIHS;             
             return TRUE;
             break;
        case 0xB2:
             //Transition back to High Speed Mode (multiple packets per transmission, i.e. long packet mode) to transfer data
             PRINT("Transmit DSI data in High Speed Mode and keep in RIHS!");
             //tavorpv2_dsiCtlSetPowerMode(dsiId, DSI_POWERMODE_HS);     
             tavorpv2_dsiCtlSetPowerMode(dsiId, DSI_POWERMODE_HS_RIHS);
             //pDisplayDriver->config.dsiConfig[dsiId].powerMode &= (~DSI_POWERMODE_LPDT);             
             pDisplayDriver->config.dsiConfig[dsiId].powerMode = DSI_POWERMODE_HS_RIHS;             
             return TRUE;
             break;
        case 0xB3:
             //Transition back to ULPS to transfer data
             PRINT("Put DSI link in Ultra Low Power state: wait until DSI in LP-11 stop state!");
             while (! (DSI_DxSSR_STOP_ST & ReadRegDword(DSI_DxSSR(dsiId)))) ;
             tavorpv2_dsiCtlSetPowerMode(dsiId, DSI_POWERMODE_ENTER_ULPS);
             pDisplayDriver->config.dsiConfig[dsiId].powerMode |= DSI_POWERMODE_ENTER_ULPS;    
             return TRUE;
             break;
        case 0xB4:
             //Transition back to ULPS to transfer data
             PRINT("Put DSI link in Power Up (exit ULPS)!");
             tavorpv2_dsiCtlSetPowerMode(dsiId, DSI_POWERMODE_EXIT_ULPS);
             pDisplayDriver->config.dsiConfig[dsiId].powerMode &= (~DSI_POWERMODE_ENTER_ULPS);    
             return TRUE;
             break;
        case 0xB5:
             //Transition back to ULPS to transfer data
             PRINT("Put DSI link in Ultra Low Power state: exit after EoF!");
             if (DSI_DxSSR_STOP_ST & ReadRegDword(DSI_DxSSR(dsiId)))
                 tavorpv2_dsiCtlSetPowerMode(dsiId, DSI_POWERMODE_ENTER_ULPS);
             pDisplayDriver->config.dsiConfig[dsiId].powerMode |= DSI_POWERMODE_ENTER_ULPS;    
             return TRUE;
             break;

        // Set DSI Link Freq
        case 0xC0:
             PRINT("Set DSI link freq to be 312Mhz!");
             //tavorpv2_dsiCtlEnable(dsiId,DSI_CTL_QDISABLE);
             if (dsiId == 0)
             {
                 pDisplayDriver->config.dsiConfig[0].dsiLinkClock = DSI_LINK_CLK_312M;
                 tavorpv2_dsiSetIntClock(dsiId,pDisplayDriver->config.dsiConfig[0].dsiLinkClock);
             }
             else
             {
                  pDisplayDriver->config.dsiConfig[1].dsiLinkClock = DSI_LINK_CLK_312M;
                  tavorpv2_dsiSetIntClock(dsiId,pDisplayDriver->config.dsiConfig[1].dsiLinkClock);
             }
             tavorpv2_dsiCtlSetPhyTimer(dsiId,DSI_LINK_CLK_312M);
             //tavorpv2_dsiCtlEnable(dsiId,DSI_CTL_ENABLE);
             return TRUE;
             break;
        case 0xC1:
             //Transition to Low Power Mode to transfer data
             PRINT("Set DSI link freq to be 52Mhz!");
             tavorpv2_dsiCtlEnable(dsiId, DSI_CTL_DISABLE);
             
             if (dsiId == 0)
             {
                 pDisplayDriver->config.dsiConfig[0].dsiLinkClock = DSI_LINK_CLK_52M;
                 tavorpv2_dsiSetIntClock(dsiId,pDisplayDriver->config.dsiConfig[0].dsiLinkClock);
             }
             else
             {
                  pDisplayDriver->config.dsiConfig[1].dsiLinkClock = DSI_LINK_CLK_52M;
                  tavorpv2_dsiSetIntClock(dsiId,pDisplayDriver->config.dsiConfig[1].dsiLinkClock);
             }
             tavorpv2_dsiCtlSetPhyTimer(dsiId,DSI_LINK_CLK_52M);     
             tavorpv2_dsiCtlEnable(dsiId, DSI_CTL_ENABLE);
             return TRUE;
             break;
        case 0xC2:
             PRINT("Set DSI link freq to be 104Mhz!");
             //tavorpv2_dsiCtlEnable(dsiId,DSI_CTL_QDISABLE);
             if (dsiId == 0)
             {
                 pDisplayDriver->config.dsiConfig[0].dsiLinkClock = DSI_LINK_CLK_104M;
                 tavorpv2_dsiSetIntClock(dsiId,pDisplayDriver->config.dsiConfig[0].dsiLinkClock);
             }
             else
             {
                  pDisplayDriver->config.dsiConfig[1].dsiLinkClock = DSI_LINK_CLK_104M;
                  tavorpv2_dsiSetIntClock(dsiId,pDisplayDriver->config.dsiConfig[1].dsiLinkClock);
             }
             //tavorpv2_lcdSetFIClock(pDisplayDriver->config.dsiConfig[0].dsiLinkClock,pDisplayDriver->config.dsiConfig[1].dsiLinkClock);
             tavorpv2_dsiCtlSetPhyTimer(dsiId,DSI_LINK_CLK_104M);
             //tavorpv2_dsiCtlEnable(dsiId,DSI_CTL_ENABLE);
             return TRUE;
             break;
        case 0xC3:
             PRINT("Set DSI link freq to be 156Mhz!");
             //tavorpv2_dsiCtlEnable(dsiId,DSI_CTL_QDISABLE);
             if (dsiId == 0)
             {
                 pDisplayDriver->config.dsiConfig[0].dsiLinkClock = DSI_LINK_CLK_156M;
                 tavorpv2_dsiSetIntClock(dsiId,pDisplayDriver->config.dsiConfig[0].dsiLinkClock);
             }
             else
             {
                  pDisplayDriver->config.dsiConfig[1].dsiLinkClock = DSI_LINK_CLK_156M;
                  tavorpv2_dsiSetIntClock(dsiId,pDisplayDriver->config.dsiConfig[1].dsiLinkClock);
             }
             tavorpv2_dsiCtlSetPhyTimer(dsiId,DSI_LINK_CLK_156M);
             //tavorpv2_dsiCtlEnable(dsiId,DSI_CTL_ENABLE);
             return TRUE;
             break;
        case 0xC4:
             PRINT("Set DSI link freq to be 208Mhz!");
             //tavorpv2_dsiCtlEnable(dsiId,DSI_CTL_QDISABLE);
             if (dsiId == 0)
             {
                 pDisplayDriver->config.dsiConfig[0].dsiLinkClock = DSI_LINK_CLK_208M;
                 tavorpv2_dsiSetIntClock(dsiId,pDisplayDriver->config.dsiConfig[0].dsiLinkClock);
             }
             else
             {
                  pDisplayDriver->config.dsiConfig[1].dsiLinkClock = DSI_LINK_CLK_208M;
                  tavorpv2_dsiSetIntClock(dsiId,pDisplayDriver->config.dsiConfig[1].dsiLinkClock);
             }
             tavorpv2_dsiCtlSetPhyTimer(dsiId,DSI_LINK_CLK_208M);
             //tavorpv2_dsiCtlEnable(dsiId,DSI_CTL_ENABLE);
             return TRUE;
             break;
        case 0xC5:
             PRINT("Set DSI link freq to be DDR2 !");
             pDisplayDriver->config.dsiConfig[dsiId].dsiLinkClock = DSI_LINK_CLK_DDR2;
             tavorpv2_dsiSetIntClock(dsiId,pDisplayDriver->config.dsiConfig[dsiId].dsiLinkClock);
             tavorpv2_dsiCtlSetPhyTimer(dsiId,DSI_LINK_CLK_DDR2);
             return TRUE;
             break;
		 case 0xC6:
			  PRINT("Set DSI link freq to be 624Mhz!");
			  pDisplayDriver->config.dsiConfig[dsiId].dsiLinkClock = DSI_LINK_CLK_624M;
			  tavorpv2_dsiSetIntClock(dsiId,pDisplayDriver->config.dsiConfig[dsiId].dsiLinkClock);
			  tavorpv2_dsiCtlSetPhyTimer(dsiId,DSI_LINK_CLK_624M);
			  return TRUE;
			  break;

        // Sending BTA/Trig code
        case 0xE0:     
             //Send DSI Triiger code
             PRINT("Trig a soft reset (0x62) on DSI link ");
             tavorpv2_dsiSetTrigCode(dsiId, DSI_TRIG_CODE_RESET);
             return TRUE;
             break;
         case 0xE1:   
             //Issue a BTA manually
             PRINT("Trig a BTA on DSI interface ");
             tavorpv2_dsiIssueBTA(dsiId, TRUE);
             return TRUE;
             break;
         // Break out of goto/start label loop
         case 0xF0:
             PRINT("Break and reset DSI command FIFO!");
             tavorpv2_dsiStopTraffic(dsiId);
             return TRUE;
             break;
         //Enable/Disable DSI/LCD test
        case 0xF1:
             PRINT("Quick Disable DSI/LCD!");
             tavorpv2_drvLcdDisable(pDisplayDriver);
             return TRUE;
             break;
        case 0xF2:
             PRINT("Normal Disable DSI/LCD!");
             tavorpv2_dsiCtlEnable(0, DSI_CTL_DISABLE);
             tavorpv2_dsiCtlEnable(1, DSI_CTL_DISABLE);
             tavorpv2_dispCtlEnable(DISP_CTL_DISABLE);
             return TRUE;
             break;
         case 0xF3:
             PRINT("Reconfig and Enable LCD/DSI!");
             tavorpv2_drvLcdEnable(pDisplayDriver);
             return TRUE;
             break;
        case 0xF4:
             // Only enable LCD controller
             tavorpv2_dispCtlEnable(DISP_CTL_ENABLE);
             return TRUE;
             break;
		case 0xf5:
			 PRINT("dispFrameBuffer=");
			 for (i=0; i < 30; i++) {
			 	PRINTN("0x%4x ",dispFrameBuffer[i]);
			 	}
			 break;
        case 0xf6:
			 // Stop traffic on DSI interfaces
			 lcdStopFlag = TRUE;
		     break;	 
	     case 0xf7:
		    // Stop traffic on DSI interfaces
			  tavorpv2_lcdSetGpios();
			  break;  
		case 0xFF:
             //This is dummy test to clear warning info when compiling
             return TRUE;
             tavorpv2_drvLcdIntEvtWait(pDisplayDriver);
             tavorpv2_dsi1DmaInt();
             tavorpv2_dispCopyToLcdFromDsi(pDisplayDriver, 0, 0, 0, 0, 0);
             break;
        default:
           break;
    }

    // Start sending command seq to DSI COMMAND FIFO
    tavorpv2_dsiSendCmdSeq(pDisplayDriver, dsiId);
    
    return TRUE;

}

/*************************************************************************
*
* tavorpv2_drvLcdConfigInterfaceTiming - Set DSI PHY timing
*
* This routine set DSI controller's D-PHY timing.
*
* RETURNS: void
*
* ERRNO: N/A
*
* 
*/
static void tavorpv2_drvLcdConfigInterfaceTiming(DispDrv *pDisplayDriver, DWORD regA, DWORD regB, DWORD regC, BYTE displayId)
{

    // Retrieve DSI controller ID by using displayId
    DWORD dsiId = tavorpv2_dispGetDsiId(pDisplayDriver, displayId);

    // DSI interface PHY timing
    tavorpv2_dispConfigureInterfaceTiming(dsiId, regA, regB, regC);
}


/*************************************************************************
*
* tavorpv2_dispGetDsiId - find DSI Id based on display ID
*
* This routine finds the DSI controller's ID (0 or 1) based on the dispalyId.
*
* RETURNS: DSI id if find the matching display ID; otherwise
*          0xff when no match
*
* ERRNO: N/A
*
* 
*/
static DWORD tavorpv2_dispGetDsiId(DispDrv *pDisplayDriver, DWORD displayId)
{
    SWORD i = 0;
    lcdDsiConfig *dsiConfig;
    
    // Find matched DSI 
    for ( i = 0; i < DSI_CTL_MAX; i++)
    {
        dsiConfig = &(pDisplayDriver->config.dsiConfig[i]);

        if (dsiConfig->status == DSI_ENABLED)
        {
            if (dsiConfig->displayId == displayId)
                return i;
            else if (( dsiConfig->panels > 1) 
                     && ( displayId == (dsiConfig->displayId + 1)))
                return i;
        }
    }    
        
    return (0xff);
}    


/*************************************************************************
*
* tavorpv2_dispModeFind - find a mode
*
* This routine finds a graphics device mode from a list of available modes.
* The mode list specified by <pDisplayDriver->pModeList> is searched for a mode that 
* matches the mode specfied by <dsiLinkClock> and <panelId>.  
*
* RETURNS: Index within the mode list for a matching entry; otherwise
*          -1 when no match
*
* ERRNO: N/A
*
* 
*/
static DWORD tavorpv2_dispModeFind(DispDrv *pDisplayDriver, BYTE dsiLinkClock, BYTE panelId)
{
    DWORD ix;
    DWORD modeIx = 0xff;
    
    for (ix = 0; ix <NELEMENTS(tavorpv2_lcdModeTable); ix++)
    {
    if ((pDisplayDriver->pModeList[ix].tClock == (WORD) dsiLinkClock) && 
        (pDisplayDriver->pModeList[ix].panelId == panelId))
        {
            /* match founded */
            modeIx = ix;
            break;
        }
    }
 
    return (modeIx);
}


// Clear DSI statistics
static void tavorpv2_dsiResetStatis(DispDrv *pDisplayDriver, DWORD dsiId)
{
    lcdDsiStats *dsiStats = &(pDisplayDriver->statistics.dsiStats[dsiId]);

    memset(dsiStats, 0, sizeof(lcdDsiStats));
}


// Initilaize frame buffer to display color bar on the first LCD panel (Big-Endian for Fetch Unit DMA)
static void tavorpv2_dispInitFrmBuf(DispDrv *pDisplayDriver)
{
    DWORD inc, i = 0;
    WORD width, height,bpp = 0;
    lcdMode *pMode = NULL;
    
    Tavor2DispDrv *pTavor2DisplayDriver = (Tavor2DispDrv *) pDisplayDriver;

    for (i = 0; i < TAVORPV2_DSI_CTL; i++)
        if (pDisplayDriver->config.dsiConfig[i].status == DSI_ENABLED)
            pMode = pDisplayDriver->config.dsiConfig[i].pMode;
    
    // Initialize frame buffer if frame buffer is big enough for the LCD panel
    if (( pMode != NULL) && (pTavor2DisplayDriver->pDispFrameBuffer != NULL))
    {
        // Initialize Frame Buffer to be a color Bar based on the first LCD panel settings (Suppose RBG565)
        if (pMode != NULL)
        {
            height = pMode->height; 
            width = pMode->width;
            bpp = (pMode->colorDepth) >> 3;
        }
		else
		{
		    height = TAVORPV2_LCD_MAX_HEIGHT;
		    width = TAVORPV2_LCD_MAX_WIDTH;
		    bpp = TAVORPV2_LCD_DEF_BPP;
		}		
        // divide by 16
        inc = (height*width*bpp) >> 4;
        
        // Fill in colors
        i = 0;
        while( i < (width*height*bpp)/2)
        {
            if ( i < inc)
            {
                if ( bpp == 3) {
                    pTavor2DisplayDriver->pDispFrameBuffer[i] = 0x0000;
                    pTavor2DisplayDriver->pDispFrameBuffer[i + 1] = 0x0000;
                    pTavor2DisplayDriver->pDispFrameBuffer[i + 2] = 0x0000;
                }
                else
                    // black : R = 0; G = 0; B = 0;
                    pTavor2DisplayDriver->pDispFrameBuffer[i] = 0x0000;
            }    
            else if ( (i >= inc) && i < ( 2*inc)) 
            {
                if ( bpp == 3)
                {
                    pTavor2DisplayDriver->pDispFrameBuffer[i] = 0x0000;
                    pTavor2DisplayDriver->pDispFrameBuffer[i+1] = 0x00FF;
                    pTavor2DisplayDriver->pDispFrameBuffer[i+2] = 0xFF00;
                }
                else 
                    // Red: R = 1; G = 0; B = 0;
                    pTavor2DisplayDriver->pDispFrameBuffer[i] = 0xF800;
            }        
            else if ( (i >= 2*inc) && i < ( 3*inc))
            {
                if ( bpp == 3)
                {
                    pTavor2DisplayDriver->pDispFrameBuffer[i] = 0xFF00;
                    pTavor2DisplayDriver->pDispFrameBuffer[i+1] = 0x0000;
                    pTavor2DisplayDriver->pDispFrameBuffer[i+2] = 0x00FF;
                }
                else
                        // Green: R = 0; G = 1; B = 0;
                        pTavor2DisplayDriver->pDispFrameBuffer[i] = 0x07E0;
            }
            else if ( (i >= 3*inc) && i < ( 4*inc)) 
            {
                    if ( bpp == 3)
                    {
                        pTavor2DisplayDriver->pDispFrameBuffer[i] = 0x00FF;
                        pTavor2DisplayDriver->pDispFrameBuffer[i+1] = 0xFF00;
                        pTavor2DisplayDriver->pDispFrameBuffer[i+2] = 0x0000;
                    }
                    else
                        // Blue: R = 0; G = 0; B = 1;
                        pTavor2DisplayDriver->pDispFrameBuffer[i] = 0x001F;
        
            }
            else if ( (i >= 4*inc) && i < ( 5*inc)) 
            {
                    if ( bpp == 3)
                    {
                        pTavor2DisplayDriver->pDispFrameBuffer[i] = 0xFFFF;
                        pTavor2DisplayDriver->pDispFrameBuffer[i+1] = 0xFF00;
                        pTavor2DisplayDriver->pDispFrameBuffer[i+2] = 0x00FF;
                    }
                    else
                         // Cyan: R = 0; G = 1; B = 1
                        pTavor2DisplayDriver->pDispFrameBuffer[i] = 0x07FF;
        
            }
            else if ( (i >= 5*inc) && i < ( 6*inc)) 
            {
                    if ( bpp == 3)
                    {
                        pTavor2DisplayDriver->pDispFrameBuffer[i] = 0xFF00;
                        pTavor2DisplayDriver->pDispFrameBuffer[i+1] = 0x00FF;
                        pTavor2DisplayDriver->pDispFrameBuffer[i+2] = 0xFFFF;
                    }
                    else
                        // Yellow: R = 1; G = 1; B = 0
                        pTavor2DisplayDriver->pDispFrameBuffer[i] = 0xFFE0;
            }
            else if ( (i >= 6*inc) && i < ( 7*inc)) 
            {
                    if ( bpp == 3)
                    {
                        pTavor2DisplayDriver->pDispFrameBuffer[i] = 0x00FF;
                        pTavor2DisplayDriver->pDispFrameBuffer[i+1] = 0xFFFF;
                        pTavor2DisplayDriver->pDispFrameBuffer[i+2] = 0xFF00;
                    }
                    else
                        // Magenta: R = 1; G = 0; B = 1
                        pTavor2DisplayDriver->pDispFrameBuffer[i] = 0xF81F;
        
            }
            else if ( (i >= 7*inc) && i < ( 8*inc))
            {
                    if ( bpp == 3)
                    {
                        pTavor2DisplayDriver->pDispFrameBuffer[i] = 0xFFFF;
                        pTavor2DisplayDriver->pDispFrameBuffer[i+1] = 0xFFFF;
                        pTavor2DisplayDriver->pDispFrameBuffer[i+2] = 0xFFFF;
                    }
                    else
                        // White: R = 1; G = 1; B = 1 (all others are white)
                        pTavor2DisplayDriver->pDispFrameBuffer[i] = 0xFFFF;
            }
            //adjust position
            if (bpp == 3) 
                i = i + 3;
            else
                i++;
        }
    }
    else
    {
        // Default to "white" if frame buffer is not big enough
        for (i = 0; i < TAVORPV2_LCD_DEF_BPP* TAVORPV2_LCD_MAX_WIDTH* TAVORPV2_LCD_MAX_HEIGHT; i++)
        {
            // Default to white
            dispFrameBuffer[i] = 0xFF;
        }
    }
    return;
}


/* Initialize TPO DSI Smart Panel */
static BOOL tavorpv2_dcsProteusLcdInit(DispDrv *pDisplayDriver, DWORD dsiId, DWORD panelId)
{
    BYTE width,height;
    WORD colorDepth, regVal;
    BOOL rc = FALSE;
    lcdMode *pMode = NULL;
    
    if (dsiId >= TAVORPV2_DSI_CTL) 
    {
        WARN("tavorpv2_dcsSmartLcdInit: Only 2 DSI controllers are supported on PV2!");
        goto cleanUp;
    }    

    PRINT("tavorpv2_dcsSmartLcdInit: Initialize Proteus panel with DCS command!");

    pMode = pDisplayDriver->config.dsiConfig[dsiId].pMode;

    // Get the Required LCD Panel Color Depth
    colorDepth = pMode->colorDepth;
    
    //Force to be RGB24?
    if ((colorDepth != 24) && (colorDepth != 16))
    {
        PRINT("tavorpv2_dcsSmartLcdInit: TPO LCD panel only supports RGB24 and RGB16 format!");
        goto cleanUp;
    }
	

    // Only use 1 active lane
    tavorpv2_dsiCtlSetActiveLane(dsiId, 0x01);      

    // Force to LPDT mode
    tavorpv2_dsiCtlSetPowerMode(dsiId, DSI_POWERMODE_LPDT);      

    // Enable DSI controller if not enabled before sending BTA/Reset
    tavorpv2_dsiCtlEnable(dsiId, DSI_CTL_ENABLE);

    // Reset LCD panel and keeps low to 3 ms
    tavorpv2_lcdResx(2);

    // Reset Comand FIFO buffer
    tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     

    // Set NOP delay to a minimum value  for delay ms level purpose (1 cycle  = 3.2 ns for 312Mhz)
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_SET_DLY_MULT, 0x00, 0x00, 0x00);

    // Escape Entry code: LPDT
    // Step 1: Release MCS Protection
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_MCAP, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);
    // SELDL = 1
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_FMAIS, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x90, 0, 0);
    // Dummy Cycle
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_NULL_PACKET, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x05, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);

    // force to execut the command 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
	//tavorpv2_dsiSendCmdSeqWithDma(pDisplayDriver, dsiId);

    //RE_HwDelay(1*RE_MILLISECOND);

    // Escape Entry code RAR: 0110_0010 : 
    tavorpv2_dsiSetTrigCode(dsiId, DSI_TRIG_CODE_RESET);

    RE_HwDelay(2*RE_MILLISECOND);

	// Stop State (LP-11)

    // Reset Comand FIFO buffer
    tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     
	
    // Step 2: MCAP=2'h0
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_MCAP, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);

    // Step 3: Read device code to verify communication between processor and driver IC
    //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_READ_NO_PARAMETER, 0, 0);
    //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_READ_DEV_CODE, 0, 0);
    //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    // "Read Status" causes BTA
    //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_READ_STATUS, 0x00, 0, 0);
    //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x10, 0x00, 0x00);
   
    // Manufactuer command settings (Panel Setting)
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_LONG_WRITE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x07, 0, 0); // Length LS byte
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_POWER_SETTINGS1, 0, 0); // Set Frame 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x01, 0, 0); // DDVDH set up for freq
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x61, 0, 0); // VCL setup
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x01, 0, 0);  // factor bit1-0
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xA5, 0, 0);  // VGL level bit7-4
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x88, 0, 0);  // Dummy data
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // Dummy data

    // Power Settings 2
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_LONG_WRITE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x04, 0, 0); // Length LS byte
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_POWER_SETTINGS2, 0, 0); // Set Frame 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x0B, 0, 0); // VRGE level  - 4.7 ?
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xB3, 0, 0); // grayscale level 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x10, 0, 0);  // dummy data


    // Power Settings 3
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_LONG_WRITE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x03, 0, 0); // Length LS byte
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_POWER_SETTINGS3, 0, 0); // Set Frame 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x33, 0, 0); // current adjustment for LCD power circuit
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x03, 0, 0); // dummy data 

    // VCOM Settings 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_LONG_WRITE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x07, 0, 0); // Length LS byte
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_VCOM_SETTINGS, 0, 0); // Set Frame 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0); // DDVDH set up for freq
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0); // VCL setup
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x48, 0, 0);  // factor bit1-0
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // VGL level bit7-4
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x35, 0, 0);  // Dummy data
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // Dummy data

    // Panel Driving Setting 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_LONG_WRITE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x06, 0, 0); // Length LS byte
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_PANEL_DRIVING, 0, 0); // Set Frame 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0); // REV: bit4 - NW/NB, BGR: bit1 - RGB order, SS bit0 - Panel Mount Direction
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0); // HRE: bit 6-4, horizontal resolution 480 pixels
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xDB, 0, 0);  // Active lines
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x03, 0, 0);  // Back protch: bit 2-0
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // Dummy data

    // Set Display V-Timing
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_LONG_WRITE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x06, 0, 0); // Length LS byte
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_DISPLAY_VTIMING, 0, 0); // Set Frame 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x83, 0, 0); // BC: bit7 - frame/line inversion, front porch: bit2-0
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0); // Dummy para
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // Dummy para
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // Dummy data
	
    // Set Display H -Timing
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_LONG_WRITE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x12, 0, 0); // Length LS byte
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_DISPLAY_HTIMING, 0, 0); // Set Frame 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x93, 0, 0); // BC: bit7 - frame/line inversion, front porch: bit2-0
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0); // Dummy para
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // Dummy para
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x93, 0, 0);  // 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // Dummy data
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x8E, 0, 0);  // 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x04, 0, 0);  // Dummy data
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x12, 0, 0);  // 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x04, 0, 0);  // Dummy data
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // Dummy data
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // Dummy data
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // Dummy data
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);  // Dummy data
  

    // Set Display RGB Switch order
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_LONG_WRITE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x03, 0, 0); // Length LS byte
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_DISPLAY_RGB_ORRDER, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0); // Start column address (MSB)
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x99, 0, 0);

    // LTPS Interface Mode command ?

	// LTPS Interface mode ?
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_LONG_WRITE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x03, 0, 0); // Length LS byte
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xC4, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x4D, 0, 0); // Start column address (MSB)
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x83, 0, 0);

	// LTPS Interface control 1
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_LONG_WRITE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x05, 0, 0); // Length LS byte
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_LTPS_INTERFACE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x7F, 0, 0); // Start column address (MSB)
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x7E, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x20, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);
	
	// LTPS Interface control 2 ?
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_LONG_WRITE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x05, 0, 0); // Length LS byte
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_LTPS_INTERFACE+1, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0); // Start column address (MSB)
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x10, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);

    // Power On/Off control
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_GENERIC_LONG_WRITE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x03, 0, 0); // Length LS byte
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_POWER_ONOFF, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x52, 0, 0); 
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xFF, 0, 0);

    // Insert Interrupt proc for the last command
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
	//tavorpv2_dsiSendCmdSeqWithDma(pDisplayDriver, dsiId);

    RE_HwDelay(2*RE_MILLISECOND);

    tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     

	// User command
    // 480*440*24 
    // Set Maximum Return Packet Size : 2
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_SET_MAX_RETURN_PACKET_SIZE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x02, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);

    // DCS Long Write to set start/end of Column Address
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x05, 0, 0); // Length LS byte
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_COLUMN_ADDRESS, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0); // Start column address (MSB)
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    regVal = pMode->width - 1; 
    width = (BYTE)(( regVal & 0xFF00) >> 8);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x01, 0, 0); // End column address (MSB)
    width =(BYTE) (regVal & 0x00FF);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xDF, 0, 0); // LSB

    // DCS Long Write to set start/end Page Address
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_LONG_WRITE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x05, 0, 0); // Length LS byte
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_PAGE_ADDRESS, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0); // Start Page address (MSB)
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    regVal = pMode->height - 1; 
    height = (BYTE)(( regVal & 0xFF00) >> 8);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x01, 0, 0); // End Page address (MSB)
    height =(BYTE) (regVal & 0x00FF);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0xB7, 0, 0); // LSB

    // Set Address Mode
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_SHORT_WRITE_1_PARAMETER, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_ADDRESS_MODE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x00, 0, 0);

    // Set Pixel Format to match with Panel (RGB24 by default)
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_SHORT_WRITE_1_PARAMETER, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_PIXEL_FORMAT, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x07, 0, 0);

    // Dummy Cycle
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_NULL_PACKET, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x05, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);

    // Insert Interrupt proc for the last command
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
	//tavorpv2_dsiSendCmdSeqWithDma(pDisplayDriver, dsiId);

    // Switch to HS mode and send one frame of pixels
    //tavorpv2_dsiCtlSetPowerMode(dsiId, DSI_POWERMODE_HS_NORIHS);      
	//tavorpv2_dispCopyToLcd(pDisplayDriver,(DWORD ) (&dispFrameBuffer[0]),0x01B8,0x01E0,0,0);

    // Switch back to LPDT mode
    //tavorpv2_dsiCtlSetPowerMode(dsiId, DSI_POWERMODE_LPDT);      
    RE_HwDelay(2*RE_MILLISECOND);
    
    // Reset Comand FIFO buffer
    tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     

	//Step 3: Turn On Peripheral DSI command
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_SHORT_WRITE_NO_PARAMETER, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_SET_DISPLAY_ON, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);

    //Force "Sleep Out"
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_SHORT_WRITE_NO_PARAMETER, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_EXIT_SLEEP_MODE, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);

    // Dummy Cycle
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_NULL_PACKET, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0x05, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);

    // Insert Interrupt proc for the last command
    tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 
	//tavorpv2_dsiSendCmdSeqWithDma(pDisplayDriver, dsiId);

    RE_HwDelay(120*RE_MILLISECOND);

    // Return to DSI_POWERMODE_HS_NORIHS  mode
    tavorpv2_dsiCtlSetPowerMode(dsiId, DSI_POWERMODE_HS_NORIHS);      

    // Reset Comand FIFO buffer
    //tavorpv2_dsiResetCmdFifoBuf(pDisplayDriver, dsiId);     

    // Step 3: Read device code to verify communication between processor and driver IC
    //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DSI_DATA_DCS_READ_NO_PARAMETER, 0, 0);
    //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, DCS_CMD_READ_DEV_CODE, 0, 0);
    //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_COMMAND_WRITE, 0, 0, 0);
    // "Read Status" causes BTA
    //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_READ_STATUS, 0x00, 0, 0);
    //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_NO_OPERATION, 0x10, 0x00, 0x00);

    // Insert Interrupt proc for the last command
    //tavorpv2_dsiAddCmd2Buffer(pDisplayDriver, dsiId,  LCD_CMD_INTERRUPT_THE_PROCESSOR, 0x42, 0x00, 0x00); 

    // Initialization Finisehd
    rc = TRUE;
cleanUp:
    return rc;
}

