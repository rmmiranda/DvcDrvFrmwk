/*****************************************************************************
 * Filename:    hdmi_reg.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     March 2010
 *
 * Description: The MMP specific header file for the hdmi driver. Developed
 *              specifically for the platform software driver framework.
 *
 *****************************************************************************/

#ifndef __HDMI_MMP_H__
#define __HDMI_MMP_H__

#define HDMI_PIXEL_CLOCK_MAX        148500000
#define HDMI_PIXEL_REP_CLOCK_MAX    148500000
#define HDMI_TMDS_CLOCK_MAX         222750000
#define HDMI_AUDIO_REF_CLOCK_MAX    24576000
#define HDMI_I2S_SERIAL_CLOCK_MAX   12288000

#define HDMI_AUDIO_FIFO_DEPTH       512 // bytes
#define HDMI_AUDIO_FIFO_WIDTH       32 // bits

#define HDMI_REG_BASE_ADDR          0xD420BC00

/* direct addressable registers */
#define HDMI_PHY_CFG_0_REG          (HDMI_REG_BASE_ADDR + 0x0008)
#define HDMI_PHY_CFG_CP(x)          (((x)&0x0FF) << 24)
#define HDMI_PHY_CFG_EAMP(x)        (((x)&0xFFF) << 12)
#define HDMI_PHY_CFG_DAMP(x)        (((x)&0xFFF) <<  0)

#define HDMI_PHY_CFG_1_REG          (HDMI_REG_BASE_ADDR + 0x000C)
#define HDMI_PHY_CFG_AJ_D(x)        (((x)&0x000F) << 28)
#define HDMI_PHY_CFG_SVTX(x)        (((x)&0x0FFF) << 16)
#define HDMI_PHY_CFG_IDRV(x)        (((x)&0xFFFF) <<  0)

#define HDMI_PHY_CFG_2_REG          (HDMI_REG_BASE_ADDR + 0x0010)
#define HDMI_PHY_CFG_TP_EN          (0x0F << 28)
#define HDMI_PHY_CFG_RESET_TX       (0x01 << 27)
#define HDMI_PHY_CFG_POLSWAP_TX     (0x0F << 23)
#define HDMI_PHY_CFG_PD_TX          (0x0F << 19)
#define HDMI_PHY_CFG_PD_IREF        (0x01 << 18)
#define HDMI_PHY_CFG_LOOPBACK       (0x0F << 14)
#define HDMI_PHY_CFG_INV_CK20T      (0x0F << 10)
#define HDMI_PHY_CFG_AUX(x)         (((x)&0x3F) <<  4)
#define HDMI_PHY_CFG_AJ_EN          (0x0F <<  0)

#define HDMI_PHY_CFG_3_REG          (HDMI_REG_BASE_ADDR + 0x0014)
#define HDMI_PHY_CFG_HPD            ((0x0) << 10)
#define HDMI_PHY_CFG_TXDRVX2(x)     (((x)&0x0F) <<  6)
#define HDMI_PHY_CFG_TPC(x)         (((x)&0x1F) <<  1)
#define HDMI_PHY_CFG_SYNC           (0x01 <<  0)

#define HDMI_AUDIO_CFG_REG          (HDMI_REG_BASE_ADDR + 0x0018)
#define I2S_CLOCK_DELAY             (1 << 11)
#define WSP_ACTIVE_HIGH             (1 << 10)
#define FSP_ACTIVE_LOW              (1 <<  9)
#define CLKP_RISING_EDGE            (1 <<  8)
#define XDATDLY_0_BIT_DELAY         (0 <<  6)
#define XDATDLY_1_BIT_DELAY         (1 <<  6)
#define XDATDLY_2_BIT_DELAY         (2 <<  6)
#define XWDLEN_8_BIT                (0 <<  3)
#define XWDLEN_12_BIT               (1 <<  3)
#define XWDLEN_16_BIT               (2 <<  3)
#define XWDLEN_20_BIT               (3 <<  3)
#define XWDLEN_24_BIT               (4 <<  3)
#define XWDLEN_32_BIT               (5 <<  3)
#define XWDLEN_MASK					(7 <<  3)
#define XFERLEN_2_CHANNEL           (1 <<  0)
#define XFERLEN_8_CHANNEL           (7 <<  0)
#define XFERLEN_MASK				(7 <<  0)

#define HDMI_CLOCK_CFG_REG          (HDMI_REG_BASE_ADDR + 0x001C)
#define HDMI_CLOCK_CFG_PRCLK_DIV(x) (((x)&0x0F) << 13)
#define HDMI_CLOCK_CFG_TCLK_DIV(x)  (((x)&0x0F) <<  9)
#define HDMI_CLOCK_CFG_MCLK_DIV(x)  (((x)&0x0F) <<  5)
#define HDMI_CLOCK_CFG_EN           (0x01 <<  4)
#define HDMI_CLOCK_CFG_RTC(x)       (((x)&0x03) <<  2)
#define HDMI_CLOCK_CFG_WTC(x)       (((x)&0x03) <<  0)

#define HDMI_PLL_CFG_0_REG          (HDMI_REG_BASE_ADDR + 0x0020)
#define HDMI_PLL_CFG_DCCI(x)        (((x)&0x03) << 29)
#define HDMI_PLL_CFG_INTPI(x)       (((x)&0x03) << 27)
#define HDMI_PLL_CFG_CLK_DET_EN     (0x01 << 26)
#define HDMI_PLL_CFG_DCC_CTRL       (0x01 << 25)
#define HDMI_PLL_CFG_KVCO(x)        (((x)&0x1F) << 20)
#define HDMI_PLL_CFG_KVCO_MASK      (0x1F << 20)
#define HDMI_PLL_CFG_EN_TST_CLK     (0x01 << 19)
#define HDMI_PLL_CFG_POSTDIV(x)     (((x)&0x1F) << 14)
#define HDMI_PLL_CFG_POSTDIV_MASK   (0x1F << 14)
#define HDMI_PLL_CFG_ICP(x)         (((x)&0x0F) << 10)
#define HDMI_PLL_CFG_VDDR10_PLL(x)  (((x)&0x03) <<  8)
#define HDMI_PLL_CFG_VDDR_RING(x)   (((x)&0x03) <<  6)
#define HDMI_PLL_CFG_VDDR12_INTP(x) (((x)&0x03) <<  4)
#define HDMI_PLL_CFG_VDDR15_VCO(x)  (((x)&0x03) <<  2)
#define HDMI_PLL_CFG_REGPUMP(x)     (((x)&0x03) <<  0)

