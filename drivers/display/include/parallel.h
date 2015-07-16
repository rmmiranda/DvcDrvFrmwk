/*****************************************************************************
 * Filename:    parallel.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Victor Chen
 *
 * Created:     July 16, 2010
 *
 * Description: This header file defines display subsystem public interface for with parallel LCD.  
 * 
 *
 *****************************************************************************/
#ifndef __DISPLAY_LCDC_H__
#define __DISPLAY_LCDC_H__

#include "display.h"


/***************************************************************************
 * Function:      DispParallelInit
 * Description:  API to retrive DSS parallel interface configurations such as GPIOs and initialize any 
 *                   variables here.
 *
 * Note:          Derived from LcdCtrlInit() on Nessus TavorTD LCD driver
 **************************************************************************/
BOOL DispParallelInit(void);

/***************************************************************************
 * Function:      DispParallelEnable
 * Description:  API to initialize DSS parallel interface  
 *              
 * Note:          Derived from LcdCtrlInit() on Nessus TavorTD LCD driver
 **************************************************************************/
BOOL DispParallelEnable(void);

/***************************************************************************
 * Function:      DispParallelDisable
 * Description:  API to disable paralle interface   
 *              
 * Note:          Derived from LcdCtrlInit() on Nessus TavorTD LCD driver
 **************************************************************************/
BOOL DispParallelDisable(void);

/***************************************************************************
 * Function:      DispParallelSendVideoPacket
 * Description:  API to send video packet to paralle interface   
 *              
 * Note:          Derived from SVP file 
 **************************************************************************/
void DispParallelSendVideoPacket(DisplayId_t displayId);

/***************************************************************************
 * Function:      DispParallelReadVideoPacket
 * Description:  API to read video packet (pixels) from LCD mdoule   
 *              
 * Return:        actual number of pixels  
 **************************************************************************/
DWORD DispParallelReadVideoPacket(DisplayId_t displayId, BYTE *buf, DWORD pixels);

/***************************************************************************
 * Function:      DispParallelWriteData
 * Description:   Write data to parallel interface.
 *              
 * Note:          Internally, A0 bit on the bus is used to control either data or command. It's derived from 
 *                  LcdCtrlInit() on Nessus TavorTD LCD driver
 **************************************************************************/
void DispParallelWriteData(LcdModule_t *lcdModule, WORD data);

/***************************************************************************
 * Function:     DispParallelWriteCmd
 * Description:  Write command to parallel interface  
 *              
 * Note:          Derived from LcdCtrlInit() on Nessus TavorTD LCD driver
 **************************************************************************/
void DispParallelWriteCmd(LcdModule_t *lcdModule, WORD data);

/***************************************************************************
 * Function:      DispParallelReadData
 * Description:  Read pixel data back from parallel interface  
 *              
 * Note:          Derived from LcdCtrlInit() on Nessus TavorTD LCD driver
 **************************************************************************/
DWORD DispParallelReadData(LcdModule_t *lcdModule);

/***************************************************************************
 * Function:      DispParallelReadCmd
 * Description:   Read non-pixel data back parallel interface  
 *              
 * Note:          Derived from LcdCtrlInit() on Nessus TavorTD LCD driver
 **************************************************************************/
DWORD DispParallelReadCmd(LcdModule_t *lcdModule);

/***************************************************************************
 * Function:      DispParallelConfigTiming
 * Description:   Config parallel interface timings based on peripheral LCD module
 *              
 * Note:          Derived from LcdCtrlInit() on Nessus TavorTD LCD driver
 **************************************************************************/
void DispParallelConfigTiming(LcdModule_t *lcdModule);
/* Byte or WORD */

#endif
