/*****************************************************************************
 * Filename:    ldo.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     February 2010
 *
 * Description: This file defines the Roundrock specific macros for the LDO 
 *              (BqHuge) power control of the MiniOS.
 *
 ****************************************************************************/
 
#ifndef __ROUNDROCK_LDO_H__
#define __ROUNDROCK_LDO_H__

/* LDO (BqHuge) Assignments */
#define LDO_MIPI            3
#define LDO_CAMERA          1

/* LDO (BqHuge) Start Settings */
#define LDO_MIPI_STARTON    0
#define LDO_CAMERA_STARTON  0

/* LDO (BqHuge) Voltage Settings */
#define LDO_MIPI_VOLTAGE    1250
#define LDO_CAMERA_VOLTAGE  1800

#endif // __ROUNDROCK_LDO_H__