#define HDMI_PLL_CFG_1_REG          (HDMI_REG_BASE_ADDR + 0x0024)
#define HDMI_PLL_CFG_TEST_MON(x)    (((x)&0x04) << 25)
#define HDMI_PLL_CFG_EN_PANNEL      (0x01 << 24)
#define HDMI_PLL_CFG_EN_HDMI        (0x01 << 23)
#define HDMI_PLL_CFG_FO_RDY_ADJ     (0x01 << 22)
#define HDMI_PLL_CFG_FO_INNER(x)    (((x)&0x3FFFF) << 4)
#define HDMI_PLL_CFG_FO_INNER_MASK  (0x3FFFF << 4)
#define HDMI_PLL_CFG_MODE(x)        (((x)&0x03) << 2)
#define HDMI_PLL_CFG_EN_FINE_DCC    (0x01 << 1)
#define HDMI_PLL_CFG_EN_DCC         (0x01 << 0)

#define HDMI_PLL_CFG_2_REG          (HDMI_REG_BASE_ADDR + 0x0028)
#define HDMI_PLL_CFG_IPM(x)         (((x)&0x1F) << 24)
#define HDMI_PLL_CFG_FO_ADJ(x)      (((x)&0x3FFFF) << 0)

#define HDMI_PLL_CFG_3_REG          (HDMI_REG_BASE_ADDR + 0x002C)
#define HDMI_PLL_CFG_DCC_CH_RSTL    (0x1F << 23)
#define HDMI_PLL_CFG_LOCK           (0x01 << 22)
#define HDMI_PLL_CFG_DCC_PI_RSTL    (0x1F << 17)
#define HDMI_PLL_CFG_TEST_MODE      (0x01 << 16)
#define HDMI_PLL_CFG_FBDIV(x)       (((x)&0x1FF) << 7)
#define HDMI_PLL_CFG_FBDIV_MASK     (0x1FF << 7)
#define HDMI_PLL_CFG_REFDIV(x)      (((x)&0x1F) <<  2)
#define HDMI_PLL_CFG_REFDIV_MASK    (0x1F <<  2)
#define HDMI_PLL_CFG_RSTB           (0x01 <<  1)
#define HDMI_PLL_CFG_ON             (0x01 <<  0)


/* indirect addresable registers */
static BYTE HdmiReadRegByte( DWORD offs ) {
	WriteRegDword( HDMI_REG_BASE_ADDR+0x04, (offs&0x1FFF)|0x40000000 );
	return (BYTE)ReadRegDword( HDMI_REG_BASE_ADDR+0x00 );
}

static void HdmiWriteRegByte( DWORD offs, DWORD value ) {
	WriteRegDword( HDMI_REG_BASE_ADDR+0x00, value&0xFF );
	WriteRegDword( HDMI_REG_BASE_ADDR+0x04, (offs&0x1FFF)|0x80000000 );
	return;
}

static void HdmiWriteRegByteOr( DWORD offs, DWORD value ) {
	HdmiWriteRegByte( offs, HdmiReadRegByte(offs)|value );
	return;
}

static void HdmiWriteRegByteAnd( DWORD offs, DWORD value ) {
	HdmiWriteRegByte( offs, HdmiReadRegByte(offs)&value );
	return;
}

#define HDMI_ACR_N0_OFFS            (0x0000)
#define HDMI_ACR_N1_OFFS            (0x0001)
#define HDMI_ACR_N2_OFFS            (0x0002)
#define HDMI_ACR_CTS0_OFFS          (0x0004)
#define HDMI_ACR_CTS1_OFFS          (0x0005)
#define HDMI_ACR_CTS2_OFFS          (0x0006)
#define HDMI_ACR_CTRL_OFFS          (0x0007)
 #define MCLK_SEL_128FS              (0 << 2)
 #define MCLK_SEL_256FS              (1 << 2)
 #define MCLK_SEL_384FS              (2 << 2)
 #define CTS_SEL                     (1 << 1)
 #define ACR_EN                      (1 << 0)
#define HDMI_ACR_STS0_OFFS          (0x0008)
#define HDMI_ACR_STS1_OFFS          (0x0009)
#define HDMI_ACR_STS2_OFFS          (0x000A)
#define HDMI_AUD_CTRL_OFFS          (0x000B)
 #define VALIDITY                    (1 << 7)
 #define I2S_DBG_EN                  (1 << 6)
 #define I2S_MODE_SPDIF              (1 << 5)
 #define I2S_CH_EN_PORT_0            (1 << 1)
 #define I2S_CH_EN_PORT_1            (2 << 1)
 #define I2S_CH_EN_PORT_2            (4 << 1)
 #define I2S_CH_EN_PORT_3            (8 << 1)
 #define AUD_EN                      (1 << 0)
#define HDMI_I2S_CTRL_OFFS          (0x000C)
 #define SCK_NEGATIVE_EDGE_LATCH     (1 << 4)
 #define ACTIVE_HIGH_WS_L_CHAN       (1 << 3)
 #define RIGHT_JUSTIFIED             (1 << 2)
 #define LSB_SHIFTED_FIRST           (1 << 1)
 #define NO_CLOCK_DELAY              (1 << 0)
#define HDMI_I2S_DLEN_OFFS          (0x000D)
#define HDMI_I2S_DBG_LFT0_OFFS      (0x0010)
#define HDMI_I2S_DBG_LFT1_OFFS      (0x0011)
#define HDMI_I2S_DBG_LFT2_OFFS      (0x0012)
#define HDMI_I2S_DBG_LFT3_OFFS      (0x0013)
#define HDMI_I2S_DBG_RIT0_OFFS      (0x0014)
#define HDMI_I2S_DBG_RIT1_OFFS      (0x0015)
#define HDMI_I2S_DBG_RIT2_OFFS      (0x0016)
#define HDMI_I2S_DBG_RIT3_OFFS      (0x0017)
#define HDMI_CHSTS_0_OFFS           (0x0018)
#define HDMI_CHSTS_1_OFFS           (0x0019)
#define HDMI_CHSTS_2_OFFS           (0x001A)
#define HDMI_CHSTS_3_OFFS           (0x001B)
#define HDMI_CHSTS_4_OFFS           (0x001C)
#define HDMI_FIFO_CTRL_OFFS         (0x001D)
 #define I_FIFO_RST                  (1 << 1)
 #define FIFO_RST                    (1 << 0)
#define HDMI_MEMSIZE_L_OFFS         (0x001E)
#define HDMI_MEMSIZE_H_OFFS         (0x001F)
#define HDMI_GCP_CFG0_OFFS          (0x0020)
 #define PP_SW_VAL                   (1 << 3)
 #define DEF_PHASE                   (1 << 2)
 #define AVMUTE                      (1 << 1)
 #define GCP_EN                      (1 << 0)
