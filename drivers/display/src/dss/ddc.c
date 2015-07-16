/*****************************************************************************
 * Filename:    ddc.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     Oct 6, 2010
 *
 * Description: Display subsystem HDMI DDC channel implementation
 *
 *****************************************************************************/

#include "rei.h"
#include "framework.h"
#include "hdmi.h"
#include "ddc.h"
#include "i2c.h"

/*
 ***************************************************
 ***************************************************
 *
 * Macro definitions
 *
 ***************************************************
 ***************************************************
 */

#define I2C_ADDR_E_DDC_EDID	0xA0
#define I2C_ADDR_E_DDC_EXT	0x60
#define I2C_ADDR_HDCP_PRI	0x74
#define I2C_ADDR_HDCP_SEC	0x76

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
BOOL DdcGetEdid(HdmiCtrl_t *hdmi)
{
	Edid_t *edid;
	BYTE i2cBuffer[128];
	BYTE i2cSegment = 0;
	BYTE checksum = 0;
	DWORD retval;
	DWORD index;
	DWORD index2;

	/* hdmi tx should be in the connected state */
	if( hdmi->State != HDMITX_STATE_CONNECTED ) {
		WARN( "Driver is not in the appropriate state to read the EDID." );
		return FALSE;
	}

	/* get a handle to the hdmi edid data structure */
	edid = &(hdmi->EDID);

	/* read VESA EDID block */
	retval = I2cWriteRead(I2C_DEV_DDC, I2C_ADDR_E_DDC_EDID, &i2cSegment, 1, I2C_ADDR_E_DDC_EDID, &i2cBuffer[0], 128);
	if( retval != -1 ) {
		/* verify checksum for data integrity */
		for( index = 0; index < 128; index++ )
			checksum += i2cBuffer[index];
		if( checksum ) {
			WARNN( "Driver read an invalid VESA EDID block (checksum: 0x%08x).", checksum );
			return FALSE;
		}
	} else {
		WARN( "Driver could not read a valid VESA EDID block." );
		return FALSE;
	}

	PRINT( "VESA EDID block retrieved." );

	/* process the VESA EDID block header */
	for( index=0; index<8; index++ ) {
		*(((BYTE *)(&edid->VESA.Header))+index) = i2cBuffer[index];
	}

	/* process the VESA EDID block, product and manufacturer info */
	edid->VESA.ManufacturerID[0] = (char)(((i2cBuffer[8] & 0x7C) >> 2) + (BYTE)('A') - 1);
	edid->VESA.ManufacturerID[1] = (char)(((i2cBuffer[8] & 0x03) + ((i2cBuffer[9] & 0xE0) >> 5)) + (BYTE)('A') - 1);
	edid->VESA.ManufacturerID[2] = (char)((i2cBuffer[9] & 0x1F) + (BYTE)('A') - 1);
	edid->VESA.ManufacturerID[3] = '\n';
	edid->VESA.ProductIDCode = i2cBuffer[10] + (i2cBuffer[11] << 8);
	edid->VESA.SerialNumber = i2cBuffer[12] + (i2cBuffer[13] << 8) + (i2cBuffer[14] << 16) + (i2cBuffer[15] << 24);
	edid->VESA.ManufactureWeek = i2cBuffer[16];
	edid->VESA.ManufactureYear = 1990 + i2cBuffer[17];
	edid->VESA.VersionNumber = i2cBuffer[18];
	edid->VESA.RevisionNumber = i2cBuffer[19];

	/* process the VESA EDID video input subblock */
	edid->VESA.VideoInput.Digital = ((i2cBuffer[20] & 0x80) ? TRUE : FALSE);
	edid->VESA.VideoInput.Level = ((i2cBuffer[20] & 0x60) >> 5);
	edid->VESA.VideoInput.BlankToBlackSetup = ((i2cBuffer[20] & 0x10) ? TRUE : FALSE );
	edid->VESA.VideoInput.SeparateSyncs = ((i2cBuffer[20] & 0x08) ? TRUE : FALSE);
	edid->VESA.VideoInput.CompositeSync = ((i2cBuffer[20] & 0x04) ? TRUE : FALSE);
	edid->VESA.VideoInput.SyncOnGreen = ((i2cBuffer[20] & 0x02) ? TRUE : FALSE);
	edid->VESA.VideoInput.DFP1xCompatible = ((i2cBuffer[20] & 0x01) ? TRUE : FALSE);
	edid->VESA.VideoInput.MaxHImageSize = i2cBuffer[21];
	edid->VESA.VideoInput.MaxVImageSize = i2cBuffer[22];
	edid->VESA.VideoInput.DisplayGamma = i2cBuffer[23];
	edid->VESA.VideoInput.StandbySupport = ((i2cBuffer[24] & 0x80) ? TRUE : FALSE);
	edid->VESA.VideoInput.SuspendSupport = ((i2cBuffer[24] & 0x40) ? TRUE : FALSE);
	edid->VESA.VideoInput.ActiveOffSupport = ((i2cBuffer[24] & 0x20) ? TRUE : FALSE);
	edid->VESA.VideoInput.DisplayTypeSupported = ((i2cBuffer[24] & 0x18) >> 3);
	edid->VESA.VideoInput.StdColorSpaceSupport = ((i2cBuffer[24] & 0x04) ? TRUE : FALSE);
	edid->VESA.VideoInput.PrefTimingModeSupport = ((i2cBuffer[24] & 0x02) ? TRUE : FALSE);
	edid->VESA.VideoInput.DefaultGTFSupported = ((i2cBuffer[24] & 0x01) ? TRUE : FALSE);

	/* process the VESA EDID chromacity coordinates subblock */
	edid->VESA.Chromacity.RedX = ((i2cBuffer[27]&0xC0) << 2) + (i2cBuffer[29]&0xC0) + ((i2cBuffer[31]&0xC0) >> 2) + ((i2cBuffer[33]&0xC0) >> 4) + ((i2cBuffer[25]&0xC0) >> 6);
	edid->VESA.Chromacity.RedY = ((i2cBuffer[27]&0x30) << 4) + ((i2cBuffer[29]&0x30) << 2) + (i2cBuffer[31]&0x30) + ((i2cBuffer[33]&0x30) >> 2) + ((i2cBuffer[25]&0x30) >> 4);
	edid->VESA.Chromacity.GreenX = ((i2cBuffer[27]&0x0C) << 6) + ((i2cBuffer[29]&0x0C) << 4) + ((i2cBuffer[31]&0x0C) << 2) + (i2cBuffer[33]&0x0C) + ((i2cBuffer[25]&0x0C) >> 2);
	edid->VESA.Chromacity.GreenY = ((i2cBuffer[27]&0x03) << 8) + ((i2cBuffer[29]&0x03) << 6) + ((i2cBuffer[31]&0x03) << 4) + ((i2cBuffer[33]&0x03) << 2) + (i2cBuffer[25]&0x03);
	edid->VESA.Chromacity.BlueX = ((i2cBuffer[28]&0xC0) << 2) + (i2cBuffer[30]&0xC0) + ((i2cBuffer[32]&0xC0) >> 2) + ((i2cBuffer[34]&0xC0) >> 4) + ((i2cBuffer[26]&0xC0) >> 6);
	edid->VESA.Chromacity.BlueY = ((i2cBuffer[28]&0x30) << 4) + ((i2cBuffer[30]&0x30) << 2) + (i2cBuffer[32]&0x30) + ((i2cBuffer[34]&0x30) >> 2) + ((i2cBuffer[26]&0x30) >> 4);
	edid->VESA.Chromacity.WhiteX = ((i2cBuffer[28]&0x0C) << 6) + ((i2cBuffer[30]&0x0C) << 4) + ((i2cBuffer[32]&0x0C) << 2) + (i2cBuffer[34]&0x0C) + ((i2cBuffer[26]&0x0C) >> 2);
	edid->VESA.Chromacity.WhiteY = ((i2cBuffer[28]&0x03) << 8) + ((i2cBuffer[30]&0x03) << 6) + ((i2cBuffer[32]&0x03) << 4) + ((i2cBuffer[34]&0x03) << 2) + (i2cBuffer[26]&0x03);

	/* process the VESA EDID established timings subblock */
	edid->VESA.EstTimings.Timing_720x400_70Hz = ((i2cBuffer[35] & 0x80) ? TRUE : FALSE);
	edid->VESA.EstTimings.Timing_720x400_88Hz = ((i2cBuffer[35] & 0x40) ? TRUE : FALSE);
	edid->VESA.EstTimings.Timing_640x480_60Hz = ((i2cBuffer[35] & 0x20) ? TRUE : FALSE);
	edid->VESA.EstTimings.Timing_640x480_67Hz = ((i2cBuffer[35] & 0x10) ? TRUE : FALSE);
	edid->VESA.EstTimings.Timing_640x480_72Hz = ((i2cBuffer[35] & 0x08) ? TRUE : FALSE);
	edid->VESA.EstTimings.Timing_640x480_75Hz = ((i2cBuffer[35] & 0x04) ? TRUE : FALSE);
	edid->VESA.EstTimings.Timing_800x600_56Hz = ((i2cBuffer[35] & 0x02) ? TRUE : FALSE);
	edid->VESA.EstTimings.Timing_800x600_60Hz = ((i2cBuffer[35] & 0x01) ? TRUE : FALSE);
	edid->VESA.EstTimings.Timing_800x600_72Hz = ((i2cBuffer[35] & 0x80) ? TRUE : FALSE);
	edid->VESA.EstTimings.Timing_800x600_75Hz = ((i2cBuffer[36] & 0x40) ? TRUE : FALSE);
	edid->VESA.EstTimings.Timing_832x624_75Hz = ((i2cBuffer[36] & 0x20) ? TRUE : FALSE);
	edid->VESA.EstTimings.Timing_1024x768i_87Hz = ((i2cBuffer[36] & 0x10) ? TRUE : FALSE);
	edid->VESA.EstTimings.Timing_1024x768_60Hz = ((i2cBuffer[36] & 0x08) ? TRUE : FALSE);
	edid->VESA.EstTimings.Timing_1024x768_70Hz = ((i2cBuffer[36] & 0x04) ? TRUE : FALSE);
	edid->VESA.EstTimings.Timing_1024x768_75Hz = ((i2cBuffer[36] & 0x02) ? TRUE : FALSE);
	edid->VESA.EstTimings.Timing_1280x1024_75Hz = ((i2cBuffer[36] & 0x01) ? TRUE : FALSE);
	edid->VESA.EstTimings.Timing_1152x870_75Hz = ((i2cBuffer[37] & 0x01) ? TRUE : FALSE);

	/* process the VESA EDID standard timings subblock */
	for( index=0; index<8; index++ ) {
		BYTE *HResolution = &(edid->VESA.StdTimingID[index].HResolution);
		BYTE *VResolution = &(edid->VESA.StdTimingID[index].VResolution);
		BYTE *AspectRatio = &(edid->VESA.StdTimingID[index].AspectRatio);
		BYTE *VFrequency = &(edid->VESA.StdTimingID[index].VFrequency);
		*HResolution = (i2cBuffer[(2*index)+38] * 8) + 248;
		*VFrequency = (i2cBuffer[(2*index)+39] & 0x3F) + 60;
		*AspectRatio = i2cBuffer[(2*index)+39] >> 6;
		switch( *AspectRatio ) {
			case 0x00: *VResolution = (*HResolution * 8) / 5; break;
			case 0x01: *VResolution = (*HResolution * 4) / 3; break;
			case 0x10: *VResolution = (*HResolution * 5) / 4; break;
			case 0x11: *VResolution = (*HResolution * 16) / 9; break;
			default:
				break;
		}

	}

	/* process the VESA EDID detailed timing descriptors */
	for( index=0; index<4; index++ ) {
		int offset = (18 * index);
		edid->VESA.Descriptors[index].PixelClock = (i2cBuffer[offset+54] + (i2cBuffer[offset+55] << 8)) * 10000;
		if( edid->VESA.Descriptors[index].PixelClock ) {
			edid->VESA.Descriptors[index].HActive = i2cBuffer[offset+56] + ((i2cBuffer[offset+58] & 0xF0) << 4);
			edid->VESA.Descriptors[index].HBlanking = i2cBuffer[offset+57] + ((i2cBuffer[offset+58] & 0x0F) << 8);
			edid->VESA.Descriptors[index].VActive = i2cBuffer[offset+59] + ((i2cBuffer[offset+61] & 0xF0) << 4);
			edid->VESA.Descriptors[index].VBlanking = i2cBuffer[offset+60] + ((i2cBuffer[offset+61] & 0x0F) << 8);
			edid->VESA.Descriptors[index].HSyncOffs = i2cBuffer[offset+62] + ((i2cBuffer[offset+65] & 0xC0) << 2);
			edid->VESA.Descriptors[index].HSyncPulseWidth = i2cBuffer[offset+63] + ((i2cBuffer[offset+65] & 0x30) << 4);
			edid->VESA.Descriptors[index].VSyncOffs = ((i2cBuffer[offset+64] & 0xF0) >> 4) + ((i2cBuffer[offset+65] & 0x0C) << 2);
			edid->VESA.Descriptors[index].VSyncPulseWidth = (i2cBuffer[offset+64] & 0x0F) + ((i2cBuffer[offset+65] & 0x03) << 4);
			edid->VESA.Descriptors[index].HImageSize = i2cBuffer[offset+66] + ((i2cBuffer[offset+68] & 0xF0) << 4);
			edid->VESA.Descriptors[index].VImageSize = i2cBuffer[offset+67] + ((i2cBuffer[offset+68] & 0x0F) << 8);
			edid->VESA.Descriptors[index].HBorder = i2cBuffer[offset+69];
			edid->VESA.Descriptors[index].VBorder = i2cBuffer[offset+70];
			edid->VESA.Descriptors[index].Interlaced = (i2cBuffer[offset+71] & 0x80) ? TRUE : FALSE;
			edid->VESA.Descriptors[index].Stereo = (i2cBuffer[offset+71] & 0x60) ? TRUE : FALSE;
			edid->VESA.Descriptors[index].SSync = (i2cBuffer[offset+71] & 0x18) ? TRUE : FALSE;
			edid->VESA.Descriptors[index].VSync = (i2cBuffer[offset+71] & 0x04) ? TRUE : FALSE;
			edid->VESA.Descriptors[index].HSync = (i2cBuffer[offset+71] & 0x02) ? TRUE : FALSE;
			edid->VESA.Descriptors[index].StereoMode = (i2cBuffer[offset+71] & 0x01) ? TRUE : FALSE;
		} else {
			edid->VESA.Descriptors[index].BlockType = i2cBuffer[offset+57];
			switch( edid->VESA.Descriptors[index].BlockType ) {
				case 0xFF:
				case 0xFE:
				case 0xFC:
					memcpy((void *)edid->VESA.Descriptors[index].BlockContents.Text, (void *)(i2cBuffer+offset+59), 13);
					break;
				case 0xFD:
					edid->VESA.Descriptors[index].BlockContents.MinVFrequency = i2cBuffer[offset+59];
					edid->VESA.Descriptors[index].BlockContents.MaxVFrequency = i2cBuffer[offset+60];
					edid->VESA.Descriptors[index].BlockContents.MinHFrequency = i2cBuffer[offset+61] * 1000;
					edid->VESA.Descriptors[index].BlockContents.MaxHFrequency = i2cBuffer[offset+62] * 1000;
					edid->VESA.Descriptors[index].BlockContents.PixelClock = i2cBuffer[offset+63] * 10 * 1000000;
					edid->VESA.Descriptors[index].BlockContents.GTFToggle = i2cBuffer[offset+64] + (i2cBuffer[offset+65] << 8);
					edid->VESA.Descriptors[index].BlockContents.StartHFrequency = i2cBuffer[offset+66];
					edid->VESA.Descriptors[index].BlockContents.C = i2cBuffer[offset+67] / 2;
					edid->VESA.Descriptors[index].BlockContents.GTFToggle = i2cBuffer[offset+68] + (i2cBuffer[offset+69] << 8);
					edid->VESA.Descriptors[index].BlockContents.StartHFrequency = i2cBuffer[offset+70];
					edid->VESA.Descriptors[index].BlockContents.StartHFrequency = i2cBuffer[offset+71] / 2;
					break;
				case 0xFB:
					edid->VESA.Descriptors[index].BlockContents.WhiteIndex[0].Flag = (i2cBuffer[offset+59]) ? TRUE : FALSE;
					edid->VESA.Descriptors[index].BlockContents.WhiteIndex[0].WhiteX = ((i2cBuffer[offset+61]&0x0C) << 2) + (i2cBuffer[offset+62]&0x0C) + ((i2cBuffer[offset+60]&0x0C) >> 2);
					edid->VESA.Descriptors[index].BlockContents.WhiteIndex[0].WhiteY = ((i2cBuffer[offset+61]&0x03) << 4) + ((i2cBuffer[offset+62]&0x03) << 2) + (i2cBuffer[offset+60]&0x03);
					edid->VESA.Descriptors[index].BlockContents.WhiteIndex[0].Gamma = (i2cBuffer[offset+63] / 100) + 1;
					edid->VESA.Descriptors[index].BlockContents.WhiteIndex[1].Flag = (i2cBuffer[offset+64]) ? TRUE : FALSE;
					edid->VESA.Descriptors[index].BlockContents.WhiteIndex[1].WhiteX = ((i2cBuffer[offset+66]&0x0C) << 2) + (i2cBuffer[offset+67]&0x0C) + ((i2cBuffer[offset+65]&0x0C) >> 2);
					edid->VESA.Descriptors[index].BlockContents.WhiteIndex[1].WhiteY = ((i2cBuffer[offset+66]&0x03) << 4) + ((i2cBuffer[offset+67]&0x03) << 2) + (i2cBuffer[offset+65]&0x03);
					edid->VESA.Descriptors[index].BlockContents.WhiteIndex[1].Gamma = (i2cBuffer[offset+68] / 100) + 1;
					break;
				case 0xFA:
					for( index2=0; index2<6; index2++ ) {
						BYTE *HResolution = &(edid->VESA.Descriptors[index].BlockContents.StdTimingID[index2].HResolution);
						BYTE *VResolution = &(edid->VESA.Descriptors[index].BlockContents.StdTimingID[index2].VResolution);
						BYTE *AspectRatio = &(edid->VESA.Descriptors[index].BlockContents.StdTimingID[index2].AspectRatio);
						BYTE *VFrequency = &(edid->VESA.Descriptors[index].BlockContents.StdTimingID[index2].VFrequency);
						*HResolution = (i2cBuffer[(2*index2)+offset+59] * 8) + 248;
						*VFrequency = (i2cBuffer[(2*index2)+offset+60] & 0x3F) + 60;
						*AspectRatio = i2cBuffer[(2*index2)+offset+60] >> 6;
						switch( *AspectRatio ) {
							case 0x00: *VResolution = (*HResolution * 8) / 5; break;
							case 0x01: *VResolution = (*HResolution * 4) / 3; break;
							case 0x10: *VResolution = (*HResolution * 5) / 4; break;
							case 0x11: *VResolution = (*HResolution * 16) / 9; break;
							default:
								break;
						}
					}
					break;
			}
		}
	}

	/* process the VESA EDID extension flag and block checksum */
	edid->VESA.ExtensionFlag = i2cBuffer[126];
	edid->VESA.Checksum = i2cBuffer[127];

	/* look for the first CEA extension block */
	for( i2cSegment = 0; i2cSegment < edid->VESA.ExtensionFlag; i2cSegment++ ) {
		retval = I2cWriteRead(I2C_DEV_DDC, I2C_ADDR_E_DDC_EXT, &i2cSegment, 1, I2C_ADDR_E_DDC_EDID, &i2cBuffer[0], 128);
		if( retval != -1 ) {
			if( i2cBuffer[0] == 0x02 ) {
				/* found the first CEA extension block, check data integrity */
				for( index = 0; index < 128; index++ )
					checksum += i2cBuffer[index];
				if( checksum ) {
					WARNN( "Driver read an invalid CEA EDID extension block (checksum: 0x%08x).", checksum );
					return FALSE;
				}
				break;
			} else {
				/* check the next extension */
				continue;
			}
		} else {
			WARN( "Driver could not read a valid CEA EDID extenstion block." );
			return FALSE;
		}
	}

	/* fail if no CEA EDID extension block is found */
	if( i2cSegment == edid->VESA.ExtensionFlag ) {
		WARN( "Driver could not found a valid CEA EDID extension block.");
		return FALSE;
	}

	PRINT( "CEA EDID extension block found and retrieved." );

	/* process the CEA EDID extension block info */
	edid->CEA.ExtensionTag = i2cBuffer[0];
	edid->CEA.RevisionNumber = i2cBuffer[1];
	edid->CEA.DesciptorOffs  = i2cBuffer[2];
	edid->CEA.UnderscanSupport = (i2cBuffer[3] & 0x80) ? TRUE : FALSE;
	edid->CEA.BasicAudioSupport = (i2cBuffer[3] & 0x40) ? TRUE : FALSE;
	edid->CEA.Y444Support = (i2cBuffer[3] & 0x20) ? TRUE : FALSE;
	edid->CEA.Y422Support = (i2cBuffer[3] & 0x10) ? TRUE : FALSE;
	edid->CEA.TotalDescriptors = (i2cBuffer[3] & 0x0F);

	/* process the CEA EDID extension data block collections */
	if( edid->CEA.DesciptorOffs > 0x04 ) {
		/* there are data blocks present */
		BYTE *nextDB, *currDB = &(i2cBuffer[4]);
		while( currDB < (BYTE *)&(i2cBuffer[edid->CEA.DesciptorOffs]) ) {
			nextDB = (BYTE *)(currDB + (*currDB & 0x1F) + 1);
			switch( (*currDB & 0xE0) >> 5 ) {
				/* audio data block */
				case 0x01:
					for( currDB += 1; currDB < nextDB; currDB += 3) {
						index = (*(currDB+0) & 0x78) >> 3;
						edid->CEA.AudioBlocks[index].CodeSupported = TRUE;
						edid->CEA.AudioBlocks[index].Channels = *(currDB+0) & 0x07;
						edid->CEA.AudioBlocks[index].SF192KHzSupport = (*(currDB+1) & 0x40) ? TRUE : FALSE;
						edid->CEA.AudioBlocks[index].SF176KHzSupport = (*(currDB+1) & 0x20) ? TRUE : FALSE;
						edid->CEA.AudioBlocks[index].SF96KHzSupport = (*(currDB+1) & 0x10) ? TRUE : FALSE;
						edid->CEA.AudioBlocks[index].SF88KHzSupport = (*(currDB+1) & 0x08) ? TRUE : FALSE;
						edid->CEA.AudioBlocks[index].SF48KHzSupport = (*(currDB+1) & 0x04) ? TRUE : FALSE;
						edid->CEA.AudioBlocks[index].SF44KHzSupport = (*(currDB+1) & 0x02) ? TRUE : FALSE;
						edid->CEA.AudioBlocks[index].SF32KHzSupport = (*(currDB+1) & 0x01) ? TRUE : FALSE;
						if( index == 1 ) {
							edid->CEA.AudioBlocks[index].SS24BitSupport = (*(currDB+2) & 0x40) ? TRUE : FALSE;
							edid->CEA.AudioBlocks[index].SS20BitSupport = (*(currDB+2) & 0x40) ? TRUE : FALSE;
							edid->CEA.AudioBlocks[index].SS16BitSupport = (*(currDB+2) & 0x40) ? TRUE : FALSE;
						} else {
							edid->CEA.AudioBlocks[index].MaxBitrate = *(currDB+2) * 8000;
						}
					}
					break;
				/* video data block */
				case 0x02:
					for( currDB += 1; currDB < nextDB; currDB += 1) {
						index = *currDB & 0x7F;
						edid->CEA.VideoBlocks[index].CodeSupported = TRUE;
						edid->CEA.VideoBlocks[index].NativeResolution = (*currDB & 0x80) ? TRUE : FALSE;
					}
					break;
				/* vendor-specific data block */
				case 0x03:
					edid->CEA.VendorBlock.IEEERegID = (DWORD)(*(currDB+1) + (*(currDB+2) << 8) + (*(currDB+3) << 8));
					edid->CEA.VendorBlock.PhysAddr = (WORD)(*(currDB+4) + (*(currDB+5) << 8));
					edid->CEA.VendorBlock.AISupport = (*(currDB+6) & 0x80) ? TRUE : FALSE;
					edid->CEA.VendorBlock.DC48Support = (*(currDB+6) & 0x40) ? TRUE : FALSE;
					edid->CEA.VendorBlock.DC36Support = (*(currDB+6) & 0x20) ? TRUE : FALSE;
					edid->CEA.VendorBlock.DC30Support = (*(currDB+6) & 0x10) ? TRUE : FALSE;
					edid->CEA.VendorBlock.DCY444Support = (*(currDB+6) & 0x08) ? TRUE : FALSE;
					edid->CEA.VendorBlock.DVISupport = (*(currDB+6) & 0x01) ? TRUE : FALSE;
					edid->CEA.VendorBlock.MaxTMDSClock = (DWORD)(*(currDB+7) * 5000000);
					if( (currDB+8) < nextDB ) {
						if( *(currDB+8) & 0x80 ) {
							edid->CEA.VendorBlock.VideoLatency = *(currDB+9);
							edid->CEA.VendorBlock.AudioLatency = *(currDB+10);
							if( *(currDB+8) & 0x40 ) {
								edid->CEA.VendorBlock.VideoILatency = *(currDB+11);
								edid->CEA.VendorBlock.AudioILatency = *(currDB+12);
							}
						}
					}
					break;
				/* speaker allocation data block */
				case 0x04:
					edid->CEA.SpeakerBlock.RLC_RRC = (*(currDB+1) & 0x40) ? TRUE : FALSE;
					edid->CEA.SpeakerBlock.FLC_FRC = (*(currDB+1) & 0x20) ? TRUE : FALSE;
					edid->CEA.SpeakerBlock.RC = (*(currDB+1) & 0x10) ? TRUE : FALSE;
					edid->CEA.SpeakerBlock.RL_RR = (*(currDB+1) & 0x08) ? TRUE : FALSE;
					edid->CEA.SpeakerBlock.FC = (*(currDB+1) & 0x04) ? TRUE : FALSE;
					edid->CEA.SpeakerBlock.LFE = (*(currDB+1) & 0x02) ? TRUE : FALSE;
					edid->CEA.SpeakerBlock.FL_FR = (*(currDB+1) & 0x01) ? TRUE : FALSE;
					break;
			}
			/* advance to the next data block */
			currDB = nextDB;
		}
	}

	/* process the CEA EDID extension detailed timing descriptors */
	if( edid->CEA.DesciptorOffs > 0x00 ) {
		/* there are descriptors present */
		for( index = 0; index < edid->CEA.TotalDescriptors; index++ ) {
			int offset = edid->CEA.DesciptorOffs + (18 * index);
			edid->CEA.Descriptors[index].PixelClock = (i2cBuffer[offset+0] + (i2cBuffer[offset+1] << 8)) * 10000;
			if( edid->CEA.Descriptors[index].PixelClock ) {
				edid->CEA.Descriptors[index].HActive = i2cBuffer[offset+2] + ((i2cBuffer[offset+4] & 0xF0) << 4);
				edid->CEA.Descriptors[index].HBlanking = i2cBuffer[offset+3] + ((i2cBuffer[offset+4] & 0x0F) << 8);
				edid->CEA.Descriptors[index].VActive = i2cBuffer[offset+5] + ((i2cBuffer[offset+7] & 0xF0) << 4);
				edid->CEA.Descriptors[index].VBlanking = i2cBuffer[offset+6] + ((i2cBuffer[offset+7] & 0x0F) << 8);
				edid->CEA.Descriptors[index].HSyncOffs = i2cBuffer[offset+8] + ((i2cBuffer[offset+11] & 0xC0) << 2);
				edid->CEA.Descriptors[index].HSyncPulseWidth = i2cBuffer[offset+9] + ((i2cBuffer[offset+11] & 0x30) << 4);
				edid->CEA.Descriptors[index].VSyncOffs = ((i2cBuffer[offset+10] & 0xF0) >> 4) + ((i2cBuffer[offset+11] & 0x0C) << 2);
				edid->CEA.Descriptors[index].VSyncPulseWidth = (i2cBuffer[offset+10] & 0x0F) + ((i2cBuffer[offset+11] & 0x03) << 4);
				edid->CEA.Descriptors[index].HImageSize = i2cBuffer[offset+12] + ((i2cBuffer[offset+14] & 0xF0) << 4);
				edid->CEA.Descriptors[index].VImageSize = i2cBuffer[offset+13] + ((i2cBuffer[offset+14] & 0x0F) << 8);
				edid->CEA.Descriptors[index].HBorder = i2cBuffer[offset+15];
				edid->CEA.Descriptors[index].VBorder = i2cBuffer[offset+16];
				edid->CEA.Descriptors[index].Interlaced = (i2cBuffer[offset+17] & 0x80) ? TRUE : FALSE;
				edid->CEA.Descriptors[index].Stereo = (i2cBuffer[offset+17] & 0x60) ? TRUE : FALSE;
				edid->CEA.Descriptors[index].SSync = (i2cBuffer[offset+17] & 0x18) ? TRUE : FALSE;
				edid->CEA.Descriptors[index].VSync = (i2cBuffer[offset+17] & 0x04) ? TRUE : FALSE;
				edid->CEA.Descriptors[index].HSync = (i2cBuffer[offset+17] & 0x02) ? TRUE : FALSE;
				edid->CEA.Descriptors[index].StereoMode = (i2cBuffer[offset+17] & 0x01) ? TRUE : FALSE;
			} else {
				edid->CEA.Descriptors[index].BlockType = i2cBuffer[offset+3];
				switch( edid->CEA.Descriptors[index].BlockType ) {
					case 0xFF:
					case 0xFE:
					case 0xFC:
						memcpy((void *)edid->CEA.Descriptors[index].BlockContents.Text, (void *)(i2cBuffer+offset+5), 13);
						break;
					case 0xFD:
						edid->CEA.Descriptors[index].BlockContents.MinVFrequency = i2cBuffer[offset+5];
						edid->CEA.Descriptors[index].BlockContents.MaxVFrequency = i2cBuffer[offset+6];
						edid->CEA.Descriptors[index].BlockContents.MinHFrequency = i2cBuffer[offset+7] * 1000;
						edid->CEA.Descriptors[index].BlockContents.MaxHFrequency = i2cBuffer[offset+8] * 1000;
						edid->CEA.Descriptors[index].BlockContents.PixelClock = i2cBuffer[offset+9] * 10 * 1000000;
						edid->CEA.Descriptors[index].BlockContents.GTFToggle = i2cBuffer[offset+10] + (i2cBuffer[offset+11] << 8);
						edid->CEA.Descriptors[index].BlockContents.StartHFrequency = i2cBuffer[offset+12];
						edid->CEA.Descriptors[index].BlockContents.C = i2cBuffer[offset+13] / 2;
						edid->CEA.Descriptors[index].BlockContents.GTFToggle = i2cBuffer[offset+14] + (i2cBuffer[offset+15] << 8);
						edid->CEA.Descriptors[index].BlockContents.StartHFrequency = i2cBuffer[offset+16];
						edid->CEA.Descriptors[index].BlockContents.StartHFrequency = i2cBuffer[offset+17] / 2;
						break;
					case 0xFB:
						edid->CEA.Descriptors[index].BlockContents.WhiteIndex[0].Flag = (i2cBuffer[offset+5]) ? TRUE : FALSE;
						edid->CEA.Descriptors[index].BlockContents.WhiteIndex[0].WhiteX = ((i2cBuffer[offset+7]&0x0C) << 2) + (i2cBuffer[offset+8]&0x0C) + ((i2cBuffer[offset+6]&0x0C) >> 2);
						edid->CEA.Descriptors[index].BlockContents.WhiteIndex[0].WhiteY = ((i2cBuffer[offset+7]&0x03) << 4) + ((i2cBuffer[offset+8]&0x03) << 2) + (i2cBuffer[offset+6]&0x03);
						edid->CEA.Descriptors[index].BlockContents.WhiteIndex[0].Gamma = (i2cBuffer[offset+9] / 100) + 1;
						edid->CEA.Descriptors[index].BlockContents.WhiteIndex[1].Flag = (i2cBuffer[offset+10]) ? TRUE : FALSE;
						edid->CEA.Descriptors[index].BlockContents.WhiteIndex[1].WhiteX = ((i2cBuffer[offset+12]&0x0C) << 2) + (i2cBuffer[offset+13]&0x0C) + ((i2cBuffer[offset+11]&0x0C) >> 2);
						edid->CEA.Descriptors[index].BlockContents.WhiteIndex[1].WhiteY = ((i2cBuffer[offset+12]&0x03) << 4) + ((i2cBuffer[offset+13]&0x03) << 2) + (i2cBuffer[offset+11]&0x03);
						edid->CEA.Descriptors[index].BlockContents.WhiteIndex[1].Gamma = (i2cBuffer[offset+14] / 100) + 1;
						break;
					case 0xFA:
						for( index2=0; index2<6; index2++ ) {
							BYTE *HResolution = &(edid->CEA.Descriptors[index].BlockContents.StdTimingID[index2].HResolution);
							BYTE *VResolution = &(edid->CEA.Descriptors[index].BlockContents.StdTimingID[index2].VResolution);
							BYTE *AspectRatio = &(edid->CEA.Descriptors[index].BlockContents.StdTimingID[index2].AspectRatio);
							BYTE *VFrequency = &(edid->CEA.Descriptors[index].BlockContents.StdTimingID[index2].VFrequency);
							*HResolution = (i2cBuffer[(2*index2)+offset+5] * 8) + 248;
							*VFrequency = (i2cBuffer[(2*index2)+offset+6] & 0x3F) + 60;
							*AspectRatio = i2cBuffer[(2*index2)+offset+6] >> 6;
							switch( *AspectRatio ) {
								case 0x00: *VResolution = (*HResolution * 8) / 5; break;
								case 0x01: *VResolution = (*HResolution * 4) / 3; break;
								case 0x10: *VResolution = (*HResolution * 5) / 4; break;
								case 0x11: *VResolution = (*HResolution * 16) / 9; break;
								default:
									break;
							}
						}
						break;
				}
			}
		}
	}

	/* process the CEA EDID extension block checksum */
	edid->CEA.Checksum = i2cBuffer[127];

    PRINT( "HDMI TX retrieved EDID successfully." );

	return TRUE;
}
