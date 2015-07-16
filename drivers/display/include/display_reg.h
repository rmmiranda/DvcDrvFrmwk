/*****************************************************************************
 * Filename:    display_reg.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Victor Chen
 *
 * Created:     Aug 19th, 2010
 *
 * Description: Registers Marvell TavorTD (copy from tavortd_lcdctl.h on Nessus Tavortd LCD driver) .
 *
 *****************************************************************************/
#ifndef __DISPLAY_REG_H__
#define __DISPLAY_REG_H__

/* ---------------------------------------------------------------------------
* Register definitions for Display (LCD) controller 
* --------------------------------------------------------------------------- */

//
// Macro Defines
//
#define LCD_IGNORE_FREQCHANGE
#define LCD_DISPLAY_CLK1_NOM_MHZ        312 // MHz
#define LCD_DISPLAY_CLK2_NOM_MHZ        312 // MHz
    
//
// Registers
//
    
#define LCD_CTL_BASE        0xD420B000
#define PMU_BASE            0xD4282800
    
// Global control
#define LCD_TOP_CTRL_REG                ( LCD_CTL_BASE + 0x01DC ) 
#define CFG_MERGE_SCLKTCLK              ( 1 << 20 )     // Clock domain selection
#define CFG_T2S_S2TB                    ( 1 << 21 )     // 1 = tclk mapped to Panel, 0 = sclk mapped to TV (unusable)
#define CFG_ALL2PNORTV_PNANDTV          ( 3 << 22 )     // Panel path DMA goes to Panel, TV path DMA goes to TV
#define CFG_ALL2PNORTV_PN               ( 1 << 22 )     // Panel path DMA goes to Panel, TV path DMA goes to TV
    
// TV Video Path DMA Registers - We currently don't use the TV path at all it isn't clear whether these registers
// will even exist in all revisions of the hardware and writing to them if they don't exist may cause problems
#define LCD_TVD_START_ADDR_Y_REG(n)     ( LCD_CTL_BASE + 0x0000 + (((n)&0x1)*0x0010) )  // Frame 0-1 Y,U,V,Command start address
#define LCD_TVD_START_ADDR_U_REG(n)     ( LCD_CTL_BASE + 0x0004 + (((n)&0x1)*0x0010) )
#define LCD_TVD_START_ADDR_V_REG(n)     ( LCD_CTL_BASE + 0x0008 + (((n)&0x1)*0x0010) )
#define LCD_TVD_START_ADDR_C_REG(n)     ( LCD_CTL_BASE + 0x000C + (((n)&0x1)*0x0010) )
#define LCD_TVD_PITCH_YC_REG            ( LCD_CTL_BASE + 0x0020 )                       // Y,C line length
#define LCD_TVD_PITCH_UV_REG            ( LCD_CTL_BASE + 0x0024 )                       // U,V line length
#define LCD_TVD_OVSA_HPXL_VLN_REG       ( LCD_CTL_BASE + 0x0028 )                       // Starting position on screen
#define LCD_TVD_HPXL_VLN_REG            ( LCD_CTL_BASE + 0x002C )                       // Source size
#define LCD_TVDZM_HPXL_VLN_REG          ( LCD_CTL_BASE + 0x0030 )                       // Destination size (after zooming)
    
// TV Graphics Path DMA Registers - We currently don't use the TV path at all it isn't clear whether these registers
// will even exist in all revisions of the hardware and writing to them if they don't exist may cause problems
#define LCD_TVG_START_ADDR_REG(n)       ( LCD_CTL_BASE + 0x0030 + (((n)&0x1)*0x0004) )  // Frame 0-1 Y,U,V,Command start address
#define LCD_TVG_PITCH_REG               ( LCD_CTL_BASE + 0x003C )                       // Y,C line length
#define LCD_TVG_OVSA_HPXL_VLN_REG       ( LCD_CTL_BASE + 0x0040 )                       // Starting position on screen
#define LCD_TVG_HPXL_VLN_REG            ( LCD_CTL_BASE + 0x0044 )                       // Source size
#define LCD_TVGZM_HPXL_VLN_REG          ( LCD_CTL_BASE + 0x0048 )                       // Destination size (after zooming)
    
