/*****************************************************************************
* Filename:    display.h
*
* Copyright 2009, Research In Motion Ltd
*
* Author:      Victor Chen
*
* Created:     April 24th, 2009
*
* Description: This header file defines the display driver public interface
*
*****************************************************************************/
#ifndef __LCD_DEFS_H__
#define __LCD_DEFS_H__

#include "rei.h"
#include "driver.h"

//Maximum supported DSI controllers
#define DSI_CTL_MAX           0x02
//Maximum LCD panels connected to display subsystem  (2 for DSI0 and one for DSI1 on TavorPV2)
#define LCD_PANEL_MAX         0x03


// Commands  exported to verification code
typedef enum
{

    // Reset(Empty) DSI command fifo
    DISP_CNTRL_DSI_RESET_CMD_FIFO = 0x01,
    // Write to DSI command fifo with one command entry
    DISP_CNTRL_DSI_WRITE_ONE_CMD,
    // Send/execute all DSI command entries in command fifo 
    DISP_CNTRL_DSI_EXECUTE_CMD_FIFO,

    // Write commands sequence to DSI Command fifo and then execute (internally driver will  do: reset fifo +write cmds + execute cmds)  
    DISP_CNTRL_WRITE_DSI,

    // Write pixels or commands (BTA, trigger escape code etc) to LCD panel/DSI link    
    DISP_CNTRL_WRITE_LCD,

    // Read data back from LCD panel/DSI link (could be pixels from LCD panel, trigger code etc)
    DISP_CNTRL_READ_LCD,

    // Dump All LCD/DSI Registers
    DISP_DUMP_REGS,
    // Reset DSI statistics
    DISP_CNTRL_DSI_RESET_STATISTICS,

    // ULPS command
    DISP_CNTRL_DSI_ENTER_ULPS,
    DISP_CNTRL_DSI_EXIT_ULPS,

	DISP_CNTRL_DSI_INIT_LCD_PANEL,
	DISP_CNTRL_DSI_STOP_TRAFFIC,
} DispCntrl;



// Data type used for DISP_CNTRL_WRITE_LCD/DISP_CNTRL_READ_LCD/DISP_CNTRL_WRITE_DSI , could be sending/returned data type
typedef enum
{
    // Returned DSI trigger message
    DISP_DATA_DSI_TRIGGER = 0x01,

    // Returned data in response to a preceding read request from DSI
    DISP_DATA_DSI_RESPONSE = 0x02,

    // presence of accumulated errors in preceding trasmission
    DISP_DATA_DSI_ACK,

    //  BTA command
    DISP_DATA_DSI_BTA,

    // Pixels write to LCD frame buffer
    DISP_DATA_PIXELS,

    // DSI commands sequence
    DISP_DATA_DSI_CMD,

    // DSI test API command
    // Display color bar on LCD panel
    DISP_TEST_COLOR_BAR,
} DispData;

// DSI Error Control: Stop or Continous when there is error happens
typedef enum {
    DSI_ERR_CONT = 1,
    DSI_ERR_STOP = 2
}DsiErrControl;

// Src/Dst Pixel format to or from LCD controller
typedef enum
{
    DISP_PIXELFRT_RGB16 = 0x00,
	DISP_PIXELFRT_RGB18 = 0x01,
    // RGB24 unpacked (loose)
    DISP_PIXELFRT_RGB24_unpacked = 0x02,
    // RGB24 packed
    DISP_PIXELFRT_RGB24 = 0x03,
    DISP_PIXELFRT_RGB32 = 0x04,
    // YUV format
    DISP_PIXELFRT_YCbCr_420 = 0x04,
    DISP_PIXELFRT_YCbCr_422 = 0x05,
    DISP_PIXELFRT_YCbCr_444 = 0x06,
    DISP_PIXELFRT_YCbCr_422_interleaved = 0x07,
} DispPixelFormat;


// DSI Power Mode for transmission and between packet transmission
typedef enum
{
    // HS mode 
    DSI_POWERMODE_HS = 0x00,

    // Low Power Data Transmission 
    DSI_POWERMODE_LPDT = 0x01,

    // High Speed Mode for Data Transmission : Remain In High Speed between packet transmission (will enable multiple packets per transmission feature)
    DSI_POWERMODE_HS_RIHS = 0x02,
    // High Speed Mode for Data Transmission : Return to Low Power Mode after every packet (disable multiple packets per transmission)
    DSI_POWERMODE_HS_NORIHS = 0x03,
    
    // Ultra Low Power State
    DSI_POWERMODE_ENTER_ULPS = 0x04,
    // Exit ULPS state
    DSI_POWERMODE_EXIT_ULPS,
    
} lcdDsiPowerMode;


