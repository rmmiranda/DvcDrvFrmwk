/*****************************************************************************
 * Filename:    rei.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Jan 07, 2009
 *
 * Description: 
 *
 ****************************************************************************/
 
#ifndef __REI_H__
#define __REI_H__

#include "../rei/include/rei_basetype.h"

#ifdef MINIOS
    #include "../rei/include/rei_minios_gpio.h"
    #include "../rei/include/rei_minios_clocks.h"
#endif
#ifdef NESSUS
    #include "../rei/include/rei_gpio.h"
    #include "../rei/include/rei_dma.h"
    #include "../rei/include/rei_clocks.h"
#endif

#include "../rei/include/rei_delay.h"
#include "../rei/include/rei_message.h"
#include "../rei/include/rei_critical.h"
#include "../rei/include/rei_interrupt.h"
#include "../rei/include/rei_debug.h"
#include "../rei/include/rei_timer.h"
#include "../rei/include/rei_rtc.h"
#include "../rei/include/rei_metrics.h"
#include "../test/include/rei_test_cmds.h"

/* Register access macro  (Copied from file armos/common/include/register.h)*/
#define WriteRegByte( REG, VALUE )      ((*((IOBYTE*)(REG)))=(VALUE))
#define WriteRegWord( REG, VALUE )      ((*((IOWORD*)(REG)))=(VALUE))
#define WriteRegDword( REG, VALUE )     ((*((IODWORD*)(REG)))=(VALUE))
#define WriteRegQword( REG, VALUE )     ((*((IOQWORD*)(REG)))=(VALUE))

#define WriteRegByteAnd( REG, VALUE )   ((*((IOBYTE*)(REG)))&=(VALUE))
#define WriteRegWordAnd( REG, VALUE )   ((*((IOWORD*)(REG)))&=(VALUE))
#define WriteRegDwordAnd( REG, VALUE )  ((*((IODWORD*)(REG)))&=(VALUE))

#define WriteRegByteOr( REG, VALUE )    ((*((IOBYTE*)(REG)))|=(VALUE))
#define WriteRegWordOr( REG, VALUE )    ((*((IOWORD*)(REG)))|=(VALUE))
#define WriteRegDwordOr( REG, VALUE )   ((*((IODWORD*)(REG)))|=(VALUE))

#define WriteRegDwordMaskAnd( REG, MASK, VALUE )  ((*((IODWORD*)(REG))) = (*((IODWORD*)(REG)) & ~(MASK)) & (VALUE))
#define WriteRegDwordMaskOr( REG, MASK, VALUE )   ((*((IODWORD*)(REG))) = (*((IODWORD*)(REG)) & ~(MASK)) | (VALUE))

#define ReadRegByte( REG ) (*((IOBYTE*)(REG)))
#define ReadRegWord( REG ) (*((IOWORD*)(REG)))
#define ReadRegDword( REG ) (*((IODWORD*)(REG)))
#define ReadRegQword( REG ) (*((IOQWORD*)(REG)))

#define ReadDummyRegByte( REG ) { BYTE _temp; \
                                  _temp = ReadRegByte( REG ); \
                                  _temp = _temp; }

#define ReadDummyRegWord( REG ) { WORD _temp; \
                                  _temp = ReadRegWord( REG ); \
                                  _temp = _temp; }

#define ReadDummyRegDword( REG ) { DWORD _temp; \
                                  _temp = ReadRegDword( REG ); \
                                  _temp = _temp; }

#define ReadDummyRegQword( REG ) { QWORD _temp; \
                                  _temp = ReadRegQword( REG ); \
                                  _temp = _temp; }

/* 
 * RE interface functions 
 */
#ifdef BOOTROM
extern RE_KickWatchdog(void);
#endif
#endif

/*
 *  Rei init function
 */
void ReiInit (void);
