/*****************************************************************************
 * Filename:    rei_metrics.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     January 2009
 *
 * Description: This file defines the REI(runtime environment interface) for 
 *              platform software metrics.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *
 ****************************************************************************/
 
#ifndef __REI_METRICS_H__
#define __REI_METRICS_H__

#include "rei_basetype.h"

/* REI interface */

/* REI structure defines */
#define RE_MetricsDataStruct            MetricsDataStruct

/* ==========================================================================*/
/* DO NOT use these declarations directly in platform software. Use the REI 
 * defines instead */
#include "metrics.h"

#endif // __REI_METRICS_H__
