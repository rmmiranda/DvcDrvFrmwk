/*****************************************************************************
 * Filename:    rei_dma.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     February 2009
 *
 * Description: This file defines the REI(runtime environment interface) for 
 *              DMA control.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *
 ****************************************************************************/
 
#ifndef __REI_DMA_H__
#define __REI_DMA_H__

/* REI interface for DMA */
#define RE_DmaGetChannelStatus          DmaGetChannelStatus
#define RE_DmaCreateDescriptor          DmaCreateDescriptor
#define RE_DmaLoadDescriptor            DmaLoadDescriptor
#define RE_DmaEditDescriptor            DmaEditDescriptor
#define RE_DmaSetupSingleTransfer       DmaSetupSingleTransfer
#define RE_DmaRegisterIsr               DmaRegisterIsr
#define RE_DmaRegisterExIsr             DmaRegisterExIsr
#define RE_DmaEnableChannelInterrupt    DmaEnableChannelInterrupt
#define RE_DmaDisableChannelInterrupt   DmaDisableChannelInterrupt
#define RE_DmaEnableChannel             DmaEnableChannel
#define RE_DmaDisableChannel            DmaDisableChannel

/* REI structure defines for DMA */
#define RE_DmaChannelStatus             DmaChannelStatus
#define RE_DmaErr                       DmaErr
#define RE_DmaAddressType               DmaAddressType
#define RE_DmaRequest                   DmaRequest
#define RE_DmaWidth                     DmaWidth
#define RE_DmaBurstSize                 DmaBurstSize
#define RE_DmaDescriptorConfigParam     DmaDescriptorConfigParam
#define RE_DmaFlags                     DmaFlags
#define RE_DmaBufferLength              DmaBufferLength
#define RE_DmaDescriptorFlags           DmaDescriptorFlags
#define RE_DmaChannel                   DmaChannel
#define RE_DmaAddress                   DmaAddress
#define RE_DmaInternalConfig            DmaInternalConfig
#define RE_DmaEndpoint                  DmaEndpoint
#define RE_DmaOptionalConfig            DmaOptionalConfig
#define RE_DmaChipSpecificConfig        DmaChipSpecificConfig
#define RE_DmaDescriptor                DmaDescriptor
#define RE_DmaChannelConfig             DmaChannelConfig
#define RE_DmaDescriptorConfig          DmaDescriptorConfig
#define RE_DmaIrqHandler                DmaIrqHandler
#define RE_DmaExIrqHandler              DmaExIrqHandler

/* ==========================================================================*/
/* DO NOT use these declarations directly in platform software. Use the REI 
 * defines instead */
#define __BASETYPE_H__
#include "dma.h"

#endif // __REI_DMA_H__
