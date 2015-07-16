/*****************************************************************************
 * Filename:    camif_tavorpv2.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Mar 9th, 2009
 *
 * Description: This file implements the HW specific code for camera driver on PV2.
 *
 *****************************************************************************/
/* include header files */
#include "string.h"
#include "rei.h"
#include "camera.h"
#include "camif_internal.h"
#include "camif_tavorpv2.h"
#include "camif_i2c.h"

/* Nessus header */
#ifdef NESSUS
#include "ldo.h"
#include "cache.h"
#endif

#define SRCGROUP GROUP_PLT_DRV
#define SRCFILE  FILE_CAM_DEV

/* PV2 PAD configuration */
#ifdef PLATFORM_RIMSPRUCEWOOD
/* Sprucewood form factor*/
#define     CAM_EN          GPIO_101            /* ISP PDN */
#define     CAM_RST_N       GPIO_102            /* ISP RST */
#define     CAM_MCLK        GPIO_84             /* ISP SYSCLK */  /* Note B0 will use different GPIO */
#define     CAM_PWR_EN      GPIO_103            /* Sensor PWR_EN */
#define     CAM_LDO_EN      GPIO_100            /* ISP LDO_EN */
#define     LDO_MIPI        2
#define     LDO_CAMERA      3

/* ISP SYSCLK is connected to CAM_MCLK (GPIO84)*/
static const PadConfiguration camMclk = {  
    6,                          // altFunctionMode: CI_MCLK
    PAD_DIRECTION_OUTPUT,       // initialDirection
    FALSE,                      // initialLevel
    FALSE,                      // pullUpEnable
    FALSE,                      // pullDownEnable
    PAD_INTERRUPT_NONE,         // interruptType
    NULL,                       // interruptHandler
    FALSE,                      // interruptsInitiallyEnabled
    FALSE                       // interruptDebounceEnabled
};

#else
/* Caprock rev2 big board */
#define     CAM_EN          GPIO_44             /* ISP PDN */
#define     CAM_RST_N       GPIO_45             /* ISP RST */
#define     CAM_MCLK        GPIO_46             /* ISP SYSCLK */
#define     CAM_PWR_EN      GPIO_75             /* Sensor PWR_EN */
#define     CAM_LDO_EN      GPIO_76             /* ISP LDO_EN */
#define     LDO_MIPI        3
#define     LDO_CAMERA      1

/* ISP SYSCLK is connected to CAM_MCLK (GPIO46)*/
static const PadConfiguration camMclk = {  
    2,                          // altFunctionMode: MN_CLK
    PAD_DIRECTION_OUTPUT,       // initialDirection
    FALSE,                      // initialLevel
    FALSE,                      // pullUpEnable
    FALSE,                      // pullDownEnable
    PAD_INTERRUPT_NONE,         // interruptType
    NULL,                       // interruptHandler
    FALSE,                      // interruptsInitiallyEnabled
    FALSE                       // interruptDebounceEnabled
};

#endif

static const PadConfiguration camGpioOutput = {
    0,                          // altFunctionMode: GPIO
    PAD_DIRECTION_OUTPUT,       // initialDirection
    FALSE,                      // initialLevel
    FALSE,                      // pullUpEnable
    FALSE,                      // pullDownEnable
    PAD_INTERRUPT_NONE,         // interruptType
    NULL,                       // interruptHandler
    FALSE,                      // interruptsInitiallyEnabled
    FALSE                       // interruptDebounceEnabled
};

static RE_MESSAGE camMsg;

/* DMA descriptor array */
 static __align(16) CamDmaDescriptor dmaDescPtr[2][3];
    
/* private function forward declaration */
static void isrSci0(void);
static void isrSci1(void);
static void isrCsi(void);

FLOAT STV9xxF900Conversion(BYTE upper, BYTE lower);

// external LDO functions
extern BqHugeLdoVoltage(DWORD i, DWORD j);
extern BqHugeLdoEnable(DWORD i);
extern BqHugeLdoDisable(DWORD i);


/* ===========================================================================
 * Generic camera driver API
 * ===========================================================================*/
/*****************************************************************************
 * Function:    CamifEnableInterleaving 
 * Description: Set interleaving mode
 * Parameters:  
 * Return:
 *
 *****************************************************************************/
void CamifEnableInterleaving(CamInterleavingMode mode)
{
    camConfig.interleavingMode = mode;
    
    /* Interleaving mode requires two capture interface controllers */
    if (mode == CAM_INTERLEAVING_DISABLED)
    {
        camConfig.sciIds = CAM_SCI_0;
    }    
    else        
    {
        camConfig.sciIds = CAM_SCI_0 | CAM_SCI_1;
    }
}

/*****************************************************************************
 * Function:    CamifMovieStart 
 * Description: Start Movie
 * Parameters:  
 * Return:
 *
 *****************************************************************************/
void CamifStartMovie(CamPxlFmt fmt) 
{
    DWORD memAddr[3];
    
    /* Set pixel format */
    if(camConfig.sciIds & CAM_SCI_0)
        sciConfig(0).pxlFmtInput = sciConfig(0).pxlFmtOutput = fmt;
    
    /* Video interleaving: D1: YUV (Video), D2: RGB565 (VF) */
    if(camConfig.sciIds & CAM_SCI_1 && camConfig.interleavingMode == CAM_INTERLEAVING_VIDEO_VF)
        sciConfig(1).pxlFmtInput = sciConfig(1).pxlFmtOutput = CAM_PIXEL_FMT_RGB_565;
        
    /* 2. Enable SCI */
    CamifEnable();
    
    /* 3.a Setup DMA and FIFO for SCI0*/
    if(fmt == CAM_PIXEL_FMT_RGB_565 | fmt == CAM_PIXEL_FMT_RGB_666 | fmt == CAM_PIXEL_FMT_RGB_888)
    {
        memAddr[0] = (DWORD) camifFrameBuffer;
        CamifDmaSetup(0, memAddr, CAM_FRM_SIZE(2));
    }
    else if(fmt == CAM_PIXEL_FMT_YCBCR_422)
    {
        memAddr[0] = (DWORD) camifFrameBuffer;
        memAddr[1] = (DWORD) (camifFrameBuffer + CAM_FRM_SIZE(1));
        memAddr[2] = (DWORD) (camifFrameBuffer + CAM_FRM_SIZE(1)*2);
        CamifDmaSetup(0, memAddr, CAM_FRM_SIZE(1));
    }
    
    /* 3.b Setup DMA and FIFO for SCI1 in interleaving mode*/
    if(camConfig.sciIds & CAM_SCI_1 && camConfig.interleavingMode == CAM_INTERLEAVING_VIDEO_VF)
    {
        memAddr[0] = (DWORD) camifFrameBufferVF;
        CamifDmaSetup(1, memAddr, CAM_FRM_SIZE_VF);
    }
    
    /* 4. Start ISP streaming */
    CamDrv.IspSvpFunc(SVPCF_SETUP_MOVIE);
    camStatus.ispMode = CAM_ISP_MOVIE;    
}

/*****************************************************************************
 * Function:    CamifMovieStart 
 * Description: Start Movie
 * Parameters:  
 * Return:
 *
 *****************************************************************************/
