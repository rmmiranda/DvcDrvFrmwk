/*****************************************************************************
* Filename:    dispif_tavorpv2.h
*
* Copyright 2009, Research In Motion Ltd
*
* Author:      Victor Chen
*
* Created:     May 26th, 2009
*
* Description: This header file defines the PV2 Display and DSI controller 
*              register map
*
*****************************************************************************/
#ifndef __DISPHAL_TAVORPV2_H__
#define __DISPHAL_TAVORPV2_H__

#include "dispif_internal.h"

/* By default, Tavor PV2 chip is MG1_A0 */
#define TAVORPV2_HWVER_MG1

/* Tavor PV2 has 2 DSI controllers */
#define TAVORPV2_DSI_CTL                         0x02

/* Register status transition timeout value after read/write */
#define TAVORPV2_REG_TIMEOUT                     1000*RE_MICROSECOND

//Maximum LCD panel size supported on Tavor PV2
#define TAVORPV2_LCD_MAX_WIDTH                   864
#define TAVORPV2_LCD_MAX_HEIGHT                  480

// Default bytes per pixels for LCD controller
#define TAVORPV2_LCD_DEF_BPP                     0x03

#define DSI_CMD_FIFO_LEN                         128

#define DSI_CMD_FIFO_BUF_LEN                     1024

// Reset PIN for LCD Panel
#define DISP_LCD_RST_N GPIO_77


// Work-Around for Z0/Z1/A0: Maximu pixels each time write to DSI comamnd FIFO
//#define DSI_CMD_FIFO_WKA_PIXELS                  42

/* ---------------------------------------------------------------------------
* Register definitions for Display (LCD) controller 
* --------------------------------------------------------------------------- */
#define LCD_REGBASE                             0x44100000

#define LCD_CTL                                 (LCD_REGBASE)
    #define LCD_EN                              ( 1u << 31)
    #define LCD_QD                              ( 1 << 30)
    #define LCD_EN_INT_EN                       ( 1 << 29)
    #define LCD_DIS_INT_EN                      ( 1 << 28)
    #define LCD_QD_INT_EN                       ( 1 << 27)
    #define LCD_AXI32_EN                        (  1 << 23)
    #define LCD_GMIX_INT_EN                     ( 1 << 17)
    #define LCD_GWIN_INT_EN                     ( 1 << 16)
    #define LCD_GFETCH_INT_EN                   ( 1 << 15)

#define LCD_CTL_STS                             (LCD_REGBASE + 0x0004)
    #define LCD_EN_STS                          ( 1 << 29)
    #define LCD_QD_STS                          ( 1 << 27)

/* Interrupt Register */
#define LCD_CTL_INT_STS                         (LCD_REGBASE + 0x0100)
    #define GMIX_INT_STS                        ( 1 << 10)
    #define GWIN_INT_STS                        ( 1 << 9)
    #define GFETCH_INT_STS                      ( 1 << 8)
    #define LCD_EN_INT_STS                      ( 1 << 2)
    #define LCD_DIS_INT_STS                     ( 1 << 1)
    #define LCD_QD_INT_STS                      ( 1 << 0)
    
#define LCD_FETCH_INT_STS0                      (LCD_REGBASE + 0x0580)
#define LCD_FETCH_INT_STS1                      (LCD_REGBASE + 0x0584)
#define LCD_GWIN_INT_STS                        (LCD_REGBASE + 0x010C)

#define LCD_WIN0_INT_STS                        (LCD_REGBASE + 0x061C)
#define LCD_WIN1_INT_STS                        (LCD_REGBASE + 0x065C)
#define LCD_WIN2_INT_STS                        (LCD_REGBASE + 0x069C)
#define LCD_WIN3_INT_STS                        (LCD_REGBASE + 0x06DC)
#define LCD_WIN4_INT_STS                        (LCD_REGBASE + 0x07DC)
#define LCD_CRSR_INT_STS                        (LCD_REGBASE + 0x071C)


#define LCD_CONV0_INT_STS                       (LCD_REGBASE + 0x1028)
#define LCD_CONV1_INT_STS                       (LCD_REGBASE + 0x2028)


/* Channel: 0 - 6 */
#define LCD_FETCH_CTL(i)                         (LCD_REGBASE + 0x0200 + 0x40*i)
    #define SP_MODIFIED_FRAME                    ( 1 << 11)
    #define SP_MODE                              ( 1 << 10)    
    #define BUS_ERR_INT_EN                       ( 1 << 8)
       #define START_FR_INT_EN                   ( 1 << 7)
    #define END_FR_INT_EN                        ( 1 << 6)
    #define SRC_FOR(n)                           ( (n) << 2)
    #define CHAN_EN                              ( 1 << 0)

#define LCD_FETCH_STS(i)                         (LCD_REGBASE + 0x0204 + 0x40*i)
    #define CHAN_STS                             ( 1 << 0)
    #define SRC_FOR_STS                          ( 0x7 << 2)

#define LCD_NXT_DESC_ADDR(i)                     (LCD_REGBASE + 0x0208 + 0x40*i)
#define LCD_FR_ID(i)                             (LCD_REGBASE + 0x020C + 0x40*i)
#define LCD_FR_ADDR(i)                           (LCD_REGBASE + 0x0210 + 0x40*i)
#define LCD_CH_CMD(i)                            (LCD_REGBASE + 0x0214 + 0x40*i)
#define LCD_FR_BRANCH(i)                         (LCD_REGBASE + 0x0218 + 0x40*i)