/* DSI link bit clock (Mbps) */
typedef enum
{
    DSI_LINK_CLK_52M   = 0x00,
    DSI_LINK_CLK_104M  = 0x01,
    DSI_LINK_CLK_156M  = 0x02,
    DSI_LINK_CLK_208M  = 0x03,
    DSI_LINK_CLK_312M  = 0x04,
    DSI_LINK_CLK_DDR2  = 0x05,
    DSI_LINK_CLK_624M  = 0x06,
    DSI_LINK_CLK_UNDEF = 0x07,
}DSI_LINK_CLOCK;


/* Supported DSI Command, DSI data and DCS Command (Most from BBU Code) */
//DSI command
typedef enum 
{
    LCD_CMD_READ_STATUS = 0,
    LCD_CMD_AUTO_STATUS_CHECK,
    LCD_CMD_WAIT_FOR_BUSY,
    LCD_CMD_READ_FRAME_BUFFER,
    LCD_CMD_LOAD_MATCH_REGISTER,
    LCD_CMD_COMMAND_WRITE,
    LCD_CMD_DATA_WRITE,
    LCD_CMD_LINE_DATA_WRITE,
    LCD_WAIT_FOR_VSYNC,
    LCD_CMD_SET_DLY_MULT,
    LCD_CMD_NO_OPERATION,
    LCD_CMD_INTERRUPT_THE_PROCESSOR,
    LCD_CMD_SET_GPIO,    
    LCD_CMD_EXECUTE_LOOP_BUFFER=0x10,
    LCD_CMD_FLUSH_LOOP_BUFFER,
    LCD_CMD_START_LABEL,
    LCD_CMD_GOTO_START,        
    LCD_CMD_DISABLE_OUTPUT=0x1D,
} LcdCmd;

// DSI data followed DSI command (send from Processor to Peripheral)
typedef enum 
{
    DSI_DATA_VSYNC_START = 0x01,
    DSI_DATA_COLOR_MODE_OFF = 0x02,

    DSI_DATA_GENERIC_SHORT_WRITE_NO_PARAMETER = 0x03,
    DSI_DATA_GENERIC_READ_NO_PARAMETER = 0x04,
    DSI_DATA_DCS_SHORT_WRITE_NO_PARAMETER = 0x05,
    DSI_DATA_DCS_READ_NO_PARAMETER=0x06,

    DSI_DATA_END_OF_TRANSMISSION_PACKET = 0x08,
    DSI_DATA_NULL_PACKET = 0x09,

    DSI_DATA_RGB_565_PACKET=0x0E,    
    DSI_DATA_VSYNC_END=0x11,
    DSI_DATA_COLOR_MODE_ON = 0x12,

    DSI_DATA_GENERIC_SHORT_WRITE_1_PARAMETER = 0x13,
    DSI_DATA_GENERIC_READ_1_PARAMETER = 0x14,
    DSI_DATA_DCS_SHORT_WRITE_1_PARAMETER = 0x15,

    DSI_DATA_BLANKING_PACKET = 0x19,
    DSI_DATA_RGB_666_PACKET=0x1E,    

    DSI_DATA_HSYNC_START=0x21,
    DSI_DATA_SHUT_DOWN_PERIPHERAL = 0x22,
    DSI_DATA_GENERIC_SHORT_WRITE_2_PARAMETER = 0x23,

    DSI_DATA_GENERIC_READ_2_PARAMETER = 0x24,
    
    DSI_DATA_GENERIC_LONG_WRITE = 0x29,

    DSI_DATA_RGB_UNPACKED_666_PACKET=0x2E,    

    DSI_DATA_HSYNC_END=0x31,
    DSI_DATA_TURN_ON_PERIPHERAL=0x32,

    DSI_DATA_SET_MAX_RETURN_PACKET_SIZE = 0x37,
    DSI_DATA_DCS_LONG_WRITE=0x39,
    //Packed 24-bit RGB
    DSI_DATA_RGB_888_PACKET=0x3E,    

} DsiData;