// TV Hardware Cursor Registers (unused) - same caveat as TV Video Path DMA and TV Graphics Path DMA registers
#define LCD_TVC_OVSA_HPXL_VLN_REG       ( LCD_CTL_BASE + 0x004C )                       // Starting position on screen
#define LCD_TVC_HPXL_VLN_REG            ( LCD_CTL_BASE + 0x0050 )                       // Cursor size
    
// TV Control Registers - same caveat as other TV registers
#define LCD_TV_V_H_TOTAL_REG            ( LCD_CTL_BASE + 0x0054 )                       // Total screen size (active panel only)
#define LCD_TV_V_H_ACTIVE_REG           ( LCD_CTL_BASE + 0x0058 )                       // Active screen size
#define LCD_TV_H_PORCH_REG              ( LCD_CTL_BASE + 0x005C )
#define LCD_TV_V_PORCH_REG              ( LCD_CTL_BASE + 0x0060 )
    
// TV Path DMA Control Registers - same caveat as other TV registers
#define LCD_TV_CTRL_REG(n)              ( LCD_CTL_BASE + 0x0080 + (((n)&0x1)*0x0004) )
    
// Panel Video Frame Registers (layer 1 - Overlay plane)
#define LCD_DMA_START_ADDR_Y_REG(n)     ( LCD_CTL_BASE + 0x00C0 + (((n)&0x1)*0x0010) )  // Frame 0-1 Y,U,V,Command start address
#define LCD_DMA_START_ADDR_U_REG(n)     ( LCD_CTL_BASE + 0x00C4 + (((n)&0x1)*0x0010) )
#define LCD_DMA_START_ADDR_V_REG(n)     ( LCD_CTL_BASE + 0x00C8 + (((n)&0x1)*0x0010) )
#define LCD_DMA_START_ADDR_C_REG(n)     ( LCD_CTL_BASE + 0x00CC + (((n)&0x1)*0x0010) )
#define LCD_DMA_PITCH_YC_REG            ( LCD_CTL_BASE + 0x00E0 )                       // Y,C line length
#define LCD_DMA_PITCH_UV_REG            ( LCD_CTL_BASE + 0x00E4 )                       // U,V line length
#define LCD_DMA_OVSA_HPXL_VLN_REG       ( LCD_CTL_BASE + 0x00E8 )                       // Starting position on screen
#define LCD_DMA_HPXL_VLN_REG            ( LCD_CTL_BASE + 0x00EC )                       // Source size
#define LCD_DMAZM_HPXL_VLN_REG          ( LCD_CTL_BASE + 0x00F0 )                       // Destination size (after zooming)
#define LCD_DMAVLD_YC                   ( LCD_CTL_BASE + 0x014C )                       // Y,C line length (in bytes, from system memory) and update flags  (TODO diff vs pitch?)
#define LCD_DMAVLD_UV                   ( LCD_CTL_BASE + 0x0150 )                       // U,V line length (in bytes, from system memory) (TODO diff vs pitch?)
    
// Panel Graphics Frame Registers (layer 0 - Base plane)
#define LCD_GRA_START_ADDR_REG(n)       ( LCD_CTL_BASE + 0x00F4 + (((n)&0x1)*0x0004) )  // Frame 0-1 RGB frame start address
#define LCD_GRA_PITCH_REG               ( LCD_CTL_BASE + 0x00FC )                       // Line length
#define LCD_GRA_OVSA_HPXL_VLN_REG       ( LCD_CTL_BASE + 0x0100 )                       // Starting position on screen
#define LCD_GRA_HPXL_VLN_REG            ( LCD_CTL_BASE + 0x0104 )                       // Source size
#define LCD_GRAZM_HPXL_VLN_REG          ( LCD_CTL_BASE + 0x0108 )                       // Destination size (after zooming)
#define LCD_TVGGRAVLD_HLEN_REG          ( LCD_CTL_BASE + 0x0154 )                       // TV and Panel graphic line length (in bytes, from system memory) (TODO diff vs pitch?)
#define LCD_GRA_CUTHPXL_REG             ( LCD_CTL_BASE + 0x01CC )                       // Graphic Partial Display Horizonal Control

