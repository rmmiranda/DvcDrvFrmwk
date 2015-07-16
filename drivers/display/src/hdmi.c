/*****************************************************************************
 * Filename:    hdmi.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     May 2010
 *
 * Description: The task-specific interface code of the hdmi driver. Developed
 *              specifically for the platform software driver framework.
 *
 *****************************************************************************/

#include <string.h>

#include "rei.h"
#include "framework.h"
#include "gpio_platform.h"
#include "hdmi_internal.h"
#include "edid.h"
#include "hdmi.h"
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

#define SRCGROUP			GROUP_PLT_FRMWRK
#define SRCFILE				FILE_DISP_HDMI

#define COLOR_DEPTH_MAX		(CD_MAX - CD_24BIT)
#define SAMPLE_FREQ_MAX		(SF_MAX - SF_32KHZ)

#define I2C_DEV_DDC			I2C_DEV(4)

/*
 ***************************************************
 ***************************************************
 *
 * Function forward declarations
 *
 ***************************************************
 ***************************************************
 */

int HdmiTxHwInitialize(void);
int HdmiTxHwEnable(BOOL enable);
int HdmiTxHwTransmit(BOOL transmit);
int HdmiTxHwConfigure(
	BOOL hdmi_mode,
	ColorDepth_t color_depth,
	ColorIndicator_t color_indicator,
	AudioChannel_t channel_count,
	SampleFreq_t sample_frequency,
	SampleSize_t sample_size,
	BOOL interleave_mode,
	WORD h_active,
	WORD h_front,
	WORD h_sync,
	WORD h_back,
	WORD v_active,
	WORD v_front,
	WORD v_sync,
	WORD v_back,
	PixelRepetition_t pixel_repetition,
	TmdsClock_t tmds_clock);
int HdmiTxHwGetSecureVal( SecureValue_t secval, BYTE *secbuf );
int HdmiTxHwSetSecureVal( SecureValue_t secval, BYTE *secbuf );
int HdmiTxHwPacketize( PacketType_t packet, void *payload );
int HdmiTxHwEncrypt(void);
void HdmiTxDetectHandler(void);

int DispHwConfigure(
	BYTE mode,
	BYTE source_format,
	BOOL interleave_mode,
	WORD h_active,
	WORD h_front,
	WORD h_sync,
	WORD h_back,
	WORD v_active,
	WORD v_front,
	WORD v_sync,
	WORD v_back);
int DispHwTransmit(BOOL transmit);

int AudioHwConfigure(
	AudioChannel_t channel_count,
	SampleFreq_t sample_freq,
	SampleSize_t sample_size);
int AudioHwTransmit(BOOL transmit);

/*
 ***************************************************
 ***************************************************
 *
 * Enum types and data structure definitions
 *
 ***************************************************
 ***************************************************
 */


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

/* HDMI TX Task Data Structure */
typedef struct {
	HdmiTxState_t		State;
	Edid_t				EDID;
	AcrPacket_t			ACR;
	AudioPacket_t		AUD;
	GcPacket_t			GC;
	AcpPacket_t			ACP;
	IsrcPacket_t		ISRC1;
	IsrcPacket_t		ISRC2;
	OneBitAudioPacket_t	ONE;
	DstAudioPacket_t	DST;
	HbrAudioPacket_t	HBR;
	GamutPacket_t		GAMUT;
	AviInfoFrame_t		AVI;
	SpdInfoFrame_t		SPD;
	AudioInfoFrame_t	AUDIF;
	MpegInfoFrame_t		MPEG;
	VendorInfoFrame_t	VENDOR;
} HdmiTxTask_t;

/*
 ***************************************************
 ***************************************************
 *
 * Static global variable declarations
 *
 ***************************************************
 ***************************************************
 */

/* HDMI TX Task Object */
static HdmiTxTask_t HdmiTxTask;

/* HDMI Detect Pad Configuration */
static const RE_PadConfiguration HdmiTxDetectPadConfig = {
    HDMI_DET_FN,                // altFunctionMode
    PAD_DIRECTION_INPUT,        // initialDirection
    FALSE,                      // initialLevel
    FALSE,                      // pullUpEnable
    FALSE,                      // pullDownEnable
    PAD_INTERRUPT_BOTH_EDGES,   // interruptType
    HdmiTxDetectHandler,        // interruptHandler
    FALSE,                      // interruptsInitiallyEnabled
    FALSE                       // interruptDebounceEnabled
};

/* HDMI Pixel Clock Values */
static DWORD HdmiPixelClockValue[] = {
     25174000, // 25.2/1.001 MHz
     25200000, // 25.2 MHz
     27000000, // 27 MHz
     27027000, // 27*1.001 MHz
     54000000, // 54 MHz
     54054000, // 54*1.001 MHz
     59400000, // 59400 MHz
     72000000, // 72 MHz
     74175000, // 74.25/1.001 MHz
     74250000, // 74.25 MHz
    108000000, // 108 MHz
    148351000, // 148.5/1.001 MHz
    148500000, // 148.5 MHz
    297000000, // 297 MHz
};