// Dsi Data type returned from peripheral (send from peripheral to processor)
typedef enum
{
    // Acknowledge and Error Report
    DSI_RET_DATA_ACK_WITH_ERROR = 0x02,
    // End of Transmission packet (EoTp)    
    DSI_RET_DATA_EOTP = 0x08,
    // Generic Short Read Response, 1 byte returned
    DSI_RET_DATA_GEENRIC_SHORT_READ_1_BYTE = 0x11,
    // Generic Short Read Response, 2 bytes returned
    DSI_RET_DATA_GENERIC_SHORT_READ_2_BYTE = 0x12,
    // Generic Long Read Response
    DSI_RET_DATA_GENERIC_LONG_READ = 0x1A,
    // DCS Long Read Response
    DSI_RET_DATA_DCS_LONG_READ = 0x1C,
    // DCS Short Read Response, 1 byte returned
    DSI_RET_DATA_DCS_SHORT_READ_1_BYTE = 0x21,
    // DCS Short Read Response, 2 bytes returned
    DSI_RET_DATA_DCS_SHORT_READ_2_BYTE = 0x22,
} DsiRetData;


// DSI Error report packet
// SoT Error
#define DSI_PACKET_ERROR_SOT                    (1 << 0)
// SoT Sync Error
#define DSI_PACKET_ERROR_SOT_SYNC               (1 << 1)
// EoT Sync Error
#define DSI_PACKET_ERROR_EOT_SYNC               (1 << 2)
// Escape Mode Entry Command Error
#define DSI_PACKET_ERROR_ESCAPE                 (1 << 3)
// Low-Power Transmit Sync Error
#define DSI_PACKET_ERROR_LP_TRANSMIT_SYNC       (1 << 4)
// HS Receive Timoutout Error
#define DSI_PACKET_ERROR_HS_RECEIVE_TIMEOUT     (1 << 5)
// Flase Control Error
#define DSI_PACKET_ERROR_FALSE_CONTROL          (1 << 6)
// ECC Error single-bit
#define DSI_PACKET_ERROR_ECC_1BIT               (1 << 8)
// ECC Error multi-bit
#define DSI_PACKET_ERROR_ECC_MBIT               (1 << 9)
// Checksum Error
#define DSI_PACKET_ERROR_CHECKSUM               (1 << 10)
// DSI Data Type Not Recognized
#define DSI_PACKET_ERROR_DATA_UNKNOWN           (1 << 11)
// DSI VC ID Invalid 
#define DSI_PACKET_ERROR_VCID_INVALID           (1 << 12)
// Invalid Transmission Length 
#define DSI_PACKET_ERROR_LENGTH_INVALID         (1 << 13)
// DSI Protocol Violation 
#define DSI_PACKET_ERROR_PROTOCOL_VIOLATION     (1 << 15)


