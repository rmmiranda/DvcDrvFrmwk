/*****************************************************************************
 * Filename:    display_cmd.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     June 2009
 *
 * Description: Contains declaration listings of all the interface functions
 *              of the display command module, which is used for sending dis-
 *              play commands (DSI packets and DCS commands) to the display
 *              module.  
 *
 *****************************************************************************/

#ifndef __DISPLAY_CMD_H__
#define __DISPLAY_CMD_H__

#include "rei.h"
#include "driver.h"
#include "display.h"
#include "lcd_defs.h"

/*
 ***************************************************
 ***************************************************
 *
 * Exportable Defines
 *
 ***************************************************
 ***************************************************
 */

/**
 * Address mode
 */
#define DCS_ADDRESS_MODE_BOTTOM_TOP     0x80
#define DCS_ADDRESS_MODE_RIGHT_LEFT     0x40
#define DCS_ADDRESS_MODE_REVERSE        0x20
#define DCS_ADDRESS_MODE_REFRESH_BT     0x10
#define DCS_ADDRESS_MODE_BGR            0x08
#define DCS_ADDRESS_MODE_REFRESH_RL     0x04
#define DCS_ADDRESS_MODE_FLIP_H         0x02
#define DCS_ADDRESS_MODE_FLIP_V         0x01
#define DCS_ADDRESS_MODE_MASK           0xFF

/**
 * Tear mode
 */
#define DCS_TEAR_V_BLANK                0x00
#define DCS_TEAR_VH_BLANK               0x01
#define DCS_TEAR_MASK                   0x01

/**
 * Gamma curve
 */
#define DCS_GAMMA_CURVE_NONE            0x00
#define DCS_GAMMA_CURVE_0               0x01
#define DCS_GAMMA_CURVE_1               0x02
#define DCS_GAMMA_CURVE_2               0x04
#define DCS_GAMMA_CURVE_3               0x08
#define DCS_GAMMA_CURVE_MASK            0x0F

/**
 * Pixel format
 */
#define DCS_PIXEL_DPI_3BPP              0x10
#define DCS_PIXEL_DPI_8BPP              0x20
#define DCS_PIXEL_DPI_12BPP             0x30
#define DCS_PIXEL_DPI_16BPP             0x50
#define DCS_PIXEL_DPI_18BPP             0x60
#define DCS_PIXEL_DPI_24BPP             0x70
#define DCS_PIXEL_DBI_3BPP              0x01
#define DCS_PIXEL_DBI_8BPP              0x02
#define DCS_PIXEL_DBI_12BPP             0x03
#define DCS_PIXEL_DBI_16BPP             0x05
#define DCS_PIXEL_DBI_18BPP             0x06
#define DCS_PIXEL_DBI_24BPP             0x07
#define DCS_PIXEL_DPI_MASK              0x70
#define DCS_PIXEL_DBI_MASK              0x07
#define DCS_PIXEL_MASK                  0x77

/**
 * RGB channel mask
 */
#define DCS_R_CHANNEL_MASK_12BPP        0x0F
#define DCS_R_CHANNEL_MASK_16BPP        0x3E
#define DCS_R_CHANNEL_MASK_18BPP        0x3F
#define DCS_R_CHANNEL_MASK_24BPP        0xFF
#define DCS_G_CHANNEL_MASK_12BPP        0x0F
#define DCS_G_CHANNEL_MASK_16BPP        0x3F
#define DCS_G_CHANNEL_MASK_18BPP        0x3F
#define DCS_G_CHANNEL_MASK_24BPP        0xFF
#define DCS_B_CHANNEL_MASK_12BPP        0x0F
#define DCS_B_CHANNEL_MASK_16BPP        0x3E
#define DCS_B_CHANNEL_MASK_18BPP        0x3F
#define DCS_B_CHANNEL_MASK_24BPP        0xFF

/**
 * Display mode
 */
#define DCS_DISPLAY_MODE_V_SCROLL_ON    0x80
#define DCS_DISPLAY_MODE_INVERSION_ON   0x20
#define DCS_DISPLAY_MODE_GCSEL_2        0x01
#define DCS_DISPLAY_MODE_GCSEL_3        0x02
#define DCS_DISPLAY_MODE_GCSEL_4        0x03
#define DCS_DISPLAY_MODE_GCSEL_MASK     0x03
#define DCS_DISPLAY_MODE_MASK           0xA3

