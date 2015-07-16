/*****************************************************************************
 * Filename:    camif.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Mar 9th, 2009
 *
 * Description: This file implements the common code (HW independent) and interface
 *              to the driver framework for camera driver.
 *
 *****************************************************************************/
/* include header files */

/* standard library header files */
#include <string.h>

#include "driver.h"
#include "camif_drvfwk.h"

#define SRCGROUP GROUP_PLT_DRV
#define SRCFILE  FILE_CAM_FWK

/* ===========================================================================
 * Data Structure and Macros 
 * ===========================================================================*/
CamDriver CamDrv;

__align(32) BYTE camifFrameBuffer[CAM_FRM_SIZE_MAX];
__align(32) BYTE camifFrameBufferVF[CAM_FRM_SIZE_VF];

void cbProcessFrame(MESSAGE * msg);

static CamResolution reslMap[CAM_RESL_INVALID] =
{
	{ CAM_RESL_INVALID, 		0,		0,		0	},
    { CAM_RESL_VGA_640_480,     640,    480,    86	},
    { CAM_RESL_SVGA_800_600,    800,    600,    87	},
    { CAM_RESL_720P_1280_720,   1280,   720,    164	},
    { CAM_RESL_SXGA_1280_1024,  1280,   1024,   88	},
    { CAM_RESL_UXGA_1600_1200,  1600,   1200,   89	},
    { CAM_RESL_1MP_1152_864,    1152,   864,    97	},
    { CAM_RESL_3MP_2048_1536,   2048,   1536,   161	},
    { CAM_RESL_5MP_2624_1984,   2624,   1984,   162	},
    { CAM_RESL_CUSTOM,          1800,   1400,   96	}
};
/* ===========================================================================
 * Platform Driver Framework Interface Functions
 * ===========================================================================*/
 /*****************************************************************************
 * Function:    DrvCamGetStatus 
 * Description: Drv Framework API - return a pointer to the driver status
 * Parameters:
 * Return:
 *
 *****************************************************************************/
void * DrvCamGetStatus(Driver_t * drvPtr)
{
    return (void *)&(((CamDriver *)drvPtr)->status);
}

/*****************************************************************************
 * Function:    DrvCamGetConfig 
 * Description: Drv Framework API - return a pointer to the driver configuration
 * Parameters:
 * Return:
 *
 *****************************************************************************/
void * DrvCamGetConfig(Driver_t * drvPtr)
{
    return (void *)&(((CamDriver *)drvPtr)->config);
}

/*****************************************************************************
 * Function:    DrvCamGetStats 
 * Description: Drv Framework API - return a pointer to the driver statistics
 * Parameters:
 * Return:
 *
 *****************************************************************************/
void * DrvCamGetStats(Driver_t * drvPtr)
{
    return (void *)&(((CamDriver *)drvPtr)->statistics);
}

/*****************************************************************************
 * Function:    DrvCamControl 
 * Description: Drv Framework API - Common driver control function
 * Parameters:
 * Return:
 *
 *****************************************************************************/