// Supported DCS command (Ref: MIPI Allicance Standard for DCS Ver 1.01)
typedef enum 
{
    // DCS standard set
    DCS_CMD_NOP = 0x00,
    DCS_CMD_SOFT_RESET = 0x01,
    DCS_CMD_GET_RED_CHANNEL = 0x06,
    DCS_CMD_GET_GREEN_CHANNEL = 0x07,
    DCS_CMD_GET_BLUE_CHANNEL = 0x08,
    DCS_CMD_GET_POWER_MODE = 0x0A,
    DCS_CMD_GET_ADDRESS_MODE = 0x0B,
    DCS_CMD_GET_PIXEL_FORMAT = 0x0C,    
    DCS_CMD_GET_DISPLAY_MODE = 0x0D,
    DCS_CMD_GET_SIGNAL_MODE = 0x0E,    
    DCS_CMD_GET_DIAGNOSTIC_RESULT = 0x0F,    
    DCS_CMD_ENTER_SLEEP_MODE = 0x10,
    DCS_CMD_EXIT_SLEEP_MODE = 0x11,
    DCS_CMD_ENTER_PARTIAL_MODE = 0x12,
    DCS_CMD_ENTER_NORMAL_MODE = 0x13,
    DCS_CMD_EXIT_INVERT_MODE = 0x20,    
    DCS_CMD_ENTER_INVERT_MODE = 0x21,
    DCS_CMD_SET_GAMMA_CURVE = 0x26,
    DCS_CMD_SET_DISPLAY_OFF = 0x28,
    DCS_CMD_SET_DISPLAY_ON = 0x29,
    DCS_CMD_SET_COLUMN_ADDRESS=0x2A,
    DCS_CMD_SET_PAGE_ADDRESS=0x2B,
    DCS_CMD_WRITE_MEMORY_START=0x2C,
    DCS_CMD_WRITE_LUT = 0x2D,
    DCS_CMD_READ_MEMORY_START = 0x2E,
    DCS_CMD_SET_PARTIAL_AREA = 0x30,
    DCS_CMD_SET_SCROLL_AREA = 0x33,
    DCS_CMD_SET_TEAR_OFF = 0x34,
    DCS_CMD_SET_TEAR_ON = 0x35,    
    DCS_CMD_SET_ADDRESS_MODE=0x36,
    DCS_CMD_SET_SCROLL_START = 0x37,
    DCS_CMD_EXIT_IDLE_MODE = 0x38,
    DCS_CMD_ENTER_IDLE_MODE = 0x39,    
    DCS_CMD_SET_PIXEL_FORMAT=0x3A,
    DCS_CMD_WRITE_MEMORY_CONTINUE=0x3C,
    DCS_CMD_READ_MEMORY_CONTINUE=0x3E,
    DCS_CMD_SET_TEAR_SCANLINE = 0x44,
    DCS_CMD_GET_SCANLINE = 0x45,
    DCS_CMD_READ_DDB_START = 0xA1,
    DCS_CMD_READ_DDB_CONTINUE = 0xA8,

    // Samsung specific (lemur)
    DCS_CMD_READ_DSI_ERRORS = 0x05,
    DCS_CMD_ALL_PIXELS_OFF = 0x22,
    DCS_CMD_ALL_PIXELS_ON  = 0x23,
    DCS_CMD_WRITE_PROFILE_VALUES = 0x50,
    DCS_CMD_WRITE_BRIGHTNESS = 0x51,
    DCS_CMD_READ_BRIGHTNESS = 0x52,
    DCS_CMD_WRITE_CTRL = 0x53,
    DCS_CMD_READ_CTRL = 0x54,
    DCS_CMD_WRITE_CONTENT_ADAPTIVE_BRIGHTNESS = 0x55,
    DCS_CMD_READ_CONTENT_ADAPTIVE_BRIGHTNESS = 0x56,
    DCS_CMD_WRITE_HYSTERESIS = 0x57,
    DCS_CMD_WRITE_GAMMA_SETTING = 0x58,
    DCS_CMD_READ_FS_VALUE_MSB = 0x5A,
    DCS_CMD_READ_FS_VALUE_LSB = 0x5B,
    DCS_CMD_READ_MEDIAN_FILTERE_FS_VALUE_MSB = 0x5C,
    DCS_CMD_READ_MEDIAN_FILTERE_FS_VALUE_LSB = 0x5D,
    DCS_CMD_WRITE_CABC_MINIMUM_BRIGHTNESS = 0x5E,
    DCS_CMD_READ_CABC_MINIMUM_BRIGHTNESS = 0x5F,
    DCS_CMD_WRITE_PROFILE_VALUE_KBBC = 0x60,
    DCS_CMD_WRITE_KEYBOARD_BRIGHTNESS = 0x61,
    DCS_CMD_READ_KEYBOARD_BRIGHTNESS = 0x62,
    DCS_CMD_WRITE_CTRL_KBBC = 0x63,
    DCS_CMD_READ_CTRL_KBBC = 0x64,
    DCS_CMD_READ_ABC_CHECKSUM = 0x69,
    DCS_CMD_READ_BLACK_WHITE = 0x70,
    DCS_CMD_READ_Bkx = 0x71,
    DCS_CMD_READ_Bky = 0x72,
    DCS_CMD_READ_Wx = 0x73,
    DCS_CMD_READ_Wy = 0x74,
    DCS_CMD_READ_REDGREEN_LOW_BITS = 0x75,
    DCS_CMD_READ_Rx = 0x76,
    DCS_CMD_READ_Ry = 0x77,
    DCS_CMD_READ_Gx = 0x78,
    DCS_CMD_READ_Gy = 0x79,
    DCS_CMD_READ_BLUEACOLOR_LOW_BITS = 0x7A,
    DCS_CMD_READ_Bx = 0x7B,
    DCS_CMD_READ_By = 0x7C,
    DCS_CMD_READ_Ax = 0x7D,
    DCS_CMD_READ_Ay = 0x7E,
    DCS_CMD_READ_FIRST_CHECKSUM = 0xAA,
    DCS_CMD_READ_CONTINUE_CHECKSUM = 0xAF,

    // Renasis specific (R69425)
    // Manufacturer Command Access Protect
    DCS_CMD_SET_MCAP = 0xB0,
    // Low Power Mode Control
    DCS_CMD_SET_LPMC = 0xB1,
    // Frame Memory Access and Interface Setting
    DCS_CMD_SET_FMAIS = 0xB3,
    // Read Checksum and ECC Error Counter
    DCS_CMD_READ_RCEEC = 0xB5,
    // Set MDDI control
    DCS_CMD_SET_MDDI_CTL = 0xB7,
	// Set Backlight control 1
    DCS_CMD_SET_BKLT_CTL1= 0xB8,
    // Set Backlight control 2
    DCS_CMD_SET_BKLT_CTL2= 0xB9,
    // Set Backlight control 3
    DCS_CMD_SET_BKLT_CTL3= 0xBA,
    // YUV Format Change 
    DCS_CMD_SET_YUV = 0xBC,
	// Resizing Control 
	DCS_CMD_SET_RESIZING = 0xBD,
	// Device Code Read 
	DCS_CMD_READ_DEV_CODE = 0xBF,
	// Panel Control - Panel Driving Setting
	DCS_CMD_SET_PANEL_DRIVING = 0xC0,
	// Panel Control - Display V-Timing Setting
	DCS_CMD_SET_DISPLAY_VTIMING = 0xC1,
	// Panel Control - Display H-Timing Setting
	DCS_CMD_SET_DISPLAY_HTIMING = 0xC5,
	// Panel Control - Display RGB switch order
	DCS_CMD_SET_DISPLAY_RGB_ORRDER = 0xC6,
	// Gamma Control - Gamma Set A
	DCS_CMD_SET_GAMMA_SETA = 0xC8,
	// Gamma Control - Gamma Set A
	DCS_CMD_SET_GAMMA_SETB = 0xC9,
	// Gamma Control - Gamma Set A
	DCS_CMD_SET_GAMMA_SETC = 0xCA,
    // LTPS interface control
    DCS_CMD_SET_LTPS_INTERFACE = 0xCC,
    // Power control
    DCS_CMD_SET_POWER_SETTINGS1 = 0xD0,
	DCS_CMD_SET_POWER_SETTINGS2 = 0xD1,
	DCS_CMD_SET_POWER_SETTINGS3 = 0xD2,
	DCS_CMD_SET_VCOM_SETTINGS = 0xD5,
    // Sequence control
    DCS_CMD_SET_POWER_ONOFF = 0xD9,
    // NVM control
    DCS_CMD_SET_NVM_ACCESS = 0xE0,
    DCS_CDD_SET_DDB_WRT_CTL = 0xE1,
    DCS_CMD_SET_NVM_DATA_LOAD = 0xFE,


    // Toschiba eDISCO specific (evb)
    DCS_CMD_SET_THSSI_OFF = 0x80,
    DCS_CMD_SET_THSSI_ON = 0x81,
    DCS_CMD_SET_DISPLAY_BUFFER_IO_CONTROL = 0x82,
    DCS_CMD_GET_DISPLAY_BUFFER_IO_CONTROL = 0x83,
    DCS_CMD_SET_TE_VS_OUTPUT_TIMINGS = 0x84,
    DCS_CMD_SET_TE_VS_FALL_TIMINGS_MSB = 0x85,
    DCS_CMD_SET_TE_VS_FALL_TIMINGS_CSB = 0x86,
    DCS_CMD_SET_TE_VS_FALL_TIMINGS_LSB = 0x87,
    DCS_CMD_SET_TE_VS_RISE_TIMINGS_MSB = 0x88,
    DCS_CMD_SET_TE_VS_RISE_TIMINGS_CSB = 0x89,
    DCS_CMD_SET_TE_VS_RISE_TIMINGS_LSB = 0x8A,
    DCS_CMD_SET_OUTPUT_VERTICAL_TIMINGS = 0x8b,
    DCS_CMD_GET_OUTPUT_VERTICAL_FRONT_PORCH = 0x8C,
    DCS_CMD_GET_OUTPUT_VERTICAL_SYNC_PULSE = 0x8D,
    DCS_CMD_GET_OUTPUT_VERTICAL_BACK_PORCH = 0x8E,
    DCS_CMD_GET_OUTPUT_ACTIVE_LINES_MSB = 0x8F,
    DCS_CMD_GET_OUTPUT_ACTIVE_LINES_CSB = 0x90,
    DCS_CMD_GET_OUTPUT_ACTIVE_LINES_LSB = 0x91,
    DCS_CMD_SET_OUTPUT_HORIZONTAL_TIMINGS = 0x92,
    DCS_CMD_GET_OUTPUT_HORIZONTAL_FRONT_PORCH = 0x93,
    DCS_CMD_GET_OUTPUT_HORIZONTAL_SYNC_PULSE = 0x94,
    DCS_CMD_GET_OUTPUT_HORIZONTAL_BACK_PORCH = 0x95,
    DCS_CMD_GET_OUTPUT_ACTIVE_PIXEL_MSB = 0x96,
    DCS_CMD_GET_OUTPUT_ACTIVE_PIXEL_CSB = 0x97,
    DCS_CMD_GET_OUTPUT_ACTIVE_PIXEL_LSB = 0x98,
    DCS_CMD_SET_SPECIAL_COMMAND_SET = 0x9D,
    DCS_CMD_SET_OUTPUT_PIXEL_CLOCK_FREQUENCY = 0x9E,
    DCS_CMD_GET_OUTPUT_PIXEL_CLOCK_FREQUENCY = 0x9F,
    DCS_CMD_READ_ID1 = 0xDA,
    DCS_CMD_READ_ID2 = 0xDB,
    DCS_CMD_READ_ID3 = 0xDC,
    DCS_CMD_WRITE_INDEX_REGISTER = 0xFB,
    DCS_CMD_READ_EDISCO_REGISTER = 0xFC,
    DCS_CMD_WRITE_EDISCO_REGISTER = 0xFD

} DcsCmd;