static AcrPacket_t HdmiAcrParam[TC_MAX][COLOR_DEPTH_MAX][SAMPLE_FREQ_MAX] = {
    /* Based on 25.2/1.001 MHz pixel clock */
    {{{  28125, 4576 },{   31250, 7007 },{  28125, 6864 },{   31250,14014 },{  28125,13728 },{   31250, 28028 },{  28125,27456 }},
     {{ 140620,18304 },{   39062, 7007 },{  93747,18304 },{   39062,14014 },{  93747,36608 },{   39062, 28028 },{  93747,73216 }},
     {{  84375, 9152 },{   46875, 7007 },{  56250, 9152 },{   46875,14014 },{  56250,18304 },{   46875, 28028 },{  56250,36608 }},
     {{  56250, 4576 },{   62500, 7007 },{  56250, 6864 },{   62500,14014 },{  56250,13728 },{   62500, 28028 },{  56250,27456 }}},
     /* Based on 25.2 MHz pixel clock */
    {{{  25200, 4096 },{   28000, 6272 },{  25200, 6144 },{   28000,12544 },{  25200,12288 },{   28000, 25088 },{  25200,24576 }},
     {{  31500, 4096 },{   35000, 6272 },{  31500, 6144 },{   35000,12544 },{  31500,12288 },{   35000, 25088 },{  31500,24576 }},
     {{  37800, 4096 },{   42000, 6272 },{  37800, 6144 },{   42000,12544 },{  37800,12288 },{   42000, 25088 },{  37800,24576 }},
     {{  50400, 4096 },{   56000, 6272 },{  50400, 6144 },{   56000,12544 },{  50400,12288 },{   56000, 25088 },{  50400,24576 }}},
      /* Based on 27 MHz pixel clock */
    {{{  27000, 4096 },{   30000, 6272 },{  27000, 6144 },{   30000,12544 },{  27000,12288 },{   30000, 25088 },{  27000,24576 }},
     {{  33750, 4096 },{   37500, 6272 },{  33750, 6144 },{   37500,12544 },{  33750,12288 },{   37500, 25088 },{  33750,24576 }},
     {{  40500, 4096 },{   45000, 6272 },{  40500, 6144 },{   45000,12544 },{  40500,12288 },{   45000, 25088 },{  40500,24576 }},
     {{  54000, 4096 },{   60000, 6272 },{  54000, 6144 },{   60000,12544 },{  54000,12288 },{   60000, 25088 },{  54000,24576 }}},
      /* Based on 27*1.001 MHz pixel clock */
    {{{  27027, 4096 },{   30030, 6272 },{  27027, 6144 },{   30030,12544 },{  27027,12288 },{   30030, 25088 },{  27027,24576 }},
     {{ 135135,16384 },{   37537, 6272 },{  90090,16384 },{   37537,12544 },{  90090,32768 },{   37537, 25088 },{  90090,65536 }},
     {{  81081, 8192 },{   45045, 6272 },{  54054, 8192 },{   45045,12544 },{  54054,16384 },{   45045, 25088 },{  54054,32768 }},
     {{  54054, 4096 },{   60060, 6272 },{  54054, 6144 },{   60060,12544 },{  54054,12288 },{   60060, 25088 },{  54054,24576 }}},
      /* Based on 54 MHz pixel clock */
    {{{  54000, 4096 },{   60000, 6272 },{  54000, 6144 },{   60000,12544 },{  54000,12288 },{   60000, 25088 },{  54000,24576 }},
     {{  67500, 4096 },{   75000, 6272 },{  67500, 6144 },{   75000,12544 },{  67500,12288 },{   75000, 25088 },{  67500,24576 }},
     {{  81000, 4096 },{   90000, 6272 },{  81000, 6144 },{   90000,12544 },{  81000,12288 },{   90000, 25088 },{  81000,24576 }},
     {{ 108000, 4096 },{  120000, 6272 },{ 108000, 6144 },{  120000,12544 },{ 108000,12288 },{  120000, 25088 },{ 108000,24576 }}},
      /* Based on 54*1.001 MHz pixel clock */
    {{{  54054, 4096 },{   60060, 6272 },{  54054, 6144 },{   60060,12544 },{  54054,12288 },{   60060, 25088 },{  54054,24576 }},
     {{  67567, 4096 },{   75075, 6272 },{  67567, 6144 },{   75075,12544 },{  67567,12288 },{   75075, 25088 },{  67567,24576 }},
     {{  81081, 4096 },{   90090, 6272 },{  81081, 6144 },{   90090,12544 },{  81081,12288 },{   90090, 25088 },{  81081,24576 }},
     {{ 108108, 4096 },{  120120, 6272 },{ 108108, 6144 },{  120120,12544 },{ 108108,12288 },{  120120, 25088 },{ 108108,24576 }}},
      /* Based on 59.4 MHz pixel clock */
    {{{  59400, 4096 },{   66000, 6272 },{  59400, 6144 },{   66000,12544 },{  59400,12288 },{   66000, 25088 },{  59400,24576 }},
     {{  74250, 4096 },{   82500, 6272 },{  74250, 6144 },{   82500,12544 },{  74250,12288 },{   82500, 25088 },{  74250,24576 }},
     {{  89100, 4096 },{   99000, 6272 },{  89100, 6144 },{   99000,12544 },{  89100,12288 },{   99000, 25088 },{  89100,24576 }},
     {{ 118800, 4096 },{  132000, 6272 },{ 118800, 6144 },{  132000,12544 },{ 118800,12288 },{  132000, 25088 },{ 118800,24576 }}},
      /* Based on 72 MHz pixel clock */
    {{{  72000, 4096 },{   80000, 6272 },{  72000, 6144 },{   80000,12544 },{  72000,12288 },{   80000, 25088 },{  72000,24576 }},
     {{  90000, 4096 },{  100000, 6272 },{  90000, 6144 },{  100000,12544 },{  90000,12288 },{  100000, 25088 },{  90000,24576 }},
     {{ 108000, 4096 },{  120000, 6272 },{ 108000, 6144 },{  120000,12544 },{ 108000,12288 },{  120000, 25088 },{ 108000,24576 }},
     {{ 144000, 4096 },{  160000, 6272 },{ 144000, 6144 },{  160000,12544 },{ 144000,12288 },{  160000, 25088 },{ 144000,24576 }}},
      /* Based on 74.25/1.001 MHz pixel clock */
    {{{ 210937,11648 },{  234375,17836 },{ 140625,11648 },{  234375,35672 },{ 140625,23296 },{  234375, 71344 },{ 140625,46592 }},
     {{ 263671,11648 },{  292968,17836 },{ 175781,11648 },{  292968,35672 },{ 175781,23296 },{  292968, 71344 },{ 175781,46592 }},
     {{ 316406,11648 },{  351562,17836 },{ 210937,11648 },{  351562,35672 },{ 210937,23296 },{  351562, 71344 },{ 210937,46592 }},
     {{ 421875,11648 },{  468750,17836 },{ 281250,11648 },{  468750,35672 },{ 281250,23296 },{  468750, 71344 },{ 281250,46592 }}},
      /* Based on 74.25 MHz pixel clock */
    {{{  74250, 4096 },{   82500, 6272 },{  74250, 6144 },{   82500,12544 },{  74250,12288 },{   82500, 25088 },{  74250,24576 }},
     {{ 111375, 4096 },{  103125, 6272 },{  92812, 6144 },{  103125,12544 },{  92812,12288 },{  103125, 25088 },{  92812,24576 }},
     {{  81000, 4096 },{  123750, 6272 },{ 111375, 6144 },{  123750,12544 },{ 111375,12288 },{  123750, 25088 },{ 111375,24576 }},
     {{ 148500, 4096 },{  165000, 6272 },{ 148500, 6144 },{  165000,12544 },{ 148500,12288 },{  165000, 25088 },{ 148500,24576 }}},
      /* Based on 108 MHz pixel clock */
    {{{ 108000, 4096 },{  120000, 6272 },{ 108000, 6144 },{  120000,12544 },{ 108000,12288 },{  120000, 25088 },{ 108000,24576 }},
     {{ 135000, 4096 },{  150000, 6272 },{ 135000, 6144 },{  150000,12544 },{ 135000,12288 },{  150000, 25088 },{ 135000,24576 }},
     {{ 162000, 4096 },{  180000, 6272 },{ 162000, 6144 },{  180000,12544 },{ 162000,12288 },{  180000, 25088 },{ 162000,24576 }},
     {{ 216000, 4096 },{  240000, 6272 },{ 216000, 6144 },{  240000,12544 },{ 216000,12288 },{  240000, 25088 },{ 216000,24576 }}},
       /* Based on 148.5/1.001 MHz pixel clock */
    {{{ 421875,11648 },{  234375, 8918 },{ 140625, 5824 },{  234375,17836 },{ 140625,11648 },{  234375, 35672 },{ 140625,23296 }},
     {{ 527344,11648 },{ 1171870,35672 },{ 703122,23296 },{ 1171870,71344 },{ 703122,46592 },{ 1171870,142688 },{ 703122,93184 }},
     {{ 632812,11648 },{  703125,17836 },{ 421875,11648 },{  703125,35672 },{ 421875,23296 },{  703125, 71344 },{ 421875,46592 }},
     {{ 843750,11648 },{  468750, 8918 },{ 281250, 5824 },{  468750,17836 },{ 281250,11648 },{  468750, 35672 },{ 281250,23296 }}},
      /* Based on 148.5 MHz pixel clock */
    {{{ 148500, 4096 },{  165000, 6272 },{ 148500, 6144 },{  165000,12544 },{ 148500,12288 },{  165000, 25088 },{ 148500,24576 }},
     {{ 185625, 4096 },{  206250, 6272 },{ 185625, 6144 },{  206250,12544 },{ 185625,12288 },{  206250, 25088 },{ 185625,24576 }},
     {{ 222750, 4096 },{  247500, 6272 },{ 222750, 6144 },{  247500,12544 },{ 222750,12288 },{  247500, 25088 },{ 222750,24576 }},
     {{ 297777, 4096 },{  330000, 6272 },{ 297000, 6144 },{  330000,12544 },{ 297000,12288 },{  330000, 25088 },{ 297000,24576 }}},
      /* Based on 297 MHz pixel clock */
    {{{ 297000, 4096 },{  330000, 6272 },{ 297000, 6144 },{  330000,12544 },{ 297000,12288 },{  330000, 25088 },{ 297000,24576 }},
     {{ 371250, 4096 },{  412500, 6272 },{ 371250, 6144 },{  412500,12544 },{ 371250,12288 },{  412500, 25088 },{ 371250,24576 }},
     {{ 445500, 4096 },{  495000, 6272 },{ 445500, 6144 },{  495000,12544 },{ 445500,12288 },{  495000, 25088 },{ 445500,24576 }},
     {{ 594000, 4096 },{  660000, 6272 },{ 594000, 6144 },{  660000,12544 },{ 594000,12288 },{  660000, 25088 },{ 594000,24576 }}},
};

