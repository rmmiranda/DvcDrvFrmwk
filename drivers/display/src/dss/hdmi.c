/*****************************************************************************
 * Filename:    hdmi.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     Aug 30, 2010
 *
 * Description: Display subsystem HDMI driver implementation
 *
 *****************************************************************************/

#include <string.h>

#include "rei.h"
#include "framework.h"
#include "gpio_platform.h"
#include "display_internal.h"
#include "display_reg.h"
#include "hdmi.h"
#include "hdmi_reg.h"
#include "ddc.h"

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

#define COLOR_DEPTH_INDEX(x) ((x) - CD_24BIT)
#define SAMPLE_FREQ_INDEX(x) ((x) - SF_32KHZ)
#define SAMPLE_SIZE_INDEX(x) ((x) - SS_16BIT)

#define HdmiCtrlConfig		(DisplayDriver.config.hdmiConfig)
#define HdmiCtrlStatus		(DisplayDriver.status.hdmiStatus)
#define HdmiCtrlStatis		(DisplayDriver.status.hdmiConfig)
#define HdmiController		(DisplayDriver.hdmiCtl)

/*
 ***************************************************
 ***************************************************
 *
 * Static global variable declarations
 *
 ***************************************************
 ***************************************************
 */

//static AcrPacket_t HdmiAcrParam[TC_MAX][COLOR_DEPTH_INDEX(CD_MAX)][SAMPLE_FREQ_INDEX(SF_MAX)] = {
static AcrPacket_t HdmiAcrParam[][][] = {
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
    { 1920,   88,  44, 148, 1080,  2, 5, 15, 'I', AR_16X9, PR_0, TC_74P250MHZ }, // (05) 1920x1080i @59.94/60 Hz
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

static BYTE SampleFreqCSB[] = { 0x03, 0x00, 0x02, 0x08, 0x0A, 0x0C, 0x0E };
static BYTE SampleSizeCSB[] = { 0x02, 0x03, 0x0B };
static BYTE I2SDataLength[] = { 0x10, 0x14, 0x18 };

/*
 ***************************************************
 ***************************************************
 *
 * static function definitions
 *
 ***************************************************
 ***************************************************
 */

static void HdmiClockConfig(
	TmdsClock_t tmds_clock,
	PixelRepetition_t pixel_repetition,
	ColorDepth_t color_depth)
{
	DWORD postDiv;
	DWORD fbDiv;
	DWORD refDiv;
	DWORD freqOffsInner;
	DWORD kvco;
	DWORD pClkDiv;
	DWORD prClkDiv;
	DWORD tClkDiv;
	DWORD mClkDiv;

	/* determine appropriate pll parameters */
	switch( tmds_clock ) {
		case TC_25P175MHZ:
		case TC_25P200MHZ:
			/* set hdmi pll to 125.875MHz, TMDS clock divider to 5 */
			kvco = 0x0C;
			postDiv = 0x04;
			freqOffsInner = 0x4EC5;
			fbDiv = 38;
			refDiv = 16;
			break;
		case TC_74P175MHZ:
		case TC_74P250MHZ:
			/* set hdmi pll to 371.25MHz, TMDS clock divider to 5 */
			kvco = 0x03;
			postDiv = 0x02;
			freqOffsInner = 0x084B;
			fbDiv = 57;
			refDiv = 16;
			break;
		case TC_27P000MHZ:
		case TC_27P027MHZ:
			/* set hdmi pll to 135MHz, TMDS clock divider to 5 */
			kvco = 0x0B;
			postDiv = 0x04;
			freqOffsInner = 0x35CB;
			fbDiv = 41;
			refDiv = 16;
			break;
		case TC_54P000MHZ:
		case TC_54P054MHZ:
			/* set hdmi pll to 270MHz, TMDS clock divider to 5 */
			kvco = 0x0B;
			postDiv = 0x02;
			freqOffsInner = 0x35CB;
			fbDiv = 41;
			refDiv = 16;
			break;
		case TC_108P00MHZ:
			/* set hdmi pll to 540MHz, TMDS clock divider to 5 */
			kvco = 0x0B;
			postDiv = 0x01;
			freqOffsInner = 0x35CB;
			fbDiv = 41;
			refDiv = 16;
			break;
		case TC_59P400MHZ:
		case TC_72P000MHZ:
		case TC_297P00MHZ:
			WARNN( "Hardware could not support such TMDS clock (%d).", tmds_clock );
		case TC_148P35MHZ:
		case TC_148P50MHZ:
		default:
			/* set hdmi pll to 742.5MHz, TMDS clock divider to 5 */
			kvco = 0x03;
			postDiv = 0x01;
			freqOffsInner = 0x084B;
			fbDiv = 57;
			refDiv = 16;
			break;
	}

	/* set TMDS clock to the fixed value 5 */
	tClkDiv = 5;

	/* compute pixel repetition clock divider */
	switch( color_depth ) {
		case CD_30BIT:
			prClkDiv = (5 * tClkDiv) / 4;
			break;
		case CD_36BIT:
			prClkDiv = (3 * tClkDiv) / 2;
			break;
		case CD_48BIT:
			prClkDiv = 2 * tClkDiv;
			break;
		case CD_24BIT:
		default:
			prClkDiv = tClkDiv;
			break;
	}

	/* compute pixel clock divider */
	pClkDiv = prClkDiv * (pixel_repetition + 1);

	/* start hdmi reference and bus clocks */
	WriteRegDwordOr( PMUA_DISP1_CTRL_REG, DISP_AXI_CLK_EN | DISP_AXI_CLK_RST );
	WriteRegDwordOr( PMUA_DISP1_CTRL_REG, HDMI_I2S_CLK_EN | HDMI_REF_CLK_EN );

	/* reset phy and enable termination resistor */
	WriteRegDword( HDMI_PHY_CFG_2_REG, 0x00000000 );
	WriteRegDword( HDMI_PHY_CFG_2_REG, HDMI_PHY_CFG_RESET_TX );
	WriteRegDword( HDMI_PHY_CFG_2_REG, HDMI_PHY_CFG_AUX(1) );
	WriteRegDword( HDMI_PHY_CFG_3_REG, HDMI_PHY_CFG_SYNC );

	/* power down hdmi pll */
	WriteRegDwordAnd( HDMI_PLL_CFG_3_REG, ~(HDMI_PLL_CFG_ON | HDMI_PLL_CFG_RSTB) );

	/* configure hdmi pll registers */
	WriteRegDwordAnd( HDMI_PLL_CFG_0_REG, ~(HDMI_PLL_CFG_KVCO_MASK | HDMI_PLL_CFG_POSTDIV_MASK) );
	WriteRegDwordAnd( HDMI_PLL_CFG_1_REG, ~(HDMI_PLL_CFG_FO_INNER_MASK) );
	WriteRegDwordAnd( HDMI_PLL_CFG_3_REG, ~(HDMI_PLL_CFG_FBDIV_MASK | HDMI_PLL_CFG_REFDIV_MASK) );
	WriteRegDwordOr( HDMI_PLL_CFG_0_REG, HDMI_PLL_CFG_KVCO(kvco) | HDMI_PLL_CFG_POSTDIV(postDiv) );
	WriteRegDwordOr( HDMI_PLL_CFG_1_REG, HDMI_PLL_CFG_FO_INNER(freqOffsInner) );
	WriteRegDwordOr( HDMI_PLL_CFG_3_REG, HDMI_PLL_CFG_FBDIV(fbDiv) | HDMI_PLL_CFG_REFDIV(refDiv) );

	/* power up and reset hdmi pll */
	WriteRegDwordOr( HDMI_PLL_CFG_3_REG, HDMI_PLL_CFG_ON );
	RE_HwDelay( 100 * RE_MICROSECOND );
	WriteRegDwordOr( HDMI_PLL_CFG_3_REG, HDMI_PLL_CFG_RSTB );
	while( (ReadRegDword(HDMI_PLL_CFG_3_REG) & HDMI_PLL_CFG_LOCK) != HDMI_PLL_CFG_LOCK )
		PRINT( "Waiting for HDMI PLL to Lock ..." );
	RE_HwDelay( 500 * RE_MICROSECOND );

	/* derive mclk divider from SSPA1 mclk (128xFs) rather than from SSPA1 sclk (64xFs) */
	mClkDiv = (ReadRegDword( SSPA_AUD_CTRL0_REG ) & SSPA1_CLK_DIV2_PREE_MASK) >> SSPA1_CLK_DIV2_PREE_SHIFT;
	if( mClkDiv == ((ReadRegDword( SSPA_AUD_CTRL0_REG ) & SSPA1_CLK_DIV1_PREE_MASK) >> SSPA1_CLK_DIV1_PREE_SHIFT) ) {
		/* there are 8 audio channels since SSPA1 mclk = sclk = 256xFs */
		mClkDiv *= 2;
	}

	/* set hdmi clock dividers */
	WriteRegDword( HDMI_CLOCK_CFG_REG,
			HDMI_CLOCK_CFG_PRCLK_DIV(prClkDiv) |
			HDMI_CLOCK_CFG_TCLK_DIV(tClkDiv) |
			HDMI_CLOCK_CFG_MCLK_DIV(mClkDiv) |
			HDMI_CLOCK_CFG_RTC(1) |
			HDMI_CLOCK_CFG_WTC(1) );
	PRINTN( "Pixel Clock Divider: %d", pClkDiv );
	PRINTN( "Pixel Repetition Clock Divider: %d", prClkDiv );
	PRINTN( "TMDS Clock Divider: %d", tClkDiv );
	PRINTN( "Audio Clock Divider: %d", mClkDiv );

	/* set TMDS clock */
    HdmiWriteRegByte( HDMI_TDATA3_0_OFFS, 0xE0);
    HdmiWriteRegByte( HDMI_TDATA3_1_OFFS, 0x83);
    HdmiWriteRegByte( HDMI_TDATA3_2_OFFS, 0x0F);

	/* select hdmi pll for tv path clock and set clock divider fixed to 5 */
	WriteRegDwordAnd( LCD_TCLK_DIV_REG, ~(TCLK_SOURCE_MASK | HDMI_TCLKNX_DIV_MASK | CLK_TV_INT_DIV_MASK) );
	WriteRegDwordOr( LCD_TCLK_DIV_REG, TCLK_SOURCE_HDMI_PLL | HDMI_TCLKNX_DIV(pixel_repetition + 1) | CLK_TV_INT_DIV(pClkDiv) );

	return;
}

void HdmiPacketConfig( PacketType_t packet, void *payload )
{
    int index;
    BYTE checksum;

    switch( packet ) {

        case PT_NULL:
            /* automatically generated by the hardware
             * if there are no Data island packets to be
             * sent in two consecutive fields.
             */
            break;

        case PT_AUDIO_CLOCK_REGENERATION:
        {
            /* get a handle to the packet payload */
        	AcrPacket_t *ACR = (AcrPacket_t *)(payload);

            /* set up ACR N registers */
            HdmiWriteRegByte( HDMI_ACR_N0_OFFS, ((ACR->Numerator & 0x000000FF) >>  0) );
            HdmiWriteRegByte( HDMI_ACR_N1_OFFS, ((ACR->Numerator & 0x0000FF00) >>  8) );
            HdmiWriteRegByte( HDMI_ACR_N2_OFFS, ((ACR->Numerator & 0x000F0000) >> 16) );

            /* determine if CTS are hardware computed */
            if( HdmiReadRegByte( HDMI_ACR_CTRL_OFFS ) & CTS_SEL ) {
                /* software to program ACR CTS registers */
                HdmiWriteRegByte( HDMI_ACR_CTS0_OFFS, ((ACR->CycleTimeStamp & 0x000000FF) >>  0) );
                HdmiWriteRegByte( HDMI_ACR_CTS1_OFFS, ((ACR->CycleTimeStamp & 0x0000FF00) >>  8) );
                HdmiWriteRegByte( HDMI_ACR_CTS2_OFFS, ((ACR->CycleTimeStamp & 0x000F0000) >> 16) );
            }

        	/* automatically generated by the hardware
             * as long as the ACR "N" and CTS registers
             * are properly set.
             */
            break;
        }

        case PT_AUDIO_SAMPLE:
            /* automatically generated by the hardware
             * if any audio samples are available in the
             * audio fifo and audio transmission is enabled.
             */
            break;

        case PT_GENERAL_CONTROL:
        {
            /* get a handle to the packet payload */
        	GcPacket_t *GC = (GcPacket_t *)(payload);

            /* configure AV mute and default phase */
            HdmiWriteRegByteOr( HDMI_GCP_CFG0_OFFS, ((GC->AvMute) ? AVMUTE : 0) );
            HdmiWriteRegByteOr( HDMI_GCP_CFG0_OFFS, GC->DefaultPhase );

            /* let hardware program pixel packing phase (pp) values */
            HdmiWriteRegByteAnd( HDMI_GCP_CFG0_OFFS, ~PP_SW_VAL );

            /* handle deep color mode (30-bit or 36-bit) if needed */
            HdmiWriteRegByteAnd( HDMI_GCP_CFG1_OFFS, ~COL_DEPTH_MASK );
            switch( GC->ColorDepth ) {
                case CD_30BIT:
                    HdmiWriteRegByteOr( HDMI_GCP_CFG1_OFFS, COL_DEPTH_10_BIT );
                    break;
                case CD_36BIT:
                    HdmiWriteRegByteOr( HDMI_GCP_CFG1_OFFS, COL_DEPTH_12_BIT );
                    break;
                case CD_24BIT:
                default:
                    HdmiWriteRegByteOr( HDMI_GCP_CFG1_OFFS, COL_DEPTH_8_BIT );
                    break;
            }

            /* automatically generated by the hardware
             * as long as the GCP configure registers
             * are properly set.
             */
            break;
        }

        case PT_AUDIO_CONTENT_PROTECTION:
        {
            /* get a handle to the packet payload */
        	AcpPacket_t *ACP = (AcpPacket_t *)(payload);

            /* software to construct the ACP packet in the
             * host PKT5 buffers and to control transmission
             * using the host PKT5 control registers.
             */

            /* construct the header packets (HB0-HB2) */
            HdmiWriteRegByte( HDMI_PKT5_BYTE_OFFS(0), packet );
            HdmiWriteRegByte( HDMI_PKT5_BYTE_OFFS(1), ACP->Type );
            HdmiWriteRegByte( HDMI_PKT5_BYTE_OFFS(2), 0x00 );
            switch( ACP->Type ) {
                case AT_GENERIC_AUDIO:
                case AT_IEC60958_AUDIO:
                    /* ACP type dependent fields (PB0-PB27) are all reserved */
                    for( index=3; index<31; index++ ) {
                        HdmiWriteRegByte( HDMI_PKT5_BYTE_OFFS(index), 0x00 );
                    }
                    break;
                case AT_DVD_AUDIO:
                    /* DVD audio specific fields (PB0-PB1) */
                    HdmiWriteRegByte( HDMI_PKT5_BYTE_OFFS(3), ACP->DvdTypeGeneration );
                    HdmiWriteRegByte( HDMI_PKT5_BYTE_OFFS(4),
                            ((ACP->DvdCopyPermission & 0x3) << 6)  |
                            ((ACP->DvdCopyNumber & 0x7) << 3) |
                            ((ACP->DvdQuality & 0x3) << 1) |
                            ((ACP->DvdTransaction & 0x1) << 0) );
                    /* all other fields (PB2-PB27) are all reserved */
                    for( index=5; index<31; index++ ) {
                        HdmiWriteRegByte( HDMI_PKT5_BYTE_OFFS(index), 0x00 );
                    }
                    break;
                case AT_SUPER_AUDIO_CD:
                    /* super audio CD specific fields (PB0-PB15) */
                    for( index=3; index<19; index++ ) {
                        HdmiWriteRegByte( HDMI_PKT5_BYTE_OFFS(index), ACP->SuperAudioCdCci1[index-3] );
                    }
                    /* all other fields (PB16-PB27) are all reserved */
                    for( index=19; index<31; index++ ) {
                        HdmiWriteRegByte( HDMI_PKT5_BYTE_OFFS(index), 0x00 );
                    }
                    break;
                default:
                    WARNN( "Invalid Audio Content Protection Type: %d", ACP->Type );
                    return;
            }

            /* configure ACP packet to transmit in every frame */
            HdmiWriteRegByteOr( HDMI_HOST_PKT_CTRL1_OFFS, PKT5_TX_MODE );
            break;
        }

        case PT_ISRC1:
        {
            /* get a handle to the packet payload */
            IsrcPacket_t *ISRC = (IsrcPacket_t *)(payload);

            /* software to construct the ISRC1 packet in the
             * host PKT1 buffers and to control transmission
             * using the host PKT1 control registers.
             */

            /* construct the header packets (HB0-HB2) */
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(0), packet );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(1),
                    ((ISRC->Continue & 0x1) << 7) |
                    ((ISRC->Valid & 0x1) << 6) |
                    ((ISRC->Status & 0x7) << 0) );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(2), 0x00 );

            /* construct ISRC1 packet specific subpackets (PB0-PB15) */
            for( index=3; index<19; index++ ) {
                HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(index), ISRC->Isrc[index-3] );
            }

            /* all other fields (PB16-PB27) are all reserved */
            for( index=19; index<31; index++ ) {
                HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(index), 0x00 );
            }

            /* configure ISRC1 packet to transmit in every frame */
            HdmiWriteRegByteOr( HDMI_HOST_PKT_CTRL1_OFFS, PKT1_TX_MODE );
            break;
        }

        case PT_ISRC2:
        {
            /* get a handle to the packet payload */
            IsrcPacket_t *ISRC = (IsrcPacket_t *)(payload);

            /* software to construct the ISRC2 packet in the
             * host PKT2 buffers and to control transmission
             * using the host PKT2 control registers.
             */

            /* construct the header packets (HB0-HB2) */
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(0), packet );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(1), 0x00 );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(2), 0x00 );

            /* construct ISRC2 packet specific subpackets (PB0-PB15) */
            for( index=3; index<19; index++ ) {
                HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(index), ISRC->Isrc[(index-3)+16] );
            }

            /* all other fields (PB16-PB27) are all reserved */
            for( index=19; index<31; index++ ) {
                HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(index), 0x00 );
            }

            /* configure ISRC2 packet to transmit in every frame */
            HdmiWriteRegByteOr( HDMI_HOST_PKT_CTRL1_OFFS, PKT2_TX_MODE );
            break;
        }

        case PT_ONEBIT_AUDIO_SAMPLE:
        case PT_DST_AUDIO:
        case PT_HBR_AUDIO_STREAM:
        case PT_GAMUT_METADATA:
            /* currently not supported packets so do nothing */
            break;

        case PT_VENDOR_INFOFRAME:
        {
            /* get a handle to the packet payload */
        	VendorInfoFrame_t *VEN = (VendorInfoFrame_t *)(payload);

        	/* software to construct the vendor infoframe packet
             * in the host PKT0 buffers and to control transmission
             * using the host PKT0 control registers.
             */

            /* construct the header packets (HB0-HB2) */
            HdmiWriteRegByte( HDMI_PKT0_BYTE_OFFS(0), packet );
            HdmiWriteRegByte( HDMI_PKT0_BYTE_OFFS(1), 0x01 );
            HdmiWriteRegByte( HDMI_PKT0_BYTE_OFFS(2), VEN->PayloadLength + 3 );

            /* construct the sub packets (PB0-PBn) */
            HdmiWriteRegByte( HDMI_PKT0_BYTE_OFFS(3), 0x00 );
            HdmiWriteRegByte( HDMI_PKT0_BYTE_OFFS(4), VEN->IEEERegistraionId & 0xFF );
            HdmiWriteRegByte( HDMI_PKT0_BYTE_OFFS(5), (VEN->IEEERegistraionId & 0xFF00) >> 8 );
            HdmiWriteRegByte( HDMI_PKT0_BYTE_OFFS(6), (VEN->IEEERegistraionId & 0xFF0000) >> 16 );
            for( index=0; index<VEN->PayloadLength; index++ ) {
                HdmiWriteRegByte( HDMI_PKT0_BYTE_OFFS(index+7), VEN->Payload[index] );
            }

            /* all other fields (PBn-PB27) are all reserved */
            for( index=VEN->PayloadLength+7; index<31; index++ ) {
                HdmiWriteRegByte( HDMI_PKT0_BYTE_OFFS(index), 0x00 );
            }

            /* compute and include checksum into the packet */
            for( checksum=0, index=3; index<31; index++ ) {
                checksum += HdmiReadRegByte( HDMI_PKT0_BYTE_OFFS(index) );
            }
            checksum = (BYTE)(0x100 - checksum);
            HdmiWriteRegByte( HDMI_PKT0_BYTE_OFFS(3), checksum );

            /* configure vendor infoframe packet to transmit in every frame */
            HdmiWriteRegByteOr( HDMI_HOST_PKT_CTRL1_OFFS, PKT0_TX_MODE );
            break;
        }

        case PT_AVI_INFOFRAME:
        {
            /* get a handle to the packet payload */
        	AviInfoFrame_t *AVI = (AviInfoFrame_t *)(payload);

            /* software to construct the AVI infoframe packet in
             * the host PKT1 buffers and to control transmission
             * using the host PKT1 control registers.
             */

            /* construct the header packets (HB0-HB2) */
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(0), packet );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(1), 0x02 );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(2), 0x0D );

            /* construct the sub packets (PB0-PB13) */
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(3), 0x00 );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(4),
                    ((AVI->ColorIndicator & 0x3) << 5) |
                    ((AVI->ActiveInfo) ? 0x10 : 0x00) |
                    ((AVI->BarInfo & 0x3) << 2) |
                    ((AVI->ScanInfo & 0x3) << 0) );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(5),
                    ((AVI->Colorimetry & 0x3) << 6) |
                    ((AVI->PictureAspectRatio & 0x3) << 4) |
                    ((AVI->FormatAspectRatio & 0xF) << 0) );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(6),
                    ((AVI->ITContent) ? 0x80 : 0x00) |
                    ((AVI->ExtendedColorimetry & 0x7) << 4) |
                    ((AVI->QuantizationRange & 0x3) << 2) |
                    ((AVI->PictureScaling & 0x3) << 0) );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(7), (AVI->VideoIdCode & 0x7F) );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(8),
            		((AVI->YCCQuantizationRange & 0x3) << 6) |
            		((AVI->ITContentType & 0x3) << 4 ) |
            		((AVI->PixelRepetition & 0xF) << 0) );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(9), (AVI->TopBarEndLineNum & 0xFF) );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(10), ((AVI->TopBarEndLineNum >> 8) & 0xFF) );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(11), (AVI->BottomBarStartLineNum & 0xFF) );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(12), ((AVI->BottomBarStartLineNum >> 8) & 0xFF) );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(13), (AVI->LeftBarEndPixelNum & 0xFF) );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(14), ((AVI->LeftBarEndPixelNum >> 8) & 0xFF) );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(15), (AVI->RightBarStartPixelNum & 0xFF) );
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(16), ((AVI->RightBarStartPixelNum >> 8) & 0xFF) );

            /* all other fields (PB14-PB27) are all reserved */
            for( index=17; index<31; index++ ) {
                HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(index), 0x00 );
            }

            /* compute and include checksum into the packet */
            for( checksum=0, index=3; index<31; index++ ) {
                checksum += HdmiReadRegByte( HDMI_PKT1_BYTE_OFFS(index) );
            }
            checksum = (BYTE)(0x100 - checksum);
            HdmiWriteRegByte( HDMI_PKT1_BYTE_OFFS(3), checksum );

            /* configure AVI infoframe packet to transmit in every frame */
            HdmiWriteRegByteOr( HDMI_HOST_PKT_CTRL1_OFFS, PKT1_TX_MODE );
            break;
        }

        case PT_SPD_INFOFRAME:
        {
            /* get a handle to the packet payload */
        	SpdInfoFrame_t *SPD = (SpdInfoFrame_t *)(payload);

        	/* software to construct the vendor infoframe packet
             * in the host PKT2 buffers and to control transmission
             * using the host PKT2 control registers.
             */

            /* construct the header packets (HB0-HB2) */
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(0), packet );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(1), 0x01 );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(2), 0x19 );

            /* construct the sub packets (PB0-PB25) */
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(3), 0x00 );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(4), SPD->VendorName[0] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(5), SPD->VendorName[1] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(6), SPD->VendorName[2] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(7), SPD->VendorName[3] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(8), SPD->VendorName[4] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(9), SPD->VendorName[5] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(10), SPD->VendorName[6] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(11), SPD->VendorName[7] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(12), SPD->ProductDescription[0] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(13), SPD->ProductDescription[1] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(14), SPD->ProductDescription[2] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(15), SPD->ProductDescription[3] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(16), SPD->ProductDescription[4] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(17), SPD->ProductDescription[5] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(18), SPD->ProductDescription[6] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(19), SPD->ProductDescription[7] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(20), SPD->ProductDescription[8] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(21), SPD->ProductDescription[9] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(22), SPD->ProductDescription[10] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(23), SPD->ProductDescription[11] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(24), SPD->ProductDescription[12] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(25), SPD->ProductDescription[13] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(26), SPD->ProductDescription[14] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(27), SPD->ProductDescription[15] );
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(28), SPD->SourceDevice );

            /* all other fields (PB26-PB27) are all reserved */
            for( index=29; index<31; index++ ) {
                HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(index), 0x00 );
            }

            /* compute and include checksum into the packet */
            for( checksum=0, index=3; index<31; index++ ) {
                checksum += HdmiReadRegByte( HDMI_PKT2_BYTE_OFFS(index) );
            }
            checksum = (BYTE)(0x100 - checksum);
            HdmiWriteRegByte( HDMI_PKT2_BYTE_OFFS(3), checksum );

            /* configure SPD infoframe packet to transmit in every frame */
            HdmiWriteRegByteOr( HDMI_HOST_PKT_CTRL1_OFFS, PKT2_TX_MODE );
            break;
        }

        case PT_AUDIO_INFOFRAME:
        {
            /* get a handle to the packet payload */
        	AudioInfoFrame_t *Audio = (AudioInfoFrame_t *)(payload);

            /* software to construct the audio infoframe packet in
             * the host PKT3 buffers and to control transmission
             * using the host PKT3 control registers.
             */

            /* construct the header packets (HB0-HB2) */
            HdmiWriteRegByte( HDMI_PKT3_BYTE_OFFS(0), packet );
            HdmiWriteRegByte( HDMI_PKT3_BYTE_OFFS(1), 0x01 );
            HdmiWriteRegByte( HDMI_PKT3_BYTE_OFFS(2), 0x0A );

            PRINTN( "Coding Type: 0x%02x", Audio->CodingType );
            PRINTN( "Channel Count: 0x%02x", Audio->ChannelCount );
            PRINTN( "Sample Freq: 0x%02x", Audio->SamplingFrequency );
            PRINTN( "Sample Size: 0x%02x", Audio->SampleSize );
            PRINTN( "Channel Alloc: 0x%02x", Audio->ChannelAllocation );
            PRINTN( "Down Mix Inhibit: 0x%02x", Audio->DownMixInhibit );
            PRINTN( "Level Shift: 0x%02x", Audio->LevelShiftValue );
            PRINTN( "LFE Playback: 0x%02x", Audio->LFEPlaybackLevel );

            /* construct the sub packets (PB0-PB13) */
            HdmiWriteRegByte( HDMI_PKT3_BYTE_OFFS(3), 0x00 );
            HdmiWriteRegByte( HDMI_PKT3_BYTE_OFFS(4),
                    ((Audio->CodingType & 0xF) << 4) |
                    ((Audio->ChannelCount & 0x7) << 0) );
            HdmiWriteRegByte( HDMI_PKT3_BYTE_OFFS(5),
                    ((Audio->SamplingFrequency & 0x7) << 2) |
                    ((Audio->SampleSize & 0x3) << 0) );
            HdmiWriteRegByte( HDMI_PKT3_BYTE_OFFS(6),(Audio->CodingType & 0xF0) >> 4 );
            HdmiWriteRegByte( HDMI_PKT3_BYTE_OFFS(7), Audio->ChannelAllocation );
            HdmiWriteRegByte( HDMI_PKT3_BYTE_OFFS(8),
                    ((Audio->DownMixInhibit) ? 0x80 : 0x00) |
                    ((Audio->LevelShiftValue & 0xf) << 3) |
					((Audio->LFEPlaybackLevel & 0x3) << 0) );

            /* all other fields (PB6-PB27) are all reserved */
            for( index=9; index<31; index++ ) {
                HdmiWriteRegByte( HDMI_PKT3_BYTE_OFFS(index), 0x00 );
            }

            /* compute and include checksum into the packet */
            for( checksum=0, index=3; index<31; index++ ) {
                checksum += HdmiReadRegByte( HDMI_PKT3_BYTE_OFFS(index) );
            }
            checksum = (BYTE)(0x100 - checksum);
            HdmiWriteRegByte( HDMI_PKT3_BYTE_OFFS(3), checksum );

            /* configure audio infoframe packet to transmit in every frame */
            HdmiWriteRegByteOr( HDMI_HOST_PKT_CTRL1_OFFS, PKT3_TX_MODE );
            break;
        }

        case PT_MPEG_INFOFRAME:
        {
            /* get a handle to the packet payload */
        	MpegInfoFrame_t *MPG = (MpegInfoFrame_t *)(payload);

        	/* software to construct the vendor infoframe packet
             * in the host PKT4 buffers and to control transmission
             * using the host PKT4 control registers.
             */

            /* construct the header packets (HB0-HB2) */
            HdmiWriteRegByte( HDMI_PKT4_BYTE_OFFS(0), packet );
            HdmiWriteRegByte( HDMI_PKT4_BYTE_OFFS(1), 0x01 );
            HdmiWriteRegByte( HDMI_PKT4_BYTE_OFFS(2), 0x0A );

            /* construct the sub packets (PB0-PB5) */
            HdmiWriteRegByte( HDMI_PKT4_BYTE_OFFS(3), 0x00 );
            HdmiWriteRegByte( HDMI_PKT4_BYTE_OFFS(4), (MPG->BitRate & 0x000000FF) >> 0 );
            HdmiWriteRegByte( HDMI_PKT4_BYTE_OFFS(5), (MPG->BitRate & 0x0000FF00) >> 8 );
            HdmiWriteRegByte( HDMI_PKT4_BYTE_OFFS(6), (MPG->BitRate & 0x00FF0000) >> 16 );
            HdmiWriteRegByte( HDMI_PKT4_BYTE_OFFS(7), (MPG->BitRate & 0xFF000000) >> 24 );
            HdmiWriteRegByte( HDMI_PKT4_BYTE_OFFS(8),
            		((MPG->FieldRepeat) ? 0x10 : 0x00) |
            		((MPG->Frame & 0x3) << 0) );

            /* all other fields (PB6-PB27) are all reserved */
            for( index=9; index<31; index++ ) {
                HdmiWriteRegByte( HDMI_PKT4_BYTE_OFFS(index), 0x00 );
            }

            /* compute and include checksum into the packet */
            for( checksum=0, index=3; index<31; index++ ) {
                checksum += HdmiReadRegByte( HDMI_PKT4_BYTE_OFFS(index) );
            }
            checksum = (BYTE)(0x100 - checksum);
            HdmiWriteRegByte( HDMI_PKT4_BYTE_OFFS(3), checksum );

            /* configure mpeg infoframe packet to transmit in every frame */
            HdmiWriteRegByteOr( HDMI_HOST_PKT_CTRL1_OFFS, PKT4_TX_MODE );
            break;
        }
    }

    return (DRIVER_NO_ERROR);
}

