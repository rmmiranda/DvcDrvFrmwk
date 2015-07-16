/******************************************************************************
 * Filename:    mmp_irq.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Jackson Mo
 *
 * Created:     Oct 2009
 *
 * Description: Define all HW and SW interrupts used in Nessus.
 *****************************************************************************/
#ifndef __MMP_IRQ_H__
#define __MMP_IRQ_H__

#if !defined( PROCESSOR_FAMILY_MMP )
 #error "This driver is for MMP processor only"
#endif

#if defined( PROCESSOR_MMP2 )
 #include "mmp2_irq.h"
#endif

/*---------------------------------------------------------------------------*/
#endif
