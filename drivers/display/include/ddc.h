/*****************************************************************************
 * Filename:    ddc.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     May 2010
 *
 * Description: The global header file for the DDC channel. Developed
 *              specifically for the platform software driver framework.
 *
 *****************************************************************************/

#ifndef __DDC_H__
#define __DDC_H__

/*
 ***************************************************
 ***************************************************
 *
 * Data structure definitions
 *
 ***************************************************
 ***************************************************
 */

/* Detailed Timing Descriptor (DTD) Block */
typedef struct {
	DWORD PixelClock;
	WORD HActive;
	WORD HBlanking;
	WORD VActive;
	WORD VBlanking;
	WORD HSyncOffs;
	WORD HSyncPulseWidth;
	WORD VSyncOffs;
	WORD VSyncPulseWidth;
	WORD HImageSize;
	WORD VImageSize;
	WORD HBorder;
	WORD VBorder;
	BOOL Interlaced;
	BOOL Stereo;
	BOOL SSync;
	BOOL VSync;
	BOOL HSync;
	BOOL StereoMode;
	BYTE BlockType;
	struct {
		char  Text[13];
		DWORD MinVFrequency;
		DWORD MaxVFrequency;
		DWORD MinHFrequency;
		DWORD MaxHFrequency;
		DWORD PixelClock;
		WORD  GTFToggle;
		DWORD StartHFrequency;
		BYTE  C;
		WORD  M;
		BYTE  K;
		BYTE  J;
		struct {
			BOOL Flag;
			WORD WhiteX;
			WORD WhiteY;
			BYTE Gamma;
		} WhiteIndex[2];
		struct {
			BYTE HResolution;
			BYTE VResolution;
			BYTE AspectRatio;
			BYTE VFrequency;
		} StdTimingID[6];
	} BlockContents;
} DtdBlock_t;

/* VESA EDID Block */
typedef struct {
	QWORD Header;
	char ManufacturerID[4];
	WORD ProductIDCode;
	DWORD SerialNumber;
	BYTE ManufactureWeek;
	WORD ManufactureYear;
	BYTE VersionNumber;
	BYTE RevisionNumber;
	struct {
		BOOL Digital;
		BYTE Level;
		BOOL BlankToBlackSetup;
		BOOL SeparateSyncs;
		BOOL CompositeSync;
		BOOL SyncOnGreen;
		BOOL DFP1xCompatible;
		BYTE MaxHImageSize;
		BYTE MaxVImageSize;
		BYTE DisplayGamma;
		BOOL StandbySupport;
		BOOL SuspendSupport;
		BOOL ActiveOffSupport;
		BYTE DisplayTypeSupported;
		BOOL StdColorSpaceSupport;
		BOOL PrefTimingModeSupport;
		BOOL DefaultGTFSupported;
	} VideoInput;
	struct {
		WORD RedX;
		WORD RedY;
		WORD GreenX;
		WORD GreenY;
		WORD BlueX;
		WORD BlueY;
		WORD WhiteX;
		WORD WhiteY;
	} Chromacity;
	struct {
		BOOL Timing_720x400_70Hz;
		BOOL Timing_720x400_88Hz;
		BOOL Timing_640x480_60Hz;
		BOOL Timing_640x480_67Hz;
		BOOL Timing_640x480_72Hz;
		BOOL Timing_640x480_75Hz;
		BOOL Timing_800x600_56Hz;
		BOOL Timing_800x600_60Hz;
		BOOL Timing_800x600_72Hz;
		BOOL Timing_800x600_75Hz;
		BOOL Timing_832x624_75Hz;
		BOOL Timing_1024x768i_87Hz;
		BOOL Timing_1024x768_60Hz;
		BOOL Timing_1024x768_70Hz;
		BOOL Timing_1024x768_75Hz;
		BOOL Timing_1280x1024_75Hz;
		BOOL Timing_1152x870_75Hz;
	} EstTimings;
	struct {
		BYTE HResolution;
		BYTE VResolution;
		BYTE AspectRatio;
		BYTE VFrequency;
	} StdTimingID[8];
	DtdBlock_t Descriptors[4];
	BYTE ExtensionFlag;
	BYTE Checksum;
} VesaBlock_t;

/* CEA EDID Extension Block */
typedef struct {
	BYTE ExtensionTag;
	BYTE RevisionNumber;
	BYTE DesciptorOffs;
	BOOL UnderscanSupport;
	BOOL BasicAudioSupport;
	BOOL Y444Support;
	BOOL Y422Support;
	BYTE TotalDescriptors;
	BYTE DataBlocks[123];
	struct {
		BOOL CodeSupported;
		BYTE Channels;
		BOOL SF192KHzSupport;
		BOOL SF176KHzSupport;
		BOOL SF96KHzSupport;
		BOOL SF88KHzSupport;
		BOOL SF48KHzSupport;
		BOOL SF44KHzSupport;
		BOOL SF32KHzSupport;
		BOOL SS24BitSupport;
		BOOL SS20BitSupport;
		BOOL SS16BitSupport;
		DWORD MaxBitrate;
	} AudioBlocks[16];
	struct {
		BOOL CodeSupported;
		BOOL NativeResolution;
	} VideoBlocks[VIC_MAX];
	struct {
		DWORD IEEERegID;
		WORD PhysAddr;
		BOOL AISupport;
		BOOL DC48Support;
		BOOL DC36Support;
		BOOL DC30Support;
		BOOL DCY444Support;
		BOOL DVISupport;
		DWORD MaxTMDSClock;
		BYTE VideoLatency;
		BYTE AudioLatency;
		BYTE VideoILatency;
		BYTE AudioILatency;
	} VendorBlock;
	struct {
		BOOL RLC_RRC;
		BOOL FLC_FRC;
		BOOL RC;
		BOOL RL_RR;
		BOOL FC;
		BOOL LFE;
		BOOL FL_FR;
	} SpeakerBlock;
	DtdBlock_t Descriptors[6];
	BYTE Checksum;
} CeaBlock_t;

/* EDID with CEA extension  */
typedef struct {
	VesaBlock_t VESA;
	CeaBlock_t  CEA;
} Edid_t;

/*
 ***************************************************
 ***************************************************
 *
 * Exportable function definitions
 *
 ***************************************************
 ***************************************************
 */

/*
 * DdcGetEdid
 *
 * Reads the extended display identification (EDID) data
 * from the HDMI receiver and stores such information into
 * the task's corresponding data structure.
 *
 * @param hdmi - The hdmi controller requiring the EDID
 * 				 data structure.
 *
 * @return TRUE if the operation is successful. Otherwise
 *         returns FALSE.
 */
BOOL DdcGetEdid(HdmiCtrl_t *hdmi);

#endif // __DDC_H__