#define LCD_GRA_CUTVLN_REG              ( LCD_CTL_BASE + 0x01D0 )                       // Graphic Partial Display Vertical Control
#define GRA_HPXL_VLN( y, x )            ( ( ( y & 0xFFF ) << 16 ) | ( x & 0xFFF ) )
#define PN_V_H_ACTIVE( y, x )           GRA_HPXL_VLN( y, x )

// Hardware Cursor Registers (unused)
#define LCD_HWC_OVSA_HPXL_VLN_REG       ( LCD_CTL_BASE + 0x010C )                       // Starting position on screen
#define LCD_HWC_HPXL_VLN_REG            ( LCD_CTL_BASE + 0x0110 )                       // Cursor size
#define LCD_HWC_RDDAT_REG               ( LCD_CTL_BASE + 0x0158 )                       // Cursor SRAM read data register
    
// Panel Control Registers
#define LCD_PN_V_H_TOTAL_REG            ( LCD_CTL_BASE + 0x0114 )                       // Total screen size (active panel only)
#define LCD_PN_V_H_ACTIVE_REG           ( LCD_CTL_BASE + 0x0118 )                       // Active screen size
#define LCD_PN_H_PORCH_REG              ( LCD_CTL_BASE + 0x011C )                       // Horizontal Porch (active panel only)
#define LCD_PN_V_PORCH_REG              ( LCD_CTL_BASE + 0x0120 )                       // Vertical Porch (active panel only)
#define LCD_PN_BLANKCOLOR_REG           ( LCD_CTL_BASE + 0x0124 )                       // Background colour
#define LCD_PN_ALPHA_COLOR_REG(n)       ( LCD_CTL_BASE + 0x0124 + (((n)&0x1)*0x0004) )  // Hardware cursor color 1 and 2
#define LCD_PN_COLORKEY_Y_REG           ( LCD_CTL_BASE + 0x0130 )                       // Y colour key control
#define LCD_PN_COLORKEY_U_REG           ( LCD_CTL_BASE + 0x0134 )                       // U colour key control
#define LCD_PN_COLORKEY_V_REG           ( LCD_CTL_BASE + 0x0138 )                       // V colour key control
#define LCD_PN_SEPXLCNT_REG             ( LCD_CTL_BASE + 0x013C )                       // VSYNC pulse pixel edge control
#define LCD_PN_GAMMA_RDDAT_REG          ( LCD_CTL_BASE + 0x015C )                       // Gamma table SRAM read data
#define LCD_PN_PALETTE_RDDAT_REG        ( LCD_CTL_BASE + 0x0160 )                       // Palette table SRAM read data
#define LCD_PN_CTRL_REG(n)              ( LCD_CTL_BASE + 0x0190 + (((n)&0x1)*0x0004) )  // Panel DMA Control 0 and 1
#define CFG_PN_NOBLENDING               ( 1u << 31 ) // Panel Path Alpha Blending Enable 
#define CFG_PN_GAMMA_ENA                ( 1u << 30 ) // Panel Path Gamma Correction Enable 
#define CFG_PN_CSBH_ENA                 ( 1u << 29 ) // Panel Path Video Contrast/Saturation/Brightness/Hue Adjust Enable
#define CFG_PN_PALETTE_ENA              ( 1u << 28 ) // Panel Path Palette Colour Enable
#define CFG_ARBFAST_ENA                 ( 1u << 27 ) // AXI Bus Arbiter Fast Mode Enable
#define CFG_HWC_ENA                     ( 1u << 26 ) // Panel Path HW Cursor Enable
#define CFG_DMA_FORMAT_RGB565           ( 0x0u << 20 ) // Panel Path Video Memory Colour Format = RGB565
#define CFG_DMA_FORMAT_RGB1555          ( 0x1u << 20 ) // Panel Path Video Memory Colour Format = RGB1555
#define CFG_DMA_FORMAT_RGB888_PACKED    ( 0x2u << 20 ) // Panel Path Video Memory Colour Format = RGB888 PACKED
#define CFG_DMA_FORMAT_RGB888_UNPACKED  ( 0x3u << 20 ) // Panel Path Video Memory Colour Format = RGB888 UNPACKED
#define CFG_DMA_FORMAT_RGBA8888         ( 0x4u << 20 ) // Panel Path Video Memory Colour Format = RGBA8888
#define CFG_DMA_FORMAT_YUV422_PACKED    ( 0x5u << 20 ) // Panel Path Video Memory Colour Format = YUV422 PACKED
#define CFG_DMA_FORMAT_YUV422_PLANAR    ( 0x6u << 20 ) // Panel Path Video Memory Colour Format = YUV422 PLANAR
#define CFG_DMA_FORMAT_YUV420_PLANAR    ( 0x7u << 20 ) // Panel Path Video Memory Colour Format = YUV420 PLANAR
#define CFG_DMA_FORMAT_PANEL_COMMAND    ( 0x8u << 20 ) // Panel Path Video Memory Colour Format = Smart Panel Command
#define CFG_DMA_FORMAT_PALETTE_COLOR    ( 0x9u << 20 ) // Panel Path Video Memory Colour Format = Palette Colour, 4 bits per
#define CFG_DMA_FORMAT_MASK             ( 0xFu << 20 ) // Panel Path Video Memory Colour Format field mask
#define CFG_GRA_FORMAT_RGB565           ( 0x0u << 16 ) // Panel Path Grapic Memory Colour Format = RGB565
#define CFG_GRA_FORMAT_RGB1555          ( 0x1u << 16 ) // Panel Path Graphic Memory Colour Format = RGB1555
#define CFG_GRA_FORMAT_RGB888_PACKED    ( 0x2u << 16 ) // Panel Path Graphic Memory Colour Format = RGB888 PACKED
#define CFG_GRA_FORMAT_RGB888_UNPACKED  ( 0x3u << 16 ) // Panel Path Video Memory Colour Format = RGB888 UNPACKED
#define CFG_GRA_FORMAT_RGBA8888         ( 0x4u << 16 ) // Panel Path Video Memory Colour Format = RGBA8888
#define CFG_GRA_FORMAT_YUV422_PACKED    ( 0x5u << 16 ) // Panel Path Video Memory Colour Format = YUV422 PACKED
#define CFG_GRA_FORMAT_MASK             ( 0xFu << 16 ) // Panel Path Video Memory Colour Format field mask
#define CFG_GRA_FORMAT_PALETTE_COLOR_4BPP  ( 0x9u << 16 ) // Panel Path Video Memory Colour Format = Palette Colour, 4 bits per
#define CFG_GRA_FORMAT_PALETTE_COLOR_8BPP  ( 0xAu << 16 ) // Panel Path Video Memory Colour Format = Palette Colour, 8 bits per
#define CFG_GRA_FTOGGLE                 ( 1u << 15 ) // Panel Path Graphic Frame Toggle Enable
#define CFG_GRA_HSMOOTH                 ( 1u << 14 ) // Panel Path Graphic Horizontal Smoothing Enable 
#define CFG_GRA_TSTMODE                 ( 1u << 13 ) // Panel Path Graphic Test Mode Enable
#define CFG_GRA_SWAPRB                  ( 1u << 12 ) // Panel Path Graphic Swap R and B Enable
#define CFG_GRA_SWAPUV                  ( 1u << 11 ) // Panel Path Graphic Swap U and V Enable
#define CFG_GRA_SWAPYU                  ( 1u << 10 ) // Panel Path Graphic Swap Y and U Enable
#define CFP_YUV2RGB_GRA                 ( 1u << 9 )  // Panel Path Graphic YUV to RGB Color Space Conversion Enable
#define CFG_GRA_ENA                     ( 1u << 8 )  // Panel Path Graphic Transfer Enable
#define CFG_DMA_FTOGGLE                 ( 1u << 7 ) // Panel Path Video Frame Toggle Enable
#define CFG_DMA_HSMOOTH                 ( 1u << 6 ) // Panel Path Video Horizontal Smoothing Enable 
#define CFG_DMA_TSTMODE                 ( 1u << 5 ) // Panel Path Video Test Mode Enable
#define CFG_DMA_SWAPRB                  ( 1u << 4 ) // Panel Path Video Swap R and B Enable
#define CFG_DMA_SWAPUV                  ( 1u << 3 ) // Panel Path Video Swap U and V Enable
#define CFG_DMA_SWAPYU                  ( 1u << 2 ) // Panel Path Video Swap Y and U Enable
#define CFP_YUV2RGB_DMA                 ( 1u << 1 )  // Panel Path Video YUV to RGB Color Space Conversion Enable
#define CFG_DMA_ENA                     ( 1u << 0 )  // Panel Path Video and Command Transfer Enable
#define CFG_FRAME_TRIG                  ( 1u << 31 ) // Panel Path DMA Transfer Tirgger
#define CFG_PN_VSYNC_MODE_DMA           ( 0u << 28 ) // Panel Path Vsync Input Triggers DMA transfer 
#define CFG_PN_VSYNC_MODE_DMA_INT       ( 1u << 28 ) // Panel Path Vsync Input Triggers DMA transfer and generate asynchronous interrupt to processor 
#define CFG_PN_VSYNC_MODE_DMA_DP        ( 0x2u << 28 ) // Panel Path Dumb Panel Vsync Input Triggers DMA transfer 
#define CFG_PN_VSYNC_MODE_DMA_FRAME_TRIG    ( 0x7u << 28 ) // Panel Path Writing 1 to CFG_FRAME_TRIG triggers DMA transfer 
#define CFG_PN_VSYNC_MODE_DMA_MASK      ( 0x7u << 28 ) // Panel Path Vsync Input Trigger Modes field mask 
#define CFG_PN_VSYNC_INV                ( 1u << 27  ) // Rising/Falling edge triggers DMA Transfer ( 1 = Falling edge )
#define COLOR_PN_KEY_MODE_Y             ( 0x1u << 24 ) // Panel Path Colour Key Mode - Video Y (or Graphic R) colour key enabled
#define COLOR_PN_KEY_MODE_U             ( 0x2u << 24 ) // Panel Path Colour Key Mode - Video U colour key enabled
#define COLOR_PN_KEY_MODE_RGB           ( 0x3u << 24 ) // Panel Path Colour Key Mode - Graphic RGB colour key enabled
#define COLOR_PN_KEY_MODE_V             ( 0x4u << 24 ) // Panel Path Colour Key Mode - Video V colour key enabled
#define COLOR_PN_KEY_MODE_YUV           ( 0x5u << 24 ) // Panel Path Colour Key Mode - Video YUV colour key enabled
#define COLOR_PN_KEY_MODE_G             ( 0x6u << 24 ) // Panel Path Colour Key Mode - Graphic G colour key enabled
#define COLOR_PN_KEY_MODE_B             ( 0x7u << 24 ) // Panel Path Colour Key Mode - Graphic B colour key enabled
#define COLOR_PN_KEY_MODE_MASK          ( 0x7u << 24 ) // Panel Path Colour Key Mode field mask
#define CFG_PN_CARRY                    ( 1u << 23 ) // Panel Path Configure Low Bits
#define CFG_LNBUF_ENA                   ( 1u << 22 ) // Panel Path Line Buffer Enable
#define CFG_GATED_ENA                   ( 1u << 21 ) // Gated Clock Enable
#define CFG_PWRDN_ENA                   ( 1u << 20 ) // Power Save Enable
#define CFG_DSCALE_HALF                 ( 1u << 18 ) // Scaling down by 1/2
#define CFG_DSCALE_QUARTER              ( 2u << 18 ) // Scaling down by 1/4
#define CFG_DSCALE_MASK                 ( 0x3u << 18 ) // Scaling field mask
#define CFG_PN_ALPHA_MODE_MASK          ( 0xFFu << 8 ) // Range for video alpha blending - 0xFF = 100% video - 0x0 = 0% video
#define CFG_PXLCMD                      ( 0x81u ) // Pixel Command - value of 0x81 defined by Marvell
    