/* Other Fetch Unit Registers */
#define LCD_CH_ERR_ADDR                          (LCD_REGBASE + 0x05A0)
#define LCD_CH_INT_ID                            (LCD_REGBASE + 0x05A4)
#define LCD_CLRSPC_COEFF0                        (LCD_REGBASE + 0x05B0)
#define LCD_CLRSPC_COEFF1                        (LCD_REGBASE + 0x05B4)
#define LCD_CLRSPC_COEFF2                        (LCD_REGBASE + 0x05B8)
#define LCD_CLRSPC_COEFF3                        (LCD_REGBASE + 0x05BC)

#define LCD_CHx_ALPHA(n)                         (LCD_REGBASE + 0x05C0 + n * 0x04)
#define LCD_CHx_CLR_MATCH(n)                     (LCD_REGBASE + 0x05E0 + n * 0x04)


/* Window Generator 0 - 3 */
#define LCD_WINx_CTL(n)                          (LCD_REGBASE + 0x0600 + n*0x40)
    #define WIN_EN_STS                           (0x01 << 0)
    #define WIN_URUN_INT_EN                      (0x01 << 1)
    #define WIN_EOF_INT_EN                       (0x01 << 2)
    #define WIN_SOF_INT_EN                       (0x01 << 3)
    #define WIN_XRES(n)                          (((n) & 0xFF) << 8)
    #define WIN_YRES(n) 	                     (((n) & 0xFF) << 20)

#define LCD_WINx_CFG(n)                          (LCD_REGBASE + 0x0604 + n*0x40)
#define LCD_WINx_CROP0(n)                        (LCD_REGBASE + 0x060C + n*0x40)
#define LCD_WINx_CROP1(n)                        (LCD_REGBASE + 0x0610 + n*0x40)
#define LCD_WINx_SCALE_PTR(n)                    (LCD_REGBASE + 0x0614 + n*0x40)
#define LCD_WINx_SCALE_RW(n)                     (LCD_REGBASE + 0x0618 + n*0x40)
#define LCD_WINx_INT_STS(n)                      (LCD_REGBASE + 0x061C + n*0x40)

/* Window Generator 4 */
#define LCD_WIN4_CTL                             (LCD_REGBASE + 0x07C0)
#define LCD_WIN4_CFG                             (LCD_REGBASE + 0x07C4)
#define LCD_WIN4_CROP0                           (LCD_REGBASE + 0x07CC)
#define LCD_WIN4_CROP1                           (LCD_REGBASE + 0x07D0)
#define LCD_WIN4_SCALE_PTR                       (LCD_REGBASE + 0x07D4)
#define LCD_WIN4_SCALE_RW                        (LCD_REGBASE + 0x07D8)

/* Cursor Generator 0 */
#define LCD_CRSR0_CTRL                           (LCD_REGBASE + 0x0700)
#define LCD_CRSR0_CLR0                           (LCD_REGBASE + 0x0704)
#define LCD_CRSR0_CLR1                           (LCD_REGBASE + 0x0708)
#define LCD_CRSR0_CLR2                           (LCD_REGBASE + 0x070C)
#define LCD_CRSR0_CLR3                           (LCD_REGBASE + 0x0710)
#define LCD_CRSR0_PTR                            (LCD_REGBASE + 0x0714)
#define LCD_CRSR0_RW                             (LCD_REGBASE + 0x0718)


/* Mixer 0 and 1 */
#define LCD_MIXERx_CTL0(n)                       (LCD_REGBASE + 0x0800 + n*0x100) 
    #define LCD_MIXERx_CTL0_MIX_EN               (0x01 << 0)
    #define LCD_MIXERx_CTL0_BP_COLOR_USER        (0x01 << 1)
    #define LCD_MIXERx_CTL0_BP_ID_MASK           (0x1f << 2)
	#define LCD_MIXERx_CTL0_BP_ID(n)             ((n) << 2)
    #define LCD_MIXERx_CTL0_OL1_ID_MASK			 (0x1f << 8)
	#define LCD_MIXERx_CTL0_OL1_ID(n)            ((n) << 8)
    #define LCD_MIXERx_CTL0_OL2_ID_MASK 		 (0x1f << 14)
	#define LCD_MIXERx_CTL0_OL2_ID(n)            ((n) << 14)
    #define LCD_MIXERx_CTL0_OL3_ID_MASK 		 (0x1f << 20)
	#define LCD_MIXERx_CTL0_OL3_ID(n) 		     ((n) << 20)
    #define LCD_MIXERx_CTL0_CRSR_ID_MASK 		 (0x1f << 26)
	#define LCD_MIXERx_CTL0_CRSR_ID(n)           ((n) << 26)
    #define LCD_MIXERx_CTL0_DISP_UP				 (1u << 31)


#define LCD_MIXERx_CTL1(n)                       (LCD_REGBASE + 0x0804 + n*0x100)
    #define LCD_MIXERx_CTL1_DISP_EN_INT_EN       (1 << 0)
    #define LCD_MIXERx_CTL1_DISP_UPDATE_INT_EN   (1 << 1)
    #define LCD_MIXERx_CTL1_BLINK_INT_EN         (1 << 2)
    #define LCD_MIXERx_CTL1_DISP_XRES(n)         ((n) <<5)
    #define LCD_MIXERx_CTL1_DISP_YRES(n)         ((n) <<17)
    
#define LCD_MIXERx_CTL2(n)                       (LCD_REGBASE + 0x0844 + n*0x100)
    #define LCD_MIXERx_CTL2_CONV_ID(n)           ((n) << 26)