static VideoParam_t HdmiVideoParam[] = {
    {    0,    0,   0,   0,    0,  0, 0,  0, ' ', AR_4X3 , PR_0, TC_25P175MHZ },
    {  640,   16,  96,  48,  480, 10, 2, 33, 'P', AR_4X3 , PR_0, TC_25P175MHZ }, // (01) 640x480p @59.94/60 Hz
    {  720,   16,  62,  60,  480,  9, 6, 30, 'P', AR_4X3 , PR_0, TC_27P000MHZ }, // (02) 720x480p @59.94/60 Hz
    {  720,   16,  62,  60,  480,  9, 6, 30, 'P', AR_16X9, PR_0, TC_27P000MHZ }, // (03) 720x480p @59.94/60 Hz
    { 1280,  110,  40, 220,  720,  5, 5, 20, 'P', AR_16X9, PR_0, TC_74P250MHZ }, // (04) 1280x720p @59.94/60 Hz
//    { 1920,   88,  44, 148, 1080,  2, 5, 15, 'I', AR_16X9, PR_0, TC_74P250MHZ }, // (05) 1920x1080i @59.94/60 Hz
    { 1920,   88,  44, 148, 1080,  4, 5, 36, 'I', AR_16X9, PR_0, TC_74P250MHZ }, // (05) 1920x1080i @59.94/60 Hz
    { 1440,   38, 124, 114,  480,  4, 3, 15, 'I', AR_4X3 , PR_1, TC_27P000MHZ }, // (06) 720(1440)x480i @59.94/60 Hz
    { 1440,   38, 124, 114,  480,  4, 3, 15, 'I', AR_16X9, PR_1, TC_27P000MHZ }, // (07) 720(1440)x480i @59.94/60 Hz
    { 1440,   38, 124, 114,  240,  5, 3, 15, 'P', AR_4X3 , PR_1, TC_27P000MHZ }, // (08) 720(1440)x240p @59.94/60 Hz
    { 1440,   38, 124, 114,  240,  5, 3, 15, 'P', AR_16X9, PR_1, TC_27P000MHZ }, // (09) 720(1440)x240p @59.94/60 Hz
    { 2880,   76, 248, 228,  480,  4, 3, 15, 'I', AR_4X3 , PR_3, TC_54P000MHZ }, // (10) 720(2880)x480i @59.94/60 Hz
    { 2880,   76, 248, 228,  480,  4, 3, 15, 'I', AR_16X9, PR_3, TC_54P000MHZ }, // (11) 720(2880)x480i @59.94/60 Hz
    { 2880,   76, 248, 228,  240,  5, 3, 15, 'P', AR_4X3 , PR_3, TC_54P000MHZ }, // (12) 720(2880)x240p @59.94/60 Hz
    { 2880,   76, 248, 228,  240,  5, 3, 15, 'P', AR_16X9, PR_3, TC_54P000MHZ }, // (13) 720(2880)x240p @59.94/60 Hz
    { 1440,   32, 124, 120,  480,  9, 6, 30, 'P', AR_4X3 , PR_1, TC_54P000MHZ }, // (14) 720(1440)x480p @59.94/60 Hz
    { 1440,   32, 124, 120,  480,  9, 6, 30, 'P', AR_16X9, PR_1, TC_54P000MHZ }, // (15) 720(1440)x480p @59.94/60 Hz
    { 1920,   88,  44, 148, 1080,  4, 5, 36, 'P', AR_16X9, PR_0, TC_148P50MHZ }, // (16) 1920x1080p @59.94/60 Hz
    {  720,   12,  64,  68,  576,  5, 5, 39, 'P', AR_4X3 , PR_0, TC_27P000MHZ }, // (17) 720x576p @ 50 Hz
    {  720,   12,  64,  68,  576,  5, 5, 39, 'P', AR_16X9, PR_0, TC_27P000MHZ }, // (18) 720x576p @ 50 Hz
    { 1280,  440,  40, 220,  720,  5, 5, 20, 'P', AR_16X9, PR_0, TC_74P250MHZ }, // (19) 1280x720p @50 Hz
    { 1920,  528,  44, 148, 1080,  2, 5, 15, 'I', AR_16X9, PR_0, TC_74P250MHZ }, // (20) 1920x1080i @ 50 Hz
    { 1440,   24, 126, 138,  576,  2, 3, 19, 'I', AR_4X3 , PR_1, TC_27P000MHZ }, // (21) 720(1440)x576i @ 50 Hz
    { 1440,   24, 126, 138,  576,  2, 3, 19, 'I', AR_16X9, PR_1, TC_27P000MHZ }, // (22) 720(1440)x576i @ 50 Hz
    { 1440,   24, 126, 138,  288,  3, 3, 19, 'P', AR_4X3 , PR_1, TC_27P000MHZ }, // (23) 720(1440)x288p @50 Hz
    { 1440,   24, 126, 138,  288,  3, 3, 19, 'P', AR_16X9, PR_1, TC_27P000MHZ }, // (24) 720(1440)x288p @50 Hz
    { 2880,   48, 252, 276,  576,  2, 3, 19, 'I', AR_4X3 , PR_3, TC_54P000MHZ }, // (25) 720(2880)x576i @50 Hz
    { 2880,   48, 252, 276,  576,  2, 3, 19, 'I', AR_16X9, PR_3, TC_54P000MHZ }, // (26) 720(2880)x576i @50 Hz
    { 2880,   48, 252, 276,  288,  3, 3, 19, 'P', AR_4X3 , PR_3, TC_54P000MHZ }, // (27) 720(2880)x288p @50 Hz
    { 2880,   48, 252, 276,  288,  3, 3, 19, 'P', AR_16X9, PR_3, TC_54P000MHZ }, // (28) 720(2880)x288p @50 Hz
    { 1440,   24, 128, 136,  576,  5, 5, 39, 'P', AR_4X3 , PR_1, TC_54P000MHZ }, // (29) 720(1440)x576p @50 Hz
    { 1440,   24, 128, 136,  576,  5, 5, 39, 'P', AR_16X9, PR_1, TC_54P000MHZ }, // (30) 720(1440)x576p @50 Hz
    { 1920,  528,  44, 148, 1080,  4, 5, 36, 'P', AR_16X9, PR_0, TC_148P50MHZ }, // (31) 1920x1080p @50 Hz
    { 1920,  638,  44, 148, 1080,  4, 5, 36, 'P', AR_16X9, PR_0, TC_74P250MHZ }, // (32) 1920x1080p @23.97/24 Hz
    { 1920,  528,  44, 148, 1080,  4, 5, 36, 'P', AR_16X9, PR_0, TC_74P250MHZ }, // (33) 1920x1080p @25 Hz
    { 1920,   88,  44, 148, 1080,  4, 5, 36, 'P', AR_16X9, PR_0, TC_74P250MHZ }, // (34) 1920x1080p @29.97/30 Hz
    { 2880,   64, 248, 240,  480,  9, 6, 30, 'P', AR_4X3 , PR_3, TC_108P00MHZ }, // (35) 720(2880)x480p @59.94/60 Hz
    { 2880,   64, 248, 240,  480,  9, 6, 30, 'P', AR_16X9, PR_3, TC_108P00MHZ }, // (36) 720(2880)x480p @59.94/60 Hz
    { 2880,   48, 256, 272,  576,  5, 5, 39, 'P', AR_4X3 , PR_3, TC_108P00MHZ }, // (37) 720(2880)x576p @50 Hz
    { 2880,   48, 256, 272,  576,  5, 5, 39, 'P', AR_16X9, PR_3, TC_108P00MHZ }, // (38) 720(2880)x576p @50 Hz
    { 1920,   32, 168, 184, 1080, 23, 5, 57, 'I', AR_16X9, PR_0, TC_72P000MHZ }, // (39) 1920x1080i @50 Hz
    { 1920,  528,  44, 148, 1080,  2, 5, 15, 'I', AR_16X9, PR_0, TC_148P50MHZ }, // (40) 1920x1080i @100 Hz
    { 1280,  440,  40, 220,  720,  5, 5, 20, 'P', AR_16X9, PR_0, TC_148P50MHZ }, // (41) 1280x720p @100 Hz
    {  720,   12,  64,  68,  576,  5, 5, 39, 'P', AR_4X3 , PR_0, TC_54P000MHZ }, // (42) 720x576p @100 Hz
    {  720,   12,  64,  68,  576,  5, 5, 39, 'P', AR_16X9, PR_0, TC_54P000MHZ }, // (43) 720x576p @100 Hz
    { 1440,   24, 126, 138,  576,  2, 3, 19, 'I', AR_4X3 , PR_1, TC_54P000MHZ }, // (44) 720(1440)x576i @100 Hz
    { 1440,   24, 126, 138,  576,  2, 3, 19, 'I', AR_16X9, PR_1, TC_54P000MHZ }, // (45) 720(1440)x576i @100 Hz
    { 1920,   88,  44, 148, 1080,  2, 5, 15, 'I', AR_16X9, PR_0, TC_148P50MHZ }, // (46) 1920x1080i @120 Hz
    { 1280,  110,  40, 220,  720,  5, 5, 20, 'P', AR_16X9, PR_0, TC_148P50MHZ }, // (47) 1280x720p @120 Hz
    {  720,   16,  62,  60,  480,  9, 6, 30, 'P', AR_4X3 , PR_0, TC_54P000MHZ }, // (48) 720x480p @120 Hz
    {  720,   16,  62,  60,  480,  9, 6, 30, 'P', AR_16X9, PR_0, TC_54P000MHZ }, // (49) 720x480p @120 Hz
    { 1440,   38, 124, 114,  480,  4, 3, 15, 'I', AR_4X3 , PR_1, TC_54P000MHZ }, // (50) 720(1440)x480i @120 Hz
    { 1440,   38, 124, 114,  480,  4, 3, 15, 'I', AR_16X9, PR_1, TC_54P000MHZ }, // (51) 720(1440)x480i @120 Hz
    {  720,   12,  64,  68,  576,  5, 5, 39, 'P', AR_4X3 , PR_0, TC_108P00MHZ }, // (52) 720x576p @200 Hz
    {  720,   12,  64,  68,  576,  5, 5, 39, 'P', AR_16X9, PR_0, TC_108P00MHZ }, // (53) 720x576p @200 Hz
    { 1440,   24, 126, 138,  576,  2, 3, 19, 'I', AR_4X3 , PR_1, TC_108P00MHZ }, // (54) 720(1440)x576i @200 Hz
    { 1440,   24, 126, 138,  576,  2, 3, 19, 'I', AR_16X9, PR_1, TC_108P00MHZ }, // (55) 720(1440)x576i @200 Hz
    {  720,   16,  62,  60,  480,  9, 6, 30, 'P', AR_4X3 , PR_0, TC_108P00MHZ }, // (56) 720x480p @240 Hz
    {  720,   16,  62,  60,  480,  9, 6, 30, 'P', AR_16X9, PR_0, TC_108P00MHZ }, // (57) 720x480p @240 Hz
    { 1440,   38, 124, 114,  480,  4, 3, 15, 'I', AR_4X3 , PR_1, TC_108P00MHZ }, // (58) 720(1440)x480i @240 Hz
    { 1440,   38, 124, 114,  480,  4, 3, 15, 'I', AR_16X9, PR_1, TC_108P00MHZ }, // (59) 720(1440)x480i @240 Hz
    { 1280, 1760,  40, 220,  720,  5, 5, 20, 'P', AR_16X9, PR_0, TC_59P400MHZ }, // (60) 1280x720p @24 Hz
    { 1280, 2420,  40, 220,  720,  5, 5, 20, 'P', AR_16X9, PR_0, TC_74P250MHZ }, // (61) 1280x720p @25 Hz
    { 1280, 1760,  40, 220,  720,  5, 5, 20, 'P', AR_16X9, PR_0, TC_74P250MHZ }, // (62) 1280x720p @30 Hz
    { 1920,   88,  44, 148, 1080,  4, 5, 36, 'P', AR_16X9, PR_0, TC_297P00MHZ }, // (63) 1920x1080p @120 Hz
    { 1920,  528,  44, 148, 1080,  4, 5, 36, 'P', AR_16X9, PR_0, TC_297P00MHZ }, // (64) 1920x1080p @100 Hz
};