#define HDMI_GCP_CFG1_OFFS          (0x0021)
 #define COL_DEPTH_8_BIT             (4 << 4)
 #define COL_DEPTH_10_BIT            (5 << 4)
 #define COL_DEPTH_12_BIT            (6 << 4)
 #define COL_DEPTH_MASK              (7 << 4)
 #define PP(x)                       (((x)&0x0F) << 0)
#define HDMI_AUD_STS_OFFS           (0x0022)
 #define UNDERFLOW                   (1 << 3)
 #define OVERFLOW                    (1 << 2)
#define HDMI_HTOT_L_OFFS            (0x0024)
#define HDMI_HTOT_H_OFFS            (0x0025)
#define HDMI_HBLANK_L_OFFS          (0x0026)
#define HDMI_HBLANK_H_OFFS          (0x0027)
#define HDMI_VTOT_L_OFFS            (0x0028)
#define HDMI_VTOT_H_OFFS            (0x0029)
#define HDMI_VRES_L_OFFS            (0x002A)
#define HDMI_VRES_H_OFFS            (0x002B)
#define HDMI_VSTART_L_OFFS          (0x002C)
#define HDMI_VSTART_H_OFFS          (0x002D)
#define HDMI_HTOT_STS_L_OFFS        (0x002E)
#define HDMI_HTOT_STS_H_OFFS        (0x002F)
#define HDMI_HBLANK_STS_L_OFFS      (0x0030)
#define HDMI_HBLANK_STS_H_OFFS      (0x0031)
#define HDMI_VTOT_STS_L_OFFS        (0x0032)
#define HDMI_VTOT_STS_H_OFFS        (0x0033)
#define HDMI_VRES_STS_L_OFFS        (0x0034)
#define HDMI_VRES_STS_H_OFFS        (0x0035)
#define HDMI_VSTART_STS_L_OFFS      (0x0036)
#define HDMI_VSTART_STS_H_OFFS      (0x0037)
#define HDMI_VIDEO_STS_OFFS         (0x0038)
 #define INIT_OVER                   (1 << 0)
#define HDMI_VIDEO_CTRL_OFFS        (0x0039)
 #define INT_FRM_SEL                 (1 << 6)
 #define ACR_PRI_SEL_I               (1 << 3)
 #define DEBUG_CTRL                  (1 << 2)
 #define FLD_POL                     (1 << 1)
 #define IN_YC                       (1 << 0)
#define HDMI_CTRL_OFFS              (0x003A)
 #define PIX_RPT(x)                  (((x)&0x07) << 4)
 #define PIX_RPT_MASK                (7 << 4)
 #define BCH_ROT                     (1 << 2)
 #define LAYOUT_8_CHAN_AUDIO(x)      (((x)==0x07) ? (1 << 1) : 0x00)
 #define LAYOUT_8_CHAN_AUDIO_MASK    (1 << 1)
 #define HDMI_MODE(x)                (((x)==0x00000C03) ? (1 << 0) : 0x00)
 #define HDMI_MODE_MASK				 (1 << 0)
#define HDMI_PP_HW_OFFS             (0x0046)
#define HDMI_DC_FIFO_SFT_RST_OFFS   (0x0047)
#define HDMI_DC_FIFO_WR_PTR_OFFS    (0x0048)
#define HDMI_DC_FIFO_RD_PTR_OFFS    (0x0049)
#define HDMI_TDATA0_0_OFFS          (0x004C)
#define HDMI_TDATA0_1_OFFS          (0x004D)
#define HDMI_TDATA0_2_OFFS          (0x004E)
#define HDMI_TDATA1_0_OFFS          (0x0050)
#define HDMI_TDATA1_1_OFFS          (0x0051)
#define HDMI_TDATA1_2_OFFS          (0x0052)
#define HDMI_TDATA2_0_OFFS          (0x0054)
#define HDMI_TDATA2_1_OFFS          (0x0055)
#define HDMI_TDATA2_2_OFFS          (0x0056)
#define HDMI_TDATA3_0_OFFS          (0x0058)
#define HDMI_TDATA3_1_OFFS          (0x0059)
#define HDMI_TDATA3_2_OFFS          (0x005A)
#define HDMI_TDATA_SEL_OFFS         (0x005B)
 #define TDATA_SEL_REG_CHAN_3        (0 << 6)
 #define TDATA_SEL_PRBS_CHAN_3       (1 << 6)
 #define TDATA_SEL_TMDS_CHAN_2       (0 << 4)
 #define TDATA_SEL_REG_CHAN_2        (1 << 4)
 #define TDATA_SEL_PRBS_CHAN_2       (2 << 4)
 #define TDATA_SEL_TMDS_CHAN_1       (0 << 2)
 #define TDATA_SEL_REG_CHAN_1        (1 << 2)
 #define TDATA_SEL_PRBS_CHAN_1       (2 << 2)
 #define TDATA_SEL_TMDS_CHAN_0       (0 << 0)
 #define TDATA_SEL_REG_CHAN_0        (1 << 0)
 #define TDATA_SEL_PRBS_CHAN_0       (2 << 0)
#define HDMI_SWAP_CTRL_OFFS         (0x005C)
 #define CHANNEL_SWAP                (1 << 1)
 #define BIT_SWAP                    (1 << 0)
#define HDMI_AVMUTE_CTRL_OFFS        (0x005D)
#define HDMI_HOST_PKT_CTRL0_OFFS    (0x005E)
 #define PKT5_EN                     (1 << 5)
 #define PKT4_EN                     (1 << 4)
 #define PKT3_EN                     (1 << 3)
 #define PKT2_EN                     (1 << 2)
 #define PKT1_EN                     (1 << 1)
 #define PKT0_EN                     (1 << 0)
#define HDMI_HOST_PKT_CTRL1_OFFS    (0x005F)
 #define PKT5_TX_MODE                (1 << 5)
 #define PKT4_TX_MODE                (1 << 4)
 #define PKT3_TX_MODE                (1 << 3)
 #define PKT2_TX_MODE                (1 << 2)
 #define PKT1_TX_MODE                (1 << 1)
 #define PKT0_TX_MODE                (1 << 0)