int DrvCamControl(Driver_t *driver, int control, va_list args)
{
    switch (control)
    {
        case CAM_CMD_DUMP_REG:
            CamifShowReg(4, 1);
            /* Display all camera registers */
            break;
        case CAM_CMD_DRIVER_SHOW:
            CamifShowStats(0, 1);
            break;
        case CAM_CMD_START_VF:
        {
            CamifStartViewfinder(); 
            break;
        }
        case CAM_CMD_START_MV:
        {
            CamifStartMovie(CAM_PIXEL_FMT_RGB_565);
            break;
        }        
        case CAM_CMD_STOP_VF:
        case CAM_CMD_STOP_MV:
        {
            CamifStreamStop();
            break;            
        }
        case CAM_CMD_ISP_PWR_UP:
        {
            CamifIspPowerOn();
            break;
        }
        case CAM_CMD_ISP_PWR_DN:
        {
            CamifIspPowerDown();
            break;            
        }
        case CAM_CMD_ISP_REG_RD:
        {
            WORD reg;
            BYTE * valPtr;
            
            reg = VA_ARG( args, WORD );
            valPtr = VA_ARG( args, BYTE * );
            
            *valPtr = CamSensorReadReg((WORD)reg);
            break;
        }
		case CAM_CMD_ISP_REG_WR:
        {
            WORD reg;
            BYTE val;
            int * retPtr;
            
            reg = VA_ARG( args, WORD );
            val = VA_ARG( args, BYTE);
            
            retPtr = VA_ARG( args, int * );
            *retPtr = CamSensorWriteReg ((WORD)reg, val);
            
            break;
        }
        case CAM_CMD_CLEAR_STATIS:
        {
            CamifClearStats();
            break;            
        }
        case CAM_CMD_CHANGE_RESOLUTION:
        {
        	DrvFwkSetCamResolution((CamResolutionId) VA_ARG( args, WORD ));
        	break;
        }
        default:
            PRINT("Camera Driver: unknown Control command");
            return -1;
            /* invalid command */
    }
    return 0;
}

/*****************************************************************************
 * Function:    DrvCamEnable 
 * Description: Drv Framework API - Enable camera driver
 * Parameters:
 * Return: 
 *      Equivalent to CamifPowerUp
 *****************************************************************************/
int  DrvCamEnable(Driver_t * drvPtr, int unused)
{
    CamifIspPowerOn();
    CamifEnable();
    camStatus.drvEnabled = TRUE;

    return 0;
}

/*****************************************************************************
 * Function:    DrvCamDisable 
 * Description: Drv Framework API - Disable camera driver
 * Parameters:
 * Return:
 *
 *****************************************************************************/
int  DrvCamDisable(Driver_t * drvPtr, int quickDisable)
{
    CamifDisable(quickDisable);
    camStatus.drvEnabled = FALSE;
    
    return 0;
}

/*****************************************************************************
 * Function:    DrvCamInit 
 * Description: Drv Framework API - driver initialization
 * Parameters:  void
 * Return:      Pointer to variable CamDrv.
 *
 *****************************************************************************/
Driver_t * DrvFwkCamInit(void)
{        
    camApi.GetStatus = DrvCamGetStatus;
    camApi.GetConfig = DrvCamGetConfig;
    camApi.GetStats  = DrvCamGetStats;
    camApi.Control   = DrvCamControl;
    camApi.Enable    = DrvCamEnable;
    camApi.Disable   = DrvCamDisable;
    
    /* Default camera card is camel2 */
    CamDrv.IspReset = IspReset_ST987_10;
    CamDrv.IspLoadPatch = IspLoadPatch_ST987_10;
    CamDrv.IspSvpFunc = IspSvpFunc_ST987_10;  
    
    /* Register Bugdisp hook */
	PfCmdRegister("cam", "Camera(CSI) Driver", &DrvFwkCamDebug);
    
    /* register message handler */
    PfMsgRegister(PLATFORM_THREAD_ID_DRV, PLATFORM_MSG_CAM_DRV, cbProcessFrame);

    CamIfInit();
    
    /* Default configuration. The settings here will be effective after driver 
     * is enabled; so you have a chance to re-configure thsese setting before
     * enable the driver.
     */    
    sciConfig(0).dmTfs = sciConfig(1).dmTfs = CAM_DMA_TFS_32;
    sciConfig(0).pxlFmtInput = sciConfig(1).pxlFmtInput = CAM_PIXEL_FMT_RGB_565;
    sciConfig(0).pxlFmtOutput = sciConfig(1).pxlFmtOutput = CAM_PIXEL_FMT_RGB_565;
    
    /* Default: 2 data lanes */
    csiConfig.CsiLaneNum = 1;

    /* Default data rate */
    csiConfig.DataRate = CAM_CSI_DATA_RATE_600M;
       
    /* Default LCD refresh rate */
    camConfig.lcdRefreshRate = 0;
    
    /* Default movie/JPEG resolution: SIZE_VGA */
    DrvFwkSetCamResolution(CAM_RESL_VGA_640_480);
    
    /* Set frame rate */
    camConfig.ispFrameRate = 30;
        
    /* SCI selection */
    camConfig.sciIds = CAM_SCI_0;
    
    return (Driver_t *)&CamDrv;
}