/**
 * Diagnostic result
 */
#define DCS_DIAG_REGISTER_LOAD_DETECT   0x80
#define DCS_DIAG_FUNCTIONALITY_DETECT   0x40
#define DCS_DIAG_CHIP_ATTACH_DETECT     0x20
#define DCS_DIAG_GLASS_BREAK_DETECT     0x10
#define DCS_DIAG_MASK                   0xF0

/**
 * Power mode
 */
#define DCS_POWER_MODE_IDLE_ON          0x40
#define DCS_POWER_MODE_PARTIAL_ON       0x20
#define DCS_POWER_MODE_SLEEP_OFF        0x10
#define DCS_POWER_MODE_NORMAL_ON        0x08
#define DCS_POWER_MODE_DISPLAY_ON       0x04
#define DCS_POWER_MODE_MASK             0x7C

/**
 * Tearing effect line
 */
#define DCS_TE_LINE_OUT_1               0x00
#define DCS_TE_LINE_OUT_2               0x40
#define DCS_TE_LINE_OFF                 0x80
#define DCS_TE_LINE_MASK                0xC0


/*
 ***************************************************
 ***************************************************
 *
 * Exportable interface function definitions
 *
 ***************************************************
 ***************************************************
 */

/**
 * DispCmd
 *
 * Sends packets of specific display commands (DSI and DCS) to the Display
 * driver.
 *
 * @param packet - The type of command packet that will be sent.
 * @param ...    - The variable list of parameters to the command packet. 
 *
 * @return Applicable only if the command is expecting a returned data.
 *
 */
int DispCmd(DsiData packet, ...);

/**
 * DispSetDsiId
 *
 * Sets the default DSI ID.
 *
 * @param dsiId - The desired default DSI ID.
 *
 * @return none.
 *
 */
void DispSetDsiId(BYTE dsiId);

/**
 * DispSetDsiVirtChan
 *
 * Sets the virtual channel associated with the default DSI ID.
 *
 * @param channel - The desired DSI virtual channel.
 *
 * @return none.
 *
 */
void DispSetDsiVirtChan(BYTE channel);

/**
 * DispSmartPanelTransmit
 *
 * Sends sequence of display commands to the DSI command FIFO and tansmits pixel
 * data to the display pipeline for the purpose of transmiting information to the
 * DSI link. This is only used for smart panels or display modules with internal
 * frame buffer.
 *
 * @param data - Pointer to the display pixel data structure.
 *
 * @return Returns a positive value (including zero) if successful. Otherwise,
 *         returns a negative value.
 *
 */
int DispSmartPanelTransmit(Data4Lcd_t *data);

/**
 * DispSmartPanelReceive
 *
 * Sends sequence of display commands to the DSI command FIFO for the purpose
 * of receiving information from the DSI link. This is only used for smart
 * panels or display modules with internal frame buffer.
 *
 * @param data - Pointer to the display pixel data structure.
 *
 * @return Returns a positive value (including zero) if successful. Otherwise,
 *         returns a negative value.
 *
 */
int DispSmartPanelReceive(Data4Lcd_t *data);

/**
 * DispSmartPanelInit
 *
 * Initializes the attached external smart panel and prepares it for DSI/DCS 
 * command/data transmission and reception.
 *
 * @param panel - The panel ID associated to the attached external panel.
 *
 * @return Returns a positive value (including zero) if successful. Otherwise,
 *         returns a negative value.
 *
 */
int DispSmartPanelInit(BYTE panel);

/**
 * DispShowRegs
 *
 * Prints to BugDisp the current register values of the Display module.
 *
 * @param none.
 *
 * @return Returns a positive value (including zero) if successful. Otherwise,
 *         returns a negative value.
 *
 */
int DispShowRegs(void);

/**
 * DispShowStats
 *
 * Prints to BugDisp the latest statistical information of the Display module.
 *
 * @param none.
 *
 * @return Returns a positive value (including zero) if successful. Otherwise,
 *         returns a negative value.
 *
 */
int DispShowStats(void);

/**
 * DispShowConfigs
 *
 * Prints to BugDisp the current settings and configuration of the Display module.
 *
 * @param none.
 *
 * @return Returns a positive value (including zero) if successful. Otherwise,
 *         returns a negative value.
 *
 */
int DispShowConfigs(void);

#endif // __DISPLAY_CMD_H__