#define HDMI_PKT0_BYTE_OFFS(x)      (0x0060+(x))
#define HDMI_PKT1_BYTE_OFFS(x)      (0x0080+(x))
#define HDMI_PKT2_BYTE_OFFS(x)      (0x00A0+(x))
#define HDMI_PKT3_BYTE_OFFS(x)      (0x00C0+(x))
#define HDMI_PKT4_BYTE_OFFS(x)      (0x00E0+(x))
#define HDMI_PKT5_BYTE_OFFS(x)      (0x0100+(x))
#define HDMI_UBITS_0_OFFS           (0x0120)
#define HDMI_UBITS_1_OFFS           (0x0121)
#define HDMI_UBITS_2_OFFS           (0x0122)
#define HDMI_UBITS_3_OFFS           (0x0123)
#define HDMI_UBITS_4_OFFS           (0x0124)
#define HDMI_UBITS_5_OFFS           (0x0125)
#define HDMI_UBITS_6_OFFS           (0x0126)
#define HDMI_UBITS_7_OFFS           (0x0127)
#define HDMI_UBITS_8_OFFS           (0x0128)
#define HDMI_UBITS_9_OFFS           (0x0129)
#define HDMI_UBITS_10_OFFS          (0x012A)
#define HDMI_UBITS_11_OFFS          (0x012B)
#define HDMI_UBITS_12_OFFS          (0x012C)
#define HDMI_UBITS_13_OFFS          (0x012D)
#define HDMI_HBR_PKT_OFFS           (0x012E)
#define HDMI_PHY_FIFO_SOFT_RST_OFFS (0x0130)
#define HDMI_PHY_FIFO_PTRS_OFFS     (0x0131)
 #define FIFO_RD_PTRS(x)             (((x)&0x0F) << 4)
 #define FIFO_WR_PTRS(x)             (((x)&0x0F) << 0)
#define HDMI_PRBS_CTRL0_OFFS        (0x0132)
 #define PRBS_EN                     (1 << 2)
 #define TYPE_SEL_PRBS7              (0 << 0)
 #define TYPE_SEL_PRBS23             (1 << 0)
 #define TYPE_SEL_PRBS31             (2 << 0)
#define HDMI_HOST_PKT_CTRL2_OFFS    (0x0133)
#define HDMI_HOST_PKT_START_OFFS    (0x0134)
#define HDMI_AUD_CH1_SEL_OFFS       (0x0137)
#define HDMI_AUD_CH2_SEL_OFFS       (0x0138)
#define HDMI_AUD_CH3_SEL_OFFS       (0x0139)
#define HDMI_AUD_CH4_SEL_OFFS       (0x013A)
#define HDMI_AUD_CH5_SEL_OFFS       (0x013B)
#define HDMI_AUD_CH6_SEL_OFFS       (0x013C)
#define HDMI_AUD_CH7_SEL_OFFS       (0x013D)
#define HDMI_AUD_CH8_SEL_OFFS       (0x013E)
#define HDMI_HDCP_AKEY_BYTE0_OFFS(x) (0x1200+((x)*8))
#define HDMI_HDCP_AKEY_BYTE1_OFFS(x) (0x1201+((x)*8))
#define HDMI_HDCP_AKEY_BYTE2_OFFS(x) (0x1202+((x)*8))
#define HDMI_HDCP_AKEY_BYTE3_OFFS(x) (0x1203+((x)*8))
#define HDMI_HDCP_AKEY_BYTE4_OFFS(x) (0x1204+((x)*8))
#define HDMI_HDCP_AKEY_BYTE5_OFFS(x) (0x1205+((x)*8))
#define HDMI_HDCP_AKEY_BYTE6_OFFS(x) (0x1206+((x)*8))
#define HDMI_HDCP_AKSV_BYTE_0_OFFS  (0x1340)
#define HDMI_HDCP_AKSV_BYTE_1_OFFS  (0x1341)
#define HDMI_HDCP_AKSV_BYTE_2_OFFS  (0x1342)
#define HDMI_HDCP_AKSV_BYTE_3_OFFS  (0x1343)
#define HDMI_HDCP_AKSV_BYTE_4_OFFS  (0x1344)
#define HDMI_HDCP_CTRL_OFFS         (0x1350)
 #define HDMI_HDCP_MODE              (1 << 7)
 #define EESS_EN                     (1 << 6)
 #define REPEATER                    (1 << 5)
 #define ADVANCE_CIPHER              (1 << 4)
 #define ENH_LNK_VERIFICATION        (1 << 3)
 #define CIPHER_EN                   (1 << 2)
 #define READ_AKSV                   (1 << 1)
#define HDMI_HDCP_STATUS_1_OFFS     (0x1351)
 #define BKSV_READY                  (1 << 7)
 #define R0_AUTH_PASS                (1 << 6)
 #define KSV_LIST_CHECK_PASS         (1 << 5)
 #define ERR                         (1 << 4)
 #define FW_ENC_EN                   (1 << 3)
 #define READY_KEYS                  (1 << 2)
 #define AKSV_SENT                   (1 << 1)
#define HDMI_HDCP_STATUS_2_OFFS     (0x1352)
 #define VIDEO_MODE_DEFAULT          (0 << 6)
 #define VIDEO_MODE_ENCRYPTED        (1 << 6)
 #define VIDEO_MODE_UNENCRYPTED      (2 << 6)
#define HDMI_HDCP_INTR_0_OFFS       (0x1353)
 #define AKSV_READY_STATUS           (1 << 7)
 #define TX_AN_READY_STATUS          (1 << 6)
 #define TX_R0_READY_STATUS          (1 << 5)
 #define TX_RI_READY_STATUS          (1 << 4)
 #define TX_PJ_READY_STATUS          (1 << 3)
#define HDMI_HDCP_TX_AKSV_0_OFFS    (0x1354)
#define HDMI_HDCP_TX_AKSV_1_OFFS    (0x1355)
#define HDMI_HDCP_TX_AKSV_2_OFFS    (0x1356)
#define HDMI_HDCP_TX_AKSV_3_OFFS    (0x1357)
#define HDMI_HDCP_TX_AKSV_4_OFFS    (0x1358)
#define HDMI_HDCP_TX_BKSV_0_OFFS    (0x135C)
#define HDMI_HDCP_TX_BKSV_1_OFFS    (0x135D)
#define HDMI_HDCP_TX_BKSV_2_OFFS    (0x135E)
#define HDMI_HDCP_TX_BKSV_3_OFFS    (0x135F)
#define HDMI_HDCP_TX_BKSV_4_OFFS    (0x1360)
#define HDMI_HDCP_TX_AINFO_OFFS     (0x1361)
 #define ENABLE_1_1_FEATURES        (1 << 1)
#define HDMI_HDCP_RX_BCAPS_OFFS     (0x1362)
 #define HDCP_REPEATER_CAP           (1 << 6)
 #define KSV_FIFO_READY              (1 << 5)
 #define FAST                        (1 << 4)
 #define ONE_ONE_FEATURES            (1 << 1)
 #define FAST_REAUTHENTICATION       (1 << 0)
#define HDMI_HDCP_RX_BSTATUS_0_OFFS (0x1364)
 #define HDMI_MODE_RX                (1 << 4)
 #define TOPOLOGY_ERR_CASCADE        (1 << 3)
 #define REPEATER_CASCADE_DEPTH(x)   (((x)&0x07) << 0)