/*
 ***************************************************
 ***************************************************
 *
 * Exportable global variable declarations
 *
 ***************************************************
 ***************************************************
 */

/*
 ***************************************************
 ***************************************************
 *
 * static function definitions
 *
 ***************************************************
 ***************************************************
 */

/*
 * HdmiTxConfigure
 *
 * Configures HDMI TX task by reading EDID from the connected
 * HDMI device and setting the appropriate configuration values
 * based on the valid EDID.
 *
 * @param edid - Pointer to the EDID data structure.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
static int HdmiTxConfigure(
	ColorDepth_t color_depth,
	ColorIndicator_t color_indicator,
	AudioChannel_t sample_chan,
	SampleFreq_t sample_freq,
	SampleSize_t sample_size )
{
    VideoIdCode_t videoCode;
    VideoParam_t *videoParam;
    AcrPacket_t *acrParam;
    BOOL hdmiMode = TRUE;

	/* hdmi tx should either be in the connected or authenticated state */
	if( (HdmiTxTask.State != HDMITX_STATE_CONNECTED) && (HdmiTxTask.State != HDMITX_STATE_AUTHENTICATED) ) {
		WARN( "Driver is not in the appropriate state to be configured." );
		return (-1);
	}

    /* determine preferred video format parameters */
    videoCode = VIC_NO_CODE;
    while( HdmiTxTask.EDID.CEA.VideoBlocks[videoCode].NativeResolution == FALSE ) {
    	if( (int)(videoCode+1) < (int)VIC_MAX ) {
    		videoCode = (VideoIdCode_t)(videoCode + 1);
    	} else {
    		WARN( "Driver could not locate preferred video format. Will default to 640X480p format." );
    		videoCode = VIC_640X480P_60HZ;
    		break;
    	}
    }
	PRINTN( "Preferred video format found (video code: %d). ", videoCode );

    /* determine if connected device is an HDMI */
    if( HdmiTxTask.EDID.CEA.VendorBlock.IEEERegID != 0x00000C03 ) {
    	PRINT( "Connected device is not HDMI compliant." );
    	hdmiMode = FALSE;
    }

    /* get a handle to the corresponding video and acr parameters */
    videoParam = &HdmiVideoParam[videoCode];
    acrParam = &HdmiAcrParam[videoParam->TmdsClock][color_depth-CD_24BIT][sample_freq-SF_32KHZ];

	/* configure underlying Display hardware controller */
	if( DispHwConfigure(
			DM_VIDEO,
			PF_RGB888P,
			((videoParam->ScanMode == 'I') ? TRUE : FALSE),
			videoParam->HActive,
			videoParam->HFrontPorch,
			videoParam->HSync,
			videoParam->HBackPorch,
			videoParam->VActive,
			videoParam->VFrontPorch,
			videoParam->VSync,
			videoParam->VBackPorch) != DRIVER_NO_ERROR) {
		WARN( "Hardware (display controller) could not be configured." );
		return (-1);
	}

	/* configure underlying Display hardware controller */
	if( AudioHwConfigure(
			sample_chan,
			sample_freq,
			sample_size) != DRIVER_NO_ERROR) {
		WARN( "Hardware (audio controller) could not be configured." );
		return (-1);
	}
	/* configure underlying HDMI TX hardware controller */
	if( HdmiTxHwConfigure(
			hdmiMode,
			color_depth,
			color_indicator,
			sample_chan,
			sample_freq,
			sample_size,
			((videoParam->ScanMode == 'I') ? TRUE : FALSE),
			videoParam->HActive,
			videoParam->HFrontPorch,
			videoParam->HSync,
			videoParam->HBackPorch,
			videoParam->VActive,
			videoParam->VFrontPorch,
			videoParam->VSync,
			videoParam->VBackPorch,
			videoParam->PixelRepetition,
			videoParam->TmdsClock) != DRIVER_NO_ERROR) {
		WARN( "Hardware could not be configured." );
		return (-1);
	}

	/* configure general control packet */
	HdmiTxTask.GC.ColorDepth = color_depth;
	if( HdmiTxHwPacketize(PT_GENERAL_CONTROL, &(HdmiTxTask.GC)) != DRIVER_NO_ERROR ) {
		WARN( "Hardware could not construct the general control packet." );
		return (-1);
	}

	/* configure AVI infoframe packet */
	HdmiTxTask.AVI.ColorIndicator = color_indicator;
	HdmiTxTask.AVI.PictureAspectRatio = videoParam->AspectRatio;
	HdmiTxTask.AVI.VideoIdCode = videoCode;
	HdmiTxTask.AVI.PixelRepetition = videoParam->PixelRepetition;
	HdmiTxTask.AVI.BottomBarStartLineNum = videoParam->VActive + 1;
	HdmiTxTask.AVI.RightBarStartPixelNum = videoParam->HActive + 1;
	if( HdmiTxHwPacketize(PT_AVI_INFOFRAME, &(HdmiTxTask.AVI)) != DRIVER_NO_ERROR ) {
		WARN( "Hardware could not construct the AVI infoframe packet." );
		return (-1);
	}

	/* configure audio infoframe packet */
	HdmiTxTask.AUDIF.ChannelCount = sample_chan;
	HdmiTxTask.AUDIF.SamplingFrequency = sample_freq;
	HdmiTxTask.AUDIF.SampleSize = sample_size;
	HdmiTxTask.AUDIF.ChannelAllocation = (sample_chan == AL_8CH) ? 0x1F : 0x00;
	if( HdmiTxHwPacketize(PT_AUDIO_INFOFRAME, &(HdmiTxTask.AUDIF)) != DRIVER_NO_ERROR ) {
		WARN( "Hardware could not construct the audio infoframe packet." );
		return (-1);
	}

    /* construct vendor infoframe packet */
	if( HdmiTxHwPacketize(PT_VENDOR_INFOFRAME, &(HdmiTxTask.VENDOR)) != DRIVER_NO_ERROR ) {
		WARN( "Hardware could not construct the vendor infoframe packet." );
		return (-1);
	}

    /* construct source product decscriptor infoframe packet */
	if( HdmiTxHwPacketize(PT_SPD_INFOFRAME, &(HdmiTxTask.SPD)) != DRIVER_NO_ERROR ) {
		WARN( "Hardware could not construct the source product descripor infoframe packet." );
		return (-1);
	}

	/* configure audio clock regeneration packet */
	HdmiTxTask.ACR.CycleTimeStamp = acrParam->CycleTimeStamp;
    HdmiTxTask.ACR.Numerator = acrParam->Numerator;
	if( HdmiTxHwPacketize(PT_AUDIO_CLOCK_REGENERATION, &(HdmiTxTask.ACR)) != DRIVER_NO_ERROR ) {
		WARN( "Hardware could not construct the audio clock regeneration packet." );
		return (-1);
	}

    PRINT( "HDMI TX configured successfully." );

	return (DRIVER_NO_ERROR);
}