#define LCD_MIXERx_STS0(n)                       (LCD_REGBASE + 0x082C + n*0x100)
    #define MIX_EN_STS                           (1 << 0)
    #define BP_STS(n)                            ((n) << 2)
    #define OL1_STS(n)                           ((n) << 8)
    #define OL2_STS(n)                           ((n) << 14)
    #define OL3_STS(n)                           ((n) << 20)
    #define CRSR_STS(n)                          ((n) << 26)

#define LCD_MIXERx_STS1(n)                       (LCD_REGBASE + 0x0848 + n*0x100)

#define LCD_MIXERx_INT_STS(n)                    (LCD_REGBASE + 0x0840 + n*0x100)

#define LCD_GMIX_INT_STS                         (LCD_REGBASE + 0x0110)

#define LCD_MIXERx_BP_CFG0(n)                    (LCD_REGBASE + 0x0808 + n*0x100)
    #define LCD_MIXERx_CFG0_XPOS(n)              ((n) << 0)
    #define LCD_MIXERx_CFG0_YPOS(n)		         ((n) << 12)

#define LCD_MIXERx_BP_CFG1(n)                    (LCD_REGBASE + 0x080C + n*0x100)             
    #define LCD_MIXERx_CFG1_XRES(n)              ((n) << 0)
    #define LCD_MIXERx_CFG1_YRES(n)		         ((n) << 12)

#define LCD_MIXERx_OL1_CFG0(n)                   (LCD_REGBASE + 0x0810 + n*0x100)             
#define LCD_MIXERx_OL1_CFG1(n)                   (LCD_REGBASE + 0x0814 + n*0x100)             
#define LCD_MIXERx_OL2_CFG0(n)                   (LCD_REGBASE + 0x0818 + n*0x100)             
#define LCD_MIXERx_OL2_CFG1(n)                   (LCD_REGBASE + 0x081C + n*0x100)             
#define LCD_MIXERx_OL3_CFG0(n)                   (LCD_REGBASE + 0x0820 + n*0x100)             
#define LCD_MIXERx_OL3_CFG1(n)                   (LCD_REGBASE + 0x0824 + n*0x100)             
#define LCD_MIXERx_CRSR_CFG0(n)                  (LCD_REGBASE + 0x0828 + n*0x100)
#define LCD_MIXERx_BP_CLR(n)                     (LCD_REGBASE + 0x0830 + n*0x100)
#define LCD_MIXERx_OL1_CLR(n)                    (LCD_REGBASE + 0x0834 + n*0x100)
#define LCD_MIXERx_OL2_CLR(n)                    (LCD_REGBASE + 0x0838 + n*0x100)
#define LCD_MIXERx_OL3_CLR(n)                    (LCD_REGBASE + 0x083C + n*0x100)


/* Parallel Converter Register */
#define LCD_CONV0_CTL                            (LCD_REGBASE + 0x1000)
#define LCD_MIXER0_TIM0                          (LCD_REGBASE + 0x1004)
#define LCD_MIXER0_TIM1                          (LCD_REGBASE + 0x1008)
#define LCD_MIXER0_TIM2                          (LCD_REGBASE + 0x100C)
#define LCD_MIXER0_TIM3                          (LCD_REGBASE + 0x1010)
#define LCD_MIXER0_G_CTL                         (LCD_REGBASE + 0x1014)
    #define MIXERx_G_CTL_ADD_PTR_R(n)            ((n)<<0)
    #define MIXERx_G_CTL_ADD_PTR_G(n)            ((n)<<4)
    #define MIXERx_G_CTL_ADD_PTR_B(n)            ((n)<<8)
    #define MIXERx_G_CTL_Q4_C(n)                 ((n)<<16)
    #define MIXERx_G_CTL_Q3_C(n)                 ((n)<<18)
    #define MIXERx_G_CTL_Q2_C(n)                 ((n)<<20)
    #define MIXERx_G_CTL_Q1_C(n)                 ((n)<<22)
    #define MIXERx_G_CTL_G_VAL(n)                ((n)<<24)
#define LCD_MIXER0_G_DAT_RED                     (LCD_REGBASE + 0x1018)
#define LCD_MIXER0_G_DAT_GREEN                   (LCD_REGBASE + 0x101C)
#define LCD_MIXER0_G_DAT_BLUE                    (LCD_REGBASE + 0x1020)
#define LCD_MIXER0_PRSR                          (LCD_REGBASE + 0x1024)
#define LCD_CONV0_SCFIF                          (LCD_REGBASE + 0x102C)
#define LCD_MIXER0_TIM4                          (LCD_REGBASE + 0x1030)


/* ---------------------------------------------------------------------------
* Register definitions for DSI controller 
* --------------------------------------------------------------------------- */
#define DSI_REGBASE                              0x44102000

/* DSI Converter */
#define DSI_DxSCR0(i)                            (DSI_REGBASE + i * 0x1000 + 0x0000)
    #define DSI_DxSCR0_PRIM_VC                   ( 1 << 11)
    #define DSI_DxSCR0_CONV_EN                   ( 1 << 10)
	#define DSI_DxSCR0_BREAK                     ( 1 << 9)
    #define DSI_DxSCR0_FLOW_DIS                  ( 1 << 8)
	#define DSI_DxSCR0_SP_BREAK_INT_EN           ( 1 << 7)
    #define DSI_DxSCR0_SP_UNDRUN_INT_EN          ( 1 << 5)
    #define DSI_DxSCR0_SP_RD_INT_EN              ( 1 << 4)
    #define DSI_DxSCR0_SMART_INT_EN              ( 1 << 3)
    #define DSI_DxSCR0_DISP_SOF_INT_EN           ( 1 << 2)
    #define DSI_DxSCR0_DISP_EOF_INT_EN           ( 1 << 1)
    #define DSI_DxSCR0_DISP_URUN_INT_EN          ( 1 << 0)