void CamifStartPicture(void) 
{
    DWORD memAddr[3];
    DWORD length = camConfig.resolution.reslX * camConfig.resolution.reslY * 2; 

    /* Set pixel format */
    if(camConfig.sciIds & CAM_SCI_0)
        sciConfig(0).pxlFmtInput = sciConfig(0).pxlFmtOutput = CAM_PIXEL_FMT_JPEG;
    
    /* JPEG interleaving: D1: JPEG, D2: RGB565 (VF) */
    if(camConfig.sciIds & CAM_SCI_1 && camConfig.interleavingMode == CAM_INTERLEAVING_VIDEO_VF)
        sciConfig(1).pxlFmtInput = sciConfig(1).pxlFmtOutput = CAM_PIXEL_FMT_RGB_565;
        
    /* 2. Enable SCI */
    CamifEnable();
    
    /* 3.a Setup DMA and FIFO for SCI0*/
    memAddr[0] = (DWORD) camifFrameBuffer;
    CamifDmaSetup(0, memAddr, length);
    
    /* 3.b Setup DMA and FIFO for SCI1 in interleaving mode*/
    if(camConfig.sciIds & CAM_SCI_1 && camConfig.interleavingMode == CAM_INTERLEAVING_VIDEO_VF)
    {
        memAddr[0] = (DWORD) camifFrameBufferVF;
        CamifDmaSetup(1, memAddr, CAM_FRM_SIZE_VF);
    }    
    
    /* 4. Start ISP streaming */
    camStatus.ispMode = CAM_ISP_PICTURE;
    CamDrv.IspSvpFunc(SVPCF_TAKE_PICTURE);
    camStatus.ispMode = CAM_ISP_IDLE;
}

/*****************************************************************************
 * Function:    CamifViewFinderStart 
 * Description: Start viewfinder
 * Parameters:  
 * Return:
 *
 *****************************************************************************/
void CamifStartViewfinder(void) 
{
    DWORD memAddr[3];

    /* 1. Configure the camera driver */    
    camConfig.sciIds = CAM_SCI_0;
    
    sciConfig(0).pxlFmtInput = CAM_PIXEL_FMT_RGB_565;
    sciConfig(0).pxlFmtOutput = CAM_PIXEL_FMT_RGB_565;

    /* 2. Enable SCI */
    CamifEnable();
    
    /* 3. Setup DMA and FIFO */
    memAddr[0] = (DWORD) camifFrameBufferVF;
    CamifDmaSetup(0, memAddr, CAM_FRM_SIZE_VF);

    CamDrv.IspSvpFunc(SVPCF_SETUP_VF);
    camStatus.ispMode = CAM_ISP_VIEWFINDER;   
}

/*****************************************************************************
 * Function:    CamifStreamStop 
 * Description: Stop viewfinder
 * Parameters:  
 * Return:
 *
 *****************************************************************************/
void CamifStreamStop(void) 
{
    CamDrv.IspSvpFunc(SVPCF_STOP_STREAM);
    camStatus.ispMode = CAM_ISP_IDLE;       

    CamifDmaStop(0);
    CamifDisable(CAM_DISABLE_NORMAL);    
}

/* ===========================================================================
 * TAVORPV2 implementation
 * ===========================================================================*/
/*****************************************************************************
 * Function:    CamDevInit 
 * Description: Camera controller(CSI and SCI) nitialization
 * Parameters:
 * Return:
  *****************************************************************************/
DWORD CamIfInit( void ) {       
    /* Initialize the CSI status register pointer */
    camStatus.csiStatusRegPtr = (IODWORD *) CSI_CS0SR;
    
    /* Interrupt: Register CSI interrupt handlers*/    
    RegisterIntHandler( HWIRQ_CI0, isrSci0);
    RegisterIntHandler( HWIRQ_CI1, isrSci1);
    RegisterIntHandler( HWIRQ_CSI, isrCsi);

    /* Clock: D0CKEN */
    WriteRegDwordOr(D0CKEN_A, D0CKENA_CI);
    WriteRegDwordOr(D0CKEN_B, (1u << 11));
    WriteRegDwordOr(D0CKEN_C, 0x0C080000);

    /* Set HAVU frequency to 156MHz */
    WriteRegDword(ACCR_REG,   (ReadRegDword(ACCR_REG) & ~0x30000000) | 0x10000000);
    
    /* Enable MIPI */
    WriteRegDwordOr(0x42404078, 0x500u);        
    
    WriteRegDwordOr(CSI_RESERVED, 1u); 
    
    /* GPIO: Set Pad configuration */
    PadConfigure( CAM_EN,      &camGpioOutput,  NULL );
    PadConfigure( CAM_MCLK,    &camMclk,  NULL );
    PadConfigure( CAM_RST_N,   &camGpioOutput,  NULL );    
    PadConfigure( CAM_LDO_EN,  &camGpioOutput,  NULL );
    PadConfigure( CAM_PWR_EN,  &camGpioOutput,  NULL );
    
    /* Set LDO3 voltage. */
    BqHugeLdoVoltage(LDO_MIPI, 1250);
    BqHugeLdoEnable(LDO_MIPI);    
    
    /* Enable I2C */
    Hs2I2cInit(); 
        
    return 0;
}

/*****************************************************************************
 * Function:    devCamEnable 
 * Description: Enable CSI and SCI controllers
 * Parameters:  
 * Return:
 *
 *****************************************************************************/