#define LCD_PN_CONTRAST_REG             ( LCD_CTL_BASE + 0x01AC )                       // Panel Video contrast and brightness
#define LCD_PN_SATURATION_REG           ( LCD_CTL_BASE + 0x01B0 )                       // Panel Video saturation
#define LCD_PN_CBSH_HUE_REG             ( LCD_CTL_BASE + 0x01B4 )                       // Panel Video hue adjustment
#define LCD_PN_IOPAD_CONTROL_REG        ( LCD_CTL_BASE + 0x01BC )                       // Panel path I/O pad control
    
// LCD internal SRAM
#define LCD_SRAM_CTRL_REG               ( LCD_CTL_BASE + 0x0198 )                       // SRAM control
#define LCD_SRAM_WRDAT_REG              ( LCD_CTL_BASE + 0x019C )                       // SRAM write data
#define LCD_SRAM_PARA_REG(n)            ( LCD_CTL_BASE + 0x01A0 + (((n)&0x1)*0x0004) )  // SRAM read/write timing and power control (0 and 1)
#define LCD_SQULN1_CTRL                 ( LCD_CTL_BASE + 0x01E0 )                       // SRAM line buffer control
    
// Clock control
#define LCD_SCLK_DIV_REG                ( LCD_CTL_BASE + 0x01A8 )                       // Clock divider control (smart panel - used for sp writes)
#define LCD_TCLK_DIV_REG                ( LCD_CTL_BASE + 0x009C )                       // Clock divider control (tv clock - used for sp reads, since tv out removed)
#define LCD_xCLK_DIV_xCLK_SOURCE_SELECT_LCD_AXI_BUS     ( 0u << 30 )                 // S/T clk source
#define LCD_xCLK_DIV_xCLK_SOURCE_SELECT_LCD_DISP_CLK1   ( 1u << 30 )
#define LCD_xCLK_DIV_xCLK_SOURCE_SELECT_LCD_DISP_CLK2   ( 2u << 30 )
#define LCD_xCLK_DIV_xCLK_SOURCE_SELECT_MASK            ( 3u << 30 )   
#define LCD_xCLK_DISABLE                                ( 1u << 28 )                 // Pixel clk disable (if set)
#define LCD_xCLK_INT_DIV(n)                             ( ((n) & 0xFF) << 0 )       // Clk integer divider
    