#define DSI_DxCONV_GEN_NULL_BLANK(i)             (DSI_REGBASE + i * 0x1000 + 0x0034)
    #define DSI_DxCONV_PXL_FORMAT_MASK 		     (0x03)
    #define DSI_DxCONV_GEN_PXL_FORMAT_3(n)       ((n & 0x03) << 30)
    #define DSI_DxCONV_GEN_PXL_FORMAT_2(n)       ((n & 0x03) << 28)
	#define DSI_DxCONV_GEN_PXL_FORMAT(n)         ((n & 0x03) << 26)
    #define DSI_DxCONV_NULL_BLANK_DATA(n)        ( n << 0)
    
#define DSI_DxSCR1(i)                            (DSI_REGBASE + i * 0x1000 + 0x0100)
    #define DSI_DxSCR1_DSI_EN                    (1 << 0)
    #define DSI_DxSCR1_NOL_MASK                  (0x03 << 2)
    #define DSI_DxSCR1_NOL(n)                    ((n & 0x03) << 2)
    #define DSI_DxSCR1_PT						 (1 << 4)
    #define DSI_DxSCR1_VC0(n)                    ((n & 0x03) << 6)
    #define DSI_DxSCR1_VC1(n)                    ((n & 0x03) << 8)
    #define DSI_DxSCR1_VC_EN                     (1 << 10)
    #define DSI_DxSCR1_VSYNC_MODE                (1 << 11)
    #define DSI_DxSCR1_POWER_MASK                (0x19 << 23)
    #define DSI_DxSCR1_RIHS                      ( 1 << 23)
	#define DSI_DxSCR1_TRIG                      ( 1 << 24)
    #define DSI_DxSCR1_ULPS                      ( 1 << 26)
    #define DSI_DxSCR1_LPDT                      ( 1 << 27)
    #define DSI_DxSCR1_BTA                       (1u << 31)

#define DSI_DxSSR(i)                             (DSI_REGBASE + i * 0x1000 + 0x0104)
    #define DSI_DxSSR_DSI_EN_STAT                ( 1 << 0)
    #define DSI_DxSSR_L0_PM                      ( 1 << 24)
    #define DSI_DxSSR_L1_PM                      ( 1 << 25)
	#define DSI_DxSSR_L2_PM                      ( 1 << 26)
	#define DSI_DxSSR_L3_PM                      ( 1 << 27)
    #define DSI_DxSSR_STOP_ST                    ( 1 << 28)
	#define DSI_DxSSR_DIR                        ( 1u << 31)

#define DSI_DxTRIG(i)                            (DSI_REGBASE + i * 0x1000 + 0x0108)

#define DSI_DxINEN(i)                            (DSI_REGBASE + i * 0x1000 + 0x010C)
    #define DSI_DxINEN_DSI_EN                    ( 1 << 0)
    #define DSI_DxINEN_SOF_EN                    ( 1 << 1)
    #define DSI_DxINEN_SOL_EN                    ( 1 << 2)
    #define DSI_DxINEN_URUN_EN                   ( 1 << 3)
    #define DSI_DxINEN_PHY_ERR_EN                ( 1 << 4)
    #define DSI_DxINEN_ACK_ERR_EN                ( 1 << 8)
    #define DSI_DxINEN_TIMEOUT_EN				 ( 1 << 12)
    #define DSI_DxINEN_G_TIMER_EN				 ( 1 << 14)
    #define DSI_DxINEN_GTIM_RES				     ( 1 << 15)
    #define DSI_DxINEN_PROT_ERR_EN				 ( 1 << 16)
	#define DSI_DxINEN_SP_RD_INT_EN				 ( 1 << 21)
	#define DSI_DxINEN_LP_CONT_EN				 ( 1 << 24)
	#define DSI_DxINEN_TE0_INT_EN				 ( 1 << 28)
	#define DSI_DxINEN_TE1_INT_EN				 ( 1 << 29)
	#define DSI_DxINEN_TE2_INT_EN				 ( 1 << 30)
	#define DSI_DxINEN_TE3_INT_EN				 ( 1u << 31)

#define DSI_DxINST1(i)                           (DSI_REGBASE + i * 0x1000 + 0x0110)
    #define DSI_EN_ST                            ( 1 << 0)
    #define SOF_ST                               ( 1 << 1)
    #define SOL_ST                               ( 1 << 2)
    #define LPDT_SYNC_ST                         ( 1 << 4)
    #define FALSE_CTL_ERR                        ( 1 << 5)
    #define ESC_ENTRY_ERR                        ( 1 << 6)
    #define ACK_ERR_ST                           ( 1 << 7)
    #define TE0_INT                              ( 1 << 8)
    #define TE1_INT                              ( 1 << 9)
    #define TE2_INT                              ( 1 << 10)
    #define TE3_INT                              ( 1 << 11)
    #define HS_TX_TO_ERR                         ( 1 << 12)    
    #define LP_RX_TO_ERR                         ( 1 << 13)
    #define BTA_TO_ERR                           ( 1 << 14) 
    #define G_TIMER_ST                           ( 1 << 15) 
    #define ECC_1BIT                             ( 1 << 16) 
    #define ECC_2BIT                             ( 1 << 17)     
    #define CRC_ERR                              ( 1 << 18) 
    #define TRIG_ST                              ( 1 << 19) 
    #define DT_ERR_ST                            ( 1 << 20)
    #define LP0_L0                               ( 1 << 24) 
    #define LP1_L0                               ( 1 << 25) 
    #define LP0_L1                               ( 1 << 26) 
    #define LP1_L1                               ( 1 << 27) 
    #define LP0_L2                               ( 1 << 28)
    #define LP1_L2                               ( 1 << 29) 
    #define LP0_L3                               ( 1 << 30)
    #define LP1_L3                               ( 1u << 31)