BOOL CamifEnable(void) {
    DWORD sciBase;
        
    if(camStatus.drvEnabled)
    {
        WARN("SCI already enabled");
        return FALSE;
    }
    
    /* Set default interrupt mask */
    sciConfig(0).intMaskSci = sciConfig(1).intMaskSci =  0;
    
    /* Default VS mapping */
    csiConfig.vcMap[0]   = 0;
    csiConfig.vcMap[1]   = 0;  /* We're using datatype interleaving, not using virtual channel */
    csiConfig.vcMap[2]   = 2;
    csiConfig.vcMap[3]   = 3;
    
    /* Set default interrupt mask */
    csiConfig.intEnbCsi = (CSI_CS0INEN_TO | CSI_CS0INEN_GENPKT | CSI_CS0INEN_PHY_ERR |
                            CSI_CS0INEN_OVERFLOW | CSI_CS0INEN_EOF | CSI_CS0INEN_SOF | CSI_CS0INEN_PROT);

    /* Set up CSI data rate */
    if(csiConfig.DataRate == CAM_CSI_DATA_RATE_200M)
    {
        /* CSI default timer values - 100Mhz*/
        csiConfig.HsSettle   = 0x0E;   
        csiConfig.HsRxTo     = 0xFFFF;
    
        csiConfig.HsTermen   = 0x04;
        csiConfig.ClMiss     = 0x00;
        csiConfig.ClSettle   = 0x0C;
        csiConfig.ClTermen   = 0x00;
    }
    else if(csiConfig.DataRate == CAM_CSI_DATA_RATE_480M)
    {
        /* CSI default timer values - 240Mhz*/
        csiConfig.HsSettle   = 0x1E;   
        csiConfig.HsRxTo     = 0xFFFF;
    
        csiConfig.HsTermen   = 0x09;
        csiConfig.ClMiss     = 0x00;
        csiConfig.ClSettle   = 0x0C;
        csiConfig.ClTermen   = 0x00;
    }
    else if(csiConfig.DataRate == CAM_CSI_DATA_RATE_600M)
    {
        /* CSI default timer values - 240Mhz*/
        csiConfig.HsSettle   = 0x24;   
        csiConfig.HsRxTo     = 0xFFFF;
    
        csiConfig.HsTermen   = 0x0B;
        csiConfig.ClMiss     = 0x00;
        csiConfig.ClSettle   = 0x0C;
        csiConfig.ClTermen   = 0x00;
    }
    else if(csiConfig.DataRate == CAM_CSI_DATA_RATE_700M)
    {
        /* CSI default timer values - 240Mhz*/
        csiConfig.HsSettle   = 0x2A;   
        csiConfig.HsRxTo     = 0xFFFF;
    
        csiConfig.HsTermen   = 0x0D;
        csiConfig.ClMiss     = 0x00;
        csiConfig.ClSettle   = 0x0C;
        csiConfig.ClTermen   = 0x00;
    }
    else if(csiConfig.DataRate == CAM_CSI_DATA_RATE_800M)
    {
        /* CSI default timer values - 400Mhz */
        csiConfig.HsSettle   = 0x30;   
        csiConfig.HsRxTo     = 0xFFFF;
    
        csiConfig.HsTermen   = 0x0F;
        csiConfig.ClMiss     = 0x00;
        csiConfig.ClSettle   = 0x0C;
        csiConfig.ClTermen   = 0x00;
    }
    
    /* 1. SCI initialization */
    if(camConfig.sciIds & CAM_SCI_0)
    {   
        PRINT("Enabling CI-0");
        sciBase = SCI0_REGBASE;
        
        /* Set interrupt mask */
        WriteRegDword(sciBase + SCI_OFF_SCIMASK, sciConfig(CAM_SCI0_ID).intMaskSci);

        /* Input and output formats */        
        WriteRegDword(sciBase + SCI_OFF_SCICR1, 
                        SCI_SCICR1_INPUT_FMT(sciConfig(CAM_SCI0_ID).pxlFmtInput) |
                        SCI_SCICR1_OUTPUT_FMT(sciConfig(CAM_SCI0_ID).pxlFmtOutput));

        /* Enable the controller */
        WriteRegDwordOr(sciBase + SCI_OFF_SCICR0, SCI_SCICR0_CI_EN);
    }
    
    if(camConfig.sciIds & CAM_SCI_1)
    {    
        PRINT("Enabling CI-1");
        sciBase = SCI1_REGBASE;

        WriteRegDword(sciBase + SCI_OFF_SCIMASK, sciConfig(CAM_SCI1_ID).intMaskSci);

        /* Input and output formats */        
        WriteRegDword(sciBase + SCI_OFF_SCICR1,
                        SCI_SCICR1_INPUT_FMT(sciConfig(CAM_SCI1_ID).pxlFmtInput) |
                        SCI_SCICR1_OUTPUT_FMT(sciConfig(CAM_SCI1_ID).pxlFmtOutput));

        /* Enable the controller */
        WriteRegDwordOr(sciBase + SCI_OFF_SCICR0, SCI_SCICR0_CI_EN);        
    }       
    
    /* 2. CSI initialization */  
    WriteRegDwordOr(CSI_RESERVED, 1);
    
    /* INT Mask*/    
    WriteRegDword(CSI_CS0INEN, csiConfig.intEnbCsi);   
    WriteRegDword(CSI_CS0TIM0, CSI_CS0TIM0_HSTERMEN_SET(csiConfig.HsTermen) | 
                               CSI_CS0TIM0_CLMISS_SET(csiConfig.ClMiss) |
                               CSI_CS0TIM0_CLSETTLE_SET(csiConfig.ClSettle) |
                               CSI_CS0TIM0_CLTERMEN(csiConfig.ClTermen)); 
   
    WriteRegDword(CSI_CS0TIM1, CSI_CS0TIM1_HSTSETTLE_SET(csiConfig.HsSettle) | 
                               CSI_CS0TIM1_HSRXTO(csiConfig.HsRxTo));
        
    /* Need to configure the VC mapping? */
    /* Set up the CSI lane number and enable CSI controller */
    WriteRegDword(CSI_CS0CR, CSI_LANE_NUM_SET(csiConfig.CsiLaneNum - 1) |
                             CSI_VC0_CFG(csiConfig.vcMap[0]) |
                             CSI_VC1_CFG(csiConfig.vcMap[1]) |
                             CSI_VC2_CFG(csiConfig.vcMap[2]) |
                             CSI_VC3_CFG(csiConfig.vcMap[3]) |
                             CSI_CS0CR_EN);

    /* Enable interrupts */
    EnableInterrupt( HWIRQ_CSI );
    if(camConfig.sciIds & CAM_SCI_0)
        EnableInterrupt( HWIRQ_CI0 );        
    if(camConfig.sciIds & CAM_SCI_1)
        EnableInterrupt( HWIRQ_CI1 );

    camStatus.drvEnabled = TRUE;
    camStatus.errFlag = 0;
     
    return TRUE;
}

/*****************************************************************************
 * Function:    devCamDisable 
 * Description: Disable CSI and SCI controllers
 * Parameters:
 * Return:
 *
 *****************************************************************************/
BOOL CamifDisable(CamDisableMode mode) {   
    DWORD sciBase;
    
    if(camStatus.drvEnabled == FALSE)
    {
        WARN("SCI already disabled");
        return FALSE;
    }
    
    /* Disbale CSI */
    WriteRegDwordAnd(CSI_CS0CR, ~CSI_CS0CR_EN);    

    if(camConfig.sciIds & CAM_SCI_0) 
    {
        sciBase = SCI0_REGBASE;
        
        if(mode == CAM_DISABLE_NORMAL)
        {
            /* Shut down SCI */            
            WriteRegDwordAnd(sciBase + SCI_OFF_SCICR0, ~SCI_SCICR0_CAP_EN);
            //return TRUE;
        }
        
        /* Disabled SCI controller */            
        WriteRegDwordAnd(sciBase + SCI_OFF_SCICR0, ~SCI_SCICR0_CI_EN);
    }
    
    if(camConfig.sciIds & CAM_SCI_1)
    {
        sciBase = SCI1_REGBASE;

        if(mode == CAM_DISABLE_NORMAL) 
        {
            /* Shut down SCI */
            WriteRegDwordAnd(sciBase + SCI_OFF_SCICR0, ~SCI_SCICR0_CAP_EN);
        }
        /* Disabled SCI controller */
        WriteRegDwordAnd(sciBase + SCI_OFF_SCICR0, ~SCI_SCICR0_CI_EN);   
    }
    
    DisableInterrupt( HWIRQ_CSI );    
    DisableInterrupt( HWIRQ_CI0 );
    DisableInterrupt( HWIRQ_CI1 );
    
    camStatus.drvEnabled = FALSE;
        
    return TRUE;
}

/*****************************************************************************
 * Function:    CamifDmaSetup 
 * Description: CSI interrupt service routine
 * Parameters:  ciId: capture ID: 0, 1
 *              memAddr: target memory address. 
 *                       memAddr[0] for the RGB format or Y channel
 *                       memAddr[1] for the Cb channel
 *                       memAddr[2] for the Cr channel
 *              length: expected data length
 * Return:
 *
 *****************************************************************************/
