/*****************************************************************************
 * Filename:    hdmi.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     March 2010
 *
 * Description: The global header file for the hdmi driver. Developed
 *              specifically for the platform software driver framework.
 *
 *****************************************************************************/

#ifndef __HDMI_H__
#define __HDMI_H__

/*
 ***************************************************
 ***************************************************
 *
 * Enumerated types definitions
 *
 ***************************************************
 ***************************************************
 */

/* Command Controls */
typedef enum {
	HDMI_CMD_START_TRANSMIT,
	HDMI_CMD_STOP_TRANSMIT,
	HDMI_CMD_SET_AVMUTE,
	HDMI_CMD_CLEAR_AVMUTE,
	HDMI_CMD_CONNECT,
	HDMI_CMD_DISCONNECT,
	HDMI_CMD_SHOW_REGS,
} HdmiCommand_t;

/* Message Events */
typedef enum {
	HDMI_MSG_DISCONNECT,
	HDMI_MSG_CONNECT,
} HdmiMessage_t;

/* State */
typedef enum {
	HS_UNINITIALIZED = 0,
	HS_DISCONNECTED,
	HS_CONNECTED,
	HS_AUTHENTICATED,
	HS_LINK_TEST_INITIATED,
	HS_MUTED,
	HS_MAX,
} HdmiState_t;

/* TMDS Clock Index */
typedef enum {
    TC_25P175MHZ = 0,
    TC_25P200MHZ,
    TC_27P000MHZ,
    TC_27P027MHZ,
    TC_54P000MHZ,
    TC_54P054MHZ,
    TC_59P400MHZ,
    TC_72P000MHZ,
    TC_74P175MHZ,
    TC_74P250MHZ,
    TC_108P00MHZ,
    TC_148P35MHZ,
    TC_148P50MHZ,
    TC_297P00MHZ,
    TC_MAX,
} TmdsClock_t;

/* Data Island Packet Type */
typedef enum {
    PT_NULL = 0,
    PT_AUDIO_CLOCK_REGENERATION,
    PT_AUDIO_SAMPLE,
    PT_GENERAL_CONTROL,
    PT_AUDIO_CONTENT_PROTECTION,
    PT_ISRC1,
    PT_ISRC2,
    PT_ONEBIT_AUDIO_SAMPLE,
    PT_DST_AUDIO,
    PT_HBR_AUDIO_STREAM,
    PT_GAMUT_METADATA,
    PT_VENDOR_INFOFRAME = 0x81,
    PT_AVI_INFOFRAME,
    PT_SPD_INFOFRAME,
    PT_AUDIO_INFOFRAME,
    PT_MPEG_INFOFRAME,
} PacketType_t;

/* Secure Values */
typedef enum {
	SV_AINFO = 0,
	SV_AN,
	SV_AKSV,
	SV_BCAPS,
	SV_BKSV,
	SV_BSTATUS,
	SV_R0_TX,
	SV_R0_RX,
	SV_RI_TX,
	SV_RI_RX,
	SV_PJ_TX,
	SV_PJ_RX,
	SV_V_TX,
	SV_V_RX,
	SV_KSV_FIFO,
} SecureValue_t;

/* General Control Packet Pixel Packing Phase */
typedef enum {
    PP_PHASE4 = 0,
    PP_PHASE1,
    PP_PHASE2,
    PP_PHASE3,
} PixelPack_t;

/* General Control Packet Color Depth */
typedef enum {
    CD_NOT_INDICATED = 0,
    CD_24BIT = 4,
    CD_30BIT,
    CD_36BIT,
    CD_48BIT,
    CD_MAX,
} ColorDepth_t;

/* ACP Packet Type */
typedef enum {
    AT_GENERIC_AUDIO = 0,
    AT_IEC60958_AUDIO,
    AT_DVD_AUDIO,
    AT_SUPER_AUDIO_CD,
} AcpType_t;

/* ACP Packet DVD Audio Copy Permission */
typedef enum {
    DCP_N = 0,
    DCP_SINGLE,
    DCP_NO_MORE,
    DCP_UNUSED,
} DvdCopyProt_t;

/* ACP Packet DVD Audio Quality */
typedef enum {
    DAQ_CD = 0,
    DAQ_2_CH,
    DAQ_MULTI_CH,
} DvdAudioQual_t;

/* ISRC Packet Status */
typedef enum {
    IS_STARTING = 1,
    IS_INTERMEDIATE = 2,
    IS_ENDING = 4,
} IsrcStatus_t;