#define DSI_DxTEINTCNT(i)                        (DSI_REGBASE + i * 0x1000 + 0x0114)
#define DSI_Dx_G_CTL(i)                          (DSI_REGBASE + i * 0x1000 + 0x0014)

#define DSI_Dx_G_DAT_RED(i)                      (DSI_REGBASE + i * 0x1000 + 0x0018)
#define DSI_Dx_G_DAT_GREEN(i)                    (DSI_REGBASE + i * 0x1000 + 0x001C)
#define DSI_Dx_G_DAT_BLUE(i)                     (DSI_REGBASE + i * 0x1000 + 0x0020)

#define DSI_DxPRSR(i)                            (DSI_REGBASE + i * 0x1000 + 0x0024)
    #define DSI_DxPRSR_A0                        ( 1 << 8)
    #define DSI_DxPRSR_ST_OK                     ( 1 << 9)
    #define DSI_DxPRSR_CON_NT                    ( 1 << 10)
    
#define DSI_DxINST0(i)                           (DSI_REGBASE + i * 0x1000 + 0x0028)
    #define DISP_URUN_INT_STS                    ( 1 << 0)
    #define DISP_EOF_INT_STS                     ( 1 << 1)
    #define DISP_SOF_INT_STS                     ( 1 << 2)
    #define SP_RD_INT_STS                        ( 1 << 4)
    #define SP_UNDRUN_INT_STS                    ( 1 << 5)
    #define SP_GEN_INT_STS                       ( 1 << 6)
    #define BREAK_INT_STS                        ( 1 << 7)

#define DSI_DxADAT(i)                            (DSI_REGBASE + i * 0x1000 + 0x0128)
#define DSI_DxCFIF(i)                            (DSI_REGBASE + i * 0x1000 + 0x002C)

#define DSI_DxTIM0(i)                            (DSI_REGBASE + i * 0x1000 + 0x0130)
#define DSI_DxTIM1(i)                            (DSI_REGBASE + i * 0x1000 + 0x0134)

#define DSI_DxPHY_TIM0(i)                        (DSI_REGBASE + i * 0x1000 + 0x013C)
    #define DSI_DxPHY_TIME0_LPX(n)               ((n & 0xFF) << 24)
    #define DSI_DxPHY_TIME0_HSPREP(n)            ((n & 0xFF) << 16)
    #define DSI_DxPHY_TIME0_HSZERO(n)            ((n & 0xFF) << 8)
    #define DSI_DxPHY_TIME0_HSTRAIL(n)           (n & 0xFF)

#define DSI_DxPHY_TIM1(i)                        (DSI_REGBASE + i * 0x1000 + 0x0140)
    #define DSI_DxPHY_TIME1_HSEXIT(n)            ((n & 0xFF) << 24)
    #define DSI_DxPHY_TIME1_TAGO(n)              ((n & 0xFF) << 16)
    #define DSI_DxPHY_TIME1_CLZERO(n)            ((n & 0xFF) << 8)
    #define DSI_DxPHY_TIME1_CLTRAIL(n)           (n & 0xFF)
#define DSI_DxPHY_TIM2(i)                        (DSI_REGBASE + i * 0x1000 + 0x0144)
    #define DSI_DxPHY_TIME2_TAGET(n)             ((n & 0xFF) << 16)
    #define DSI_DxPHY_TIME2_WAKEUP(n)            (n & 0xFF)
#define DSI_DxPHY_CAL(i)                         (DSI_REGBASE + i * 0x1000 + 0x0148)
    #define DSI_DxPHY_CAL_MIPI_REN_BYPASS(n)     ((n & 0xFF) << 4)
    #define DSI_DxPHY_CAL_MIPI_RCOMP_LOAD        ( 1 << 0)
    #define DSI_DxPHY_CAL_MIPI_RCOMP_CLKSEL		 ( 1 << 1)
    #define DSI_DxPHY_CAL_MIPI_BG_VREF_EN		 ( 1 << 2)
    #define DSI_DxPHY_CAL_MIPI_RCOMP_CLK_EXT	 ( 1u << 31)
    

// DSI Set Primary Virtual Channel
#define DxSCR0_PRIMVC_MASK                       0x00000800
#define DxSCR0_SET_PRIMVC(x)                     (( (x) << 11 ) & DxSCR0_PRIMVC_MASK) 