#define HDMI_HDCP_RX_BSTATUS_1_OFFS (0x1365)
 #define TOPOLOGY_ERR_ATTACH         (1 << 7)
 #define DOWNSTREAM_DEVICES_NUM(x)   (((x)&0x7F) << 0)
#define HDMI_HDCP_TX_AN_0_OFFS      (0x1368)
#define HDMI_HDCP_TX_AN_1_OFFS      (0x1369)
#define HDMI_HDCP_TX_AN_2_OFFS      (0x136A)
#define HDMI_HDCP_TX_AN_3_OFFS      (0x136B)
#define HDMI_HDCP_TX_AN_4_OFFS      (0x136C)
#define HDMI_HDCP_TX_AN_5_OFFS      (0x136D)
#define HDMI_HDCP_TX_AN_6_OFFS      (0x136E)
#define HDMI_HDCP_TX_AN_7_OFFS      (0x136F)
#define HDMI_HDCP_TX_M0_0_OFFS      (0x1370)
#define HDMI_HDCP_TX_M0_1_OFFS      (0x1371)
#define HDMI_HDCP_TX_M0_2_OFFS      (0x1372)
#define HDMI_HDCP_TX_M0_3_OFFS      (0x1373)
#define HDMI_HDCP_TX_M0_4_OFFS      (0x1374)
#define HDMI_HDCP_TX_M0_5_OFFS      (0x1375)
#define HDMI_HDCP_TX_M0_6_OFFS      (0x1376)
#define HDMI_HDCP_TX_M0_7_OFFS      (0x1377)
#define HDMI_HDCP_TX_R0_0_OFFS      (0x1378)
#define HDMI_HDCP_TX_R0_1_OFFS      (0x1379)
#define HDMI_HDCP_RX_R0_0_OFFS      (0x137A)
#define HDMI_HDCP_RX_R0_1_OFFS      (0x137B)
#define HDMI_HDCP_TX_RI_0_OFFS      (0x137C)
#define HDMI_HDCP_TX_RI_1_OFFS      (0x137D)
#define HDMI_HDCP_RX_RI_0_OFFS      (0x137E)
#define HDMI_HDCP_RX_RI_1_OFFS      (0x137F)
#define HDMI_HDCP_TX_PJ_OFFS        (0x1380)
#define HDMI_HDCP_RX_PJ_OFFS        (0x1381)
#define HDMI_HDCP_FIX_CLR_0_OFFS    (0x1384)
#define HDMI_HDCP_FIX_CLR_1_OFFS    (0x1385)
#define HDMI_HDCP_FIX_CLR_2_OFFS    (0x1386)
#define HDMI_HDCP_KINIT_0_OFFS      (0x1388)
#define HDMI_HDCP_KINIT_1_OFFS      (0x1389)
#define HDMI_HDCP_KINIT_2_OFFS      (0x138A)
#define HDMI_HDCP_KINIT_3_OFFS      (0x138B)
#define HDMI_HDCP_KINIT_4_OFFS      (0x138C)
#define HDMI_HDCP_KINIT_5_OFFS      (0x138D)
#define HDMI_HDCP_KINIT_6_OFFS      (0x138E)
#define HDMI_HDCP_BINIT_0_OFFS      (0x1390)
#define HDMI_HDCP_BINIT_1_OFFS      (0x1391)
#define HDMI_HDCP_BINIT_2_OFFS      (0x1392)
#define HDMI_HDCP_BINIT_3_OFFS      (0x1393)
#define HDMI_HDCP_BINIT_4_OFFS      (0x1394)
#define HDMI_HDCP_BINIT_5_OFFS      (0x1395)
#define HDMI_HDCP_BINIT_6_OFFS      (0x1396)
#define HDMI_HDCP_BINIT_7_OFFS      (0x1397)
#define HDMI_HDCP_INTR_CLR_OFFS     (0x1398)
 #define AKSV_READY_CLR              (1 << 7)
 #define TX_AN_READY_CLR             (1 << 6)
 #define TX_R0_READY_CLR             (1 << 5)
 #define TX_RI_READY_CLR             (1 << 4)
 #define TX_PJ_READY_CLR             (1 << 3)
#define HDMI_HDCP_INTR_MASK_OFFS    (0x1399)
 #define AKSV_READY_MASK             (1 << 7)
 #define TX_AN_READY_MASK            (1 << 6)
 #define TX_R0_READY_MASK            (1 << 5)
 #define TX_RI_READY_MASK            (1 << 4)
 #define TX_PJ_READY_MASK            (1 << 3)

/*
 ***************************************************
 ***************************************************
 *
 * Defines for other HDMI-related subsystems
 *
 ***************************************************
 ***************************************************
 */

#define GRAPHICS_MODE				0
#define VIDEO_MODE					1

#define DISP_REG_BASE_ADDR          0xD420B000

#define LCD_TVD_START_ADDR_Y0_REG	(DISP_REG_BASE_ADDR + 0x0000)
#define LCD_TVD_START_ADDR_U0_REG	(DISP_REG_BASE_ADDR + 0x0004)
#define LCD_TVD_START_ADDR_V0_REG	(DISP_REG_BASE_ADDR + 0x0008)
#define LCD_TVD_START_ADDR_C0_REG	(DISP_REG_BASE_ADDR + 0x000C)
#define LCD_TVD_START_ADDR_Y1_REG	(DISP_REG_BASE_ADDR + 0x0010)
#define LCD_TVD_START_ADDR_U1_REG	(DISP_REG_BASE_ADDR + 0x0014)
#define LCD_TVD_START_ADDR_V1_REG	(DISP_REG_BASE_ADDR + 0x0018)
#define LCD_TVD_START_ADDR_C1_REG	(DISP_REG_BASE_ADDR + 0x001C)
#define LCD_TVD_PITCH_YC_REG		(DISP_REG_BASE_ADDR + 0x0020)
#define LCD_TVD_PITCH_UV_REG		(DISP_REG_BASE_ADDR + 0x0024)
#define LCD_TVD_OVSA_HPXL_VLN_REG	(DISP_REG_BASE_ADDR + 0x0028)
#define LCD_TVD_HPXL_VLN_REG		(DISP_REG_BASE_ADDR + 0x002C)
#define LCD_TVDZM_HPXL_VLN_REG		(DISP_REG_BASE_ADDR + 0x0030)
#define LCD_TVG_START_ADDR0_REG		(DISP_REG_BASE_ADDR + 0x0034)
#define LCD_TVG_START_ADDR1_REG		(DISP_REG_BASE_ADDR + 0x0038)
#define LCD_TVG_PITCH_REG			(DISP_REG_BASE_ADDR + 0x003C)
#define LCD_TVG_OVSA_HPXL_VLN_REG	(DISP_REG_BASE_ADDR + 0x0040)
#define LCD_TVG_HPXL_VLN_REG		(DISP_REG_BASE_ADDR + 0x0044)
#define LCD_TVGZM_HPXL_VLN_REG		(DISP_REG_BASE_ADDR + 0x0048)
#define LCD_TV_V_H_TOTAL_REG		(DISP_REG_BASE_ADDR + 0x0054)
#define LCD_TV_V_H_ACTIVE_REG		(DISP_REG_BASE_ADDR + 0x0058)
#define LCD_TV_H_PORCH_REG			(DISP_REG_BASE_ADDR + 0x005C)
#define LCD_TV_V_PORCH_REG			(DISP_REG_BASE_ADDR + 0x0060)
#define LCD_TV_BLANKCOLOR_REG		(DISP_REG_BASE_ADDR + 0x0064)
#define LCD_TV_COLORKEY_Y_REG		(DISP_REG_BASE_ADDR + 0x0070)
#define LCD_TV_COLORKEY_U_REG		(DISP_REG_BASE_ADDR + 0x0074)
#define LCD_TV_COLORKEY_V_REG		(DISP_REG_BASE_ADDR + 0x0078)
#define LCD_TV_SEPXLCNT_REG			(DISP_REG_BASE_ADDR + 0x007C)

