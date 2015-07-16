/*****************************************************************************
 * Filename:    rei_ldo.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     February 2009
 *
 * Description: This file defines the REI(runtime environment interface) for 
 *              LDO (power) group control.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *
 ****************************************************************************/
 
#ifndef __REI_LDO_H__
#define __REI_LDO_H__

#include "rei_basetype.h"

/* REI interface for LDO */
#define RE_LdoEnable        LdoEnable
#define RE_LdoDisable       LdoDisable
#define RE_LdoSetLevel      LdoSetLevel

/* REI structure defines for LDO */
#define RE_LdoGroup         LdoGroup

/* ==========================================================================*/
/* DO NOT use these declarations directly in platform software. Use the REI 
 * defines instead */
typedef DWORD LdoGroup;

extern void LdoEnable( LdoGroup ldo );
extern void LdoDisable( LdoGroup ldo );
extern void LdoSetlevel( LdoGroup ldo, DWORD level );

#define RE_LDO_SIM_LEVEL_1_8V  0
#define RE_LDO_SIM_LEVEL_3_0V  1

#endif // __REI_LDO_H__