// Panel Status and Debug Registers
#define LCD_SLV_DBG_REG                 ( LCD_CTL_BASE + 0x0164 )                       // Slave path status/debug register
#define LCD_FRAME_CNT_REG               ( LCD_CTL_BASE + 0x017C )                       // Frame count (read only)
    
// Interface Control Registers
#define LCD_SMPN_CTRL_REG               ( LCD_CTL_BASE + 0x0188 )                       // Smart Panel control
#define LCD_SMPN_CTRL_REG_SET_TXHIGH( n )   ( ((n) & 0xFu) << 16 )
#define LCD_SMPN_CTRL_REG_SET_RXHIGH( n )   ( ((n) & 0xFu) << 24 )
#define LCD_SMPN_CTRL_REG_SET_TXLOW( n )    ( ((n) & 0xFu) << 20 )
#define LCD_SMPN_CTRL_REG_SET_RXLOW( n )    ( ((n) & 0xFu) << 28 )
#define LCD_SMTN_CTLR_RXTX_MASK             ( 0xFFFFu << 16)
#define CFG_SMPN_ENA                        ( 1u << 0 )
#define CFG_SLV_ENA                         ( 1u << 4 )
#define CFG_SLV_ONLY                        ( 1u << 12 )
#define CFG_SMPN_MODE_MASK                  ( 0xFu << 8 )
#define CFG_SMPN_MODE_RGB565                ( 0x2u << 8 )
#define CFG_SMPN_MODE_RGB666				( 0x1u << 8 )
#define CFG_SMPN_MODE_RGB888				( 0x0u << 8 )
#define CFG_SWAP_BYTES                      ( 1u << 6 )
    