#define LCD_TV_CTRL0_REG			(DISP_REG_BASE_ADDR + 0x0080)
#define CFG_TVDFORMAT(x)			(((((x)>0xB)?0:(x))&(0xF))<<20)
#define CFG_TVDFORMAT_MASK			(0xF<<20)
#define CFG_TVGFORMAT(x)			(((((((x)>0x5)&&((x)<0x9))||((x)>0xB))?0:(x))&(0xF))<<16)
#define CFG_TVGFORMAT_MASK			(0xF<<16)
#define CFG_TVG_HSMOOTH				(0x1<<14)
#define CFG_YUV2RGB_TVG				(0x1<<9)
#define CFG_TVG_ENA					(0x1<<8)
#define CFG_TVD_HSMOOTH				(0x1<<6)
#define CFG_YUV2RGB_TVD				(0x1<<1)
#define CFG_TVD_ENA					(0x1<<0)

#define LCD_TV_CTRL1_REG			(DISP_REG_BASE_ADDR + 0x0084)
#define CFG_TV_SYNC_MODE(x)			(((x)&0x7)<<28)
#define CFG_TV_SYNC_MODE_MASK		(0x70000000)
#define CFG_TV_INTERLACE_EN			(0x1 << 22)
#define CFG_TV_ALPHA_MODE(x)		(((x)&0x3)<<16)
#define CFG_TV_ALPHA_MODE_MASK		(0x00030000)
#define CFG_TV_ALPHA(x)		        (((x)&0xFF)<<8)
#define CFG_TV_ALPHA_MASK		    (0x0000FF00)
#define CFG_TV_NIB					(0x1 << 0)

#define LCD_TV_CONTRAST_REG			(DISP_REG_BASE_ADDR + 0x0088)
#define LCD_TV_SATURATION_REG		(DISP_REG_BASE_ADDR + 0x008C)
#define LCD_TV_CBSH_HUE_REG			(DISP_REG_BASE_ADDR + 0x0090)

#define LCD_TVIF_CTRL_REG			(DISP_REG_BASE_ADDR + 0x0094)
#define CFG_HDMIMODE_RGB888			(0x0 << 24)
#define CFG_HDMIMODE_MASK			(0x0F000000)
#define CFG_TVIF_ENA				(0x1 << 0)

#define LCD_TVIOPAD_CTRL_REG		(DISP_REG_BASE_ADDR + 0x0098)

#define LCD_TCLK_DIV_REG			(DISP_REG_BASE_ADDR + 0x009C)
#define TCLK_SOURCE_HDMI_PLL		(0xC0000000)
#define TCLK_SOURCE_MASK		    (0xC0000000)
#define HDMI_TCLKNX_DIV(x)  		(((x)&0xFF) << 16)
#define HDMI_TCLKNX_DIV_MASK		(0xFF << 16)
#define CFG_TCLK_DISABLE			(1 << 28)
#define CLK_TV_INT_DIV(x)			(((x)&0xFF) << 0)
#define CLK_TV_INT_DIV_MASK			(0xFF << 0)

#define LCD_DMA_START_ADDR_Y0_REG	(DISP_REG_BASE_ADDR + 0x00C0)
#define LCD_DMA_START_ADDR_U0_REG	(DISP_REG_BASE_ADDR + 0x00C4)
#define LCD_DMA_START_ADDR_V0_REG	(DISP_REG_BASE_ADDR + 0x00C8)
#define LCD_DMA_START_ADDR_C0_REG	(DISP_REG_BASE_ADDR + 0x00CC)
#define LCD_DMA_START_ADDR_Y1_REG	(DISP_REG_BASE_ADDR + 0x00D0)
#define LCD_DMA_START_ADDR_U1_REG	(DISP_REG_BASE_ADDR + 0x00D4)
#define LCD_DMA_START_ADDR_V1_REG	(DISP_REG_BASE_ADDR + 0x00D8)
#define LCD_DMA_START_ADDR_C1_REG	(DISP_REG_BASE_ADDR + 0x00DC)
#define LCD_DMA_PITCH_YC_REG		(DISP_REG_BASE_ADDR + 0x00E0)
#define LCD_DMA_PITCH_UV_REG		(DISP_REG_BASE_ADDR + 0x00E4)
#define LCD_DMA_OVSA_HPXL_VLN_REG	(DISP_REG_BASE_ADDR + 0x00E8)
#define LCD_DMA_HPXL_VLN_REG		(DISP_REG_BASE_ADDR + 0x00EC)
#define LCD_DMAZM_HPXL_VLN_REG		(DISP_REG_BASE_ADDR + 0x00F0)
#define LCD_GRA_START_ADDR0_REG		(DISP_REG_BASE_ADDR + 0x00F4)
#define LCD_GRA_START_ADDR1_REG		(DISP_REG_BASE_ADDR + 0x00F8)
#define LCD_GRA_PITCH_REG			(DISP_REG_BASE_ADDR + 0x00FC)
#define LCD_GRA_OVSA_HPXL_VLN_REG	(DISP_REG_BASE_ADDR + 0x0100)
#define LCD_GRA_HPXL_VLN_REG		(DISP_REG_BASE_ADDR + 0x0104)
#define LCD_GRAZM_HPXL_VLN_REG		(DISP_REG_BASE_ADDR + 0x0108)
#define LCD_PN_V_H_TOTAL_REG		(DISP_REG_BASE_ADDR + 0x0114)
#define LCD_PN_V_H_ACTIVE_REG		(DISP_REG_BASE_ADDR + 0x0118)
#define LCD_PN_H_PORCH_REG			(DISP_REG_BASE_ADDR + 0x011C)
#define LCD_PN_V_PORCH_REG			(DISP_REG_BASE_ADDR + 0x0120)
#define LCD_PN_BLANKCOLOR_REG		(DISP_REG_BASE_ADDR + 0x0124)
#define LCD_PN_COLORKEY_Y_REG		(DISP_REG_BASE_ADDR + 0x0130)
#define LCD_PN_COLORKEY_U_REG		(DISP_REG_BASE_ADDR + 0x0134)
#define LCD_PN_COLORKEY_V_REG		(DISP_REG_BASE_ADDR + 0x0138)
#define LCD_PN_SETPXLCNT_REG		(DISP_REG_BASE_ADDR + 0x013C)

