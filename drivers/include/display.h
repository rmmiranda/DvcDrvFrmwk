/*****************************************************************************
 * Filename:    display.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Victor Chen
 *
 * Created:     March 16, 2010
 *
 * Description: This header file defines the display subsystem public interfaces and core data structures
 *
 *****************************************************************************/
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "rei.h"
#include "framework.h"

#include "display_ctrls.h"

#define NELEMENTS(array) sizeof(array)/sizeof((array)[0])

/**
 * DisplayId_t  type  - Used by applications to specify the desitnation peripheral LCD modules
 *                            when transmitting or recieving data to it.
 */
typedef enum 
{
    DISPLAY_ID_0 = 0,    
    DISPLAY_ID_1,
    DISPLAY_ID_CNT
} DisplayId_t;

/**
  * Surface Id  - used to specify the layer (backplane or overlay) on the screen. 
  */
typedef enum  
{
    CURSOR_SURFACE = 0x01,
    PRIMARY_SURFACE,
    OVERLAY_SURFACE_1,
    OVERLAY_SURFACE_2,
    OVERLAY_SURFACE_MAX,
}DispSurfaceId_t;


/* Pixel Format Definitions  */
typedef enum 
{
    PIXEL_FMT_UNKNOWN = 0,
    PIXEL_FMT_RGB565,
    PIXEL_FMT_RGB1555,
    PIXEL_FMT_RGB888P,
    PIXEL_FMT_RGB888UP,
    PIXEL_FMT_RGBA888,
	PIXEL_FMT_RGB888A,

    PIXEL_FMT_YUV422P,              /* YUV422 packed */
    PIXEL_FMT_YUV422C,              /* YUV422 planar */
    PIXEL_FMT_YUV420,               /* YUV420 planar */
    PIXEL_FMT_YUV444,

    PIXEL_FMT_PALETTE4,
    PIXEL_FMT_PALETTE8,

    PIXEL_FMT_YUYV422,
    PIXEL_FMT_YVYU422,
    PIXEL_FMT_VYUY422,

} pixelFmt_t;

/* Ioctl */
typedef enum
{
    DISP_CMD_CLEAR_LCD,
    DISP_CMD_SET_BACKGROUND_COLOR,
    DISP_CMD_SET_ROTATION,
    DISP_CMD_CLEAR_STATIS,
    DISP_CMD_STOP_TRANSMISSION,
    DISP_CMD_DSI_SETVC,
    DISP_CMD_DUMP_REGS,
    DISP_CMD_CNT
} DISP_COMMAND_ENUM;

typedef enum {
    ROTATE_ANGLE_NONE=0,
    ROTATE_ANGLE_90,
    ROTATE_ANGLE_180,
    ROTATE_ANGLE_270
} DispRotAngle_t;

/**
  * DispSurface_t - Define a surface in system memory corresopnding one layer on LCD panel. A pixel pipeline 
  *                       will configure itself and fetch pixels from surface based on this structure.
  *  
  */
typedef struct DispSurface
{
    BOOL valid;
    DisplayId_t displayId;      /* displayId to identify a destionation LCD module for application layer usage */
    DispSurfaceId_t surfaceId;  /* SurfaceId to determine primary or overlay surface */

    pixelFmt_t pixelFormat;    /* Pixel Format for the surface */
    DWORD pRGBAddr;            /* Physical Address of the frame buffer for RGB format */
    DWORD pYAddr;              /* Physical Address of the including frame buffer */
    DWORD pUAddr;              /* Physical Address of the including frame buffer */
    DWORD pVAddr;              /* Physical Address of the including frame buffer */

    /* Size (resoulation) */
    WORD width;
    WORD height;

    /* Overlay only for position and scaling info */
    WORD xPos;
    WORD yPos;
    WORD xSrcSz;
    WORD ySrcSz;
    WORD xDstSz;
    WORD yDstDz;
}DispSurface_t;

/* Public GUI related APIs used by upper layer */

/***************************************************************************
 * Function:      DispShowSurface
 * Description:  API to show pixels of a surface (frame buffer) on the speicified LCD module 
 *
 * @param pSurface - The data structure includes the information of frame buffer to be sent over 
 *                            display subsystem: destionation panel, pixel format, size, physical address
 *                            of frame buffer.
 * 
 * @return TRUE if the operation is successful. Otherwise
 *         returns FALSE.
 *
 * Note: Based on pSurface, display driver knows which pixel pipeline should be used and how to 
 *         configure it        
 **************************************************************************/
BOOL DispShowSurface(DispSurface_t *pSurface);


/***************************************************************************
 * Function:     DispMoveSurface
 * Description:  Move the surface on a specified LCD module to the position (xPos, yPos)  
 *
 * @param pSurface - The data structure includes the position information of frame buffer to be 
 *                            sent over display subsystem.
 * 
 * @return TRUE if the operation is successful. Otherwise
 *         returns FALSE.
 *
 * Note:    Display driver won't reconfigure pixel pipeline but just "move" the original pixture to 
 *            another place.        
 **************************************************************************/
BOOL DispMoveSurface(DispSurface_t *pSurface);

/***************************************************************************
 * Function:     DispReadLcdPixels
 * Description:  Read pixels back from the speicified LCD module 
 *
 * @param displayId - the destionation of LCD panel. We could have upto 2 panels.
 *             buf        - BUffer for returning data
 *             pixels     - how many pixels to be read back.
 * 
 * @return TRUE if the operation is successful. Otherwise returns FALSE.
 *
 * Note:   None
 **************************************************************************/
BOOL DispReadLcdPixels(DisplayId_t displayId, BYTE *buf, DWORD pixels);
 
#endif