#define LCD_DUMB_CTRL_REG               ( LCD_CTL_BASE + 0x01B8 )                       // Dumb active panel control
#define LCD_READ_IOPAD_REG              ( LCD_CTL_BASE + 0x0148 )                       // IO pad state
#define LCD_SPI_RXDATA_REG              ( LCD_CTL_BASE + 0x0140 )                       // SPI read data (unused)
#define LCD_SPI_CTRL_REG                ( LCD_CTL_BASE + 0x0180 )                       // SPI control (unused)
#define LCD_SPI_TXDATA_REG              ( LCD_CTL_BASE + 0x0184 )                       // SPI data transfer
    
#define LCD_ISA_RXDATA_REG              ( LCD_CTL_BASE + 0x0144 )                       // Parallel read data
#define LCD_SLV_PORT_REG                ( LCD_CTL_BASE + 0x018C )                       // Parallel AHB Slave Path Data Port (Output FIFO)
#define ISA_RXDATA_DATA_MASK            0x00FFu
#define ISA_RXDATA_GET_DATA( x )        ( (x) & ISA_RXDATA_DATA_MASK )
    
// Interrupt Registers
#define LCD_SPU_IRQ_ENA_REG             ( LCD_CTL_BASE + 0x01C0 )                       // LCD interrupt control
#define LCD_SPU_IRQ_ISR_REG             ( LCD_CTL_BASE + 0x01C4 )                       // LCD interrupt status
#define LCD_SPU_IRQ_RSR_REG             ( LCD_CTL_BASE + 0x01C8 )                       // LCD interrupt reset status
#define DMA_FRAME_IRQ0_ENA              ( 1u << 31 )                                     // Panel Path DMA Frame 0 done IRQ
#define DMA_FRAME_IRQ1_ENA              ( 1u << 30 )                                     // Panel Path DMA Frame 1 done IRQ
#define DMA_FRAME_FF_UNDERFLOW_ENA      ( 1u << 29 )                                     // Panel Path DMA Frame FIFO underflow IRQ
#define DMA_M2A_IRQ_ENA                 ( 1u << 28 )                                     // AXI Bus Error IRQ
#define GRA_FRAME_IRQ0_ENA              ( 1u << 27 )                                     // Panel Path Graphic Frame 0 IRQ
#define GRA_FRAME_IRQ1_ENA              ( 1u << 26 )                                     // Panel Path Graphic Frame 1 IRQ
#define GRA_FF_UNDERFLOW_ENA            ( 1u << 25 )                                     // Panel Path FIFO Underflow IRQ
#define VSYNC_IRQ_ENA                   ( 1u << 23 )                                     // Panel Path VSYNC IRQ
#define DUMB_FRAME_DONE_IRQ_ENA         ( 1u << 22 )                                     // Panel Path Dumb Panel Frame Done IRQ
#define TWC_FRAME_DONE_IRQ_ENA          ( 1u << 21 )                                     // Panel Path Smart Panel Frame Done IRQ
#define HWC_FRAME_DONE_IRQ_ENA          ( 1u << 20 )                                     // Panel Path Hardware Cursor Frame Done IRQ
#define SLV_IRQ_ENA 			        ( 1u << 19 )									 /* AHB Slave Path all command empty IRQ */
#define SPI_IRQ_ENA                     ( 1u << 18 )                                     // SPI bus transfer complete IRQ
#define PWRON_IRQ_ENA                   ( 1u << 17 )                                     // Power Donw Request IRQ Enable
#define ERR_IRQ_ENA                     ( 1u << 16 )                                     // AXI Bus Latency Too Long IRQ Enable
#define TVD_FRAME_IRQ0_ENA 	        	( 1u << 15 )									 
#define TVD_FRAME_IRQ1_ENA	         	( 1u << 14 )									 
#define TVD_FF_UNDERFLOW_ENA         	( 1u << 13 )									 
#define TV_VSYNC_IRQ_ENA 	         	( 1u << 12 )									 
#define TVG_FRAME_IRQ0                  ( 1u << 11 )									
#define TVG_FRAME_IRQ1	                ( 1u << 10 )									
#define TVG_FF_UNDERFLOW            	( 1u <<  9 )				
#define TVIF_FRAMEDONE 		        	( 1u <<  8 )	
#define PN2_DMA_FRAME_IRQ0_ENA	        ( 1u <<  7 )						
#define PN2_DMA_FRAME_IRQ1_ENA       	( 1u <<  6 )						
#define ALL_IRQ_ENA                     (  SLV_IRQ_ENA  | DMA_FRAME_IRQ0_ENA | DMA_FRAME_IRQ1_ENA | DMA_FRAME_FF_UNDERFLOW_ENA | \
                                           DMA_M2A_IRQ_ENA | GRA_FRAME_IRQ0_ENA | GRA_FRAME_IRQ1_ENA | GRA_FF_UNDERFLOW_ENA | \
                                           VSYNC_IRQ_ENA |    DUMB_FRAME_DONE_IRQ_ENA | TWC_FRAME_DONE_IRQ_ENA | HWC_FRAME_DONE_IRQ_ENA | \
                                           SPI_IRQ_ENA |  PWRON_IRQ_ENA | ERR_IRQ_ENA )
    
