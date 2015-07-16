/*****************************************************************************
 * Filename:    cam_internal.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Mar 9th, 2009
 *
 * Description: This header file defines the driver internal interface
 *
 *****************************************************************************/
#ifndef __CAM_INTERNAL_H__
#define __CAM_INTERNAL_H__

#include "rei.h"
#include "framework.h"
#include "camera.h"

#define CAM_MAX_DMA_DESP        3

typedef unsigned short i2cAddress;

typedef struct {
    DWORD   descriptor;      /* Next descsriptor address */
    DWORD   source;         /* Source address */
    DWORD   target;         /* Traget address */
    DWORD   command;        /* Command */
} CamDmaDescriptor;

/* Define of Camera driver structure */
typedef struct {
    /* Camera driver public info   */
    Driver_t drvApi;
    
    /* Camera driver public info */
    /* Driver configuration */
    CamConfig config;
    
    /* Driver status */
    CamStatus status;
    
    /* Driver statistics */
    CamStatis statistics;
    
    /* Camera driver private data members */
    RE_Semaphore        sema;   /* Semaphore */
    RE_MESSAGE          msgQ;   /* Message Q */
    BOOL                frmReceived;
        
    /* ISP function pointers */
    void     (* IspReset) (void);
    void     (* IspLoadPatch) (void);
    void     (* IspSvpFunc) (int);
} CamDriver;

/* Camera common API */
void CamifStartViewfinder(void);
void CamifStartMovie(CamPxlFmt fmt);
void CamifStartPicture(void);
void CamifStreamStop(void) ;
void CamifEnableInterleaving(CamInterleavingMode mode);

/* Hardware specific */
DWORD CamIfInit( void ) ;
BOOL  CamifEnable(void) ;
BOOL  CamifDisable(int mode);
BOOL  CamifIspPowerOn( void );
BOOL  CamifIspPowerDown( void );
int   CamifDmaSetup(int ciId, DWORD memAddr[3], DWORD length);
int   CamifDmaStop(int ciId);

/* Debugging functions */
void CamifShowStats(DWORD category, DWORD verbose);
void CamifShowReg(DWORD category, DWORD verbose);
void CamifClearStats(void);

/* ISP */
BYTE CamSensorReadReg(WORD reg);
int  CamSensorWriteReg(WORD reg, BYTE value);
BOOL CamSensorWaitStatus( WORD reg, BYTE value );
void CamSensorWriteAuto( DWORD args, ... );

enum{
    SVPCF_INIT,
    SVPCF_GET_I2C_ADDR,
    SVPCF_GET_I2C_WIDTH,
    SVPCF_DETECT,
    SVPCF_POWER_ON,
	SVPCF_STREAM_COUNTER,
    SVPCF_SETUP_VF,
	SVPCF_STOP_STREAM,
    SVPCF_SETUP_MOVIE,
	SVPCF_SETUP_TEST_MODE,
	SVPCF_STOP_TEST_MODE,
    SVPCF_SETUP_PICTURE,
    SVPCF_TAKE_PICTURE, 
    SVPCF_TAKE_PIC_DONE,
    SVPCF_AF_START,
    SVPCF_AF_CANCEL,
    SVPCF_AF_RELAX,
    SVPCF_IS_AF_DONE,
    SVPCF_VALIDATE_TP,
    SVPCF_GET_UPTIME,
    SVPCF_SET_TP,
    SVPCF_FOCUS_INF,
    SVPCF_FOCUS_MACRO
};

/* ISP functions */
void IspSvpFunc_ST987_10(int svpCmd);
void IspLoadPatch_ST987_10( void );
void IspReset_ST987_10(void);

void IspSvpFunc_ST987_09(int svpCmd);
void IspLoadPatch_ST987_09( void );
void IspReset_ST987_09(void);

#define CAM_SCI_0     (1 << CAM_SCI0_ID)
#define CAM_SCI_1     (1 << CAM_SCI1_ID)

/* Image resolution */
#define CAM_PXL_WIDTH_VF   480
#ifdef PLATFORM_RIMSPRUCEWOOD
	#define CAM_PXL_HEIGHT_VF  360
#else
	#define CAM_PXL_HEIGHT_VF  320
#endif
#define CAM_FRM_SIZE_MAX    (1600*1200*3) /* For  UXGA */
#define CAM_FRM_SIZE(bpp)        (camConfig.resolution.reslX * camConfig.resolution.reslY * bpp)
#define CAM_FRM_SIZE_VF     (CAM_PXL_WIDTH_VF * CAM_PXL_HEIGHT_VF * 2)

/* frame buffer */
extern BYTE camifFrameBuffer[CAM_FRM_SIZE_MAX];
extern BYTE camifFrameBufferVF[CAM_FRM_SIZE_VF];

/* Convenient macros */
extern CamDriver CamDrv;
#define     camApi          (CamDrv.drvApi)
#define     camStatis       (CamDrv.statistics)
#define     camConfig       (CamDrv.config)
#define     camStatus       (CamDrv.status)
#define     sciConfig(i)    (CamDrv.config.sciConfig[i])
#define     sciStatis(i)    (CamDrv.statistics.sciStatis[i])
#define     csiConfig       (CamDrv.config.csiConfig)
#define     csiStatis       (CamDrv.statistics.csiStatis)

#define   TIME_START(x)     {x = RimGetNativeTicks();}
#define   TIME_END(x)       (((RimGetNativeTicks() - x)* 1000)/RimGetNativeTicksPerSecond())

#endif