// Dsi Data type returned from peripheral (send from peripheral to processor)
typedef enum
{
    // Reset Trigger 
    DSI_TRIG_CODE_RESET = 0x62,
    // Unknown3 Trigger 
    DSI_TRIG_CODE_UNKNOWN3 = 0x5D,
    // Unknown4 Trigger 
    DSI_TRIG_CODE_UNKNOWN4 = 0x21,
    // Unknown5 Trigger 
    DSI_TRIG_CODE_UNKNOWN5 = 0xA0,

    // Acknowledge Trigger 
    DSI_TRIG_CODE_ACK = 0x21,
    // TE Trigger 
    DSI_TRIG_CODE_TE = 0x5D,

} DsiTrigCode;


// LCD driver initialization Flag
typedef enum
{
    lcdCtrlUnIntialized = 0x00,
    lcdCtrlIntialized = 0x01,
} lcdControllerFlag;


#define DSI_ENABLED 0x01
#define DSI_DISABLED 0x02


typedef enum
{
   // No connection
    LCD_CONNECT2NULL =  0x00,
    
    // Directly connect to DSI
    LCD_CONNECT2DSI0 =  0x01,
    LCD_CONNECT2DSI1 =  0x02,
    // Connect to DSI with Deserializer
    LCD_CONNECT2DSI0_DES  =  0x03,
    LCD_CONNECT2DSI1_DES =  0x04,

    // Connect to LCD controller Parallel 
    LCD_CONNECT2PAL =  0x04,

} lcdPanelConnection;

