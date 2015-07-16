/*****************************************************************************
 * Filename:    camera.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Mar 9th, 2009
 *
 * Description: This header file defines the camera driver public interface
 *
 *****************************************************************************/
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "rei.h"
#include "driver.h"

#define NUM_OF_SCI           2

/* Quick capture mode */
typedef enum {
    CAM_ISP_DISABLED,
    CAM_ISP_IDLE,
    CAM_ISP_VIEWFINDER,
    CAM_ISP_PICTURE,
    CAM_ISP_MOVIE
} CamISPMode;

/* SCI input/ouput pixel format */
typedef enum {
    CAM_PIXEL_FMT_RAW_8       = 0x00,
    CAM_PIXEL_FMT_RAW_10      = 0x02,
    CAM_PIXEL_FMT_YCBCR_422   = 0x03,
    CAM_PIXEL_FMT_RGB_565     = 0x05,
    CAM_PIXEL_FMT_RGB_666     = 0x06,
    CAM_PIXEL_FMT_RGB_888     = 0x07,
    CAM_PIXEL_FMT_JPEG        = 0x08
} CamPxlFmt;
   
typedef enum {
    CAM_RESL_VGA_640_480 		= 1,
    CAM_RESL_SVGA_800_600,
    CAM_RESL_720P_1280_720,
    CAM_RESL_SXGA_1280_1024,
    CAM_RESL_UXGA_1600_1200,
    CAM_RESL_1MP_1152_864,
    CAM_RESL_3MP_2048_1536,
    CAM_RESL_5MP_2624_1984,
    CAM_RESL_CUSTOM,
    CAM_RESL_INVALID
}CamResolutionId;

typedef struct {
    CamResolutionId index;
    int reslX;
    int reslY;
    int ispResolutionId;
}CamResolution;

typedef enum {
    CAM_INTERLEAVING_DISABLED,
    CAM_INTERLEAVING_JPEG_VF,
    CAM_INTERLEAVING_VIDEO_VF
}CamInterleavingMode;

/* SCI DMA FIFO Transfer Size */
typedef enum {
    CAM_DMA_TFS_32            = 0,
    CAM_DMA_TFS_64            = 1
} CamDmaTfs;        

/* SCI Disable mode */
typedef enum {
    CAM_DISABLE_NORMAL,
    CAM_DISABLE_QUICK
} CamDisableMode;

/* SCI Index Bit Mask*/
typedef enum {
    CAM_SCI0_ID                = 0,
    CAM_SCI1_ID                = 1,
    CAM_SCI_TOTAL              = 2,
} CamSciId;

/* CSI DPHY Data Rate */
typedef enum {
    CAM_CSI_DATA_RATE_200M  = 1,       /* 200Mbps */
    CAM_CSI_DATA_RATE_480M  = 2,       /* 480Mbps */
    CAM_CSI_DATA_RATE_600M  = 3,       /* 600Mbps */
    CAM_CSI_DATA_RATE_700M  = 4,       /* 700Mbps */
    CAM_CSI_DATA_RATE_800M  = 5        /* 800Mbps */
}CamCsiDataRate;

/* CSI Error Control */
typedef enum {
    CAM_ERR_CONT = 1,
    CAM_ERR_STOP = 2
}CamErrControl;

/* Camera message */
typedef enum {
    CAM_MSG_TIMER_EXPIRED = 0,
    CAM_MSG_TIMER_START,
    CAM_MSG_TIMER_STOP,
    CAM_MSG_NEW_FRAME,
    CAM_MSG_CSI_ERR
}CamMsg;

/* Camera test mode */
typedef enum {
    CAM_TST_MODE_DISABLED = 0,
    CAM_TST_MODE_COLOR_BAR,
    CAM_TST_MODE_GRADUATEDCOLOURBARS,
    CAM_TST_MODE_DIAGONALGREYSCALE,
    CAM_TST_MODE_PSEUDORANDOM,
    CAM_TST_MODE_HORIZONTALGREYSCALE,
    CAM_TST_MODE_VERTICALGREYSCALE,
    CAM_TST_MODE_SOLIDCOLOUR
}CamTestMode;

/* Driver configuration */
typedef struct {
    DWORD                   sciIds;
    struct {
        DWORD               intEnbCsi;
        DWORD               CsiLaneNum;
        CamCsiDataRate      DataRate;
        CamErrControl       errControl;
        BYTE                vcMap[4];
        DWORD               csiTimer0;
        DWORD               csiTimer1;
        WORD                HsTermen;       /* CSxTim0 */
        WORD                ClMiss;         /* CSxTim0 */
        WORD                ClSettle;       /* CSxTim0 */
        WORD                ClTermen;       /* CSxTim0 */
        WORD                HsSettle;       /* CSxTim1 */
        WORD                HsRxTo;         /* CSxTim1 */
    }csiConfig;        
    struct {
        DWORD               intMaskSci;
        CamPxlFmt           pxlFmtInput;
        CamPxlFmt           pxlFmtOutput;
        CamDmaTfs           dmTfs;
        DWORD               pxlWidth;
        DWORD               pxlHeight;
    }sciConfig[NUM_OF_SCI];    
    
    /* ISP configuration */
    CamTestMode             ispTestMode;
    DWORD                   ispFrameRate;
    CamResolution           resolution;
    CamInterleavingMode     interleavingMode;
    int                     lcdRefreshRate; /* 0 - disable LCD */
} CamConfig;
    
/* Driver status */
typedef struct {
    BOOL        drvEnabled ;      /* Driver Enabled, Disabled */
    CamISPMode  ispMode ;         /* ISP Enabled, Disabled */
    int         errFlag;
    IODWORD *   csiStatusRegPtr;  /* Pointer to the CSI status register */
} CamStatus;

typedef struct {
    DWORD sciSof;
    DWORD sciSofx;
    DWORD sciDbs;
    DWORD sciDes;     /* SCI DMA Error */
    DWORD sciOfo;     /* Output FIFO Overflow */    
    DWORD sciEof;
    DWORD sciEofx;    /* FIFO EOF */
    DWORD sciSint;
    DWORD sciDmaBte;        
    DWORD sciDmaReq;                
}SciStatisType;

/* Driver statistics */
typedef struct {

    SciStatisType sciStatis[NUM_OF_SCI];
    
    struct {
        DWORD csiHsRxTo3;
        DWORD csiHsRxTo2;
        DWORD csiHsRxTo1;
        DWORD csiHsRxTo0;
        DWORD csiErrCrc;
        DWORD csiErr2Bit;
        DWORD csiErr1Bit;
        DWORD csiErrCtrl;
        DWORD csiErrSoTSync;
        DWORD csiErrSoT;
        DWORD csiErrBPOverflow;
        DWORD csiErrLMOverflow;    
        DWORD csiSof;
        DWORD csiEof;
    }csiStatis;
    
} CamStatis;

typedef enum
{
    CAM_CMD_DRIVER_SHOW,
    CAM_CMD_DUMP_REG,
    CAM_CMD_START_VF,
    CAM_CMD_STOP_VF,
    CAM_CMD_START_MV,
    CAM_CMD_STOP_MV,    
    CAM_CMD_TAKE_PICTURE,
    CAM_CMD_ISP_PWR_UP,
    CAM_CMD_ISP_PWR_DN,
    CAM_CMD_ISP_REG_RD,
    CAM_CMD_ISP_REG_WR,
    CAM_CMD_CLEAR_STATIS,
    CAM_CMD_CHANGE_RESOLUTION
} CAM_COMMAND_ENUM;
#endif
