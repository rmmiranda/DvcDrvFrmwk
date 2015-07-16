/*****************************************************************************
 * Filename:    camif_drvfwk.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Mar 9th, 2009
 *
 * Description: This header file defines the interface between driver framework 
 *              and camera driver
 *
 *****************************************************************************/
#ifndef __CAM_DRVFWK_H__
#define __CAM_DRVFWK_H__

#include "rei.h"
#include "camera.h"
#include "framework.h"
#include "camif_internal.h"

/* Driver dramework API */
void DrvFwkCamDebug(char * cmd, DWORD arg1, DWORD arg2);
void DrvFwkCamIspTest(void);
void DrvFwkSetCamResolution(CamResolutionId id);

#endif