/*****************************************************************************
 * Function:    cbProcessFrame 
 * Description: Local frame processing function. Forward the incoming image frame 
 *              to the display driver and reset the DMA registers to prepare for 
 *              the next frame.
 * Parameters:  id - not used.
 * Return:      void
 *
 *****************************************************************************/
void cbProcessFrame(MESSAGE * msg)
{
    DWORD   subMsg = msg->SubMsg;
    
    if(camStatus.drvEnabled == FALSE)
    {
        WARN("Camera driver already disabled");
        return;
    }

    PRINTN("Camera driver: message - %d", subMsg);
        
    if((CamMsg)subMsg == CAM_MSG_NEW_FRAME)
    {        
        static int fcnt = 0;
        fcnt++;
        if((camConfig.lcdRefreshRate > 0)&& (fcnt % camConfig.lcdRefreshRate == 0))
        {            
            if(camStatus.ispMode == CAM_ISP_VIEWFINDER)
            {
	            CacheExecute(DATA_CACHE, CACHE_INVALIDATE, (DWORD)camifFrameBufferVF, CAM_FRM_SIZE_VF);
            }
            else
            {
                int y;
                CacheExecute(DATA_CACHE, CACHE_INVALIDATE, (DWORD)camifFrameBuffer, CAM_FRM_SIZE(2));
                
                /* down scale to VF */
                for( y = 0; y < camConfig.resolution.reslY; y++)
                {
                    memcpy(camifFrameBufferVF + (y * CAM_PXL_WIDTH_VF * 2), 
                           camifFrameBuffer   + (y * camConfig.resolution.reslX * 2),  
                           CAM_PXL_WIDTH_VF * 2);
                }                
            }
	    
            /* Display the image  */
            {
            #include "bitmap.h"
            extern void LcdBitBltToLcdBufferInt( const BitMap * src, int srcX, int srcY, int wide,
                              int high, BYTE displayId, int destX, int destY, int Op );
            extern void LcdUpdateInt( BYTE displayId, int x, int y, int width, int height );

            BitMap bm;
            
            bm.bType = ROWWISE_16BIT_COLOUR_BITMAP;
            bm.wide = CAM_PXL_WIDTH_VF;
            bm.high = CAM_PXL_HEIGHT_VF;            
            bm.data = (BYTE *)camifFrameBufferVF;
            bm.stride = CAM_PXL_WIDTH_VF * 2;
      
            LcdBitBltToLcdBufferInt( &bm, 0, 0,
                    CAM_PXL_WIDTH_VF, CAM_PXL_HEIGHT_VF,
                    0, 0, 0, 0 );
            LcdUpdateInt( 0, 0, 0, CAM_PXL_WIDTH_VF, CAM_PXL_HEIGHT_VF);  
            }
            fcnt = 0;            
        }
    }
    else
        WARN("Unknown message");
}

void DrvFwkSetCamResolution(CamResolutionId id)
{
    camConfig.resolution.index = id;
    camConfig.resolution.reslX= reslMap[id].reslX;
    camConfig.resolution.reslY = reslMap[id].reslY;
    camConfig.resolution.ispResolutionId = reslMap[id].ispResolutionId;
    PRINT4N("Set Camera resolution to %d: (%d x %d); ISP %d",
                        camConfig.resolution.index,
                        camConfig.resolution.reslX, camConfig.resolution.reslY,
                        camConfig.resolution.ispResolutionId);
}