static BOOL HdmiConfig(
	ColorDepth_t color_depth,
	ColorIndicator_t color_indicator,
	AudioChannel_t sample_chan,
	SampleFreq_t sample_freq,
	SampleSize_t sample_size )
{
    VideoIdCode_t videoCode;
    TmdsClock_t tmdsClock;

	/* hdmi tx should either be in the connected or authenticated state */
	if( (HdmiController.State != HS_CONNECTED) && (HdmiController.State != HS_AUTHENTICATED) ) {
		WARN( "Driver is not in the appropriate state to be configured." );
		return FALSE;
	}

    /* determine preferred video format parameters */
    videoCode = VIC_NO_CODE;
    while( HdmiController.EDID.CEA.VideoBlocks[videoCode].NativeResolution == FALSE ) {
    	if( (int)(videoCode+1) < (int)VIC_MAX ) {
    		videoCode = (VideoIdCode_t)(videoCode + 1);
    	} else {
    		WARN( "Driver could not locate preferred video format. Will default to 640X480p format." );
    		videoCode = VIC_640X480P_60HZ;
    		break;
    	}
    }

	/* setup audio clock regeneration packet */
    tmdsClock = HdmiVideoParam[videoCode].TmdsClock;
	HdmiController.ACR.CycleTimeStamp = HdmiAcrParam[tmdsClock][COLOR_DEPTH_INDEX(color_depth)][SAMPLE_FREQ_INDEX(sample_freq)].CycleTimeStamp;
    HdmiController.ACR.Numerator = HdmiAcrParam[tmdsClock][COLOR_DEPTH_INDEX(color_depth)][SAMPLE_FREQ_INDEX(sample_freq)].Numerator;

	/* setup general control packet */
    HdmiController.GC.AvMute = FALSE;
    HdmiController.GC.PixelPacking = PP_PHASE1;
	HdmiController.GC.ColorDepth = color_depth;
    HdmiController.GC.DefaultPhase = TRUE;

	/* setup AVI infoframe packet */
	HdmiController.AVI.ColorIndicator = color_indicator;
	HdmiController.AVI.ActiveInfo = TRUE;
	HdmiController.AVI.BarInfo = BI_INVALID;
	HdmiController.AVI.ScanInfo = SI_NO_DATA;
	HdmiController.AVI.Colorimetry = C_NO_DATA;
	HdmiController.AVI.PictureAspectRatio = HdmiVideoParam[videoCode].AspectRatio;
	HdmiController.AVI.FormatAspectRatio = AF_SAME;
	HdmiController.AVI.ITContent = TRUE;
	HdmiController.AVI.ExtendedColorimetry = EC_XVYCC601;
	HdmiController.AVI.QuantizationRange = QR_DEFAULT;
	HdmiController.AVI.PictureScaling = PS_UNKNOWN;
	HdmiController.AVI.VideoIdCode = videoCode;
    HdmiController.AVI.YCCQuantizationRange = YQ_FULL;
    HdmiController.AVI.ITContentType = ICT_GRAPHICS;
	HdmiController.AVI.PixelRepetition = HdmiVideoParam[videoCode].PixelRepetition;
	HdmiController.AVI.TopBarEndLineNum = 0;
	HdmiController.AVI.LeftBarEndPixelNum = 0;
	HdmiController.AVI.BottomBarStartLineNum = HdmiVideoParam[videoCode].VActive + 1;
	HdmiController.AVI.RightBarStartPixelNum = HdmiVideoParam[videoCode].HActive + 1;

    /* setup source product decscriptor infoframe packet */
	HdmiController.SPD.VendorName[0] = 'R';
	HdmiController.SPD.VendorName[1] = 'I';
	HdmiController.SPD.VendorName[2] = 'M';
	HdmiController.SPD.VendorName[3] = 'M';
	HdmiController.SPD.ProductDescription[0] = 'B';
	HdmiController.SPD.ProductDescription[1] = 'l';
	HdmiController.SPD.ProductDescription[2] = 'a';
	HdmiController.SPD.ProductDescription[3] = 'c';
	HdmiController.SPD.ProductDescription[4] = 'k';
	HdmiController.SPD.ProductDescription[5] = 'B';
	HdmiController.SPD.ProductDescription[6] = 'e';
	HdmiController.SPD.ProductDescription[7] = 'r';
	HdmiController.SPD.ProductDescription[8] = 'r';
	HdmiController.SPD.ProductDescription[9] = 'y';
	HdmiController.SPD.SourceDevice = DI_HDD_VIDEO;

	/* setup audio infoframe packet */
	HdmiController.AUDIF.CodingType = AIC_STREAM;
	HdmiController.AUDIF.ChannelCount = sample_chan;
	HdmiController.AUDIF.SamplingFrequency = sample_freq;
	HdmiController.AUDIF.SampleSize = sample_size;
	HdmiController.AUDIF.ChannelAllocation = (sample_chan == AL_8CH) ? 0x1F : 0x00;
    HdmiController.AUDIF.LevelShiftValue = LSV_0DB;
    HdmiController.AUDIF.DownMixInhibit = FALSE;
    HdmiController.AUDIF.LFEPlaybackLevel = PL_UNKNOWN;

    /* setup vendor infoframe packet */
    HdmiController.VENDOR.IEEERegistraionId = 0x0052494D;
    HdmiController.VENDOR.Payload[0] = 'B';
    HdmiController.VENDOR.Payload[1] = 'l';
    HdmiController.VENDOR.Payload[2] = 'a';
    HdmiController.VENDOR.Payload[3] = 'c';
    HdmiController.VENDOR.Payload[4] = 'k';
    HdmiController.VENDOR.Payload[5] = 'B';
    HdmiController.VENDOR.Payload[6] = 'e';
    HdmiController.VENDOR.Payload[7] = 'r';
    HdmiController.VENDOR.Payload[8] = 'r';
    HdmiController.VENDOR.Payload[9] = 'y';
    HdmiController.VENDOR.PayloadLength = 10;

    /* configure underlying Display hardware controller */
	if( DispHwConfigure(
			DM_VIDEO,
			PF_RGB888P,
			((HdmiVideoParam[videoCode].ScanMode == 'I') ? TRUE : FALSE),
			HdmiVideoParam[videoCode].HActive,
			HdmiVideoParam[videoCode].HFrontPorch,
			HdmiVideoParam[videoCode].HSync,
			HdmiVideoParam[videoCode].HBackPorch,
			HdmiVideoParam[videoCode].VActive,
			HdmiVideoParam[videoCode].VFrontPorch,
			HdmiVideoParam[videoCode].VSync,
			HdmiVideoParam[videoCode].VBackPorch) != DRIVER_NO_ERROR) {
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

	/* configure hdmi clock */
	HdmiClockConfig( tmdsClock, HdmiVideoParam[videoCode].PixelRepetition, color_depth );

	/* configure hdmi packets and infoframes */
	HdmiPacketConfig(PT_AUDIO_CLOCK_REGENERATION, &(HdmiController.ACR));
	HdmiPacketConfig(PT_GENERAL_CONTROL, &(HdmiTxTask.GC));
	HdmiPacketConfig(PT_AVI_INFOFRAME, &(HdmiController.AVI));
	HdmiPacketConfig(PT_SPD_INFOFRAME, &(HdmiController.SPD));
	HdmiPacketConfig(PT_AUDIO_INFOFRAME, &(HdmiController.AUDIF));
	HdmiPacketConfig(PT_VENDOR_INFOFRAME, &(HdmiController.VENDOR));

    /* enable the internal logic auto detection of the video format timings */
    HdmiWriteRegByteOr( HDMI_VIDEO_CTRL_OFFS, INT_FRM_SEL );

    /* set appropriate video format */
    HdmiWriteRegByteAnd( HDMI_VIDEO_CTRL_OFFS, ~IN_YC );
    HdmiWriteRegByteOr( HDMI_VIDEO_CTRL_OFFS, (color_indicator == CI_RGB) ? 0 : IN_YC );

    /* configure audio registers based on number of audio channels */
    switch( sample_chan ) {
        case AC_8CH:
            /* enable 8-channel audio packetization */
            WriteRegDwordAnd( HDMI_AUDIO_CFG_REG, ~XFERLEN_MASK );
            WriteRegDwordOr( HDMI_AUDIO_CFG_REG, XFERLEN_8_CHANNEL );
            HdmiWriteRegByteOr( HDMI_AUD_CTRL_OFFS, I2S_CH_EN_PORT_0 );
            HdmiWriteRegByteOr( HDMI_AUD_CTRL_OFFS, I2S_CH_EN_PORT_1 );
            HdmiWriteRegByteOr( HDMI_AUD_CTRL_OFFS, I2S_CH_EN_PORT_2 );
            HdmiWriteRegByteOr( HDMI_AUD_CTRL_OFFS, I2S_CH_EN_PORT_3 );
            break;
        case AC_2CH:
        default:
            /* enable 2-channel audio packetization */
            WriteRegDwordAnd( HDMI_AUDIO_CFG_REG, ~XFERLEN_MASK );
            WriteRegDwordOr( HDMI_AUDIO_CFG_REG, XFERLEN_2_CHANNEL );
            HdmiWriteRegByteOr( HDMI_AUD_CTRL_OFFS, I2S_CH_EN_PORT_0 );
            HdmiWriteRegByteAnd( HDMI_AUD_CTRL_OFFS, ~I2S_CH_EN_PORT_1 );
            HdmiWriteRegByteAnd( HDMI_AUD_CTRL_OFFS, ~I2S_CH_EN_PORT_2 );
            HdmiWriteRegByteAnd( HDMI_AUD_CTRL_OFFS, ~I2S_CH_EN_PORT_3 );
            break;
    }

    /* configure audio registers based on sample size and freq */
    HdmiWriteRegByte( HDMI_CHSTS_3_OFFS,  SampleFreqCSB[SAMPLE_FREQ_INDEX(sample_freq)] );
    HdmiWriteRegByte( HDMI_CHSTS_4_OFFS,  SampleSizeCSB[SAMPLE_SIZE_INDEX(sample_size)] );
    HdmiWriteRegByte( HDMI_I2S_DLEN_OFFS, I2SDataLength[SAMPLE_SIZE_INDEX(sample_size)] );

    /* use validity bit during audio packetization */
    HdmiWriteRegByteOr( HDMI_AUD_CTRL_OFFS, VALIDITY );

    /* set hdmi or dvi mode, pixel repetition and audio layout */
    HdmiWriteRegByteAnd( HDMI_CTRL_OFFS, ~(PIX_RPT_MASK | HDMI_MODE_MASK | LAYOUT_8_CHAN_AUDIO_MASK) );
    HdmiWriteRegByteOr( HDMI_CTRL_OFFS, PIX_RPT(pixel_repetition) );
    HdmiWriteRegByteOr( HDMI_CTRL_OFFS, HDMI_MODE(HdmiController.EDID.CEA.VendorBlock.IEEERegID) );
    HdmiWriteRegByteOr( HDMI_CTRL_OFFS, LAYOUT_8_CHAN_AUDIO(channel_count) );

	return TRUE;
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
 * HdmiInit
 *
 * Performs initialization of the HDMI device driver.
 *
 * @param None.
 *
 * @return TRUE if the operation is successful. Otherwise
 *         returns FALSE.
 */
BOOL HdmiInit(void)
{
	/* set initial state to uninitialized */
	HdmiController.State = HS_UNINITIALIZED;

	/* zero out all data structures */
	memset( (void *)&(HdmiController.ACR), 0x00, sizeof(AcrPacket_t) );
    memset( (void *)&(HdmiController.AUD), 0x00, sizeof(AudioPacket_t) );
    memset( (void *)&(HdmiController.GC), 0x00, sizeof(GcPacket_t) );
    memset( (void *)&(HdmiController.AVI), 0x00, sizeof(AviInfoFrame_t) );
    memset( (void *)&(HdmiController.SPD), 0x00, sizeof(SpdInfoFrame_t) );
    memset( (void *)&(HdmiController.AUDIF), 0x00, sizeof(AudioInfoFrame_t) );
    memset( (void *)&(HdmiController.MPEG), 0x00, sizeof(MpegInfoFrame_t) );
    memset( (void *)&(HdmiController.VENDOR), 0x00, sizeof(VendorInfoFrame_t) );
    memset( (void *)&(HdmiController.EDID), 0x00, sizeof(Edid_t) );

	/* setup and enable hdmi detection interrupt */
    RE_PadClear( HDMI_DET, PAD_PULL_UP );
    RE_PadClear( HDMI_DET, PAD_PULL_DOWN );
    RE_PadInterruptDisable( HDMI_DET );
    RE_PadSet( HDMI_DET, PAD_INPUT_ENABLE );
    RE_PadSetMode( HDMI_DET, HDMI_DET_FN );
    RE_PadInterruptConfigure( HDMI_DET, PAD_INTERRUPT_BOTH_EDGES );
    RE_PadRegisterIntHandler( HDMI_DET, HdmiTxDetectHandler );
    RE_PadInterruptClear( HDMI_DET );
    RE_PadInterruptEnable( HDMI_DET );
    RE_EnableInterrupt( HWIRQ_GPIO );

    /* register message handler */
    PfMsgRegister(PLATFORM_THREAD_ID_DRV, PLATFORM_MSG_HDMI_DRV, HdmiTxMsgHandler);

	/* initialize phy control registers */
	WriteRegDword( HDMI_PHY_CFG_0_REG, 0x00492B6D );
	WriteRegDword( HDMI_PHY_CFG_1_REG, 0x0000FFFF );
	WriteRegDword( HDMI_PHY_CFG_2_REG, 0x08780000 );
	WriteRegDword( HDMI_PHY_CFG_3_REG, 0x00000000 );
	HdmiWriteRegByte( HDMI_PHY_FIFO_SOFT_RST_OFFS, 0x00 );

	/* initialize pll and clock control registers */
	WriteRegDword( HDMI_PLL_CFG_0_REG, 0x2C003D79 );
	WriteRegDword( HDMI_PLL_CFG_1_REG, 0x00000007 );
	WriteRegDword( HDMI_PLL_CFG_2_REG, 0x02000000 );
	WriteRegDword( HDMI_PLL_CFG_3_REG, 0x00000001 );
	WriteRegDword( HDMI_CLOCK_CFG_REG, 0x0001E00A );

	/* initialize audio-related control registers */
	WriteRegDword( HDMI_AUDIO_CFG_REG, 0x00000969 );
	HdmiWriteRegByte( HDMI_AUD_CTRL_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_I2S_CTRL_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_FIFO_CTRL_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_MEMSIZE_L_OFFS, 0xFF );
	HdmiWriteRegByte( HDMI_MEMSIZE_H_OFFS, 0x01 );
	HdmiWriteRegByte( HDMI_HBR_PKT_OFFS, 0x00 );

	/* initialize audio clock regeneration control register */
	HdmiWriteRegByte( HDMI_ACR_CTRL_OFFS, 0x00 );

	/* initialize general control packet control registers */
	HdmiWriteRegByte( HDMI_GCP_CFG0_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_GCP_CFG1_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_AVMUTE_CTRL_OFFS, 0x00 );

	/* initialize video-related control registers */
	HdmiWriteRegByte( HDMI_VIDEO_CTRL_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_DC_FIFO_SFT_RST_OFFS, 0x00 );

	/* initialize hdmi control register */
	HdmiWriteRegByte( HDMI_CTRL_OFFS, 0x00 );

	/* initialize host packet control registers */
	HdmiWriteRegByte( HDMI_HOST_PKT_CTRL0_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_HOST_PKT_CTRL1_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_HOST_PKT_CTRL2_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_HOST_PKT_START_OFFS, 0x00 );

	/* initialize hdcp-related control registers */
	HdmiWriteRegByte( HDMI_HDCP_CTRL_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_HDCP_STATUS_1_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_HDCP_TX_AINFO_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_HDCP_RX_BCAPS_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_HDCP_RX_BSTATUS_0_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_HDCP_RX_BSTATUS_1_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_HDCP_RX_R0_0_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_HDCP_RX_R0_1_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_HDCP_RX_RI_0_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_HDCP_RX_RI_1_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_HDCP_RX_PJ_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_HDCP_FIX_CLR_0_OFFS, 0x80 );
	HdmiWriteRegByte( HDMI_HDCP_FIX_CLR_1_OFFS, 0xFF );
	HdmiWriteRegByte( HDMI_HDCP_FIX_CLR_2_OFFS, 0x80 );
	HdmiWriteRegByte( HDMI_HDCP_INTR_CLR_OFFS, 0x00 );
	HdmiWriteRegByte( HDMI_HDCP_INTR_MASK_OFFS, 0x00 );

	/* transition to disconnected state */
	HdmiController.State = HS_DISCONNECTED;

	return (TRUE);
}

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
BOOL HdmiEnable(void)
{
	ColorDepth_t color_depth = CD_24BIT;
	ColorIndicator_t color_indicator = CI_RGB;

	/* hdmi tx should either be in the connected or authenticated state */
	if( (HdmiController.State != HS_CONNECTED) && (HdmiController.State != HS_AUTHENTICATED) ) {
		WARN( "Driver is not in the appropriate state to be enabled." );
		return FALSE;
	}

	/* derived color indicator from config */
	if( (HdmiCtrlConfig.SrcFormat == PF_YUV422P) |
		(HdmiCtrlConfig.SrcFormat == PF_YUV422PL) |
		(HdmiCtrlConfig.SrcFormat == PF_YUV420PL) ) {
		color_indicator = CI_YCC422;
	}

	/* configure driver and controller */
	if( HdmiConfigure(
			color_depth,
			color_indicator,
			(AudioChannel_t)HdmiCtrlConfig.AudChanCount,
			(SampleFreq_t)HdmiCtrlConfig.SampleFreq,
			(SampleSize_t)HdmiCtrlConfig.SampleSize ) != DRIVER_NO_ERROR ) {
		WARN("Driver could not be configured." );
		return FALSE;
	}

	/* enable hdmi clock */
	WriteRegDwordOr( HDMI_PLL_CFG_1_REG, HDMI_PLL_CFG_EN_PANNEL | HDMI_PLL_CFG_EN_HDMI  | HDMI_PLL_CFG_FO_RDY_ADJ );
	WriteRegDwordOr( HDMI_CLOCK_CFG_REG, HDMI_CLOCK_CFG_EN );

	/* reset the hdmi phy fifo */
	HdmiWriteRegByte( HDMI_PHY_FIFO_PTRS_OFFS, FIFO_RD_PTRS(0x0) | FIFO_WR_PTRS(0x8) );
	HdmiWriteRegByte( HDMI_PHY_FIFO_SOFT_RST_OFFS, 0x01 );
	HdmiWriteRegByte( HDMI_PHY_FIFO_SOFT_RST_OFFS, 0x00 );

	/* reset the deep color fifo */
	HdmiWriteRegByte( HDMI_DC_FIFO_WR_PTR_OFFS, 0x01 );
	HdmiWriteRegByte( HDMI_DC_FIFO_RD_PTR_OFFS, 0x1A );
	HdmiWriteRegByte( HDMI_DC_FIFO_SFT_RST_OFFS, 0x01 );
	HdmiWriteRegByte( HDMI_DC_FIFO_SFT_RST_OFFS, 0x00 );

	/* reset the audio fifo */
	HdmiWriteRegByte( HDMI_FIFO_CTRL_OFFS, 0x01 );
	HdmiWriteRegByte( HDMI_FIFO_CTRL_OFFS, 0x00 );

    return TRUE;
}

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
BOOL HdmiDisable(void)
{
	/* hdmi tx should be at least in the connected state */
	if( HdmiController.State < HS_CONNECTED ) {
		WARN( "Driver is not in the appropriate state to be enabled." );
		return FALSE;
	}

	/* reset the hdmi phy fifo */
	HdmiWriteRegByte( HDMI_PHY_FIFO_PTRS_OFFS, FIFO_RD_PTRS(0x0) | FIFO_WR_PTRS(0x8) );
	HdmiWriteRegByte( HDMI_PHY_FIFO_SOFT_RST_OFFS, 0x01 );
	HdmiWriteRegByte( HDMI_PHY_FIFO_SOFT_RST_OFFS, 0x00 );

	/* reset the deep color fifo */
	HdmiWriteRegByte( HDMI_DC_FIFO_WR_PTR_OFFS, 0x01 );
	HdmiWriteRegByte( HDMI_DC_FIFO_RD_PTR_OFFS, 0x1A );
	HdmiWriteRegByte( HDMI_DC_FIFO_SFT_RST_OFFS, 0x01 );
	HdmiWriteRegByte( HDMI_DC_FIFO_SFT_RST_OFFS, 0x00 );

	/* reset the audio fifo */
	HdmiWriteRegByte( HDMI_FIFO_CTRL_OFFS, 0x01 );
	HdmiWriteRegByte( HDMI_FIFO_CTRL_OFFS, 0x00 );

	/* disable hdmi pll */
	WriteRegDwordAnd( HDMI_CLOCK_CFG_REG, ~HDMI_CLOCK_CFG_EN );
	WriteRegDwordAnd( HDMI_PLL_CFG_1_REG, ~(HDMI_PLL_CFG_EN_PANNEL | HDMI_PLL_CFG_EN_HDMI | HDMI_PLL_CFG_FO_RDY_ADJ) );

	/* power down and reset the pll */
	WriteRegDwordAnd( HDMI_PLL_CFG_3_REG, ~(HDMI_PLL_CFG_RSTB | HDMI_PLL_CFG_ON) );

	/* power down transmitter and disable termination resistor */
	WriteRegDword( HDMI_PHY_CFG_2_REG, HDMI_PHY_CFG_RESET_TX | HDMI_PHY_CFG_PD_TX );

	/* disable hdmi reference and bus clocks */
	WriteRegDwordAnd( PMUA_DISP1_CTRL_REG, ~(HDMI_I2S_CLK_EN | HDMI_REF_CLK_EN) );
//	WriteRegDwordAnd( PMUA_DISP1_CTRL_REG, ~(DISP_AXI_CLK_EN | DISP_AXI_CLK_RST) );

    return TRUE;
}

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
int HdmiControl(int control, va_list args)
{
	switch( control ) {
		case HDMI_CMD_START_TRANSMIT:
			HdmiTransmit(TRUE);
			break;
		case HDMI_CMD_STOP_TRANSMIT:
			HdmiTransmit(FALSE);
			break;
		case HDMI_CMD_SET_AVMUTE:
			HdmiAvMute(TRUE);
			break;
		case HDMI_CMD_CLEAR_AVMUTE:
			HdmiTxAvMute(FALSE);
			break;
		case HDMI_CMD_CONNECT:
			HdmiConnect(TRUE);
			break;
		case HDMI_CMD_DISCONNECT:
			HdmiConnect(FALSE);
			break;
		default:
			WARNN( "Invalid HDMI TX driver command: %d", control );
		    return (DRIVER_UNKNOWN_ERROR);
	}

    return (DRIVER_NO_ERROR);
}

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
BOOL HdmiTransmit(BOOL transmit)
{
	if( transmit ) {

		/* hdmi tx should be in the connected or authenticated state */
		if( (HdmiController.State != HS_CONNECTED) && (HdmiController.State != HS_AUTHENTICATED) ) {
			WARN( "Driver is not in the appropriate state to transmit." );
			return FALSE;
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

		/* auto detect video timing parameters */
		while( (HdmiReadRegByte( HDMI_VIDEO_STS_OFFS ) & INIT_OVER) != INIT_OVER )
			PRINT( "Waiting for HDMI to detect video format ..." );

		/* enable audio clock regeneration packet transmission */
		HdmiWriteRegByteOr( HDMI_ACR_CTRL_OFFS, ACR_EN );

		/* enable audio transmission */
		HdmiWriteRegByteOr( HDMI_AUD_CTRL_OFFS, AUD_EN );

		/* enable general control packet transmission */
		HdmiWriteRegByteOr( HDMI_GCP_CFG0_OFFS, GCP_EN );

		/* enable transmission of some infoframe packets */
		HdmiWriteRegByteOr( HDMI_HOST_PKT_CTRL0_OFFS, VEN_PKT_EN );
	    HdmiWriteRegByteOr( HDMI_HOST_PKT_CTRL0_OFFS, AVI_PKT_EN );
	    HdmiWriteRegByteOr( HDMI_HOST_PKT_CTRL0_OFFS, SPD_PKT_EN );
	    HdmiWriteRegByteOr( HDMI_HOST_PKT_CTRL0_OFFS, AUD_PKT_EN );

	} else {

		/* hdmi tx should be at least in the connected state */
		if( HdmiController.State < HS_CONNECTED ) {
			WARN( "Driver is not in the appropriate state to be enabled." );
			return FALSE;
		}

		/* disable audio clock regeneration packet transmission */
		HdmiWriteRegByteAnd( HDMI_ACR_CTRL_OFFS, ~ACR_EN );

		/* disable audio transmission */
		HdmiWriteRegByteAnd( HDMI_AUD_CTRL_OFFS, ~AUD_EN );

		/* disable general control packet transmission */
		HdmiWriteRegByteAnd( HDMI_GCP_CFG0_OFFS, ~GCP_EN );

		/* disable transmission of some infoframe packets */
	    HdmiWriteRegByteAnd( HDMI_HOST_PKT_CTRL0_OFFS, ~VEN_PKT_EN );
	    HdmiWriteRegByteAnd( HDMI_HOST_PKT_CTRL0_OFFS, ~AVI_PKT_EN );
	    HdmiWriteRegByteAnd( HDMI_HOST_PKT_CTRL0_OFFS, ~SPD_PKT_EN );
	    HdmiWriteRegByteAnd( HDMI_HOST_PKT_CTRL0_OFFS, ~AUD_PKT_EN );
	    HdmiWriteRegByteAnd( HDMI_HOST_PKT_CTRL0_OFFS, ~MPG_PKT_EN );

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

	}

    return TRUE;
}

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
BOOL HdmiAvMute(BOOL avmute)
{
	/* hdmi tx should either be in the connected, authenticated or muted state */
	if( avmute ) {
		if( (HdmiController.State != HS_CONNECTED) && (HdmiController.State != HS_AUTHENTICATED) ) {
			WARN( "Driver is not in the appropriate state to set AV mute." );
			return FALSE;
		}
	} else {
		if( (HdmiController.State != HS_CONNECTED) && (HdmiController.State != HS_MUTED) ) {
			WARN( "Driver is not in the appropriate state to clear AV mute." );
			return FALSE;
		}
	}

	/* configure AV mute flag */
	HdmiController.GC.AvMute = (avmute) ? TRUE : FALSE;
	if( HdmiPacketConfig(PT_GENERAL_CONTROL, &(HdmiController.GC)) ) {
		WARN( "Hardware could not re-enable the general control packet.");
		return FALSE;
	}

	/* set state appropriately */
	if( avmute ) {
		if( HdmiController.State == HS_AUTHENTICATED ) {
			/* since sending encrypted signal, transition state to muted */
			HdmiController.State = HS_MUTED;
		}
	} else {
		if( HdmiController.State == HS_MUTED ) {
			/* since encrypted signal has been muted, transition state to un-muted */
			HdmiController.State = HS_AUTHENTICATED;
		}
	}

	return TRUE;
}

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
void HdmiConnect(BOOL connect)
{
	RE_MESSAGE message;

	/* hdmi tx should be in disconnected state */
	if( (connect) && (HdmiController.State!=HS_DISCONNECTED) ) {
		WARN( "Driver is not in the appropriate state to be connected." );
		return;
	}

	/* construct and post message to read EDID */
	message.Event   = (DWORD) PLATFORM_MSG_HDMI_DRV;
	message.SubMsg  = (DWORD) ((connect) ? HDMI_MSG_CONNECT : HDMI_MSG_DISCONNECT);
	message.DataPtr = (char *) &HdmiController;
	RE_RimPostMessage( PLATFORM_THREAD_ID_DRV, &message );

	return;
}

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
void HdmiDetectHandler(void)
{
	PRINT( "HDMI device connected/disconnected" );

	/* disable and clear hdmi detect interrupt */
    RE_PadInterruptDisable( HDMI_DET );

	if( RE_PadRead(HDMI_DET, PAD_INPUT) ) {
		HdmiConnect( TRUE );
	} else {
		HdmiConnect( FALSE );
	}

	/* enable back hdmi detect interrupt */
    RE_PadInterruptEnable( HDMI_DET );

	return;
}

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
void HdmiMsgHandler(RE_MESSAGE *message)
{
    if( message->SubMsg == (DWORD) HDMI_MSG_CONNECT ) {
    	PRINT( "HDMI device connected." );
		/* transition to connected state */
    	HdmiController.State = HS_CONNECTED;
    	/* read EDID via display data channel */
    	if( !(DdcGetEdid(&(HdmiController))) ) {
    		WARN( "Driver could not read the extended display identification data." );
        }
    } else if( message->SubMsg == (DWORD) HDMI_MSG_DISCONNECT ) {
    	PRINT( "HDMI device disconnected." );
    	/* stop signal transmission */
    	if( HdmiTransmit(FALSE) ) {
    	   	/* disable the device driver */
        	if( HdmiDisable() ) {
        		/* transition to disconnected state */
        		HdmiController.State = HS_DISCONNECTED;
        	} else {
        		WARN( "Driver could not be disabled." );
        	}
       } else {
    		WARN( "Driver could not stop signal transmission." );
       }
    }

	return;
}