/* Gamut Metadata Packet GBD Profile */
typedef enum {
    GP_P0 = 0,
    GP_P1,
    GP_P2,
    GP_P3,
} GamutProfile_t;

/* Gamut Metadata Packet Sequence */
typedef enum {
    PS_INTERMEDIATE = 0,
    PS_FIRST,
    PS_LAST,
    PS_ONLY,
} GamutSequence_t;

/* AVI Infoframe RGB or YCbCr Indicator */
typedef enum {
    CI_RGB = 0,
    CI_YCC422,
    CI_YCC444,
    CI_MAX,
} ColorIndicator_t;

/* AVI Infoframe Bar Information */
typedef enum {
    BI_INVALID = 0,
    BI_V_VALID,
    BI_H_VALID,
    BI_VH_VALID,
    BI_MAX,
} BarInfo_t;

/* AVI Infoframe Scan Information */
typedef enum {
    SI_NO_DATA = 0,
    SI_OVERSCANNED,
    SI_UNDERSCANNED,
    SI_MAX,
} ScanInfo_t;

/* AVI Infoframe Colorimetry */
typedef enum {
    C_NO_DATA = 0,
    C_SMPTE170M,
    C_ITUR709,
    C_EXT_INFO_VALID,
    C_MAX,
} Colorimetry_t;

/* AVI Infoframe Picture Aspect Ratio */
typedef enum {
    AR_NO_DATA = 0,
    AR_4X3,
    AR_16X9,
    AR_MAX,
} AspectRatio_t;

/* AVI Infoframe Active Format Aspect Ratio */
typedef enum {
    AF_SAME = 8,
    AF_4X3,
    AF_16X9,
    AF_14X9,
    AF_MAX = 4,
} ActiveFormat_t;

/* AVI Infoframe Extended Colorimetry */
typedef enum {
    EC_XVYCC601 = 0,
    EC_XVYCC709,
    EC_SYCC601,
    EC_ADOBEYCC601,
    EC_ADOBERGB,
    EC_MAX,
} ExtColorimetry_t;

/* AVI Infoframe Quantization Range */
typedef enum {
    QR_DEFAULT = 0,
    QR_LIMITED,
    QR_FULL,
    QR_MAX,
} QuantiRange_t;

/* AVI Infoframe Non-Uniform Picture Scaling */
typedef enum {
    PS_UNKNOWN = 0,
    PS_H_SCALED,
    PS_V_SCALED,
    PS_VH_SCALED,
    PS_MAX,
} PictureScale_t;

/* AVI Infoframe Video Identification Code */
typedef enum {
    VIC_NO_CODE = 0,
    VIC_640X480P_60HZ,
    VIC_720X480P_60HZ_ED,
    VIC_720X480P_60HZ_HD,
    VIC_1280X720P_60HZ,
    VIC_1920X1080I_60HZ,
    VIC_1440X480I_60HZ_ED,
    VIC_1440X480I_60HZ_HD,
    VIC_1440X240P_60HZ_ED,
    VIC_1440X240P_60HZ_HD,
    VIC_2880X480I_60HZ_ED,
    VIC_2880X480I_60HZ_HD,
    VIC_2880X240P_60HZ_ED,
    VIC_2880X240P_60HZ_HD,
    VIC_1440X480P_60HZ_ED,
    VIC_1440X480P_60HZ_HD,
    VIC_1920X1080P_60HZ,
    VIC_720X576P_50HZ_ED,
    VIC_720X576P_50HZ_HD,
    VIC_1280X720P_50HZ,
    VIC_1920X1080I_50HZ,
    VIC_1440X576I_50HZ_ED,
    VIC_1440X576I_50HZ_HD,
    VIC_1440X288P_50HZ_ED,
    VIC_1440X288P_50HZ_HD,
    VIC_2880X576I_50HZ_ED,
    VIC_2880X576I_50HZ_HD,
    VIC_2880X288P_50HZ_ED,
    VIC_2880X288P_50HZ_HD,
    VIC_1440X576P_50HZ_ED,
    VIC_1440X576P_50HZ_HD,
    VIC_1920X1080P_50HZ,
    VIC_1920X1080P_24HZ,
    VIC_1920X1080P_25HZ,
    VIC_1920X1080P_30HZ,
    VIC_2880X480P_60HZ_ED,
    VIC_2880X480P_60HZ_HD,
    VIC_2880X576P_50HZ_ED,
    VIC_2880X576P_50HZ_HD,
    VIC_1920X1250_50HZ,
    VIC_1920X1080I_100HZ,
    VIC_1280X720P_100HZ,
    VIC_720X576P_100HZ_ED,
    VIC_720X576P_100HZ_HD,
    VIC_1440X576I_100HZ_ED,
    VIC_1440X576I_100HZ_HD,
    VIC_1920X1080I_120HZ,
    VIC_1280X720P_120HZ,
    VIC_720X480P_120HZ_ED,
    VIC_720X480P_120HZ_HD,
    VIC_1440X480I_120HZ_ED,
    VIC_1440X480I_120HZ_HD,
    VIC_720X576P_200HZ_ED,
    VIC_720X576P_200HZ_HD,
    VIC_1440X576I_200HZ_ED,
    VIC_1440X576I_200HZ_HD,
    VIC_720X480P_240HZ_ED,
    VIC_720X480P_240HZ_HD,
    VIC_1440X480I_240HZ_ED,
    VIC_1440X480I_240HZ_HD,
    VIC_1280X720P_24HZ,
    VIC_1280X720P_25HZ,
    VIC_1280X720P_30HZ,
    VIC_1920X1080P_120HZ,
    VIC_1920X1080P_100HZ,
    VIC_MAX,
} VideoIdCode_t;