int CamifDmaSetup(int ciId, DWORD memAddr[3], DWORD length) 
{
    CamDmaDescriptor * despPtr;
    IODWORD dmaReg, test;
    DWORD sciBase;
    
    /* Setup DMA */
    if(memAddr[0] == 0)
    {
        PRINT("Invalid Y/TGB memory pointer\n");
        return -1;
    }    
    
    sciBase = (ciId == CAM_SCI0_ID)? SCI0_REGBASE : SCI1_REGBASE;
    
    /* **** Y/RGB channel **** */
    /* 1. Get DMA Desp address */
    despPtr = &(dmaDescPtr[ciId][0]);
    
    /* Initialize DMA Despriptor */
    despPtr->descriptor = (DWORD)despPtr;                       
    despPtr->source     = 0;  /* Not used */
    despPtr->target     = SCI_DMA_ADDR((DWORD)memAddr[0]);
    despPtr->command    = SCI_DMA_LEN((DWORD)length);
    
    CacheExecute(DATA_CACHE, CACHE_CLEAN, (DWORD) despPtr, sizeof(CamDmaDescriptor));
        
    /* Load DMA Desp */
    WriteRegDword(sciBase + SCI_OFF_SCIDADDR0, (DWORD)despPtr | 1);  

    /* Enable DMA Channel */
    WriteRegDwordOr(sciBase + SCI_OFF_SCIDCSR0, SCI_SCIDCSR_RUN |SCI_SCIDCSR_BTEM);
   
    /* 3. Enable FIFO 0 */    
    WriteRegDwordOr(sciBase + SCI_OFF_SCIFIFO, 
                  SCI_SCIFIFO_FIFO0 | SCI_SCIFIFO_TFS(sciConfig(ciId).dmTfs)); 

    if(sciConfig(ciId).pxlFmtOutput == CAM_PIXEL_FMT_YCBCR_422)
    {
        /* The YCbCr uses planar memory format */
        if(memAddr[1] == NULL || memAddr[2] == NULL)
        {
            PRINT("Invalid CbCr memory pointer\n");
            return -1;
        }
            
        /* **** Cb channel **** */
        despPtr++;
    
        /* Initialize DMA Despriptor */        
        despPtr->descriptor = (DWORD)despPtr;                       /* Init DMA Desp */
        despPtr->source     = 0;  /* Not used */
        despPtr->target     = SCI_DMA_ADDR((DWORD)memAddr[1]);
        despPtr->command    = SCI_DMA_LEN((DWORD)length); 
        CacheExecute(DATA_CACHE, CACHE_CLEAN_AND_INVALIDATE, (DWORD) despPtr, sizeof(CamDmaDescriptor));
        
        /* Load DMA Desp */
        WriteRegDword(sciBase + SCI_OFF_SCIDADDR1, (DWORD)despPtr  | 1);          
        
        /* Enable DMA Channel */        
        WriteRegDwordOr(sciBase + SCI_OFF_SCIDCSR1, SCI_SCIDCSR_RUN | SCI_SCIDCSR_BTEM);

        /* Enable FIFO 1 */
        WriteRegDwordOr(sciBase + SCI_OFF_SCIFIFO, 
                        SCI_SCIFIFO_FIFO1 | SCI_SCIFIFO_TFS(sciConfig(ciId).dmTfs) );
        
        /* **** Cr channel **** */
        despPtr++;
        despPtr->descriptor = (DWORD)despPtr;                       /* Init DMA Desp */
        despPtr->source     = 0;  /* Not used */
        despPtr->target     = SCI_DMA_ADDR((DWORD)memAddr[2]);
        despPtr->command    = SCI_DMA_LEN((DWORD)length); 
        CacheExecute(DATA_CACHE, CACHE_CLEAN_AND_INVALIDATE, (DWORD) despPtr, sizeof(CamDmaDescriptor));
    
        /* Load DMA Desp */        
        WriteRegDword(sciBase + SCI_OFF_SCIDADDR2, (DWORD)despPtr | 1);  
        
        /* Enable DMA Channel */        
        WriteRegDwordOr(sciBase + SCI_OFF_SCIDCSR2, SCI_SCIDCSR_RUN | SCI_SCIDCSR_BTEM);

        /* Enable FIFO 2 */
        WriteRegDwordOr(sciBase + SCI_OFF_SCIFIFO,
                        SCI_SCIFIFO_FIFO2 | SCI_SCIFIFO_TFS(sciConfig(ciId).dmTfs) ); 
    }
                    
    /* 4. Enable capture */
    WriteRegDwordOr(sciBase + SCI_OFF_SCICR0, SCI_SCICR0_CAP_EN | SCI_SCICR0_CI_EN);
    
    return 0;    
}

/*****************************************************************************
 * Function:    drvCamStart 
 * Description: CSI interrupt service routine
 * Parameters:  ciId: capture ID: 0, 1
 *****************************************************************************/
int CamifDmaStop(int ciId)
{
    DWORD sciBase;

    sciBase = (ciId == CAM_SCI0_ID)? SCI0_REGBASE : SCI1_REGBASE;
    
    /* Disable capture */
    WriteRegDwordAnd(sciBase + SCI_OFF_SCICR0, ~(SCI_SCICR0_CI_EN | SCI_SCICR0_CAP_EN));
    
    /* Disbale FIFOs */
    WriteRegDwordAnd(sciBase + SCI_OFF_SCIFIFO,
            ~(SCI_SCIFIFO_FIFO0 | SCI_SCIFIFO_FIFO1 | SCI_SCIFIFO_FIFO2)); /* Disable FIFOs */
      
    return 0;
}

/* ===========================================================================
 * Internal ISR functions
 * ===========================================================================*/
/*****************************************************************************
 * Function:    devCamIsrCsi 
 * Description: 
 * Parameters:
 * Return:
 *
 *****************************************************************************/
