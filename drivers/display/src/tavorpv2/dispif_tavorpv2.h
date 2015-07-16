/*****************************************************************************
* Filename:    dispif_tavorpv2.h
*
* Copyright 2009, Research In Motion Ltd
*
* Author:      Victor Chen
*
* Created:     Mar 13th, 2009
*
* Description: This header file defines the PV2 Display and DSI controller 
*              register map
*
*****************************************************************************/
#ifndef __DISPIF_TAVORPV2_H__
#define __DISPIF_TAVORPV2_H__


#include "disphal_tavorpv2.h"

#define PV2_HW_VER_Z0
//#define PV2_HW_VER_Z1
//#define PV2_HW_VER_A0

//DSI Command FIFO algorithm - DMA or Software based
#define PV2_DSI_CMDFIFO_ALG_SW
//#define PV2_DSI_CMDFIFO_ALG_DMA


// DMA channel allocation should be moved to "config_dma.h" later
#define MVED_DMA_CHANNEL_0      0
#define MVED_DMA_CHANNEL_1      1
#define MVED_DMA_CHANNEL_2      2
#define MVED_DMA_CHANNEL_3      3


// BTA state machine for Marvel PV2 Z0/Z1 workaround (false controll)
typedef enum
{
    btaIssued = 0x01,
    btaCleared = 0x02,
    btaProcessing = 0x03,
    btaNull = 0x1F,
} DsiBtaSt;

// Flag for DSI event 
typedef enum
{
    //BTA interrupt
    dsiEvtFlagBta = 0x0001,    
    // Smart Panel Rad Flag
    dsiEvtFlagSpRd = 0x0004,
    // Acknowledge with Error Flag
    dsiEvtFlagAckErr = 0x0080,
    // Trig code return flag
    dsiEvtFlagTrig = 0x080000,
} DsiEventFlag;


typedef struct _Tavor2DisplayDriver {
    /* Generic Display Driver structure */
    DispDrv genDispDrv;

    /* Tavor PV2 specific resources */
    RE_CountingSemaphore dsiDmaLock[TAVORPV2_DSI_CTL];
    // MVED DMA descriptor, channel and request used by DSI
    RE_DmaDescriptor *pDsiDmaDesp[TAVORPV2_DSI_CTL];
    DWORD dsiDmaChannel[TAVORPV2_DSI_CTL];
    DWORD dsiDmaRequest[TAVORPV2_DSI_CTL];
    //dispSem;
    //dsiSem;

    /* DSI Command Buffer for DSI command FIFO, driver internal usage only */
    /* unsigned int 16bits for DSI command. To make address aligned with 4 for DMA, set it to a pointer*/
    WORD *dsiCmdFifo[TAVORPV2_DSI_CTL];    
    // Current Command FIFO Index
    WORD dsiCmdIndex[TAVORPV2_DSI_CTL];


    // DSI Command FIFO Buffer that used to as a buffer to program actual HW DSI command FIFO 
    WORD *dsiCmdFifoBuf;    
    // Rear pointer of DSI Command FIFO Buffer Index
    WORD dsiCmdFifoBufIndex;
    // Head pointer of DSI Command FIFO Buffer Index
    WORD dsiCmdFifoBufHead;


    // Display Controller variables
    // Frame Buffer and pointer
    WORD *pDispFrameBuffer;
    DWORD pDispFrmPtr;
    // Pointer for Reading
    DWORD pDispFrmRPtr;

    // DMA descriptor for fectch unit
    DWORD *pDispDmaDescriptor;
	// Window Gen Id and DMA channel for base plane and overlay surface
	DispOverlay dispBasePlane;
	DispDmaChannel dispBasePlaneDmaCh;
	DispOverlay dispOverlay;
	DispDmaChannel dispOverlayDmaCh;
	

    // Event Flag
    DWORD dsiEventFlags[TAVORPV2_DSI_CTL];
    
    // Returned data
    // Acknowledge with Error from DSI DCS device (DCS smart Panel or Deserializer
    WORD dsiAckData[TAVORPV2_DSI_CTL];
    // Read back data from DCS LCD smart panel
    WORD dsiRdData[TAVORPV2_DSI_CTL];
    volatile BYTE *lcdRdData;
	DWORD lcdRdLen;
	

    //Workaround for BTA on PV2 Z0/Z1 to keep BTA states 
    DsiBtaSt dsiBtaSt[TAVORPV2_DSI_CTL];

    // Message for post event
    RE_MESSAGE dispMsg;
 
} Tavor2DispDrv;


#endif 
