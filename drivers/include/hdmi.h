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

/* HDMI Device Driver Command Controls */
typedef enum {
	HDMITX_CMD_START_TRANSMIT,
	HDMITX_CMD_STOP_TRANSMIT,
	HDMITX_CMD_SET_AVMUTE,
	HDMITX_CMD_CLEAR_AVMUTE,
	HDMITX_CMD_CONNECT,
	HDMITX_CMD_DISCONNECT,
	HDMITX_CMD_SHOW_REGS,
} HdmiTxCommand_t;

/* HDMI Device Driver State */
typedef enum {
	HDMITX_STATE_UNINITIALIZED = 0,
	HDMITX_STATE_DISCONNECTED,
	HDMITX_STATE_CONNECTED,
	HDMITX_STATE_AUTHENTICATED,
	HDMITX_STATE_LINK_TEST_INITIATED,
	HDMITX_STATE_MUTED,
	HDMITX_STATE_MAX,
} HdmiTxState_t;

/* HDMI Device Driver Message Events */
typedef enum {
	HDMITX_MSG_DISCONNECT,
	HDMITX_MSG_CONNECT,
} HdmiTxMessage_t;

/* Display Mode */
typedef enum {
    DM_GRAPHICS = 0,
    DM_VIDEO,
} DisplayMode_t;

/* Video Output Format */
typedef enum {
    VF_NO_CODE = 0,
    VF_640X480P_60HZ,
    VF_720X480P_60HZ_ED,
    VF_720X480P_60HZ_HD,
    VF_1280X720P_60HZ,
    VF_1920X1080I_60HZ,
    VF_1440X480I_60HZ_ED,
    VF_1440X480I_60HZ_HD,
    VF_1440X240P_60HZ_ED,
    VF_1440X240P_60HZ_HD,
    VF_2880X480I_60HZ_ED,
    VF_2880X480I_60HZ_HD,
    VF_2880X240P_60HZ_ED,
    VF_2880X240P_60HZ_HD,
    VF_1440X480P_60HZ_ED,
    VF_1440X480P_60HZ_HD,
    VF_1920X1080P_60HZ,
    VF_720X576P_50HZ_ED,
    VF_720X576P_50HZ_HD,
    VF_1280X720P_50HZ,
    VF_1920X1080I_50HZ,
    VF_1440X576I_50HZ_ED,
    VF_1440X576I_50HZ_HD,
    VF_1440X288P_50HZ_ED,
    VF_1440X288P_50HZ_HD,
    VF_2880X576I_50HZ_ED,
    VF_2880X576I_50HZ_HD,
    VF_2880X288P_50HZ_ED,
    VF_2880X288P_50HZ_HD,
    VF_1440X576P_50HZ_ED,
    VF_1440X576P_50HZ_HD,
    VF_1920X1080P_50HZ,
    VF_1920X1080P_24HZ,
    VF_1920X1080P_25HZ,
    VF_1920X1080P_30HZ,
    VF_2880X480P_60HZ_ED,
    VF_2880X480P_60HZ_HD,
    VF_2880X576P_50HZ_ED,
    VF_2880X576P_50HZ_HD,
    VF_1920X1250_50HZ,
    VF_1920X1080I_100HZ,
    VF_1280X720P_100HZ,
    VF_720X576P_100HZ_ED,
    VF_720X576P_100HZ_HD,
    VF_1440X576I_100HZ_ED,
    VF_1440X576I_100HZ_HD,
    VF_1920X1080I_120HZ,
    VF_1280X720P_120HZ,
    VF_720X480P_120HZ_ED,
    VF_720X480P_120HZ_HD,
    VF_1440X480I_120HZ_ED,
    VF_1440X480I_120HZ_HD,
    VF_720X576P_200HZ_ED,
    VF_720X576P_200HZ_HD,
    VF_1440X576I_200HZ_ED,
    VF_1440X576I_200HZ_HD,
    VF_720X480P_240HZ_ED,
    VF_720X480P_240HZ_HD,
    VF_1440X480I_240HZ_ED,
    VF_1440X480I_240HZ_HD,
    VF_1280X720P_24HZ,
    VF_1280X720P_25HZ,
    VF_1280X720P_30HZ,
    VF_1920X1080P_120HZ,
    VF_1920X1080P_100HZ,
} VideoFormat_t;

/* Source Pixel Format */
typedef enum {
	PF_RGB565 = 0,
	PF_RGB1555,
	PF_RGB888P,
	PF_RGB888UP,
	PF_RGBA888,
	PF_YUV422P,
	PF_YUV422PL,
	PF_YUV420PL,
	PF_SMPNLCMD,
	PF_PALETTE4,
	PF_PALETTE8,
	PF_RGB888A,
} PixelFormat_t;