/*
 * HdmiTxGetEdid
 *
 * Reads the extended display identification (EDID) data
 * from the HDMI receiver and stores such information into
 * the task's corresponding data structure.
 *
 * @param edid - The corresponding EDID data structure in
 * 				 the task driver.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
static int HdmiTxGetEdid(Edid_t *edid)
{
	BYTE i2cBuffer[128];
	BYTE i2cSegment = 0;
	BYTE checksum = 0;
	int retval;
	int index;
	int index2;

	/* hdmi tx should be in the connected state */
	if( HdmiTxTask.State != HDMITX_STATE_CONNECTED ) {
		WARN( "Driver is not in the appropriate state to read the EDID." );
		return (-1);
	}

	/* read VESA EDID block */
	retval = I2cWriteRead(I2C_DEV_DDC, I2C_ADDR_E_DDC_EDID, &i2cSegment, 1, I2C_ADDR_E_DDC_EDID, &i2cBuffer[0], 128);
	if( retval != -1 ) {
		/* verify checksum for data integrity */
		for( index = 0; index < 128; index++ )
			checksum += i2cBuffer[index];
		if( checksum ) {
			WARNN( "Driver read an invalid VESA EDID block (checksum: 0x%08x).", checksum );
			return (-1);
		}
	} else {
		WARN( "Driver could not read a valid VESA EDID block." );
		return (-1);
	}

	PRINT( "VESA EDID block retrieved." );

	/* process the VESA EDID block header */
	for( index=0; index<8; index++ )
		*(((BYTE *)(&edid->VESA.Header))+index) = i2cBuffer[index];

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
					return (-1);
				}
				break;
			} else {
				/* check the next extension */
				continue;
			}
		} else {
			WARN( "Driver could not read a valid CEA EDID extenstion block." );
			return (-1);
		}
	}

	/* fail if no CEA EDID extension block is found */
	if( i2cSegment == edid->VESA.ExtensionFlag ) {
		WARN( "Driver could not found a valid CEA EDID extension block.");
		return (-1);
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

	return (DRIVER_NO_ERROR);
}