/* AVI Infoframe Pixel Repetition */
typedef enum {
    PR_0 = 0,
    PR_1,
    PR_2,
    PR_3,
    PR_4,
    PR_5,
    PR_6,
    PR_7,
    PR_8,
    PR_9,
} PixelRepetition_t;

/* AVI Infoframe IT Content Type */
typedef enum {
	ICT_GRAPHICS = 0,
	ICT_PHOTO,
	ICT_CINEMA,
	ICT_GAME,
} ItContentType_t;

/* AVI Infoframe YCC QUANTIZATION RANGE */
typedef enum {
	YQ_LIMITED = 0,
	YQ_FULL,
} YccQuantiRange_t;

/* SPD Infoframe Packet Source Device Information */
typedef enum {
    DI_UNKNOWN = 0,
    DI_DIGITAL_STB,
    DI_DVD,
    DI_DVHS,
    DI_HDD_VIDEO,
    DI_DVC,
    DI_DSC,
    DI_VIDEO_CD,
    DI_GAME,
    DI_PC_GENERAL,
    DI_BLU_RAY,
    DI_SUPER_AUDIO,
    DI_HD_DVD,
    DI_PMP,
} DeviceInfo_t;

/* Audio Infoframe Coding Type */
typedef enum {
    AIC_STREAM = 0,
    AIC_PCM,
    AIC_AC3,
    AIC_MPEG1,
    AIC_MP3,
    AIC_MPEG2,
    AIC_AAC_LC,
    AIC_DTS,
    AIC_ATRAC,
    AIC_DSD,
    AIC_EAC3,
    AIC_DTS_HD,
    AIC_MLP,
    AIC_DST,
    AIC_WMA_PRO,
    AIC_HE_AAC = 16,
    AIC_HE_AACV2,
    AIC_MPEG_SURROUND,
} AudioIdCode_t;

/* Audio Infoframe Channel Count */
typedef enum {
    AC_STREAM = 0,
    AC_2CH,
    AC_3CH,
    AC_4CH,
    AC_5CH,
    AC_6CH,
    AC_7CH,
    AC_8CH,
} AudioChannel_t;

/* Audio Infoframe Sampling Frequency */
typedef enum {
    SF_STREAM = 0,
    SF_32KHZ,
    SF_44P1KHZ,
    SF_48KHZ,
    SF_88P2KHZ,
    SF_96KHZ,
    SF_176P4KHZ,
    SF_192KHZ,
    SF_MAX,
} SampleFreq_t;

/* Audio Infoframe Sample Size */
typedef enum {
    SS_STREAM = 0,
    SS_16BIT,
    SS_20BIT,
    SS_24BIT,
} SampleSize_t;

/* Audio Infoframe Level Shift Value */
typedef enum {
    LSV_0DB = 0,
    LSV_1DB,
    LSV_2DB,
    LSV_3DB,
    LSV_4DB,
    LSV_5DB,
    LSV_6DB,
    LSV_7DB,
    LSV_8DB,
    LSV_9DB,
    LSV_10DB,
    LSV_11DB,
    LSV_12DB,
    LSV_13DB,
    LSV_14DB,
    LSV_15DB,
} LevelShiftValue_t;

/* Audio Infoframe LFE Playback Level */
typedef enum {
    PL_UNKNOWN = 0,
    PL_0DB,
    PL_10DB,
} PlaybackLevel_t;