/* Audio Channel Layout */
typedef enum {
    AL_STREAM = 0,
    AL_2CH,
    AL_3CH,
    AL_4CH,
    AL_5CH,
    AL_6CH,
    AL_7CH,
    AL_8CH,
} AudioLayout_t;

/* Audio Frequency */
typedef enum {
    AF_STREAM = 0,
    AF_32KHZ,
    AF_44P1KHZ,
    AF_48KHZ,
    AF_88P2KHZ,
    AF_96KHZ,
    AF_176P4KHZ,
    AF_192KHZ,
} AudioFreq_t;

/* Audio Size */
typedef enum {
    AS_STREAM = 0,
    AS_16BIT,
    AS_20BIT,
    AS_24BIT,
} AudioSize_t;

/*
 ***************************************************
 ***************************************************
 *
 * Data structure definitions
 *
 ***************************************************
 ***************************************************
 */

/* HDMI TX driver status information data structure */
typedef struct {
	DWORD Dummy;
} HdmiTxStatus_t;

/* HDMI TX driver statistical information data structure */
typedef struct {
	DWORD Dummy;
} HdmiTxStats_t;

/* HDMI TX driver configuration data structure */
typedef struct {
	DisplayMode_t DispMode;
	VideoFormat_t DestFormat;
	PixelFormat_t SrcFormat;
	AudioLayout_t AudChanCount;
	AudioFreq_t   SampleFreq;
	AudioSize_t   SampleSize;
} HdmiTxConfig_t;

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
 * HdmiTxInitialize
 *
 * Performs initialization of the HDMI TX task.
 *
 * @param None.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int HdmiTxInitialize(void);

/*
 * HdmiTxEnable
 *
 * Enables the HDMI TX device driver.
 *
 * @param config - Pointer to the instance of the driver
 * 				   specific configuration data structure.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int HdmiTxEnable(
	HdmiTxConfig_t *config);

/*
 * HdmiTxDisable
 *
 * Disables the HDMI TX device driver.
 *
 * @param None.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int HdmiTxDisable(void);

/*
 * HdmiTxStartTransmit
 *
 * Starts audio and video signal transmission as well as
 * transmission of data island packets.
 *
 * @param None.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int HdmiTxStartTransmit(void);

/*
 * HdmiTxStopTransmit
 *
 * Stops audio and video signal transmission as well as
 * transmission of data island packets.
 *
 * @param None.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int HdmiTxStopTransmit(void);

/*
 * HdmiTxAvMute
 *
 * Mutes/unmutes transmission of encrypted or unencrypted
 * and and video signals.
 *
 * @param avmute - Boolean flag which determines if audio
 * 				   and video signals will be muted or not.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int HdmiTxAvMute(BOOL avmute);

/*
 * HdmiTxConnect
 *
 * Manually connects the HDMI transmitter to an HDMI receiver
 * when hotplug detection (HPD) is not implemented.
 *
 * @param None.
 *
 * @return None.
 */
void HdmiTxConnect(void);

/*
 * HdmiTxDisconnect
 *
 * Manually disconnects the HDMI transmitter to an HDMI re-
 * ceiver when hotplug detection (HPD) is not implemented.
 *
 * @param None.
 *
 * @return None.
 */
void HdmiTxDisconnect(void);

/*
 * HdmiTxDetectHandler
 *
 * Handles events related to hotplug detection (HPD). The
 * events could either be an HDMI device connection or an
 * HDMI device disconnection
 *
 * @param None.
 *
 * @return None.
 */
void HdmiTxDetectHandler(void);

/*
 * HdmiTxMsgHandler
 *
 * Handles messages related to the HDMI device driver. One
 * message event is reading of the extended display ID data.
 *
 * @param message - Pointer to the OS message data structure.
 *
 * @return None.
 */
void HdmiTxMsgHandler(RE_MESSAGE *message);

/*
 * HdmiTxHdcpAuthenticate
 *
 * Performs HDCP authentication process wherein secured
 * values between the HDMI transmitter and receiver are
 *  shared, computed and compared for verification.
 *
 * @param None.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int HdmiTxHdcpAuthenticate(void);

/*
 * HdmiTxHdcpIrqHandler
 *
 * Handles interrupts related to HDCP authentication. Such
 * events would flag ready status information of either the
 * RI or PJ secure values.
 *
 * @param None.
 *
 * @return None.
 */
void HdmiTxHdcpIrqHandler(void);


#endif // __HDMI_H__