/* ---------------------------------------------------------------------------
* Register definitions for Serdes LM2550 controller 
* --------------------------------------------------------------------------- */
#define LM2550_VC0_CFG_CPU                       0x00
#define LM2550_CPU_MODE_MASK                     ( 0x03 << 0)
#define LM2550_CPU_MODE_8BIT                     ( 0x00 << 0)
#define LM2550_CPU_MODE_9BIT                     ( 0x01 << 0)
#define LM2550_CPU_MODE_16BIT                    ( 0x02 << 0)
#define LM2550_CPU_MODE_18BIT                    ( 0x03 << 0)
#define LM2550_DCS_ENABLE                        ( 0x01 << 2)
#define LM2550_TARGET_MASK                       ( 0x03 << 4)
#define LM2550_TARGET_OFF                        ( 0x00 << 4)
#define LM2550_TARGET_SERIAL_MASER               ( 0x01 << 4)
#define LM2550_TARGET_CPU_MASTER                 ( 0x02 << 4)
#define LM2550_VC1_CFG_CPU                       0x01
#define LM2550_VC1_CFG_SERIAL                    0x01
#define LM2550_VC2_CFG_CPU                       0x02
#define LM2550_VC2_CFG_SERIAL                    0x02
#define LM2550_SER_CFG                           0x03
#define LM2550_CPU_CFG                           0x04
#define LM2550_CPU_CFG_CPUDIV_MASK               ( 0x0F << 0 )
#define LM2550_CPU_CFG_CPUDIV(x)                 ( x << 0 )
#define LM2550_CPU_CFG_CS0_DUMMY                 ( 0x01 << 4 )
#define LM2550_CPU_CFG_CS1_DUMMY                 ( 0x01 << 5 )
#define LM2550_CPU_CFG_CS2_DUMMY                 ( 0x01 << 6 )
#define LM2550_RGB_CFG                           0x05
#define LM2550_TE_CFG                            0x06
#define LM2550_TE_CFG_TE0_RISING                 ( 0x01 << 0 )
#define LM2550_TE_CFG_TE0_FALLING                ( 0x01 << 1 )
#define LM2550_TE_CFG_TE1_RISING                 ( 0x01 << 2 )
#define LM2550_TE_CFG_TE1_FALLING                ( 0x01 << 3)
#define LM2550_TE_CFG_TE2_RISING                 ( 0x01 << 4 )
#define LM2550_TE_CFG_TE2_FALLING                ( 0x01 << 5 )
#define LM2550_CS2_CFG                           0x07
#define LM2550_MWDMD                             0x08
#define LM2550_MRCMD                             0x09
#define LM2550_MRLEN                             0x0A
#define LM2550_CRCMD0                            0x0B
#define LM2550_CRLEN0                            0x0C
#define LM2550_CRCMD1                            0x0D
#define LM2550_CRLEN1                            0x0E
#define LM2550_SRCMD0                            0x10
#define LM2550_SRLEN0                            0x11
#define LM2550_SRCMD1                            0x12
#define LM2550_SRLEN1                            0x13
#define LM2550_SELECTOR                          0x16
#define LM2550_DSI_CFGA                          0x40
#define LM2550_DSI_CFGA_SOT                      ( 0x01 << 0)
#define LM2550_DSI_CFGA_SOT_SYNC                 ( 0x01 << 1)
#define LM2550_DSI_CFGA_EOT_SYNC                 ( 0x01 << 2)
#define LM2550_DSI_CFGA_ESC_ENTRY                ( 0x01 << 3)
#define LM2550_DSI_CFGA_LPTX_SYNC                ( 0x01 << 4)
#define LM2550_DSI_CFGA_HSRX_TO                  ( 0x01 << 5)
#define LM2550_DSI_CFGA_CONTROL                  ( 0x01 << 6)
#define LM2550_DSI_CFGB                          0x41
#define LM2550_DSI_CFGB_ECC_SINGLE               ( 0x01 << 0)
#define LM2550_DSI_CFGB_ECC_MULTI                ( 0x01 << 1)
#define LM2550_DSI_CFGB_CHECKSUM                 ( 0x01 << 2)
#define LM2550_DSI_CFGB_BAD_DT                   ( 0x01 << 3)
#define LM2550_DSI_CFGB_BAD_VC                   ( 0x01 << 4)
#define LM2550_DSI_CFGB_BAD_LEN                  ( 0x01 << 5)
#define LM2550_DSI_CFGB_PROTOCOL                 ( 0x01 << 7)
#define LM2550_DSI_CFGC                          0x42
#define LM2550_DSI_CFGC_LP0                      ( 0x01 << 0)
#define LM2550_DSI_CFGC_LP1                      ( 0x01 << 1)
#define LM2550_DSI_CFGC_HOLD                     ( 0x01 << 2)
#define LM2550_DSI_CFGC_IGN_HSCMD                ( 0x01 << 4)
#define LM2550_DSI_CFGC_SYNC_SHAPE               ( 0x01 << 5)
#define LM2550_DSI_CFGC_LANES(x)                 ( (x) << 6)
#define LM2550_DSI_CFGD                          0x43
#define LM2550_DSI_CFGD_RX_TRIM(x)               ( (x) << 0)
#define LM2550_DSI_CFGD_HSRX_TO(x)               ( (x) << 4)
#define LM2550_DSI_CFGE                          0x44
#define LM2550_DSI_CFGE_LPDIVO(x)                ( (x) << 0)
#define LM2550_DSI_CFGE_LPDIVS(x)                ( (x) << 4)
#define LM2550_DSI_CFGF                          0x45
#define LM2550_DSI_CFGF_SETTLE(x)                ( (x) << 0)
#define LM2550_DSI_CFGF_LPTX_TO(x)               ( (x) << 5)
#define LM2550_DSI_CFGG                          0x46
#define LM2550_DSI_CFGG_VC0_EN                   ( 0x01 << 0)
#define LM2550_DSI_CFGG_VC1_EN                   ( 0x01 << 1)
#define LM2550_DSI_CFGG_VC2_EN                   ( 0x01 << 2)
#define LM2550_DSI_CFGG_VC3_EN                   ( 0x01 << 3)
#define LM2550_ERR_CMD                           0x47
#define LM2550_ERR_CMD_CLR_CNT                   ( 0x01 < 0)
#define LM2550_ERR_CMD_CLR_PROT                  ( 0x01 < 1)
#define LM2550_ERR_CMD_CLR_CONTEND               ( 0x01 < 2)
#define LM2550_ERR_STAT                          0x48
#define LM2550_ERR_STAT_LP0_CONTEND              ( 0x01 << 0)
#define LM2550_ERR_STAT_LP1_CONTEND              ( 0x01 << 1)
#define LM2550_ERR_STAT_NO_BTA                   ( 0x01 << 4)
#define LM2550_ERR_STAT_NO_EOT                   ( 0x01 << 5)
#define LM2550_ERR_STAT_NO_OVER                  ( 0x01 << 6)
#define LM2550_ERR_CNT                           0x49
#define LM2550_RGB_PD                            0x7B
#define LM2550_ATE_CFG                           0x7C
#define LM2550_ATE_CFG_TIMER_BP                  ( 0x01 << 0)
#define LM2550_ATE_CFG_CLKMISS_BP                ( 0x01 << 1)
#define LM2550_ATE_CFG_LOCKOUT_BP                ( 0x01 << 2)
#define LM2550_IO_DRV                            0x7D
#define LM2550_IO_DRV_PCLK                       ( 0x01 << 0)
#define LM2550_IO_DRV_VSHSDE                     ( 0x01 << 1)
#define LM2550_IO_DRV_CS                         ( 0x01 << 2)
#define LM2550_IO_DRV_CPU                        ( 0x01 << 3)
#define LM2550_IO_DRV_SER                        ( 0x01 << 4)
#define LM2550_IO_DRV_SER_MUXED                  ( 0x01 << 5)
#define LM2550_IO_DRV_ERR                        ( 0x01 << 6)
#define LM2550_IO_DRV_OTHER                      ( 0x01 << 7)
#define LM2550_DSI_RESET                         0x7E
#define LM2550_DSI_RESET_DSI                     ( 0x01 << 0)
#define LM2550_CFGVALID                          0x7F
#define LM2550_CFGVALID_CFG_VALID                ( 0x01 << 0)
#define LM2550_CFGVALID_CFG_CLK_EN               ( 0x01 << 1)
#define LM2550_CFGVALID_CFG_RGB_CLK_EN           ( 0x01 << 2)