typedef enum
{
    // Smart LCD panel with/without DSI
    PANEL_LCD_SMART_PARALLEL = 0x01,
    PANEL_LCD_SMART_DSI = 0x02,

    // Active LCD panel with/without DSI 
    PANEL_LCD_ACTIVE_PARALLEL = 0x04,
    PANEL_LCD_ACTIVE_DSI = 0x08,

    // TV-Out
    PANEL_TVOUT = 0x10,
    PANEL_TVOUT_DSI = 0x20,
} panelType;


// DSI Dcs LCD panel settings, refered by "tavorpv2_lcd_settings.dat"
typedef struct _lcdMode {

    // Panel ID -start from 0 to identify a LCD panel type
    BYTE panelId;

    /* panel type - smart LCD with parallel/smart LCD with DSI/ ativie TFT LCD/TVOUT */
    BYTE panelType;

    /* resolution */
    WORD width;
    WORD height;

    /* color depth - bits per pixel */
    WORD colorDepth;

    /* refresh rate */
    WORD tClock;

    /* D-PHY Timings - if equal 0, means there is no need to config it */
    BYTE tLPX;  
    BYTE tHSPREP; 
    BYTE tHSZERO; 
    BYTE tHSTRAIL; 
    BYTE tHSEXIT;
    BYTE tTAGO;  
    BYTE tCLZERO; 
    BYTE tCLTRAIL;
    BYTE tTAGET;  
    WORD tWAKEUP; 

    // BYTE panelConnect; // connection with LCD controller 
    WORD flags; // flags for future usuages (such as rotation angle)

    //Reserved data - could be used for registers' configurations
    DWORD Reserved[256];
} lcdMode;



#endif