static void isrCsi(void)
{
    IODWORD regValue = ReadRegDword(CSI_CS0INST);
            
    /* Clear the interrupt events */
    WriteRegDword(CSI_CS0INST, regValue);

    /* Protection interrupt*/
    if(csiConfig.intEnbCsi & CSI_CS0INEN_PROT)
    {    
        /* CRC Error */
        if(regValue & CSI_CS0INST_CRC_ERR)          
        {
            csiStatis.csiErrCrc++;
            if(camStatus.errFlag >=0) camStatus.errFlag |= CSI_CS0INST_CRC_ERR;
        }
        
        /* 2-bit Error */
        if(regValue & CSI_CS0INST_2BIT_ERR)
        {
            csiStatis.csiErr2Bit++;
            if(camStatus.errFlag >=0) camStatus.errFlag |= CSI_CS0INST_2BIT_ERR;
        }
            
        /* 1-bit Error */
        if(regValue & CSI_CS0INST_1BIT_ERR)         
        {
            csiStatis.csiErr1Bit++;
            if(camStatus.errFlag >=0) camStatus.errFlag |= CSI_CS0INST_1BIT_ERR;
        }            
    }
    
    /* Timeout interrupt */
    if(csiConfig.intEnbCsi & CSI_CS0INEN_TO)    
    {
        if(regValue & CSI_CS0INST_HSRX_TO_STS3)     csiStatis.csiHsRxTo3++;
        if(regValue & CSI_CS0INST_HSRX_TO_STS2)     csiStatis.csiHsRxTo2++;
        if(regValue & CSI_CS0INST_HSRX_TO_STS1)     csiStatis.csiHsRxTo1++;
        if(regValue & CSI_CS0INST_HSRX_TO_STS0)     csiStatis.csiHsRxTo0++;
    }
         
    /* DPHY error interrupt */    
    if(csiConfig.intEnbCsi & CSI_CS0INEN_PHY_ERR)
    {        
        /* Control Error */
        if(regValue & CSI_CS0INST_CTRL_ERR)         
        {
            csiStatis.csiErrCtrl++;    
            if(camStatus.errFlag >=0) camStatus.errFlag |= CSI_CS0INST_CTRL_ERR;
        }
            
        /* SOT Sync Error */
        if(regValue & CSI_CS0INST_SOT_SYNC_ERR)     
        {
            csiStatis.csiErrSoTSync++;
            if(camStatus.errFlag >=0) camStatus.errFlag |= CSI_CS0INST_SOT_SYNC_ERR;
        }
            
        /* SOT Error */
        if(regValue & CSI_CS0INST_SOT_ERR)          
        {
            csiStatis.csiErrSoT++;
            if(camStatus.errFlag >=0) camStatus.errFlag |= CSI_CS0INST_SOT_ERR;
        }            
    }

    /* CSI overflow interrupt */
    if(csiConfig.intEnbCsi & CSI_CS0INEN_OVERFLOW)
    {
        /* Byte to Pixel Overflow */
        if(regValue & CSI_CS0INST_BP_OVERFLOW)      csiStatis.csiErrBPOverflow++;
        /* Lane Management Overflow */
        if(regValue & CSI_CS0INST_LM_OVERFLOW)      csiStatis.csiErrLMOverflow++;
    }
    
    /* EOF interrupt */
    if((csiConfig.intEnbCsi & CSI_CS0INEN_EOF) && (regValue & CSI_CS0INST_EOF))
        csiStatis.csiEof++;

    /* SOF interrupt */
    if((csiConfig.intEnbCsi & CSI_CS0INST_SOF) && (regValue & CSI_CS0INST_SOF))
        csiStatis.csiSof++;

    /* Pending error report, generate CSI error message */
    if(csiConfig.errControl == CAM_ERR_STOP && camStatus.errFlag > 0)
    {
        WARNN("CAM_MSG_CSI_ERR - 0x%x", camStatus.errFlag);
        
        camMsg.Event   = (DWORD) PLATFORM_MSG_CAM_DRV;
        camMsg.SubMsg  = (DWORD) CAM_MSG_CSI_ERR;
        RE_RimPostMessage( PLATFORM_THREAD_ID_APP0, &camMsg );               

        camStatus.errFlag = -1;
    }  
}


/*****************************************************************************
 * Function:    isrSciCommon 
 * Description: 
 * Parameters:
 * Return:
 *
 *****************************************************************************/
static isrSciCommon(int ciId)
{
    IODWORD regValue, regValue2;
    DWORD sciBase;
    SciStatisType * sciStatPtr;

    sciStatPtr = &sciStatis(ciId);
    sciBase = (ciId == CAM_SCI0_ID)? SCI0_REGBASE : SCI1_REGBASE;
    
    /* Read SCI interrupt status */
    regValue = ReadRegDword(sciBase + SCI_OFF_SCISR);
    
    /* Clear the interrupt status */
    WriteRegDwordOr(sciBase + SCI_OFF_SCISR, regValue);
    
    /* Skip masked interrupts */
    regValue &= ~(sciConfig(0).intMaskSci);
    
    /* End of frame interrupt */
    if(regValue & SCI_SCISR_EOFX) 
    {
        sciStatPtr->sciEofx++;        
    
        /* Notify thread about a new frame */      
        camMsg.Event   = (DWORD) PLATFORM_MSG_CAM_DRV;
        camMsg.SubMsg  = (DWORD) CAM_MSG_NEW_FRAME;
        RE_RimPostMessage( PLATFORM_THREAD_ID_DRV, &camMsg );            
    }

    /* Update the statistics */
    if(regValue & SCI_SCISR_EOF )          sciStatPtr->sciEof++;    
    if(regValue & SCI_SCISR_SOF)            sciStatPtr->sciSof++;
    if(regValue & SCI_SCISR_SOFX)           sciStatPtr->sciSofx++;    
    if(regValue & SCI_SCISR_DES)            sciStatPtr->sciDes++;    
    if(regValue & SCI_SCISR_OFO )           sciStatPtr->sciOfo++;
    if(regValue & SCI_SCISR_SINT )          sciStatPtr->sciSint++;        
}

static void isrSci0(void)
{
    DisableInterrupt( HWIRQ_CI0 );
    isrSciCommon(CAM_SCI0_ID);
    EnableInterrupt( HWIRQ_CI0 );
}

static void isrSci1(void)
{
    isrSciCommon(CAM_SCI1_ID);
}

/* ===========================================================================
 * ISP Control functions
 * ===========================================================================*/
/*****************************************************************************
 * Function:    drvIspPowerOn 
 * Description: 
 * Parameters:
 * Return:
 *
 *****************************************************************************/
BOOL CamifIspPowerOn( void ) {   
    DWORD start;
    TIME_START(start);
    
    /* ISP power up sequence,. see ISP spec 4.11 */    
    /* 1. ENable LDO. ISP is connected to VDD */
    PadSet(CAM_LDO_EN, PAD_OUTPUT);
    RimSleep(5);    

    /* The default LDO1 voltage is 2.5; change it to 1.8V. */
    BqHugeLdoVoltage(LDO_CAMERA, 1800);
    BqHugeLdoEnable(LDO_CAMERA);   
    RimSleep(5);    
        
    PadSet(CAM_PWR_EN, PAD_OUTPUT);
    RimSleep(1);
            
    /* 2. Start SYSCLK (Enable MCLK)*/   
#ifdef PLATFORM_RIMSPRUCEWOOD    
    WriteRegDword(CSI_CSGCR, CSI_CSGCR_EN(1) | CSI_CSGCR_DIV(CSI_CSGCR_DIV_VALUE));
#else
    WriteRegDword(PMNR, PMNR_EN | PMNR_M(1) | PMNR_N(1));
#endif    
    RimSleep(10);
    /* 3. Set PDN high */
    PadSet(CAM_EN, PAD_OUTPUT);
    RimSleep(1);  /* Minimum 100us */

    /* 4. Reset ISP */
    PadSet(CAM_RST_N, PAD_OUTPUT);
    RimSleep(1);  /* Minimum 100us */

    /* Reset ISP and load patch */
    CamDrv.IspReset();
    CamDrv.IspLoadPatch();

    CamDrv.IspSvpFunc(SVPCF_POWER_ON);
    camStatus.ispMode = CAM_ISP_IDLE;

    PRINTN("devCamIspPowerOn function durition: %dms", TIME_ELAPSE(start));

    return TRUE;
}

/*****************************************************************************
 * Function:    drvIspPowerDown 
 * Description: 
 * Parameters:
 * Return:
 *
 *****************************************************************************/