/* ---------------------------------------------------------------------------
* Register definitions for Display/DSI controller Clock settings
* --------------------------------------------------------------------------- */
// (defined in Tavor_clockpmu.h ?)
#define D0CKEN_A_REG32                           (0x4134000C) 
    #define D0CKEN_A_LCD                         (1 << 1)

#define D0CKEN_B_REG32                           (0x41340010) 
    #define D0CKENB_HSIO                         (1 << 29)
    #define D0CKENB_VIDEO_ACC                    (1 << 11)
    #define D0CKENB_GCU                          (1 << 10)
    #define DOCKENB_WIRE                         (1 << 8)
    
#define D0CKEN_C_REG32                           (0x41340024)
    #define D0CKENC_DSITX2_ESC                   (1u << 31)
    #define D0CKENC_DSITX1_ESC                   (1 << 30)
    #define D0CKENC_GRAPHIC1                     (1 << 29)
    #define D0CKENC_GRAPHIC2                     (1 << 28)
    #define D0CKENC_PIXEL                        (1 << 23)
    #define D0CKENC_DISPLAY                      (1 << 22)
    #define D0CKENC_DSI_BIT2                     (1 << 21)
    #define D0CKENC_DSI_BIT1                     (1 << 20)
    #define D0CKENC_SYS_BUS                      (1 << 19)


#define ACCR1_REG32                                    0x41340020
    #define ACCR1_DSI_BIT1(x)                    ( (DWORD)((x) << 13))             
    #define ACCR1_DSI_BIT2(x)                    ( (DWORD)((x) << 16))
    // 624/12 Mhz
    #define DSI_BIT_12                           0   
    // 624/6 Mhz
    #define DSI_BIT_6                            1   
    // 624/4 Mhz
    #define DSI_BIT_4                            2   
    // 624/3 Mhz
    #define DSI_BIT_3                            3   
    // 624/2 Mhz
    #define DSI_BIT_2                            4 
    // GPLL
    #define DSI_BIT_GPLL                         5 
    #define DSI_BIT_MASK                         7

//#define AGENP                                    0x40F4002C
    #define AGENP_DDRPLL_CTRL                    ( 1 << 29))             
    #define AGENP_DDRPLL_DATA                    ( 1 << 28))

//#define DDRPLLR                                  0x41350004

// Embedded DMA channel for Display/DSI 
#define MVED_DADR_ADDRESS      0x56100210L
#define MVED_DCSR_ADDRESS      0x56100004L
#define MVED_DRCMR2_ADDRESS    0x56100110L
#define MVED_DRCMR3_ADDRESS    0x56100114L

//From MIPI Spec, Soft Reset trig code is 0x62			 
#define PV2_DSI_TRIGCODE_RESET (1 << 0)			 
#define PV2_DSI_TRIGCODE_TE  (1 << 1)
#define PV2_DSI_TRIGCODE_ACK (1 << 2)
#define PV2_DSI_TRIGCODE_UNKNOWN5 (1 << 3)



// Converter ID for Mixers' output device
typedef enum
{
    MIXER_CONV_ID_ALL = 0x00,
    MIXER_CONV_ID_FETCH = 0x01,
    MIXER_CONV_ID_MIXER0 = 0x08,
    MIXER_CONV_ID_MIXER1 = 0x09,
    MIXER_CONV_ID_P_CONV0 = 0x0C,
    MIXER_CONV_ID_S_CONV0= 0x0D,
    MIXER_CONV_ID_S_CONV1= 0x0E,
    MIXER_CONV_ID_WGE0 = 0x10,
    MIXER_CONV_ID_WGE1 = 0x11,
    MIXER_CONV_ID_WGE2 = 0x12,
    MIXER_CONV_ID_WGE3 = 0x13,
    MIXER_CONV_ID_WGE4 = 0x14,
    MIXER_CONV_ID_WGE_CURSOR = 0x15,
    MIXER_CONV_ID_NULL = 0x1F,
} MixerConverterID;

