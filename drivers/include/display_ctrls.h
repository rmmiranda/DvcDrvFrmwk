/*****************************************************************************
 * Filename:    display_ctrls.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Victor Chen
 *
 * Created:     March 16, 2010
 *
 * Description: This header file defines core datas structures of all the controllers (such as display controller, 
 *                  DSI controller, HDMI controller etc) of a display subsystem 
 *
 *****************************************************************************/
#ifndef __DISPLAY_CTRLS_H__
#define __DISPLAY_CTRLS_H__

#define MAX_DSI_CTL 0x02
/**
 * DisplayState_t  type  - Display Driver State Machine
 *                                
 */
typedef enum 
{
     DISP_STATE_UNINITIALIZED = 0,
     DISP_STATE_DISABLED,
     DISP_STATE_IDLE,
     DISP_STATE_SENDING,
     DISP_STATE_READING,
     DISP_STATE_WAIT_VSYNC,
     DISP_STATE_CNT
} DisplayState_t;

 /**
  * DispPipelineCfg_t   -  Pipeline configurations of display controller. Upper layer application picke up one configuration
  *                              when testing the DSS system with either paralle, DSI or HDMI or their combinations.
  *                                
  */
typedef enum  
{
     PIPELINE_DBI = 0x00,        /* pixel pipeline is configured for one parallel smart LCD module */
     PIPELINE_DSI1,              /* DSI0 is used */
     PIPELINE_DSI1_DSI2,         /* DSI0 and DSI1 are used */
     PIPELINE_DBI_DSI1,          /* One DBI +  DSI0 */
     PIPELINE_HDMI,              /* HDMI is used */
     PIPELINE_DSI1_HDMI,         /* DSI0 + HDMI */
     PIPELINE_DPI,
     PIPELINE_CNT
} DispPipelineCfg_t;

 /* DSI data lane Power Mode */
typedef enum 
{
     DSI_PM_HS = 0x01,
     DSI_PM_LPDT,
     DSI_PM_ULPS,
}DsiPowerMode_t;
 
 /* DSI controller state */
typedef enum 
{
     DSI_STATE_UNINITIALIZED = 0,
     DSI_STATE_DISABLED,
     DSI_STATE_IDLE,
     DSI_STATE_SENDING,
     DSI_STATE_READING,
     DSI_STATE_WAIT_BTA,
     DSI_STATE_ULPS,              /* In ULPS, no data transmission happened. exit ULPS before sending/reading*/
     DSI_STATE_CNT
} DsiCtlState_t;
 
/* ---------------------------------------------------------------------------
 * Display Controller  Config, Status and Statistics 
 * --------------------------------------------------------------------------- */
typedef struct DispcCfg
{
    BOOL enable;
    DispPipelineCfg_t   pipelineConfig;           /* One of pixel pipeline configurations */
    BYTE lcdMainModId;                            /* Manufacturer ID for the main LCD panel */
    BYTE lcdSubModId;                             /* manufacturer ID for the second LCD panel */
} DispcCfg_t;
    
typedef struct DispcStatus
{
    DisplayState_t state;  
    DWORD errFlag;	
} DispcStatus_t;
    
typedef struct DispcStats
{
    /* Frames sent by all pipelines */
    DWORD sendFrames;
    DWORD sendBytes;
    DWORD underFlowErr;
    DWORD overFlowErr;
} DispcStats_t;

/* ---------------------------------------------------------------------------
 * DSI controller  Config, Status and Statistics 
 * --------------------------------------------------------------------------- */
typedef struct DsiCfg
{
    BOOL           enabled;
    BOOL           bVChannel;             /* virtual channel enabled flag */
    DWORD          dataLanes;             /* Acitive DSI Data Lanes */
    DsiPowerMode_t powerMode;   
    WORD           maxRetSize;            /* maximum return size */
    WORD           tLpx;                  /* Timings: ns, convert to Mbps by (10^9/(2*tLpx))  */

    DWORD          dsiLinkSpeed; 
} DsiCfg_t;

/* DSI statistics defined by DSI spec */
typedef struct DsiStats 
{
    DWORD dsiTransFrames;                 /* Total frames send  */
    DWORD dsiTransBytes;                  /* Total bytes send */

    /* Statistics for Error Types returned from DSI Panel */
    DWORD dsiRtnAckSotErr;                /* Start of Transmission error */
    DWORD dsiRtnAckSotSyncErr;            /* SoT sync error */
    DWORD dsiRtnAckEotSyncErr;            /* End of transmission Sync error */
    DWORD dsiRtnEscModEntryCmdErr;        /* Escape mode entry command error */
    DWORD dsiRtnLpTransSyncErr;
    DWORD dsiRtnHsRcvTimeoutErr;
    DWORD dsiRtnFalseCtlErr;
    DWORD dsiRtnEcc1BitErr;
    DWORD dsiRtnEccBitsErr;
    DWORD dsiRtnChecksumErr;
    DWORD dsiRtnDataTypeErr;
    DWORD dsiRtnVcIdErr;
    DWORD dsiRtnTransLenErr;
    DWORD dsiRtnDsiProErr;
} DsiStats_t;

/* DSI Status */
typedef struct DsiStatus 
{
    DsiCtlState_t dsiCtlState;
    DWORD errFlag;
} DsiStatus_t;

/* ---------------------------------------------------------------------------
 * HDMI controller  Config, Status and Statistics 
 * --------------------------------------------------------------------------- */
/* ToDo - Add by HDMI driver later */
typedef struct HdmiCfg
{
	DisplayMode_t DispMode;
	VideoFormat_t DestFormat;
	PixelFormat_t SrcFormat;
	AudioLayout_t AudChanCount;
	AudioFreq_t   SampleFreq;
	AudioSize_t   SampleSize;
} HdmiCfg_t;

typedef struct HdmiStats 
{
    BOOL empty;
} HdmiStats_t;

typedef struct HdmiStatus
{
    BOOL empty;
} HdmiStatus_t;

/* ---------------------------------------------------------------------------
 * Display Subsystem Config, Status and Statistics 
 * --------------------------------------------------------------------------- */

/**
 * DisplayCft_t  type  - Display Subsystem Configurations (one display controller + 2 DSI controller + one HDMI)
 *                            
 */
typedef struct 
{
    DispcCfg_t   dispcConfig;
    DsiCfg_t     dsiConfig[MAX_DSI_CTL];
    HdmiCfg_t    hdmiConfig;
} DisplayCfg_t;

typedef struct 
{
    DispcStatus_t dispcStatus;
    DsiStatus_t   dsiStatus[MAX_DSI_CTL];
    HdmiStatus_t  hdmiStatus;
} DisplayStatus_t;

typedef struct 
{
    DispcStats_t  dispcStats;
    DsiStats_t    dsiStats[MAX_DSI_CTL];
    HdmiStats_t   hdmiStats;
} DisplayStats_t;

#endif