BOOL CamifIspPowerDown( void ) {  
    /* Reverse order of IspPowerON */
    
    /* 1. Reset ISP */
    PadClear(CAM_RST_N, PAD_OUTPUT);
    RimSleep(1);  /* Minimum 100us */       
    
    /* 2. Set PDN low */
    PadClear(CAM_EN, PAD_OUTPUT);
    RimSleep(1);  /* Minimum 100us */    

    /* 3. Disable MCLK */
#ifdef PLATFORM_RIMSPRUCEWOOD    
    WriteRegDword(CSI_CSGCR, 0);
#else    
    WriteRegDwordAnd(PMNR, ~(PMNR_EN | PMNR_M(1) | PMNR_N(1)));
#endif
    
    PadClear(CAM_PWR_EN, PAD_OUTPUT);
    RimSleep(1);
    
    /* 4. Disbale LDO */
    BqHugeLdoDisable(LDO_CAMERA);

    camStatus.ispMode = CAM_ISP_DISABLED;   
    
    return TRUE;    
}

/* ===========================================================================
 * Debug functions
 * ===========================================================================*/
/*****************************************************************************
 * Function:    CamifShowReg 
 * Description: 
 * Parameters:
 * Return:
 *
 *****************************************************************************/
void CamifShowReg(DWORD category, DWORD verbose)
{
    DWORD sciBase;

    /* category is hardcoded here */
    PRINT("================= Camera Controller Registers =================");
    {
        PRINT2N("                       ACCR_REG(0x%08X)             = %08X", ACCR_REG, ReadRegDword(ACCR_REG));
        PRINT2N("                       ACSR_REG(0x%08X)             = %08X", ACSR_REG, ReadRegDword(ACSR_REG));
        PRINT2N("                       ACCR1_REG(0x%08X)            = %08X", ACCR1_REG, ReadRegDword(ACCR1_REG));        
        PRINT2N("                       D0CKEN_A(0x%08X)             = %08X", D0CKEN_A, ReadRegDword(D0CKEN_A));        
        PRINT2N("                       D0CKEN_B(0x%08X)             = %08X", D0CKEN_B, ReadRegDword(D0CKEN_B));        
        PRINT2N("                       D0CKEN_C(0x%08X)             = %08X",D0CKEN_C, ReadRegDword(D0CKEN_C));        
         
        /* CSI registers */ 
        PRINT2N("Camera CSI Registers:  CS0CR(0x%08X)             = %08X", CSI_CS0CR, ReadRegDword(CSI_CS0CR));
        PRINT2N("                       CS0SR(0x%08X)             = %08X", CSI_CS0SR, ReadRegDword(CSI_CS0SR));
        PRINT2N("                       CS0INEN(0x%08X)           = %08X", CSI_CS0INEN, ReadRegDword(CSI_CS0INEN));
        PRINT2N("                       CS0INST(0x%08X)           = %08X", CSI_CS0INST, ReadRegDword(CSI_CS0INST));
        PRINT2N("                       CS0TIM0(0x%08X)           = %08X", CSI_CS0TIM0, ReadRegDword(CSI_CS0TIM0));
        PRINT2N("                       CS0TIM1(0x%08X)           = %08X", CSI_CS0TIM1, ReadRegDword(CSI_CS0TIM1));
        PRINT2N("                       CS0GENDAT(0x%08X)         = %08X", CSI_CS0GENDAT, ReadRegDword(CSI_CS0GENDAT));
        PRINT2N("                       CLK_CSGCR(0x%08X)         = %08X", CSI_CLK_CSGCR, ReadRegDword(CSI_CLK_CSGCR));
        PRINT2N("                       CSI_RESERVED(0x%08X)      = %08X", CSI_RESERVED, ReadRegDword(CSI_RESERVED));

        /* CI0 registers */        
        sciBase = SCI0_REGBASE;    
        
        PRINT2N("Camera SCI0 Registers: SCICR0(0x%08X)             = %08X",sciBase +  SCI_OFF_SCICR0, ReadRegDword(sciBase + SCI_OFF_SCICR0));
        PRINT2N("                       SCICR1(0x%08X)             = %08X",sciBase +  SCI_OFF_SCICR1, ReadRegDword(sciBase + SCI_OFF_SCICR1));
        PRINT2N("                       SCISR(0x%08X)              = %08X",sciBase +  SCI_OFF_SCISR, ReadRegDword(sciBase + SCI_OFF_SCISR));
        PRINT2N("                       SCIMASK(0x%08X)            = %08X",sciBase +  SCI_OFF_SCIMASK, ReadRegDword(sciBase + SCI_OFF_SCIMASK));
        PRINT2N("                       SCIFIFO(0x%08X)            = %08X",sciBase +  SCI_OFF_SCIFIFO, ReadRegDword(sciBase + SCI_OFF_SCIFIFO));
        PRINT2N("                       SCIFIFOSR(0x%08X)          = %08X",sciBase +  SCI_OFF_SCIFIFOSR, ReadRegDword(sciBase + SCI_OFF_SCIFIFOSR));
        if(verbose > 0)
        {
            PRINT2N("                       SCIDADDR0(0x%08X)          = %08X",sciBase +  SCI_OFF_SCIDADDR0, ReadRegDword(sciBase + SCI_OFF_SCIDADDR0));
            PRINT2N("                       SCISADDR0(0x%08X)          = %08X",sciBase +  SCI_OFF_SCISADDR0, ReadRegDword(sciBase + SCI_OFF_SCISADDR0));
            PRINT2N("                       SCITADDR0(0x%08X)          = %08X",sciBase +  SCI_OFF_SCITADDR0, ReadRegDword(sciBase + SCI_OFF_SCITADDR0));
            PRINT2N("                       SCIDCMD0(0x%08X)           = %08X",sciBase +  SCI_OFF_SCIDCMD0, ReadRegDword(sciBase + SCI_OFF_SCIDCMD0));
            PRINT2N("                       SCIDBR0(0x%08X)            = %08X",sciBase +  SCI_OFF_SCIDBR0, ReadRegDword(sciBase + SCI_OFF_SCIDBR0));
            PRINT2N("                       SCIDCSR0(0x%08X)           = %08X",sciBase +  SCI_OFF_SCIDCSR0, ReadRegDword(sciBase + SCI_OFF_SCIDCSR0));
            
            PRINT2N("                       SCIDADDR1(0x%08X)          = %08X",sciBase +  SCI_OFF_SCIDADDR1, ReadRegDword(sciBase + SCI_OFF_SCIDADDR1));
            PRINT2N("                       SCISADDR1(0x%08X)          = %08X",sciBase +  SCI_OFF_SCISADDR1, ReadRegDword(sciBase + SCI_OFF_SCISADDR1));
            PRINT2N("                       SCITADDR1(0x%08X)          = %08X",sciBase +  SCI_OFF_SCITADDR1, ReadRegDword(sciBase + SCI_OFF_SCITADDR1));
            PRINT2N("                       SCIDCMD1(0x%08X)           = %08X",sciBase +  SCI_OFF_SCIDCMD1, ReadRegDword(sciBase + SCI_OFF_SCIDCMD1));
            PRINT2N("                       SCIDBR1(0x%08X)            = %08X",sciBase +  SCI_OFF_SCIDBR1, ReadRegDword(sciBase + SCI_OFF_SCIDBR1));
            PRINT2N("                       SCIDCSR1(0x%08X)           = %08X",sciBase +  SCI_OFF_SCIDCSR1, ReadRegDword(sciBase + SCI_OFF_SCIDCSR1));
            
            PRINT2N("                       SCIDADDR2(0x%08X)          = %08X",sciBase +  SCI_OFF_SCIDADDR2, ReadRegDword(sciBase + SCI_OFF_SCIDADDR2));
            PRINT2N("                       SCISADDR2(0x%08X)          = %08X",sciBase +  SCI_OFF_SCISADDR2, ReadRegDword(sciBase + SCI_OFF_SCISADDR2));
            PRINT2N("                       SCITADDR2(0x%08X)          = %08X",sciBase +  SCI_OFF_SCITADDR2, ReadRegDword(sciBase + SCI_OFF_SCITADDR2));
            PRINT2N("                       SCIDCMD2(0x%08X)           = %08X",sciBase +  SCI_OFF_SCIDCMD2, ReadRegDword(sciBase + SCI_OFF_SCIDCMD2));            
            PRINT2N("                       SCIDBR2(0x%08X)            = %08X",sciBase +  SCI_OFF_SCIDBR2, ReadRegDword(sciBase + SCI_OFF_SCIDBR2));
            PRINT2N("                       SCIDCSR2(0x%08X)           = %08X",sciBase +  SCI_OFF_SCIDCSR2,ReadRegDword(sciBase + SCI_OFF_SCIDCSR2));            
        }
        
        /* CI1 registers */       
        sciBase = SCI1_REGBASE;    
        
        PRINT2N("Camera SCI1 Registers: SCICR0(0x%08X)             = %08X",sciBase +  SCI_OFF_SCICR0, ReadRegDword(sciBase + SCI_OFF_SCICR0));
        PRINT2N("                       SCICR1(0x%08X)             = %08X",sciBase +  SCI_OFF_SCICR1, ReadRegDword(sciBase + SCI_OFF_SCICR1));
        PRINT2N("                       SCISR(0x%08X)              = %08X",sciBase +  SCI_OFF_SCISR, ReadRegDword(sciBase + SCI_OFF_SCISR));
        PRINT2N("                       SCIMASK(0x%08X)            = %08X",sciBase +  SCI_OFF_SCIMASK, ReadRegDword(sciBase + SCI_OFF_SCIMASK));
        PRINT2N("                       SCIFIFO(0x%08X)            = %08X",sciBase +  SCI_OFF_SCIFIFO, ReadRegDword(sciBase + SCI_OFF_SCIFIFO));
        PRINT2N("                       SCIFIFOSR(0x%08X)          = %08X",sciBase +  SCI_OFF_SCIFIFOSR, ReadRegDword(sciBase + SCI_OFF_SCIFIFOSR));
        if(verbose > 0)
        {
            PRINT2N("                       SCIDADDR0(0x%08X)          = %08X",sciBase +  SCI_OFF_SCIDADDR0, ReadRegDword(sciBase + SCI_OFF_SCIDADDR0));
            PRINT2N("                       SCISADDR0(0x%08X)          = %08X",sciBase +  SCI_OFF_SCISADDR0, ReadRegDword(sciBase + SCI_OFF_SCISADDR0));
            PRINT2N("                       SCITADDR0(0x%08X)          = %08X",sciBase +  SCI_OFF_SCITADDR0, ReadRegDword(sciBase + SCI_OFF_SCITADDR0));
            PRINT2N("                       SCIDCMD0(0x%08X)           = %08X",sciBase +  SCI_OFF_SCIDCMD0, ReadRegDword(sciBase + SCI_OFF_SCIDCMD0));
            PRINT2N("                       SCIDBR0(0x%08X)            = %08X",sciBase +  SCI_OFF_SCIDBR0, ReadRegDword(sciBase + SCI_OFF_SCIDBR0));
            PRINT2N("                       SCIDCSR0(0x%08X)           = %08X",sciBase +  SCI_OFF_SCIDCSR0, ReadRegDword(sciBase + SCI_OFF_SCIDCSR0));
            
            PRINT2N("                       SCIDADDR1(0x%08X)          = %08X",sciBase +  SCI_OFF_SCIDADDR1, ReadRegDword(sciBase + SCI_OFF_SCIDADDR1));
            PRINT2N("                       SCISADDR1(0x%08X)          = %08X",sciBase +  SCI_OFF_SCISADDR1, ReadRegDword(sciBase + SCI_OFF_SCISADDR1));
            PRINT2N("                       SCITADDR1(0x%08X)          = %08X",sciBase +  SCI_OFF_SCITADDR1, ReadRegDword(sciBase + SCI_OFF_SCITADDR1));
            PRINT2N("                       SCIDCMD1(0x%08X)           = %08X",sciBase +  SCI_OFF_SCIDCMD1, ReadRegDword(sciBase + SCI_OFF_SCIDCMD1));
            PRINT2N("                       SCIDBR1(0x%08X)            = %08X",sciBase +  SCI_OFF_SCIDBR1, ReadRegDword(sciBase + SCI_OFF_SCIDBR1));
            PRINT2N("                       SCIDCSR1(0x%08X)           = %08X",sciBase +  SCI_OFF_SCIDCSR1, ReadRegDword(sciBase + SCI_OFF_SCIDCSR1));
            
            PRINT2N("                       SCIDADDR2(0x%08X)          = %08X",sciBase +  SCI_OFF_SCIDADDR2, ReadRegDword(sciBase + SCI_OFF_SCIDADDR2));
            PRINT2N("                       SCISADDR2(0x%08X)          = %08X",sciBase +  SCI_OFF_SCISADDR2, ReadRegDword(sciBase + SCI_OFF_SCISADDR2));
            PRINT2N("                       SCITADDR2(0x%08X)          = %08X",sciBase +  SCI_OFF_SCITADDR2, ReadRegDword(sciBase + SCI_OFF_SCITADDR2));
            PRINT2N("                       SCIDCMD2(0x%08X)           = %08X",sciBase +  SCI_OFF_SCIDCMD2, ReadRegDword(sciBase + SCI_OFF_SCIDCMD2));            
            PRINT2N("                       SCIDBR2(0x%08X)            = %08X",sciBase +  SCI_OFF_SCIDBR2, ReadRegDword(sciBase + SCI_OFF_SCIDBR2));
            PRINT2N("                       SCIDCSR2(0x%08X)           = %08X",sciBase +  SCI_OFF_SCIDCSR2,ReadRegDword(sciBase + SCI_OFF_SCIDCSR2));            
        }
    }        
}