/* MPEG Source InfoFrame MPEG Frame */
typedef enum {
    MF_UNKNOWN = 0,
    MF_I_PICTURE,
    MF_B_PICTURE,
    MF_P_PICTURE,
} MpegFrame_t;

/*
 ***************************************************
 ***************************************************
 *
 * Data structure definitions
 *
 ***************************************************
 ***************************************************
 */

/* Audio Clock Regeneration Packet Structure */
typedef struct {
    DWORD CycleTimeStamp;
    DWORD Numerator;
} AcrPacket_t;

/* Audio Sample Subpacket Structure */
typedef struct {
    DWORD Timeslot;
    BOOL  Parity;
    BOOL  ChannelStatus;
    BOOL  UserData;
    BOOL  Valid;
} AudioSubpacket_t;

/* Audio Sample Packet Structure */
typedef struct {
    BOOL             EightChLayout;
    BOOL             SamplesPresent[4];
    BOOL             FirstFrameBlock[4];
    BOOL             SamplesFlat[4];
    AudioSubpacket_t LeftChannel[4];
    AudioSubpacket_t RightChannel[4];
} AudioPacket_t;

/* General Control Packet Structure */
typedef struct {
    BOOL         AvMute;
    PixelPack_t  PixelPacking;
    ColorDepth_t ColorDepth;
    BOOL         DefaultPhase;
} GcPacket_t;

/* Audio Content Protection Packet Structure */
typedef struct {
    AcpType_t      Type;
    BYTE           DvdTypeGeneration;
    DvdCopyProt_t  DvdCopyPermission;
    BYTE           DvdCopyNumber;
    DvdAudioQual_t DvdQuality;
    BOOL           DvdTransaction;
    BYTE           SuperAudioCdCci1[16];
} AcpPacket_t;

/* ISRC Packet Structure */
typedef struct {
    BOOL         Continue;
    BOOL         Valid;
    IsrcStatus_t Status;
    BYTE         Isrc[32];
} IsrcPacket_t;

/* One Bit Audio Sample Packet Structure */
typedef struct {
    BOOL  EightChLayout;
    BOOL  SamplesPresent[4];
    BOOL  SamplesInvalid[4];
    DWORD SamplesChannel1[4];
    DWORD SamplesChannel2[4];
} OneBitAudioPacket_t;

/* DST (Compressed DSD) Audio Packet Structure */
typedef struct {
    BOOL FrameStart;
    BOOL SamplesInvalid;
    BOOL DoubleRate;
    BYTE DataStream[28];
} DstAudioPacket_t;

/* High-Bitrate Audio Stream Packet Structure */
typedef struct {
    BOOL             FirstFrameBlock[4];
    AudioSubpacket_t LeftChannel[4];
    AudioSubpacket_t RightChannel[4];
} HbrAudioPacket_t;

/* Gamut Metadata Packet Structure */
typedef struct {
    BOOL            NextField;
    GamutProfile_t  GbdProfile;
    BYTE            AffectedSeqNum;
    BOOL            NoCurrentGbd;
    GamutSequence_t PacketSeq;
    BYTE            CurrentSeqNum;
    WORD            GbdLength;
    BYTE            *GbdData;
} GamutPacket_t;

/* Vendor Infoframe Packet Structure */
typedef struct {
	DWORD IEEERegistraionId;
	DWORD PayloadLength;
	BYTE  Payload[24];
} VendorInfoFrame_t;

/* AVI Infoframe Packet Structure */
typedef struct {
    ColorIndicator_t  ColorIndicator;
    BOOL              ActiveInfo;
    BarInfo_t         BarInfo;
    ScanInfo_t        ScanInfo;
    Colorimetry_t     Colorimetry;
    AspectRatio_t     PictureAspectRatio;
    ActiveFormat_t    FormatAspectRatio;
    BOOL              ITContent;
    ExtColorimetry_t  ExtendedColorimetry;
    QuantiRange_t     QuantizationRange;
    PictureScale_t    PictureScaling;
    VideoIdCode_t     VideoIdCode;
    YccQuantiRange_t  YCCQuantizationRange;
    ItContentType_t   ITContentType;
    PixelRepetition_t PixelRepetition;
    WORD              TopBarEndLineNum;
    WORD              BottomBarStartLineNum;
    WORD              LeftBarEndPixelNum;
    WORD              RightBarStartPixelNum;
} AviInfoFrame_t;