typedef enum
{
    // Windows Generation Unit ID
    DISP_CTL_BASE_PLANE = 0x00,
    DISP_CTL_OVERLAY1 = 0x01,
	DISP_CTL_OVERLAY2 = 0x02,
	DISP_CTL_OVERLAY3 = 0x03,
	DISP_CTL_CURSOR = 0x04,
}DispOverlay;

typedef enum
{
    // DMA channel ID for Fetch Unit
    DISP_CTL_DMA_CH00 = 0x00,
    DISP_CTL_DMA_CH01,
    DISP_CTL_DMA_CH02,
    DISP_CTL_DMA_CH03,
    DISP_CTL_DMA_CH04,
    DISP_CTL_DMA_CH05,
    DISP_CTL_DMA_CH06,
    DISP_CTL_DMA_CH07,
}DispDmaChannel;



/* Exported APIs for DSI/Display driver */
BOOL tavorpv2_dsiCtlEnable(DWORD dsiId, DWORD enableFlag);

BOOL tavorpv2_dispCtlEnable(DWORD enableFlag);

void tavorpv2_dispCtlMixerEnable(DWORD mixerId, BOOL enableFlag);
void tavorpv2_dispSetMixConvId(DWORD mixerId, MixerConverterID converterId);
void tavorpv2_dispSetMixRes(DWORD mixerId, DWORD width, DWORD height);
BOOL tavorpv2_dispCtlSetMixerUnit(DWORD mixerId, WORD width, WORD height, DWORD x, DWORD y, DWORD overlayId);

//BOOL tavorpv2_dispCtlSetFetchUnit(DWORD dmaCh, DWORD dmaAddr, DispPixelFormat srcFormat);
BOOL tavorpv2_dispCtlSetFetchUnit(DispDmaChannel dmaCh, DWORD dmaAddr, DispPixelFormat srcFormat);
void tavorpv2_dispCtlSetSpMode(DispDmaChannel dmaCh, BOOL enFLag);


//Start/Stop fetching pixels from system frame buffer
void tavorpv2_dispCtlStartFetch(DWORD dmaCh, BOOL enFlag);


//BOOL tavorpv2_dispCtlSetWindowUnit(DWORD winGeId, WORD width, WORD height, BOOL overlay);
BOOL tavorpv2_dispCtlSetWindowUnit(DispOverlay winGeId, WORD width, WORD height);



void tavorpv2_dispCtlSetDefaultGamma(void);


void tavorpv2_dsiSetIntClock(DWORD dsiId, BYTE dsiLinkClock);

BOOL tavorpv2_dsiCtlSetPowerMode(DWORD dsiId, DWORD powerMode);
BOOL tavorpv2_dsiCtlSetActiveLane(DWORD dsiId, DWORD activeLanes);

#if defined(TAVORPV2_HWVER_MG1)		
/* Set Pixel Format of DSI when DSI DCS/Generic Long Wirte commands are used*/
BOOL tavorpv2_dsiCtlSetVcFomat(DWORD dsiId, DWORD vcNumber, DispPixelFormat srcFormat);
#endif

BOOL tavorpv2_dsiCtlSetVcMap(DWORD dsiId, DWORD panels);

BOOL tavorpv2_dsiCtlSetPhyTimer(DWORD dsiId, DWORD dsiLinkClock);

BOOL tavorpv2_dsiIssueBTA(DWORD dsiId, DWORD enFlag);
DWORD tavorpv2_dsiPerformBTA(DWORD dsiId);
DWORD tavorpv2_dsiGetLinkDirect(DWORD dsiId);


void tavorpv2_dsiWriteCmd2Fifo(DWORD dsiId, DWORD cmdEntry);

void tavorpv2_lcdSetFIClock(BYTE dsi0LinkClock, BYTE dsi1LinkClock);
void tavorpv2_lcdSetGpios(void);

void tavorpv2_dsiStopTraffic(DWORD dsiId);

void tavorpv2_dispConfigureInterfaceTiming( DWORD dsiId, DWORD PHY_TIME0, DWORD PHY_TIME1, DWORD PHY_TIME2);


//Embedded DMA Channel 
BOOL tavorpv2_dsiEnableEmDma(DWORD dsiId, DWORD descAddr);
void tavorpv2_dsiSetFlowCtronl(DWORD dsiId, BOOL enFlag);
BOOL tavorpv2_dsiInsertDly(DWORD dsiId, DWORD value);
BOOL tavorpv2_dsiCreateEmDmaDescriptors(DWORD *descAddr, DWORD srcAddr, DWORD dstAddr, DWORD size, DWORD length);
void tavorpv2_dsiSetFlowCtronl(DWORD dsiId, BOOL enFlag);


//Trigger Code
BOOL tavorpv2_dsiSetTrigCode(DWORD dsiId, DsiTrigCode trigCode);
BYTE tavorpv2_dsiGetTrigCode(DWORD dsiId);

//Workaround to enable DSI
void tavorpv2_dsiStart(DWORD dsiId);
void tavorpv2_lcdInitDsi(DWORD dsiId);



/* Interrupt Handler */


// Dump all Display/DSI controller Registers
void tavorpv2_lcdRegsDump(DWORD regSet);


#endif 