/*****************************************************************************
 * Function:    CamDrvShow 
 * Description: 
 * Parameters:  
 * Return:
 *
 *****************************************************************************/
void CamifShowStats(DWORD category, DWORD verbose)
{
	BYTE upper;
	BYTE lower;
    /* category is hardcoded here */
    PRINT("================= Camera Driver Show =================");
    {
        /* Driver status */
        PRINTN("Camera Driver Status:  drvEnabled        = %d", camStatus.drvEnabled);
        PRINTN("                       ispMode           = %d", camStatus.ispMode);                

        /* Driver statistics */
        PRINTN("Camera CSI Stats:      csiErrCrc         = %d", csiStatis.csiErrCrc);
        PRINTN("                       csiErr2Bit        = %d", csiStatis.csiErr2Bit);
        PRINTN("                       csiErr1Bit        = %d", csiStatis.csiErr1Bit);
        PRINTN("                       csiErrCtrl        = %d", csiStatis.csiErrCtrl);
        PRINTN("                       csiErrSoTSync     = %d", csiStatis.csiErrSoTSync);
        PRINTN("                       csiErrSoT         = %d", csiStatis.csiErrSoT);
        PRINTN("                       csiErrBPOverflow  = %d", csiStatis.csiErrBPOverflow);
        PRINTN("                       csiErrLMOverflow  = %d", csiStatis.csiErrLMOverflow);
        PRINTN("                       csiSof            = %d", csiStatis.csiSof);
        PRINTN("                       csiEof            = %d", csiStatis.csiEof);        
        PRINTN("Camera SCI0 Stats:     sci0Des           = %d", sciStatis(0).sciDes);
        PRINTN("                       sci0Sof           = %d", sciStatis(0).sciSof);
        PRINTN("                       sci0Sofx          = %d", sciStatis(0).sciSofx);
        PRINTN("                       sci0Ofo           = %d", sciStatis(0).sciOfo);
        PRINTN("                       sci0Eof           = %d", sciStatis(0).sciEof);
        PRINTN("                       sci0Eofx          = %d", sciStatis(0).sciEofx);
        PRINTN("                       sci0Sint          = %d", sciStatis(0).sciSint);
        PRINTN("                       sci0DmaBte        = %d", sciStatis(0).sciDmaBte);
        PRINTN("                       sci0DmaReq        = %d", sciStatis(0).sciDmaReq);
        
        PRINTN("Camera SCI1 Stats:     sci1Des           = %d", sciStatis(1).sciDes);
        PRINTN("                       sci1Sof           = %d", sciStatis(1).sciSof);
        PRINTN("                       sci1Sofx          = %d", sciStatis(1).sciSofx);
        PRINTN("                       sci1Ofo           = %d", sciStatis(1).sciOfo);
        PRINTN("                       sci1Eof           = %d", sciStatis(1).sciEof);
        PRINTN("                       sci1Eofx          = %d", sciStatis(1).sciEofx);
        PRINTN("                       sci1Sint          = %d", sciStatis(1).sciSint);
		PRINTN("                       sci1DmaBte        = %d", sciStatis(1).sciDmaBte);
		PRINTN("                       sci1DmaReq        = %d", sciStatis(1).sciDmaReq);
        
        /* Driver configuration */
        PRINTN("Camera CSI Config:     intEnbSci         = 0x%X", csiConfig.intEnbCsi);
        PRINTN("                       CsiLaneNum        = %d", csiConfig.CsiLaneNum);
        PRINTN("                       CsiDataRate       = %d (1 - 200, 2 - 480, 3 - 600, 4 - 700, 5 - 800)Mbps", csiConfig.DataRate);
        PRINTN("                       LcdRefreshRate    = %d", camConfig.lcdRefreshRate);
        PRINTN("                       ispFrameRate      = %d", camConfig.ispFrameRate);
        PRINTN("                       ispTestMode       = %d (0 = Disabled, [1-7] = Patterns", camConfig.ispTestMode);
       PRINT4N("                       Resolution        = %d: (%d x %d); ISP %d", camConfig.resolution.index,
                                                                camConfig.resolution.reslX, camConfig.resolution.reslY,
                                                                camConfig.resolution.ispResolutionId);
		PRINTN("                       vcMap[0]          = %d", csiConfig.vcMap[0]);
		PRINTN("                       vcMap[1]          = %d", csiConfig.vcMap[1]);
		PRINTN("                       vcMap[2]          = %d", csiConfig.vcMap[2]);
		PRINTN("                       vcMap[3]          = %d", csiConfig.vcMap[3]);
		PRINTN("                       HsTermen          = %d", csiConfig.HsTermen);
		PRINTN("                       ClMiss            = %d", csiConfig.ClMiss);
		PRINTN("                       ClSettle          = %d", csiConfig.ClSettle);
		PRINTN("                       ClTermen          = %d", csiConfig.ClTermen);
		PRINTN("                       HsSettle          = %d", csiConfig.HsSettle);
		PRINTN("                       HsRxTo            = %d", csiConfig.HsRxTo);

        PRINTN("Camera SCI0 Config:    intMaskSci        = 0x%X", sciConfig(0).intMaskSci);
        PRINTN("                       pxlFmtInput       = 0x%X", sciConfig(0).pxlFmtInput);
        PRINTN("                       pxlFmtOutput      = 0x%X", sciConfig(0).pxlFmtOutput);        
        PRINTN("                       dmTfs             = 0x%X", sciConfig(0).dmTfs);         
        PRINTN("Camera SCI1 Config:    intMaskSci        = 0x%X", sciConfig(1).intMaskSci);
        PRINTN("                       pxlFmtInput       = 0x%X", sciConfig(1).pxlFmtInput);
        PRINTN("                       pxlFmtOutput      = 0x%X", sciConfig(1).pxlFmtOutput);        
        PRINTN("                       dmTfs             = 0x%X", sciConfig(1).dmTfs);         

        PRINTN("ISP Status:            framesStreamed    = %d", CamSensorReadReg(0x010e));
        PRINTN("                       systemStatus      = %d (17-Sleep, 18-Idle, 19-VF, 20-Stills, 21-Movie, 24-Busy)", CamSensorReadReg(0x5d80));           
        PRINTN("                       systemErrStat     = %d (89-None, ...)", CamSensorReadReg(0x5d82)); 
        upper = CamSensorReadReg(0x479b);
        lower = CamSensorReadReg(0x479c);
        PRINT3N("                       CurrentFrameRate  = %d (Register Value = 0x%02x%02x)", (int)STV9xxF900Conversion(upper,lower) , upper, lower);
    }    
}
/*Author: Mikhail Zakharov
 * Date: January 2010
 * Performs conversion for STV9xx F900 floating point notation and
 * returns IEEE 4 byte floating point number
 * STV9xx F900 is composed of sign bit, 6 bit exponent, and 9 bit fraction
 * This code simply maps STV9xx bits to IEEE 32 bit single precision floating point
 * format
 * Input: = two bytes, representing STV9xx F900 number, upper is the MSF byte, and lower is LSF byte
 * Output: = converted IEEE floating point number
 */
FLOAT STV9xxF900Conversion(BYTE upper, BYTE lower)
{
	BYTE exponent;
	FLOAT frac;
	BYTE * frac_ptr = (BYTE *)&frac;
	exponent = ( (upper >> 1) & 0x3F  ) - 31 + 127; //convert 6 bit exponent to 8 bit
	//reconstruct IEEE floating point bit by bit
	frac_ptr[3] = ( exponent >> 1 ) | (upper & 0x80 );
	frac_ptr[2] = ( exponent & 0x1) << 7 | ( upper & 0x1 ) << 6 | ( lower >> 2 );
	frac_ptr[1] = (lower & 0x3) << 6;
	frac_ptr[0] = 0x00;
	return frac;
}

void CamifClearStats(void)
{
    memset(&CamDrv.statistics, 0, sizeof(CamStatis));
    CamifShowStats(0,1);
}