/* SPD Infoframe Packet Structure */
typedef struct {
    BYTE         VendorName[8];
    BYTE         ProductDescription[16];
    DeviceInfo_t SourceDevice;
} SpdInfoFrame_t;

/* Audio Infoframe Packet Structure */
typedef struct {
    AudioIdCode_t     CodingType;
    AudioChannel_t    ChannelCount;
    SampleFreq_t      SamplingFrequency;
    SampleSize_t      SampleSize;
    BYTE              ChannelAllocation;
    LevelShiftValue_t LevelShiftValue;
    BOOL              DownMixInhibit;
    PlaybackLevel_t   LFEPlaybackLevel;
} AudioInfoFrame_t;

/* MPEG Source Infoframe Packet Structure */
typedef struct {
    DWORD       BitRate;
    BOOL        FieldRepeat;
    MpegFrame_t Frame;
} MpegInfoFrame_t;

/* Video Parameters Data Structure */
typedef struct {
    WORD              HActive;
    WORD			  HFrontPorch;
    WORD			  HSync;
    WORD			  HBackPorch;
    WORD              VActive;
    WORD              VFrontPorch;
    WORD              VSync;
    WORD			  VBackPorch;
    char              ScanMode;
    AspectRatio_t     AspectRatio;
    PixelRepetition_t PixelRepetition;
    TmdsClock_t       TmdsClock;
} VideoParam_t;

typedef struct 
{
	HdmiState_t			State;
	Edid_t				EDID;
	AcrPacket_t			ACR;
	AudioPacket_t		AUD;
	GcPacket_t			GC;
	AviInfoFrame_t		AVI;
	SpdInfoFrame_t		SPD;
	AudioInfoFrame_t	AUDIF;
	MpegInfoFrame_t		MPEG;
	VendorInfoFrame_t	VENDOR;
} HdmiCtrl_t;

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
 * HdmiInit
 *
 * Performs initialization of the HDMI device driver.
 *
 * @param None.
 *
 * @return TRUE if the operation is successful. Otherwise
 *         returns FALSE.
 */
BOOL HdmiInit(void);

/*
 * HdmiEnable
 *
 * Enables the HDMI device driver.
 *
 * @param None.
 *
 * @return TRUE if the operation is successful. Otherwise
 *         returns FALSE.
 */
BOOL HdmiEnable(void);

/*
 * HdmiDisable
 *
 * Disables the HDMI TX device driver.
 *
 * @param None.
 *
 * @return TRUE if the operation is successful. Otherwise
 *         returns FALSE.
 */
BOOL HdmiDisable(void);

/*
 * HdmiControl
 *
 * Provides an entry point to invoke HDMI transmitter driver
 * specific functions.
 *
 * @param control - Numeric code of a particular driver function.
 * @param args    - Variable list of arguments specific to the
 *                  particular driver function.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int HdmiControl(int control, va_list args);

/*
 * HdmiTransmit
 *
 * Starts audio and video signal transmission as well as
 * transmission of data island packets.
 *
 * @param transmit - Boolean flag which determines if signal
 * 				     transmission needs to start or to stop.
 *
 * @return TRUE if the operation is successful. Otherwise
 *         returns FALSE.
 */
BOOL HdmiTransmit(BOOL transmit);

/*
 * HdmiAvMute
 *
 * Mutes/unmutes transmission of encrypted or unencrypted
 * and and video signals.
 *
 * @param avmute - Boolean flag which determines if audio
 * 				   and video signals will be muted or not.
 *
 * @return TRUE if the operation is successful. Otherwise
 *         returns FALSE.
 */
BOOL HdmiAvMute(BOOL avmute);

/*
 * HdmiConnect
 *
 * Manually connects or disconnects the HDMI transmitter to an
 * HDMI receiver when hotplug detection (HPD) is not implemented.
 *
 * @param connect - Boolean flag which determines if HDMI
 * 				    receiver has been connected or not.
 *
 * @return None.
 */
void HdmiConnect(BOOL connect);

/*
 * HdmiDetectHandler
 *
 * Handles events related to hotplug detection (HPD). The
 * events could either be an HDMI device connection or an
 * HDMI device disconnection
 *
 * @param None.
 *
 * @return None.
 */
void HdmiDetectHandler(void);

/*
 * HdmiMsgHandler
 *
 * Handles messages related to the HDMI device driver. One
 * message event is reading of the extended display ID data.
 *
 * @param message - Pointer to the OS message data structure.
 *
 * @return None.
 */
void HdmiMsgHandler(RE_MESSAGE *message);

#endif // __HDMI_H__