/*
 * HdmiTxHdcpLinkTest
 *
 * Performs HDCP authentication process wherein the in-
 * tegrity of the HDMI link between the transmitter and
 * receiver is tested in regular intervals.
 *
 * @param None.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
static void HdmiTxHdcpLinkTest(void)
{
	/* hdmi tx should be in the link test initiated state */
	if( HdmiTxTask.State != HDMITX_STATE_LINK_TEST_INITIATED ) {
		WARN( "Driver is not in the appropriate state to test the integrity of the link." );
		return;
	}

	return;
}

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
int HdmiTxInitialize(void)
{
	AcrPacket_t			*ACR	= &(HdmiTxTask.ACR);
	AudioPacket_t		*AUD	= &(HdmiTxTask.AUD);
	GcPacket_t			*GC		= &(HdmiTxTask.GC);
	AcpPacket_t			*ACP	= &(HdmiTxTask.ACP);
	IsrcPacket_t		*ISRC1	= &(HdmiTxTask.ISRC1);
	IsrcPacket_t		*ISRC2	= &(HdmiTxTask.ISRC2);
	OneBitAudioPacket_t	*ONE	= &(HdmiTxTask.ONE);
	DstAudioPacket_t	*DST	= &(HdmiTxTask.DST);
	HbrAudioPacket_t	*HBR	= &(HdmiTxTask.HBR);
	GamutPacket_t		*GAMUT	= &(HdmiTxTask.GAMUT);
	AviInfoFrame_t		*AVI	= &(HdmiTxTask.AVI);
	SpdInfoFrame_t		*SPD	= &(HdmiTxTask.SPD);
	AudioInfoFrame_t	*AUDIF	= &(HdmiTxTask.AUDIF);
	MpegInfoFrame_t		*MPEG	= &(HdmiTxTask.MPEG);
	VendorInfoFrame_t	*VENDOR	= &(HdmiTxTask.VENDOR);
	Edid_t				*EDID	= &(HdmiTxTask.EDID);

	/* set initial state to uninitialized */
	HdmiTxTask.State = HDMITX_STATE_UNINITIALIZED;

 	/* Initialization values are based on the following
	 * configuration:
	 *
	 * 640x480p @60 Hz, 4:3, 25.2MHz pixel clock
	 * 24-bit RGB, no pixel repetition
	 * 2 audio channels, 32kHz, 16bits
	 *
	 */

	ACR->CycleTimeStamp = 25200;
    ACR->Numerator = 4096;

    memset( (void *)AUD, 0x00, sizeof(AudioPacket_t) );

	GC->AvMute = FALSE;
	GC->PixelPacking = PP_PHASE1;
	GC->ColorDepth = CD_24BIT;
	GC->DefaultPhase = TRUE;

    memset( (void *)ACP, 0x00, sizeof(AcpPacket_t) );
    memset( (void *)ISRC1, 0x00, sizeof(IsrcPacket_t) );
    memset( (void *)ISRC2, 0x00, sizeof(IsrcPacket_t) );
    memset( (void *)ONE, 0x00, sizeof(OneBitAudioPacket_t) );
    memset( (void *)DST, 0x00, sizeof(DstAudioPacket_t) );
    memset( (void *)HBR, 0x00, sizeof(HbrAudioPacket_t) );
    memset( (void *)GAMUT, 0x00, sizeof(GamutPacket_t) );

	AVI->ColorIndicator = CI_RGB;
	AVI->ActiveInfo	= TRUE;
	AVI->BarInfo = BI_INVALID;
	AVI->ScanInfo = SI_NO_DATA;
	AVI->Colorimetry = C_NO_DATA;
	AVI->PictureAspectRatio	=AR_4X3;
	AVI->FormatAspectRatio = AF_SAME;
	AVI->ITContent = TRUE;
	AVI->ExtendedColorimetry = EC_XVYCC601;
	AVI->QuantizationRange = QR_DEFAULT;
	AVI->PictureScaling = PS_UNKNOWN;
	AVI->VideoIdCode = VIC_640X480P_60HZ;
    AVI->YCCQuantizationRange = YQ_FULL;
    AVI->ITContentType = ICT_GRAPHICS;
	AVI->PixelRepetition = PR_0;
	AVI->TopBarEndLineNum = 0;
	AVI->BottomBarStartLineNum = 481;
	AVI->LeftBarEndPixelNum = 0;
	AVI->RightBarStartPixelNum = 641;

	SPD->VendorName[0] = 'R';
	SPD->VendorName[1] = 'I';
	SPD->VendorName[2] = 'M';
	SPD->VendorName[3] = 'M';
	SPD->ProductDescription[0] = 'B';
	SPD->ProductDescription[1] = 'l';
	SPD->ProductDescription[2] = 'a';
	SPD->ProductDescription[3] = 'c';
	SPD->ProductDescription[4] = 'k';
	SPD->ProductDescription[5] = 'B';
	SPD->ProductDescription[6] = 'e';
	SPD->ProductDescription[7] = 'r';
	SPD->ProductDescription[8] = 'r';
	SPD->ProductDescription[9] = 'y';
	SPD->SourceDevice = DI_HDD_VIDEO;

    AUDIF->CodingType = AIC_STREAM;
    AUDIF->ChannelCount = AC_2CH;
    AUDIF->SamplingFrequency = SF_32KHZ;
    AUDIF->SampleSize = SS_16BIT;
    AUDIF->ChannelAllocation = 0x00;
    AUDIF->LevelShiftValue = LSV_0DB;
    AUDIF->DownMixInhibit = FALSE;
    AUDIF->LFEPlaybackLevel = PL_UNKNOWN;

    memset( (void *)MPEG, 0x00, sizeof(MpegInfoFrame_t) );

    VENDOR->IEEERegistraionId = 0x0052494D;
    VENDOR->Payload[0] = 'B';
    VENDOR->Payload[1] = 'l';
    VENDOR->Payload[2] = 'a';
    VENDOR->Payload[3] = 'c';
    VENDOR->Payload[4] = 'k';
    VENDOR->Payload[5] = 'B';
    VENDOR->Payload[6] = 'e';
    VENDOR->Payload[7] = 'r';
    VENDOR->Payload[8] = 'r';
    VENDOR->Payload[9] = 'y';
    VENDOR->PayloadLength = 10;

    memset( (void *)EDID, 0x00, sizeof(Edid_t) );

	/* initialize underlying hardware controller */
	if( HdmiTxHwInitialize() != DRIVER_NO_ERROR ) {
		WARN( "Hardware could not be initialized.");
		return (-1);
	}

	/* transition to disconnected state */
	HdmiTxTask.State = HDMITX_STATE_DISCONNECTED;

	/* setup and enable hdmi detection interrupt */
    RE_PadConfigure( HDMI_DET, &HdmiTxDetectPadConfig, NULL );
    RE_PadClear( HDMI_DET, PAD_OUTPUT_ENABLE );
    RE_PadSet( HDMI_DET, PAD_INPUT_ENABLE );
    RE_PadSetMode( HDMI_DET, HDMI_DET_FN );
    RE_PadInterruptClear( HDMI_DET );
    RE_PadInterruptEnable( HDMI_DET );
    RE_EnableInterrupt( HWIRQ_GPIO );

    /* register message handler */
    PfMsgRegister(PLATFORM_THREAD_ID_DRV, PLATFORM_MSG_HDMI_DRV, HdmiTxMsgHandler);

    PRINT( "HDMI TX initialized successfully." );

	/* task initialization completed */
	return (DRIVER_NO_ERROR);
}

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
	HdmiTxConfig_t *config)
{
	ColorDepth_t color_depth = CD_24BIT;
	ColorIndicator_t color_indicator = CI_RGB;

	/* hdmi tx should either be in the connected or authenticated state */
	if( (HdmiTxTask.State != HDMITX_STATE_CONNECTED) && (HdmiTxTask.State != HDMITX_STATE_AUTHENTICATED) ) {
		WARN( "Driver is not in the appropriate state to be enabled." );
		return (-1);
	}

	/* derived color indicator from config */
	if( (config->SrcFormat == PF_YUV422P) |
		(config->SrcFormat == PF_YUV422PL) |
		(config->SrcFormat == PF_YUV420PL) ) {
		color_indicator = CI_YCC422;
	}

	/* configure driver and controller */
	if( HdmiTxConfigure(
			color_depth,
			color_indicator,
			(AudioChannel_t)config->AudChanCount,
			(SampleFreq_t)config->SampleFreq,
			(SampleSize_t)config->SampleSize ) != DRIVER_NO_ERROR ) {
		WARN("Driver could not be configured." );
		return (-1);
	}

	/* enable HDMI TX hardware controller */
	if( HdmiTxHwEnable(TRUE) != DRIVER_NO_ERROR ) {
		WARN("Hardware could not be enabled." );
		return (-1);
	}

    PRINT( "HDMI TX enabled successfully." );

	return (DRIVER_NO_ERROR);
}

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
int HdmiTxDisable(void)
{
	/* hdmi tx should be at least in the connected state */
	if( HdmiTxTask.State < HDMITX_STATE_CONNECTED ) {
		WARN( "Driver is not in the appropriate state to be enabled." );
		return (-1);
	}

	/* disable HDMI TX hardware controller */
	if( HdmiTxHwEnable(FALSE) != DRIVER_NO_ERROR ) {
		WARN("Hardware could not be disabled." );
		return (-1);
	}

    PRINT( "HDMI TX disabled successfully." );

	return (DRIVER_NO_ERROR);
}

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
int HdmiTxStartTransmit(void)
{
	/* hdmi tx should be in the connected or authenticated state */
	if( (HdmiTxTask.State != HDMITX_STATE_CONNECTED) && (HdmiTxTask.State != HDMITX_STATE_AUTHENTICATED) ) {
		WARN( "Driver is not in the appropriate state to transmit." );
		return (-1);
	}

	/* command Display controller to start transmission */
	if( DispHwTransmit(TRUE) != DRIVER_NO_ERROR ){
		WARN( "Hardware (display controller) could not start transmission." );
		return (-1);
	}

	/* command audio hardware to start transmission */
	if( AudioHwTransmit(TRUE) != DRIVER_NO_ERROR ){
		WARN( "Hardware (audio) could not start transmission." );
		return (-1);
	}

	/* command HDMI TX controller to start transmission */
	if( HdmiTxHwTransmit(TRUE) != DRIVER_NO_ERROR ) {
		WARN("Hardware could not start transmission." );
		return (-1);
	}

    PRINT( "HDMI TX started transmission successfully." );

    return (DRIVER_NO_ERROR);
}

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
int HdmiTxStopTransmit(void)
{
	/* hdmi tx should be at least in the connected state */
	if( HdmiTxTask.State < HDMITX_STATE_CONNECTED ) {
		WARN( "Driver is not in the appropriate state to be enabled." );
		return (-1);
	}

	/* command HDMI TX hardware controller to stop transmission */
	if( HdmiTxHwTransmit(FALSE) != DRIVER_NO_ERROR ) {
		WARN("Hardware could not stop transmission." );
		return (-1);
	}

	/* command Display hardware controller to stop transmission */
	if( DispHwTransmit(FALSE) != DRIVER_NO_ERROR ){
		WARN( "Hardware (display controller) could not stop transmission." );
		return (-1);
	}

	/* command audio hardware to stop transmission */
	if( AudioHwTransmit(FALSE) != DRIVER_NO_ERROR ){
		WARN( "Hardware (audio) could not stop transmission." );
		return (-1);
	}

    PRINT( "HDMI TX stopped transmission successfully." );

	return (DRIVER_NO_ERROR);
}

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
int HdmiTxAvMute(BOOL avmute)
{
	GcPacket_t *GC = &(HdmiTxTask.GC);

	/* hdmi tx should either be in the connected, authenticated or muted state */
	if( avmute ) {
		if( (HdmiTxTask.State != HDMITX_STATE_CONNECTED) && (HdmiTxTask.State != HDMITX_STATE_AUTHENTICATED) ) {
			WARN( "Driver is not in the appropriate state to set AV mute." );
			return (-1);
		}
	} else {
		if( (HdmiTxTask.State != HDMITX_STATE_CONNECTED) && (HdmiTxTask.State != HDMITX_STATE_MUTED) ) {
			WARN( "Driver is not in the appropriate state to clear AV mute." );
			return (-1);
		}
	}

	/* configure AV mute flag */
	GC->AvMute = (avmute) ? TRUE : FALSE;
	if( HdmiTxHwPacketize( PT_GENERAL_CONTROL, GC ) ) {
		WARN( "Hardware could not re-enable the general control packet.");
		return (-1);
	}

	/* set state appropriately */
	if( avmute ) {
		if( HdmiTxTask.State == HDMITX_STATE_AUTHENTICATED ) {
			/* since sending encrypted signal, transition state to muted */
			HdmiTxTask.State = HDMITX_STATE_MUTED;
		}
	} else {
		if( HdmiTxTask.State == HDMITX_STATE_MUTED ) {
			/* since encrypted signal has been muted, transition state to un-muted */
			HdmiTxTask.State = HDMITX_STATE_AUTHENTICATED;
		}
	}

	/* AV mute configuration completed */
	return (DRIVER_NO_ERROR);
}

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
void HdmiTxConnect(void)
{
	RE_MESSAGE message;

	/* hdmi tx should be in disconnected state */
	if( HdmiTxTask.State != HDMITX_STATE_DISCONNECTED ) {
		WARN( "Driver is not in the appropriate state to be connected." );
		return;
	}

	/* construct and post message to read EDID */
	message.Event   = (DWORD) PLATFORM_MSG_HDMI_DRV;
	message.SubMsg  = (DWORD) HDMITX_MSG_CONNECT;
	message.DataPtr = (char *) &HdmiTxTask;
	RE_RimPostMessage( PLATFORM_THREAD_ID_DRV, &message );

	return;
}

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
void HdmiTxDisconnect(void)
{
	RE_MESSAGE message;

	/* construct and post message to stop transmission and disable driver */
	message.Event   = (DWORD) PLATFORM_MSG_HDMI_DRV;
	message.SubMsg  = (DWORD) HDMITX_MSG_DISCONNECT;
	message.DataPtr = (char *) &HdmiTxTask;
	RE_RimPostMessage( PLATFORM_THREAD_ID_DRV, &message );

	return;
}

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
void HdmiTxDetectHandler(void)
{
	PRINT( "HDMI device connected/disconnected" );

	/* disable and clear hdmi detect interrupt */
    RE_PadInterruptDisable( HDMI_DET );

	if( RE_PadRead(HDMI_DET, PAD_INPUT) ) {
		HdmiTxConnect();
	} else {
		HdmiTxDisconnect();
	}

	/* enable back hdmi detect interrupt */
    RE_PadInterruptEnable( HDMI_DET );

	/* hdmi detect interrupt handling completed */
	return;
}

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
void HdmiTxMsgHandler(RE_MESSAGE *message)
{
	HdmiTxTask_t *task = (HdmiTxTask_t *)message->DataPtr;

    if( message->SubMsg == (DWORD) HDMITX_MSG_CONNECT ) {
    	PRINT( "HDMI device connected." );
		/* transition to connected state */
		task->State = HDMITX_STATE_CONNECTED;
    	/* read EDID via display data channel */
    	if( HdmiTxGetEdid(&(task->EDID)) != DRIVER_NO_ERROR ) {
    		WARN( "Driver could not read the extended display identification data." );
        }
    } else if( message->SubMsg == (DWORD) HDMITX_MSG_DISCONNECT ) {
    	PRINT( "HDMI device disconnected." );
    	/* stop signal transmission */
    	if( HdmiTxStopTransmit() == DRIVER_NO_ERROR ) {
    	   	/* disable the device driver */
        	if( HdmiTxDisable() == DRIVER_NO_ERROR ) {
        		/* transition to disconnected state */
        		task->State = HDMITX_STATE_DISCONNECTED;
        	} else {
        		WARN( "Driver could not be disabled." );
        	}
       } else {
    		WARN( "Driver could not stop signal transmission." );
       }
    }

    /* message handling completed */
	return;
}

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
int HdmiTxHdcpAuthenticate(void)
{
	/* hdmi tx should be in the connected state */
	if( HdmiTxTask.State != HDMITX_STATE_CONNECTED ) {
		WARN( "Driver is not in the appropriate state to authenticate." );
		return (-1);
	}

	return (DRIVER_NO_ERROR);
}

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
void HdmiTxHdcpIrqHandler(void)
{
	/* hdmi tx should be in the authenticated state */
	if( HdmiTxTask.State != HDMITX_STATE_AUTHENTICATED ) {
		WARN( "Driver is not in the appropriate state to receive HDCP interrupts." );
		return;
	}

	return;
}