#define LCD_SMPN_CTRL_REG			(DISP_REG_BASE_ADDR + 0x0188)

#define LCD_PN_CTRL0_REG			(DISP_REG_BASE_ADDR + 0x0190)
#define LCD_PN_CTRL1_REG			(DISP_REG_BASE_ADDR + 0x0194)
#define LCD_SRAM_CTRL_REG			(DISP_REG_BASE_ADDR + 0x0198)
#define LCD_SRAM_WRDAT_REG			(DISP_REG_BASE_ADDR + 0x019C)
#define LCD_SRAM_PARA0_REG			(DISP_REG_BASE_ADDR + 0x01A0)
#define LCD_SRAM_PARA1_REG			(DISP_REG_BASE_ADDR + 0x01A4)
#define LCD_SCLK_DIV_REG			(DISP_REG_BASE_ADDR + 0x01A8)
#define LCD_PN_CONTRAST_REG			(DISP_REG_BASE_ADDR + 0x01AC)
#define LCD_PN_SATURATION_REG		(DISP_REG_BASE_ADDR + 0x01B0)
#define LCD_PN_CBSH_HUE_REG			(DISP_REG_BASE_ADDR + 0x01B4)
#define LCD_DUMB_CTRL_REG			(DISP_REG_BASE_ADDR + 0x01B8)
#define PN_IOPAD_CTRL_REG			(DISP_REG_BASE_ADDR + 0x01BC)

#define SPU_IRQ_ENA_REG				(DISP_REG_BASE_ADDR + 0x01C0)
#define DUMB_FRAMEDONE_ENA			(1 << 22)
#define TVIF_FRAMEDONE_ENA			(1 << 8)

#define SPU_IRQ_ISR_REG				(DISP_REG_BASE_ADDR + 0x01C4)
#define DUMB_FRAMEDONE				(1 << 22)
#define TVIF_FRAMEDONE				(1 << 8)

#define LCD_TOP_CTRL_REG			(DISP_REG_BASE_ADDR + 0x01DC)
#define CFG_TVG_4_BURSTS			(3 << 14)
#define CFG_TVD_4_BURSTS			(3 << 12)
#define CFG_CMU_TV_PATH				(1 << 1)
#define CFG_CMU_ENA					(1 << 0)

#define LCD_AFA_ALL2ONE_REG			(DISP_REG_BASE_ADDR + 0x01E8)
#define CFG_TVG_2ZMDN				(1 << 23)
#define CFG_TVD_2ZMDN				(1 << 22)
#define CFG_VSMTH_TVG				(0x3 << 18)
#define CFG_VSMTH_TVD				(0x3 << 16)

#define LCD_TV_V_H_TOTAL_FLD_REG	(DISP_REG_BASE_ADDR + 0x0280)
#define LCD_TV_V_PORCH_FLD_REG		(DISP_REG_BASE_ADDR + 0x0284)
#define LCD_TV_SEPXLCNT_FLD_REG		(DISP_REG_BASE_ADDR + 0x0288)

#define CMU_CLK_CTRL_REG			0xD420B488
#define CMU_CLK_TCLK			    (0 << 1)
#define CMU_CLK_SCLK			    (1 << 1)
#define CMU_CLK_PN2_SCLK		    (2 << 1)
#define CMU_CLK_HDMI_PLL			(3 << 1)
#define CMU_CLK_EN					(1 << 0)

#define PMUM_CGR_SP_REG				0xD4050024
#define PMUM_I2SCLK1_EN				(1 << 5)

#define PMUM_ISSCR1_REG				0xD4050040
#define PMUM_SYSCLK_EN				(1u << 31)
#define PMUM_SYSCLK_PLL1DIV2		(1 << 30)
#define PMUM_DENOM(x)				(((x)&0xFFF) << 15)
#define PMUM_DENOM_MASK				(0xFFF << 15)
#define PMUM_NOM(x)					(((x)&0x7FFF) << 0)
#define PMUM_NOM_MASK				(0x7FFF << 0)

#define PMUA_DISP1_CTRL_REG			0xD428284C
#define HDMI_I2S_CLK_EN				(1 << 14)
#define HDMI_REF_CLK_EN				(1 << 13)
#define DISP1_CLK_EN				(1 << 4)
#define DISP_AXI_CLK_EN				(1 << 3)
#define DISP1_CLK_RST				(1 << 1)
#define DISP_AXI_CLK_RST			(1 << 0)

#define PMUA_DISP2_CTRL_REG			0xD4282910
#define DISP2_CLK_EN				(1 << 4)
#define DISP2_CLK_RST				(1 << 1)

#define PMUA_AUDIO_CTRL_REG			0xD428290C
#define AUDIO_PWR_HW_MODE			(0x1 << 11)
#define AUDIO_PWR_UP				(0x3 << 9)
#define AUDIO_PWR_MASK				(0x3 << 9)
#define AUDIO_ISB_DIS				(0x1 << 8)
#define AUDIO_CLK_PLL1DIV2			(0x0 << 6)
#define AUDIO_CLK_PLL2DIV2			(0x1 << 6)
#define AUDIO_CLK_PLL2DIV3			(0x2 << 6)
#define AUDIO_CLK_PLL1DIV8			(0x3 << 6)
#define AUDIO_CLK_MASK				(0x3 << 6)
#define AUDIO_CLK_EN				(0x1 << 4)
#define AUDIO_CLK_RST				(0x1 << 1)

#define ADMA_CHAN_0_NEXT_PTR_REG	0xD42A0830
#define ADMA_CHAN_1_NEXT_PTR_REG	0xD42A0834


