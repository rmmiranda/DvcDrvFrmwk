/*****************************************************************************
 * Filename:    rei_minios_ldo.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     February 2010
 *
 * Description: This file defines the REI(runtime environment interface) for 
 *              LDO (power) group control of the MiniOS.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *
 ****************************************************************************/
 
#ifndef __REI_MINIOS_LDO_H__
#define __REI_MINIOS_LDO_H__

#include "rei_basetype.h"

/* REI interface for LDO */
#define LdoEnable        BqHugeLdoEnable
#define LdoDisable       BqHugeLdoDisable
#define LdoSetLevel      BqHugeLdoVoltage

/* ==========================================================================*/
/* DO NOT use these declarations directly in platform software. Use the REI 
 * defines instead */

extern void BqHugeLdoEnable( WORD ldo );
extern void BqHugeLdoDisable( WORD ldo );
extern WORD BqHugeLdoVoltage( WORD num, WORD value );

#endif // __REI_MINIOS_LDO_H__