// Alpha Control
#define LCD_AFA_ALL2ONE_REG                 ( LCD_CTL_BASE + 0x01E8 )                       // Mixed overlay control
    
// LCD Clock Control
#define PMUA_LCD_CLK_RES_CTRL_REG           ( PMU_BASE + 0x004C )
#define PMUA_LCD_AXI_RST_N                  ( 1u )
#define PMUA_LCD_RST_N                      ( 1u << 1 )
#define PMUA_LCD_AHCLK_RST_N                ( 1u << 2 )
#define PMUA_LCD_ALL_RST_N                  ( PMUA_LCD_AXI_RST_N | PMUA_LCD_RST_N | PMUA_LCD_AHCLK_RST_N )
#define PMUA_LCD_AXICLK_EN                  ( 1u << 3 )
#define PMUA_LCD_CLK_EN                     ( 1u << 4 )
#define PMUA_LCD_HCLK_EN                    ( 1u << 5 )
#define PMUA_LCD_ALL_CLKS_EN                ( PMUA_LCD_AXICLK_EN | PMUA_LCD_CLK_EN | PMUA_LCD_HCLK_EN )
#define PMUA_LCD_CLK_SEL_MASK               ( 1u << 6 )
#define PMUA_LCD_CLK_SEL_312MHZ             ( 0u << 6 )
#define PMUA_LCD_CLK_SEL_52MHZ              ( 1u << 6 )
#define PMUA_LCD_PST_CKEN                   ( 1u << 9 )

#define PMUA_CCIC_CLK_RES_CTRL_REG          ( PMU_BASE + 0x0050 )
#define PMUA_CCIC_AXI_RST_N                 ( 1u << 0 )
#define PMUA_CCIC_AXICLK_EN                 ( 1u << 3 ) 

#define PMUA_DMA_CLK_RES_CTRL_REG           ( PMU_BASE + 0x0064 )

#endif // __DISPLAY_REG_H__