#define ADMA_CHAN_0_CTRL_REG		0xD42A0840
#define ADMA_CHAN_1_CTRL_REG		0xD42A0844
#define ADMA_SSZ(x)					((((x)&0x7)+1) << 22)
#define ADMA_SSZ_MASK				(0x7 << 22)
#define ADMA_ABR					(0x1 << 20)
#define ADMA_CDE					(0x1 << 17)
#define ADMA_PACKMOD				(0x1 << 16)
#define ADMA_CHANACT				(0x1 << 14)
#define ADMA_FETCHND				(0x1 << 13)
#define ADMA_CHANEN					(0x1 << 12)
#define ADMA_INTMODE				(0x1 << 10)
#define ADMA_NONCHAINMOD			(0x1 << 9)
#define ADMA_BURSTLIMIT_4BYTES		(0x0 << 6)
#define ADMA_BURSTLIMIT_MASK		(0x7 << 6)
#define ADMA_DESTDIR_INC			(0x0 << 4)
#define ADMA_DESTDIR_HOLD			(0x2 << 4)
#define ADMA_DESTDIR_MASK			(0x3 << 4)
#define ADMA_SRCDIR_INC				(0x0 << 2)
#define ADMA_SRCDIR_HOLD			(0x2 << 2)
#define ADMA_SRCDIR_MASK			(0x3 << 2)

#define ADMA_CHAN_0_INT_MASK_REG	0xD42A0880
#define ADMA_CHAN_1_INT_MASK_REG	0xD42A0884
#define ADMA_CHAN_0_INT_STATUS_REG	0xD42A08A0
#define ADMA_CHAN_1_INT_STATUS_REG	0xD42A08A4
#define ADMA_COMPLETE				(0x1 << 0)

#define SSPA_RX_DATA_REG			0xD42A0C00
#define SSPA_RX_ID_REG				0xD42A0C04

#define SSPA_RX_CTRL_REG			0xD42A0C08
#define SSPA_DATDLY_1BIT			(0x1 << 19)
#define SSPA_FRLEN(x)				((((x)&0x7F)-1) << 8)
#define SSPA_FRLEN_MASK				(0x7F << 8)
#define SSPA_WDLEN_32BITS			(0x5 << 5)
#define SSPA_JST_RIGHT				(0x1 << 3)
#define SSPA_SSZ(x)					((((x)&0x7)+1) << 0)
#define SSPA_SSZ_MASK				(0x7 << 0)

#define SSPA_RX_SP_CTRL_REG			0xD42A0C0C
#define SSPA_WEN					(0x1u << 31)
#define SSPA_FWID(x)				((((x)&0xFF)-1) << 20)
#define SSPA_FWID_MASK				(0xFF << 20)
#define SSPA_MSL_INTERNAL			(0x1 << 18)
#define SSPA_FPER(x)				((((x)&0xFFF)-1) << 4)
#define SSPA_FPER_MASK				(0xFFF << 4)
#define SSPA_FFLUSH					(0x1 << 2)
#define SSPA_S_RST					(0x1 << 1)
#define SSPA_S_EN					(0x1 << 0)

#define SSPA_RX_FIFO_UL_REG			0xD42A0C10

#define SSPA_RX_INT_MSK_REG			0xD42A0C14
#define SSPA_FF_INT					(0x1 << 16)
#define SSPA_FF_INT_MASK			(0x1 << 0)

#define SSPA_RX_CORE_REG			0xD42A0C18
#define SSPA_RX_FIFO_NOFS_REG		0xD42A0C1C
#define SSPA_RX_FIFO_SIZE_REG		0xD42A0C20

#define SSPA_AUD_CTRL0_REG			0xD42A0C34
#define SSPA2_CLK_I2S2				(0x1 << 23)
#define SSPA2_CLK_DIV_PREE(x)		(((x)&0x3F) << 17)
#define SSPA2_CLK_DIV_PREE_MASK		(0x3F << 17)
#define SSPA2_CLK_DIV_EN			(0x1 << 16)
#define SSPA1_CLK_DIV2_PREE(x)		(((x)&0x3F) << 9)
#define SSPA1_CLK_DIV2_PREE_MASK	(0x3F << 9)
#define SSPA1_CLK_DIV2_PREE_SHIFT	9
#define SSPA1_CLK_DIV2_EN			(0x1 << 8)
#define SSPA1_CLK_I2S1				(0x1 << 7)
#define SSPA1_CLK_DIV1_PREE(x)		(((x)&0x3F) << 1)
#define SSPA1_CLK_DIV1_PREE_MASK	(0x3F << 1)
#define SSPA1_CLK_DIV1_PREE_SHIFT	1
#define SSPA1_CLK_DIV1_EN			(0x1 << 0)

#define SSPA_AUD_PLL_CTRL0_REG		0xD42A0C38
#define DIV_OCLK_MODULO(x)			(((x)&0x7) << 28)
#define DIV_OCLK_MODULO_MASK		(0x7 << 28)
#define FRACT(x)					(((x)&0xFFFFF) << 8)
#define FRACT_MASK					(0xFFFFF << 8)
#define ENA_DITHER					(0x1 << 7)
#define ICP(x)						(((x)&0x3) << 5)
#define ICP_MASK					(0x3 << 5)
#define DIV_FBCCLK(x)				(((x)&0x3) << 3)
#define DIV_FBCCLK_MASK				(0x3 << 3)
#define MCLKDIV10					(0x1 << 2)
#define PD_OVPROT_DIS				(0x1 << 1)
#define PLL_PWR_UP					(0x1 << 0)

#define SSPA_AUD_PLL_CTRL1_REG		0xD42A0C3C
#define AUD_PLL_LOCK				(0x1 << 16)
#define AUD_CLK_SEL					(0x1 << 11)
#define DIV_OCLK_PATTERN(x)			(((x)&0x3) << 0)
#define DIV_OCLK_PATTERN_MASK		(0x3 << 0)

#define SSPA_TX_DATA_REG			0xD42A0C80
#define SSPA_TX_ID_REG				0xD42A0C84
#define SSPA_TX_CTRL_REG			0xD42A0C88
#define SSPA_TX_SP_CTRL_REG			0xD42A0C8C
#define SSPA_TX_FIFO_LL_REG			0xD42A0C90
#define SSPA_TX_INT_MSK_REG			0xD42A0C94
#define SSPA_TX_CORE_REG			0xD42A0C98
#define SSPA_TX_FIFO_NOFS_REG		0xD42A0C9C
#define SSPA_TX_FIFO_SIZE_REG		0xD42A0CA0

#define ICU_DMA_IRQ1_MSK_REG		0xD428211C
#define ICU_DMA_IRQ1_STATUS_REG		0xD4282128

#endif // __HDMI_MMP_H__
