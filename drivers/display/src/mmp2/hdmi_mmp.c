/*****************************************************************************
 * Filename:    hdmi_mmp.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     March 2010
 *
 * Description: The MMP specific code of the hdmi driver. Developed
 *              specifically for the platform software driver framework.
 *
 *****************************************************************************/

#include "rei.h"
#include "framework.h"
#include "hdmi_internal.h"
#include "hdmi_mmp.h"

#include "gpio_platform.h"

/*
 ***************************************************
 ***************************************************
 *
 * Macro definitions
 *
 ***************************************************
 ***************************************************
 */

#define SRCGROUP 			GROUP_PLT_FRMWRK
#define SRCFILE  			FILE_DISP_HDMI_HW

#define VEN_PKT_EN			PKT0_EN
#define AVI_PKT_EN			PKT1_EN
#define SPD_PKT_EN			PKT2_EN
#define AUD_PKT_EN			PKT3_EN
#define MPG_PKT_EN			PKT4_EN

#define OVLY_TV_START_ADDR0	0x01800000
#define OVLY_TV_START_ADDR1	0x02000000

/*
 ***************************************************
 ***************************************************
 *
 * Enum types and data structure definitions
 *
 ***************************************************
 ***************************************************
 */

/*
 ***************************************************
 ***************************************************
 *
 * Static global variable declarations
 *
 ***************************************************
 ***************************************************
 */

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

static void HdmiTxHwConfigPll(
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

	/* pll configuration completed */
	return;
}

static void HdmiTxHwConfigAudio(
	AudioChannel_t channel_count,
	SampleFreq_t sample_freq,
	SampleSize_t sample_size)
{
    /* configure audio registers based on number of audio channels */
    switch( channel_count ) {
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

    /* configure audio registers based on audio bits per channel */
    switch( sample_size ) {
        case SS_20BIT:
//            WriteRegDwordAnd( HDMI_AUDIO_CFG_REG, ~XWDLEN_MASK );
//            WriteRegDwordOr( HDMI_AUDIO_CFG_REG, XWDLEN_20_BIT );
            HdmiWriteRegByte( HDMI_I2S_DLEN_OFFS, 20 );
            HdmiWriteRegByte( HDMI_CHSTS_4_OFFS, 0x03 );
            break;
        case SS_24BIT:
//            WriteRegDwordAnd( HDMI_AUDIO_CFG_REG, ~XWDLEN_MASK );
//            WriteRegDwordOr( HDMI_AUDIO_CFG_REG, XWDLEN_24_BIT );
            HdmiWriteRegByte( HDMI_I2S_DLEN_OFFS, 24 );
            HdmiWriteRegByte( HDMI_CHSTS_4_OFFS, 0x0B );
            break;
        case SS_16BIT:
        default:
//            WriteRegDwordAnd( HDMI_AUDIO_CFG_REG, ~XWDLEN_MASK );
//            WriteRegDwordOr( HDMI_AUDIO_CFG_REG, XWDLEN_16_BIT );
            HdmiWriteRegByte( HDMI_I2S_DLEN_OFFS, 16 );
            HdmiWriteRegByte( HDMI_CHSTS_4_OFFS, 0x02 );
            break;
    }

    /* configure audio registers based on audio sampling rate */
    switch( sample_freq ) {
        case SF_44P1KHZ:
            HdmiWriteRegByte( HDMI_CHSTS_3_OFFS, 0x00 );
            break;
        case SF_48KHZ:
            HdmiWriteRegByte( HDMI_CHSTS_3_OFFS, 0x02 );
            break;
        case SF_88P2KHZ:
            HdmiWriteRegByte( HDMI_CHSTS_3_OFFS, 0x08 );
            break;
        case SF_96KHZ:
            HdmiWriteRegByte( HDMI_CHSTS_3_OFFS, 0x0A );
            break;
        case SF_176P4KHZ:
            HdmiWriteRegByte( HDMI_CHSTS_3_OFFS, 0x0C );
            break;
        case SF_192KHZ:
            HdmiWriteRegByte( HDMI_CHSTS_3_OFFS, 0x0E );
            break;
        case SF_32KHZ:
        default:
            HdmiWriteRegByte( HDMI_CHSTS_3_OFFS, 0x03 );
            break;
    }

    /* use validity bit during audio packetization */
    HdmiWriteRegByteOr( HDMI_AUD_CTRL_OFFS, VALIDITY );

    return;
}

static void HdmiTxHwConfigVideo(
	ColorDepth_t color_depth,
	ColorIndicator_t color_indicator,
	BOOL interleave_mode,
	WORD h_active,
	WORD h_front,
	WORD h_sync,
	WORD h_back,
	WORD v_active,
	WORD v_front,
	WORD v_sync,
	WORD v_back)
{
	WORD h_blank = h_front + h_sync + h_back;
	WORD v_blank = v_front + v_sync + v_back;

    /* disable the internal logic auto detection of the video format timings */
    HdmiWriteRegByteAnd( HDMI_VIDEO_CTRL_OFFS, ~INT_FRM_SEL );

    /* adjust horizontal parameters based on color deep mode */
    if( color_depth == CD_30BIT ) {
    	h_active += (h_active / 4);
    	h_blank += (h_blank / 4);
    } else if( color_depth == CD_36BIT ) {
    	h_active += (h_active / 2);
    	h_blank += (h_blank / 2);
    } else if( color_depth == CD_48BIT ) {
    	h_active *= 2;
    	h_blank *= 2;
    } else {
		/* assume 24 bit non-deep color mode, no
		 * horizontal parameters adjustment needed
		 */
    }

    /* adjust vertical blanks based on interleaving mode */
    if( interleave_mode ) {
//    	v_blank *= 2;
//    	v_blank += 1;
    }

    /* set video format timings manually */
    HdmiWriteRegByte( HDMI_HTOT_L_OFFS, (h_active + h_blank) & 0xFF );
    HdmiWriteRegByte( HDMI_HTOT_H_OFFS, (h_active + h_blank) >> 0x8 );
    HdmiWriteRegByte( HDMI_HBLANK_L_OFFS, h_blank & 0xFF );
    HdmiWriteRegByte( HDMI_HBLANK_H_OFFS, h_blank >> 0x8 );
    HdmiWriteRegByte( HDMI_VTOT_L_OFFS, (v_active + v_blank) & 0xFF );
    HdmiWriteRegByte( HDMI_VTOT_H_OFFS, (v_active + v_blank) >> 0x8 );
    HdmiWriteRegByte( HDMI_VRES_L_OFFS, v_active & 0xFF );
    HdmiWriteRegByte( HDMI_VRES_H_OFFS, v_active >> 0x8 );
    HdmiWriteRegByte( HDMI_VSTART_L_OFFS, (v_front + v_sync + v_back) & 0xFF );
    HdmiWriteRegByte( HDMI_VSTART_H_OFFS, (v_front + v_sync + v_back) >> 0x8 );

    /* set appropriate video format */
    HdmiWriteRegByteAnd( HDMI_VIDEO_CTRL_OFFS, ~IN_YC );
    HdmiWriteRegByteOr( HDMI_VIDEO_CTRL_OFFS, (color_indicator == CI_RGB) ? 0 : IN_YC );

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
 * HdmiTxHwInitialize
 *
 * Performs initialization routine of the underlying HDMI
 * transmitter hardware controller.
 *
 * @param None.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int HdmiTxHwInitialize(void)
{
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

	return (DRIVER_NO_ERROR);
}

/*
 * HdmiTxHwEnable
 *
 * Enables or disables the underlying HDMI transmitter
 * hardware controller.
 *
 * @param enable - a boolean flag which determines if the
 * 				   hardware will be enabled or disabled.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int HdmiTxHwEnable(BOOL enable)
{
	if( enable ) {

		/* enable hdmi clock */
		WriteRegDwordOr( HDMI_PLL_CFG_1_REG, HDMI_PLL_CFG_EN_PANNEL | HDMI_PLL_CFG_EN_HDMI  | HDMI_PLL_CFG_FO_RDY_ADJ );
		WriteRegDwordOr( HDMI_CLOCK_CFG_REG, HDMI_CLOCK_CFG_EN );

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

	if( !enable ) {

		/* disable hdmi pll */
		WriteRegDwordAnd( HDMI_CLOCK_CFG_REG, ~HDMI_CLOCK_CFG_EN );
		WriteRegDwordAnd( HDMI_PLL_CFG_1_REG, ~(HDMI_PLL_CFG_EN_PANNEL | HDMI_PLL_CFG_EN_HDMI | HDMI_PLL_CFG_FO_RDY_ADJ) );

		/* power down and reset the pll */
		WriteRegDwordAnd( HDMI_PLL_CFG_3_REG, ~(HDMI_PLL_CFG_RSTB | HDMI_PLL_CFG_ON) );

		/* power down transmitter and disable termination resistor */
		WriteRegDword( HDMI_PHY_CFG_2_REG, HDMI_PHY_CFG_RESET_TX | HDMI_PHY_CFG_PD_TX );

		/* disable hdmi reference and bus clocks */
		WriteRegDwordAnd( PMUA_DISP1_CTRL_REG, ~(HDMI_I2S_CLK_EN | HDMI_REF_CLK_EN) );
		WriteRegDwordAnd( PMUA_DISP1_CTRL_REG, ~(DISP_AXI_CLK_EN | DISP_AXI_CLK_RST) );
	}

	return (DRIVER_NO_ERROR);
}

/*
 * HdmiTxHwTransmit
 *
 * Starts or stops signal transmission of the underlying
 * HDMI transmitter hardware controller.
 *
 * @param transmit - a boolean flag which determines if the
 * 				     hardware will start or will stop signal
 * 					 transmission.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int HdmiTxHwTransmit(BOOL transmit)
{
	if( transmit ) {

#if 1
		HdmiWriteRegByte( HDMI_VIDEO_CTRL_OFFS, 0x00 );
		HdmiWriteRegByte( HDMI_VIDEO_CTRL_OFFS, INT_FRM_SEL );
		while( (HdmiReadRegByte( HDMI_VIDEO_STS_OFFS ) & INIT_OVER) != INIT_OVER )
			PRINT( "Waiting for HDMI to detect video format ..." );
#endif

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

	}

	return (DRIVER_NO_ERROR);
}

/*
 * HdmiTxHwConfigure
 *
 * Configures the underlying HDMI transmitter hardware
 * controller.
 *
 * @param hdmi_mode - determines if the hardware is an HDMI
 * 					  device or not.
 * @param color_depth - the number of bits per pixel data.
 * @param color_indicator - determines whether RGB or YUV.
 * @param channel_count - the number of audio channels.
 * @param sample_frequency - the audio sampling frequency.
 * @param sample_size - the audio bits per channel.
 * @param interleave_mode - the interleaving/progressive mode.
 * @param h_active - the horizontal resolution in pixels.
 * @param h_front - the horizontal front blank period in pixels.
 * @param h_sync - the horizontal sync period in pixels.
 * @param h_back - the horizontal back blank period in pixels.
 * @param v_active - the vertical resolution in pixels.
 * @param v_front - the vertical front blank period in pixels.
 * @param v_sync - the vertical sync period in pixels.
 * @param v_back - the vertical back blank period in pixels.
 * @param pixel_repetition - the number of times pixel data
 * 							 will be transmitted.
 * @param pixel_clock - the operating pixel clock.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
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
	TmdsClock_t tmds_clock)
{
	/* start hdmi reference and bus clocks */
	WriteRegDwordOr( PMUA_DISP1_CTRL_REG, DISP_AXI_CLK_EN | DISP_AXI_CLK_RST );
	WriteRegDwordOr( PMUA_DISP1_CTRL_REG, HDMI_I2S_CLK_EN | HDMI_REF_CLK_EN );

	/* configure hdmi pll and TMDS clock */
	HdmiTxHwConfigPll( tmds_clock, pixel_repetition, color_depth );

    /* configure hdmi video */
    HdmiTxHwConfigVideo(
    		color_depth,
    		color_indicator,
    		interleave_mode,
    		h_active,
    		h_front,
    		h_sync,
    		h_back,
    		h_active,
    		h_front,
    		h_sync,
    		h_back );

    /* configure hdmi audio */
    HdmiTxHwConfigAudio( channel_count, sample_frequency, sample_size );

    /* set hdmi or dvi mode */
    HdmiWriteRegByteAnd( HDMI_CTRL_OFFS, ~HDMI_MODE );
    HdmiWriteRegByteOr( HDMI_CTRL_OFFS, (hdmi_mode) ? HDMI_MODE : 0x00 );

    /* configure pixel repetition */
    HdmiWriteRegByteAnd( HDMI_CTRL_OFFS, ~PIX_RPT_MASK );
    HdmiWriteRegByteOr( HDMI_CTRL_OFFS, PIX_RPT(pixel_repetition) );

    /* configure audio layout */
    HdmiWriteRegByteAnd( HDMI_CTRL_OFFS, ~LAYOUT_8_CHAN_AUDIO );
    HdmiWriteRegByteOr( HDMI_CTRL_OFFS, (channel_count == AC_8CH) ? LAYOUT_8_CHAN_AUDIO : 0 );

	/* stop hdmi reference and bus clocks */
//	WriteRegDwordAnd( PMUA_DISP1_CTRL_REG, ~(HDMI_I2S_CLK_EN | HDMI_REF_CLK_EN) );
//	WriteRegDwordAnd( PMUA_DISP1_CTRL_REG, ~(DISP_AXI_CLK_EN | DISP_AXI_CLK_RST) );

    return (DRIVER_NO_ERROR);
}

/*
 * HdmiTxHwGetSecureVal
 *
 * Reads the current value of a specific secure credential
 * used in the HDCP authentication process between an HDMI
 * source and an HDMI sink.
 *
 * @param secval - Numeric code of the secured credential
 * 				   requested to get.
 * @param secbuf - Pointer to the buffer which would hold
 * 				   the requested secured credential.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int HdmiTxHwGetSecureVal( SecureValue_t secval, BYTE *secbuf )
{
	switch( secval ) {
		case SV_AINFO:
			*(secbuf) = HdmiReadRegByte( HDMI_HDCP_TX_AINFO_OFFS );
			break;
		case SV_AN:
#if 0
			/* randomize KINIT registers */
			HdmiWriteRegByte( HDMI_HDCP_KINIT_0_OFFS, *(kinit+0) );
			HdmiWriteRegByte( HDMI_HDCP_KINIT_1_OFFS, *(kinit+1) );
			HdmiWriteRegByte( HDMI_HDCP_KINIT_2_OFFS, *(kinit+2) );
			HdmiWriteRegByte( HDMI_HDCP_KINIT_3_OFFS, *(kinit+3) );
			HdmiWriteRegByte( HDMI_HDCP_KINIT_4_OFFS, *(kinit+4) );
			HdmiWriteRegByte( HDMI_HDCP_KINIT_5_OFFS, *(kinit+5) );
			HdmiWriteRegByte( HDMI_HDCP_KINIT_6_OFFS, *(kinit+6) );
			/* randomize BINIT registers */
			HdmiWriteRegByte( HDMI_HDCP_BINIT_0_OFFS, *(binit+0) );
			HdmiWriteRegByte( HDMI_HDCP_BINIT_1_OFFS, *(binit+1) );
			HdmiWriteRegByte( HDMI_HDCP_BINIT_2_OFFS, *(binit+2) );
			HdmiWriteRegByte( HDMI_HDCP_BINIT_3_OFFS, *(binit+3) );
			HdmiWriteRegByte( HDMI_HDCP_BINIT_4_OFFS, *(binit+4) );
			HdmiWriteRegByte( HDMI_HDCP_BINIT_5_OFFS, *(binit+5) );
			HdmiWriteRegByte( HDMI_HDCP_BINIT_6_OFFS, *(binit+6) );
			HdmiWriteRegByte( HDMI_HDCP_BINIT_7_OFFS, *(binit+7) );
			/* make sure AN will be generated */
			HdmiWriteRegByteAnd( HDMI_HDCP_INTR_CLR_OFFS, ~TX_AN_READY_CLR );
			/* enable cipher to run freely for a while */
			HdmiWriteRegByteOr( HDMI_HDCP_CTRL_OFFS, CIPHER_EN );
			for( loop=0; loop<1000; loop++ );
			/* reset cipher and wait for AN to be ready */
			HdmiWriteRegByteAnd( HDMI_HDCP_CTRL_OFFS, ~CIPHER_EN );
			while( (HdmiReadRegByte(HDMI_HDCP_INTR_0_OFFS)&TX_AN_READY_STATUS) != 0 );
			/* read back generated AN */
			*((secbuf)+0) = HdmiReadRegByte( HDMI_HDCP_TX_AN_0_OFFS );
			*((secbuf)+1) = HdmiReadRegByte( HDMI_HDCP_TX_AN_1_OFFS );
			*((secbuf)+2) = HdmiReadRegByte( HDMI_HDCP_TX_AN_2_OFFS );
			*((secbuf)+3) = HdmiReadRegByte( HDMI_HDCP_TX_AN_3_OFFS );
			*((secbuf)+4) = HdmiReadRegByte( HDMI_HDCP_TX_AN_4_OFFS );
			*((secbuf)+5) = HdmiReadRegByte( HDMI_HDCP_TX_AN_5_OFFS );
			*((secbuf)+6) = HdmiReadRegByte( HDMI_HDCP_TX_AN_6_OFFS );
			*((secbuf)+7) = HdmiReadRegByte( HDMI_HDCP_TX_AN_7_OFFS );
#endif
			break;
		case SV_AKSV:
			/* make sure AKSV ready flag will be set */
			HdmiWriteRegByteAnd( HDMI_HDCP_INTR_CLR_OFFS, ~AKSV_READY_CLR );
			/* initiate AKSV read from memory */
			HdmiWriteRegByteOr( HDMI_HDCP_CTRL_OFFS, READ_AKSV );
			/* wait for AKSV to be ready */
			while( (HdmiReadRegByte(HDMI_HDCP_INTR_0_OFFS)&AKSV_READY_STATUS) != 0 );
			/* read back AKSV */
			*((secbuf)+0) = HdmiReadRegByte( HDMI_HDCP_AKSV_BYTE_0_OFFS );
			*((secbuf)+1) = HdmiReadRegByte( HDMI_HDCP_AKSV_BYTE_1_OFFS );
			*((secbuf)+2) = HdmiReadRegByte( HDMI_HDCP_AKSV_BYTE_2_OFFS );
			*((secbuf)+3) = HdmiReadRegByte( HDMI_HDCP_AKSV_BYTE_3_OFFS );
			*((secbuf)+4) = HdmiReadRegByte( HDMI_HDCP_AKSV_BYTE_4_OFFS );
			break;
		case SV_R0_TX:
			/* wait for R0 TX to be ready */
			while( (HdmiReadRegByte(HDMI_HDCP_INTR_0_OFFS) & TX_R0_READY_STATUS) != 0 );
			/* read back R0 TX */
			*((secbuf)+0) = HdmiReadRegByte( HDMI_HDCP_TX_R0_0_OFFS );
			*((secbuf)+1) = HdmiReadRegByte( HDMI_HDCP_TX_R0_1_OFFS );
			break;
		case SV_RI_TX:
			/* wait for RI TX to be ready */
			while( (HdmiReadRegByte(HDMI_HDCP_INTR_0_OFFS) & TX_RI_READY_STATUS) != 0 );
			/* read back R0 TX */
			*((secbuf)+0) = HdmiReadRegByte( HDMI_HDCP_TX_RI_0_OFFS );
			*((secbuf)+1) = HdmiReadRegByte( HDMI_HDCP_TX_RI_1_OFFS );
			break;
		case SV_PJ_TX:
			/* wait for RI TX to be ready */
			while( (HdmiReadRegByte(HDMI_HDCP_INTR_0_OFFS) & TX_PJ_READY_STATUS) != 0 );
			/* read back R0 TX */
			*((secbuf)) = HdmiReadRegByte( HDMI_HDCP_TX_PJ_OFFS );
			break;
		case SV_V_TX:
			/* compute V computation here */
			break;
		case SV_BCAPS:
		case SV_BKSV:
		case SV_BSTATUS:
		case SV_R0_RX:
		case SV_RI_RX:
		case SV_PJ_RX:
		case SV_V_RX:
		case SV_KSV_FIFO:
		default:
			/* these values can not be retrieved from the HDMI transmitter's controller */
			PRINTN( "Secured value can not be retrieved: %d", secval );
			return (-1);
	};

	/* return successfully */
	return (0);
}

/*
 * HdmiTxHwSetSecureVal
 *
 * Stores the specified value of a specific secure creden-
 * tial used in the HDCP authentication process between an
 * HDMI source and an HDMI sink.
 *
 * @param secval - Numeric code of the secured credential
 * 				   requested to set.
 * @param secbuf - Pointer to the buffer which holds the
 * 				   new value of the secured credential.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int HdmiTxHwSetSecureVal( SecureValue_t secval, BYTE *secbuf )
{
	switch( secval ) {
		case SV_BCAPS:
			HdmiWriteRegByte( HDMI_HDCP_RX_BCAPS_OFFS, *secbuf );
			break;
		case SV_BKSV:
			HdmiWriteRegByte( HDMI_HDCP_TX_BKSV_0_OFFS, *((secbuf)+0) );
			HdmiWriteRegByte( HDMI_HDCP_TX_BKSV_1_OFFS, *((secbuf)+1) );
			HdmiWriteRegByte( HDMI_HDCP_TX_BKSV_2_OFFS, *((secbuf)+2) );
			HdmiWriteRegByte( HDMI_HDCP_TX_BKSV_3_OFFS, *((secbuf)+3) );
			HdmiWriteRegByte( HDMI_HDCP_TX_BKSV_4_OFFS, *((secbuf)+4) );
			break;
		case SV_BSTATUS:
			HdmiWriteRegByte( HDMI_HDCP_RX_BSTATUS_0_OFFS, *((secbuf)+0) );
			HdmiWriteRegByte( HDMI_HDCP_RX_BSTATUS_1_OFFS, *((secbuf)+1) );
			break;
		case SV_R0_RX:
			HdmiWriteRegByte( HDMI_HDCP_RX_R0_0_OFFS, *((secbuf)+0) );
			HdmiWriteRegByte( HDMI_HDCP_RX_R0_1_OFFS, *((secbuf)+1) );
			break;
		case SV_RI_RX:
			HdmiWriteRegByte( HDMI_HDCP_RX_RI_0_OFFS, *((secbuf)+0) );
			HdmiWriteRegByte( HDMI_HDCP_RX_RI_1_OFFS, *((secbuf)+1) );
			break;
		case SV_PJ_RX:
			HdmiWriteRegByte( HDMI_HDCP_RX_PJ_OFFS, *(secbuf) );
			break;
		case SV_V_RX:
		case SV_KSV_FIFO:
			/* hardware controller does not have any registers for storage of these secured values */
			break;
		case SV_AINFO:
		case SV_AN:
		case SV_AKSV:
		case SV_R0_TX:
		case SV_RI_TX:
		case SV_PJ_TX:
		case SV_V_TX:
		default:
			/* these values can not be stored to the HDMI transmitter's controller */
			PRINTN( "Secured value can not be set: %d", secval );
			return (-1);
	};

	/* return successfully */
	return (0);
}

/*
 * HdmiTxHwPacketize
 *
 * Prepares and enables transmission of a particular data
 * island packet out through the TMDS data channels.
 *
 * @param packet  - Numeric code of the data island packet
 * 				    requested to be transmitted.
 * @param payload - Data payload corresponding to a specific
 * 					data island packet.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int HdmiTxHwPacketize( PacketType_t packet, void *payload )
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
                    return (-1);
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

/*
 * HdmiTxHwEncrypt
 *
 * Enables signal encryption of the underlying HDMI trans-
 * mitter hardware controller.
 *
 * @param None.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int HdmiTxHwEncrypt(void)
{
	return (DRIVER_NO_ERROR);
}

/*
 * HdmiTxHwShowRegs
 *
 * Dumps all the registers of the underlying HDMI transmitter
 * hardware controller.
 *
 * @param none.
 *
 * @return none.
 */
void HdmiTxHwShowRegs(void)
{
	PRINT(  "*** HDMI Transmitter Direct Registers ***" );
	PRINT(  "" );
	PRINTN( "HDMI_PHY_CFG_0: 0x%08x", ReadRegDword(HDMI_PHY_CFG_0_REG) );
	PRINTN( "HDMI_PHY_CFG_1: 0x%08x", ReadRegDword(HDMI_PHY_CFG_1_REG) );
	PRINTN( "HDMI_PHY_CFG_2: 0x%08x", ReadRegDword(HDMI_PHY_CFG_2_REG) );
	PRINTN( "HDMI_PHY_CFG_3: 0x%08x", ReadRegDword(HDMI_PHY_CFG_3_REG) );
	PRINTN( "HDMI_PLL_CFG_0: 0x%08x", ReadRegDword(HDMI_PLL_CFG_0_REG) );
	PRINTN( "HDMI_PLL_CFG_1: 0x%08x", ReadRegDword(HDMI_PLL_CFG_1_REG) );
	PRINTN( "HDMI_PLL_CFG_2: 0x%08x", ReadRegDword(HDMI_PLL_CFG_2_REG) );
	PRINTN( "HDMI_PLL_CFG_3: 0x%08x", ReadRegDword(HDMI_PLL_CFG_3_REG) );
	PRINTN( "HDMI_AUDIO_CFG: 0x%08x", ReadRegDword(HDMI_AUDIO_CFG_REG) );
	PRINTN( "HDMI_CLOCK_CFG: 0x%08x", ReadRegDword(HDMI_CLOCK_CFG_REG) );
	PRINT(  "" );
	PRINT(  "*** HDMI Transmitter Indirect Registers ***" );
	PRINT(  "" );
    PRINTN( "HDMI_ACR_N0: 0x%02x", HdmiReadRegByte( HDMI_ACR_N0_OFFS ) );
    PRINTN( "HDMI_ACR_N1: 0x%02x", HdmiReadRegByte( HDMI_ACR_N1_OFFS ) );
    PRINTN( "HDMI_ACR_N2: 0x%02x", HdmiReadRegByte( HDMI_ACR_N2_OFFS ) );
    PRINTN( "HDMI_ACR_CTS0: 0x%02x", HdmiReadRegByte( HDMI_ACR_CTS0_OFFS ) );
    PRINTN( "HDMI_ACR_CTS1: 0x%02x", HdmiReadRegByte( HDMI_ACR_CTS1_OFFS ) );
    PRINTN( "HDMI_ACR_CTS2: 0x%02x", HdmiReadRegByte( HDMI_ACR_CTS2_OFFS ) );
    PRINTN( "HDMI_ACR_CTRL: 0x%02x", HdmiReadRegByte( HDMI_ACR_CTRL_OFFS ) );
    PRINTN( "HDMI_ACR_STS0: 0x%02x", HdmiReadRegByte( HDMI_ACR_STS0_OFFS ) );
    PRINTN( "HDMI_ACR_STS1: 0x%02x", HdmiReadRegByte( HDMI_ACR_STS1_OFFS ) );
    PRINTN( "HDMI_ACR_STS2: 0x%02x", HdmiReadRegByte( HDMI_ACR_STS2_OFFS ) );
    PRINTN( "HDMI_AUD_CTRL: 0x%02x", HdmiReadRegByte( HDMI_AUD_CTRL_OFFS ) );
    PRINTN( "HDMI_I2S_CTRL: 0x%02x", HdmiReadRegByte( HDMI_I2S_CTRL_OFFS ) );
    PRINTN( "HDMI_I2S_DLEN: 0x%02x", HdmiReadRegByte( HDMI_I2S_DLEN_OFFS ) );
    PRINTN( "HDMI_I2S_DBG_LFT0: 0x%02x", HdmiReadRegByte( HDMI_I2S_DBG_LFT0_OFFS ) );
    PRINTN( "HDMI_I2S_DBG_LFT1: 0x%02x", HdmiReadRegByte( HDMI_I2S_DBG_LFT1_OFFS ) );
    PRINTN( "HDMI_I2S_DBG_LFT2: 0x%02x", HdmiReadRegByte( HDMI_I2S_DBG_LFT2_OFFS ) );
    PRINTN( "HDMI_I2S_DBG_LFT3: 0x%02x", HdmiReadRegByte( HDMI_I2S_DBG_LFT3_OFFS ) );
    PRINTN( "HDMI_I2S_DBG_RIT0: 0x%02x", HdmiReadRegByte( HDMI_I2S_DBG_RIT0_OFFS ) );
    PRINTN( "HDMI_I2S_DBG_RIT1: 0x%02x", HdmiReadRegByte( HDMI_I2S_DBG_RIT1_OFFS ) );
    PRINTN( "HDMI_I2S_DBG_RIT2: 0x%02x", HdmiReadRegByte( HDMI_I2S_DBG_RIT2_OFFS ) );
    PRINTN( "HDMI_I2S_DBG_RIT3: 0x%02x", HdmiReadRegByte( HDMI_I2S_DBG_RIT3_OFFS ) );
    PRINTN( "HDMI_FIFO_CTRL: 0x%02x", HdmiReadRegByte( HDMI_FIFO_CTRL_OFFS ) );
    PRINTN( "HDMI_MEMSIZE_L: 0x%02x", HdmiReadRegByte( HDMI_MEMSIZE_L_OFFS ) );
    PRINTN( "HDMI_MEMSIZE_H: 0x%02x", HdmiReadRegByte( HDMI_MEMSIZE_H_OFFS ) );
    PRINTN( "HDMI_GCP_CFG0: 0x%02x", HdmiReadRegByte( HDMI_GCP_CFG0_OFFS ) );
    PRINTN( "HDMI_GCP_CFG1: 0x%02x", HdmiReadRegByte( HDMI_GCP_CFG1_OFFS ) );
    PRINTN( "HDMI_HTOT_L: 0x%02x", HdmiReadRegByte( HDMI_HTOT_L_OFFS ) );
    PRINTN( "HDMI_HTOT_H: 0x%02x", HdmiReadRegByte( HDMI_HTOT_H_OFFS ) );
    PRINTN( "HDMI_HBLANK_L: 0x%02x", HdmiReadRegByte( HDMI_HBLANK_L_OFFS ) );
    PRINTN( "HDMI_HBLANK_H: 0x%02x", HdmiReadRegByte( HDMI_HBLANK_H_OFFS ) );
    PRINTN( "HDMI_VTOT_L: 0x%02x", HdmiReadRegByte( HDMI_VTOT_L_OFFS ) );
    PRINTN( "HDMI_VTOT_H: 0x%02x", HdmiReadRegByte( HDMI_VTOT_H_OFFS ) );
    PRINTN( "HDMI_VRES_L: 0x%02x", HdmiReadRegByte( HDMI_VRES_L_OFFS ) );
    PRINTN( "HDMI_VRES_H: 0x%02x", HdmiReadRegByte( HDMI_VRES_H_OFFS ) );
    PRINTN( "HDMI_VSTART_L: 0x%02x", HdmiReadRegByte( HDMI_VSTART_L_OFFS ) );
    PRINTN( "HDMI_VSTART_H: 0x%02x", HdmiReadRegByte( HDMI_VSTART_H_OFFS ) );
    PRINTN( "HDMI_HTOT_STS_L: 0x%02x", HdmiReadRegByte( HDMI_HTOT_STS_L_OFFS ) );
    PRINTN( "HDMI_HTOT_STS_H: 0x%02x", HdmiReadRegByte( HDMI_HTOT_STS_H_OFFS ) );
    PRINTN( "HDMI_HBLANK_STS_L: 0x%02x", HdmiReadRegByte( HDMI_HBLANK_STS_L_OFFS ) );
    PRINTN( "HDMI_HBLANK_STS_H: 0x%02x", HdmiReadRegByte( HDMI_HBLANK_STS_H_OFFS ) );
    PRINTN( "HDMI_VTOT_STS_L: 0x%02x", HdmiReadRegByte( HDMI_VTOT_STS_L_OFFS ) );
    PRINTN( "HDMI_VTOT_STS_H: 0x%02x", HdmiReadRegByte( HDMI_VTOT_STS_H_OFFS ) );
    PRINTN( "HDMI_VRES_STS_L: 0x%02x", HdmiReadRegByte( HDMI_VRES_STS_L_OFFS ) );
    PRINTN( "HDMI_VRES_STS_H: 0x%02x", HdmiReadRegByte( HDMI_VRES_STS_H_OFFS ) );
    PRINTN( "HDMI_VSTART_STS_L: 0x%02x", HdmiReadRegByte( HDMI_VSTART_STS_L_OFFS ) );
    PRINTN( "HDMI_VSTART_STS_H: 0x%02x", HdmiReadRegByte( HDMI_VSTART_STS_H_OFFS ) );
    PRINTN( "HDMI_VIDEO_STS: 0x%02x", HdmiReadRegByte( HDMI_VIDEO_STS_OFFS ) );
    PRINTN( "HDMI_VIDEO_CTRL: 0x%02x", HdmiReadRegByte( HDMI_VIDEO_CTRL_OFFS ) );
    PRINTN( "HDMI_CTRL: 0x%02x", HdmiReadRegByte( HDMI_CTRL_OFFS ) );
    PRINTN( "HDMI_PP_HW: 0x%02x", HdmiReadRegByte( HDMI_PP_HW_OFFS ) );
    PRINTN( "HDMI_DC_FIFO_SFT_RST: 0x%02x", HdmiReadRegByte( HDMI_DC_FIFO_SFT_RST_OFFS ) );
    PRINTN( "HDMI_DC_FIFO_WR_PTR: 0x%02x", HdmiReadRegByte( HDMI_DC_FIFO_WR_PTR_OFFS ) );
    PRINTN( "HDMI_DC_FIFO_RD_PTR: 0x%02x", HdmiReadRegByte( HDMI_DC_FIFO_RD_PTR_OFFS ) );
    PRINTN( "HDMI_TDATA0_0: 0x%02x", HdmiReadRegByte( HDMI_TDATA0_0_OFFS ) );
    PRINTN( "HDMI_TDATA0_1: 0x%02x", HdmiReadRegByte( HDMI_TDATA0_1_OFFS ) );
    PRINTN( "HDMI_TDATA0_2: 0x%02x", HdmiReadRegByte( HDMI_TDATA0_2_OFFS ) );
    PRINTN( "HDMI_TDATA1_0: 0x%02x", HdmiReadRegByte( HDMI_TDATA1_0_OFFS ) );
    PRINTN( "HDMI_TDATA1_1: 0x%02x", HdmiReadRegByte( HDMI_TDATA1_1_OFFS ) );
    PRINTN( "HDMI_TDATA1_2: 0x%02x", HdmiReadRegByte( HDMI_TDATA1_2_OFFS ) );
    PRINTN( "HDMI_TDATA2_0: 0x%02x", HdmiReadRegByte( HDMI_TDATA2_0_OFFS ) );
    PRINTN( "HDMI_TDATA2_1: 0x%02x", HdmiReadRegByte( HDMI_TDATA2_1_OFFS ) );
    PRINTN( "HDMI_TDATA2_2: 0x%02x", HdmiReadRegByte( HDMI_TDATA2_2_OFFS ) );
    PRINTN( "HDMI_TDATA3_0: 0x%02x", HdmiReadRegByte( HDMI_TDATA3_0_OFFS ) );
    PRINTN( "HDMI_TDATA3_1: 0x%02x", HdmiReadRegByte( HDMI_TDATA3_1_OFFS ) );
    PRINTN( "HDMI_TDATA3_2: 0x%02x", HdmiReadRegByte( HDMI_TDATA3_2_OFFS ) );
    PRINTN( "HDMI_TDATA_SEL: 0x%02x", HdmiReadRegByte( HDMI_TDATA_SEL_OFFS ) );
    PRINTN( "HDMI_SWAP_CTRL: 0x%02x", HdmiReadRegByte( HDMI_SWAP_CTRL_OFFS ) );
    PRINTN( "HDMI_AVMUTE_CTRL: 0x%02x", HdmiReadRegByte( HDMI_AVMUTE_CTRL_OFFS ) );
    PRINTN( "HDMI_HOST_PKT_CTRL0: 0x%02x", HdmiReadRegByte( HDMI_HOST_PKT_CTRL0_OFFS ) );
    PRINTN( "HDMI_HOST_PKT_CTRL1: 0x%02x", HdmiReadRegByte( HDMI_HOST_PKT_CTRL1_OFFS ) );
    PRINTN( "HDMI_HBR_PKT: 0x%02x", HdmiReadRegByte( HDMI_HBR_PKT_OFFS ) );
    PRINTN( "HDMI_PHY_FIFO_SOFT_RST: 0x%02x", HdmiReadRegByte( HDMI_PHY_FIFO_SOFT_RST_OFFS ) );
    PRINTN( "HDMI_PHY_FIFO_PTRS: 0x%02x", HdmiReadRegByte( HDMI_PHY_FIFO_PTRS_OFFS ) );
    PRINTN( "HDMI_PRBS_CTRL0: 0x%02x", HdmiReadRegByte( HDMI_PRBS_CTRL0_OFFS ) );
    PRINTN( "HDMI_HOST_PKT_CTRL2: 0x%02x", HdmiReadRegByte( HDMI_HOST_PKT_CTRL2_OFFS ) );
    PRINTN( "HDMI_HOST_PKT_START: 0x%02x", HdmiReadRegByte( HDMI_HOST_PKT_START_OFFS ) );
    PRINTN( "HDMI_AUD_CH1_SEL: 0x%02x", HdmiReadRegByte( HDMI_AUD_CH1_SEL_OFFS ) );
    PRINTN( "HDMI_AUD_CH2_SEL: 0x%02x", HdmiReadRegByte( HDMI_AUD_CH2_SEL_OFFS ) );
    PRINTN( "HDMI_AUD_CH3_SEL: 0x%02x", HdmiReadRegByte( HDMI_AUD_CH3_SEL_OFFS ) );
    PRINTN( "HDMI_AUD_CH4_SEL: 0x%02x", HdmiReadRegByte( HDMI_AUD_CH4_SEL_OFFS ) );
    PRINTN( "HDMI_AUD_CH5_SEL: 0x%02x", HdmiReadRegByte( HDMI_AUD_CH5_SEL_OFFS ) );
    PRINTN( "HDMI_AUD_CH6_SEL: 0x%02x", HdmiReadRegByte( HDMI_AUD_CH6_SEL_OFFS ) );
    PRINTN( "HDMI_AUD_CH7_SEL: 0x%02x", HdmiReadRegByte( HDMI_AUD_CH7_SEL_OFFS ) );
    PRINTN( "HDMI_AUD_CH8_SEL: 0x%02x", HdmiReadRegByte( HDMI_AUD_CH8_SEL_OFFS ) );

    return;
}

/*
 ***************************************************
 ***************************************************
 *
 * Other HDMI-related xxportable function definitions
 *
 ***************************************************
 ***************************************************
 */

#include "hdmi.h"

static __align(32) BYTE fbaddr[] = {
//#include "bbtorch_640x480p.rgb"
#include "bbbold2_720x480p.rgb"
//#include "bumblebee_1920x1080p.rgb"
};


/*
 * DispHwConfigure
 *
 * Configures the underlying display hardware controller.
 *
 * @param mode - the display mode (ie, graphics or video).
 * @param source_format - the pixel format of the source.
 * @param interleave_mode - the interleaving/progressive mode.
 * @param h_active - the horizontal resolution in pixels.
 * @param h_front - the horizontal front blank period in pixels.
 * @param h_sync - the horizontal sync period in pixels.
 * @param h_back - the horizontal back blank period in pixels.
 * @param v_active - the vertical resolution in pixels.
 * @param v_front - the vertical front blank period in pixels.
 * @param v_sync - the vertical sync period in pixels.
 * @param v_back - the vertical back blank period in pixels.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
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
	WORD v_back)
{
	WORD h_blank = h_front + h_sync + h_back;
	WORD v_blank = v_front + v_sync + v_back;

#if 0
	h_active = 720;
	h_front = 16;
	h_sync = 62;
	h_back = 60;
	v_active = 480;
	v_front = 9;
	v_sync = 6;
	v_back = 30;
#endif

	/* enable dislay controller and bus clocks */
	WriteRegDwordOr( PMUA_DISP1_CTRL_REG, DISP_AXI_CLK_EN | DISP_AXI_CLK_RST );
	WriteRegDwordOr( PMUA_DISP1_CTRL_REG, DISP1_CLK_EN | DISP1_CLK_RST );

	if( mode == DM_GRAPHICS ) {

		/* set source graphic object color format */
	    WriteRegDwordAnd( LCD_TV_CTRL0_REG, ~(CFG_TVGFORMAT_MASK | CFG_TVG_HSMOOTH | CFG_YUV2RGB_TVG) );
	    WriteRegDwordOr( LCD_TV_CTRL0_REG, CFG_TVGFORMAT(source_format) | CFG_TVG_HSMOOTH );
    	if( (source_format == PF_YUV422P) || (source_format == PF_YUV422PL) || (source_format == PF_YUV420PL) ) {
    	    WriteRegDwordOr( LCD_TV_CTRL0_REG, CFG_YUV2RGB_TVG );
    	}
	    WriteRegDword( LCD_AFA_ALL2ONE_REG, CFG_TVG_2ZMDN | CFG_VSMTH_TVG );

	    /* set starting address of the graphic in system memory */
	    WriteRegDword( LCD_TVG_START_ADDR0_REG, (DWORD) fbaddr ); //OVLY_TV_START_ADDR0 );
	    WriteRegDword( LCD_TVG_START_ADDR1_REG, (DWORD) fbaddr ); //OVLY_TV_START_ADDR1 );

	    /* set address pitch in bytes of the graphic */
	    if( (source_format == PF_RGB565) || (source_format == PF_RGB1555) ||
	    	(source_format == PF_YUV422P) || (source_format == PF_YUV422PL) || (source_format == PF_YUV420PL) ) {
	    	WriteRegDword( LCD_TVG_PITCH_REG, (2*h_active) );
	    } else if( source_format == PF_RGB888P ) {
	    	WriteRegDword( LCD_TVG_PITCH_REG, (3*h_active) );
	    } else if( (source_format == PF_RGB888UP) || (source_format == PF_RGBA888) || (source_format == PF_RGB888A) ) {
	    	WriteRegDword( LCD_TVG_PITCH_REG, (4*h_active) );
	    } else if( source_format == PF_PALETTE8 ) {
	    	WriteRegDword( LCD_TVG_PITCH_REG, h_active );
	   	} else if( source_format == PF_PALETTE4 ) {
	    	WriteRegDword( LCD_TVG_PITCH_REG, (h_active/2) );
	   	}

	    /* set screen starting point, source size and destination size */
	    WriteRegDword( LCD_TVG_OVSA_HPXL_VLN_REG, 0x00000000 );
	    WriteRegDword( LCD_TVG_HPXL_VLN_REG, ((v_active<<16)|h_active) );
	    WriteRegDword( LCD_TVGZM_HPXL_VLN_REG, ((v_active<<16)|h_active) );

	} else {

		/* set source video object color format */
	    WriteRegDwordAnd( LCD_TV_CTRL0_REG, ~(CFG_TVDFORMAT_MASK | CFG_TVD_HSMOOTH | CFG_YUV2RGB_TVD) );
	    WriteRegDwordOr( LCD_TV_CTRL0_REG, CFG_TVDFORMAT(source_format) | CFG_TVD_HSMOOTH );
    	if( (source_format == PF_YUV422P) || (source_format == PF_YUV422PL) || (source_format == PF_YUV420PL) ) {
    	    WriteRegDwordOr( LCD_TV_CTRL0_REG, CFG_YUV2RGB_TVD );
    	}
	    WriteRegDword( LCD_AFA_ALL2ONE_REG, CFG_TVD_2ZMDN | CFG_VSMTH_TVD );

	    /* set starting address of Y/U/V in system memory */
	    WriteRegDword( LCD_TVD_START_ADDR_Y0_REG, (DWORD) fbaddr ); //OVLY_TV_START_ADDR0 );
	    WriteRegDword( LCD_TVD_START_ADDR_Y1_REG, (DWORD) fbaddr ); //OVLY_TV_START_ADDR1 );
	    WriteRegDword( LCD_TVD_START_ADDR_C0_REG, 0x00000000 );
	    WriteRegDword( LCD_TVD_START_ADDR_C1_REG, 0x00000000 );
	    if( (source_format == PF_YUV422PL) || (source_format == PF_YUV422PL) ) {
	        WriteRegDword( LCD_TVD_START_ADDR_U0_REG, OVLY_TV_START_ADDR0 + (h_active*v_active) );
	        WriteRegDword( LCD_TVD_START_ADDR_U1_REG, OVLY_TV_START_ADDR1 + (h_active*v_active) );
	        if( source_format == PF_YUV422PL ) {
	            WriteRegDword( LCD_TVD_START_ADDR_V0_REG, OVLY_TV_START_ADDR0 + (h_active*v_active*3/2) );
	            WriteRegDword( LCD_TVD_START_ADDR_V1_REG, OVLY_TV_START_ADDR1 + (h_active*v_active*3/2) );
	        } else {
	            WriteRegDword( LCD_TVD_START_ADDR_V0_REG, OVLY_TV_START_ADDR0 + (h_active*v_active*3/2) );
	            WriteRegDword( LCD_TVD_START_ADDR_V1_REG, OVLY_TV_START_ADDR1 + (h_active*v_active*3/2) );
	        }
	    }

	    /* set address pitch in bytes of Y/U/V */
	    if( (source_format == PF_RGB565) || (source_format == PF_RGB1555) || (source_format == PF_YUV422P) ) {
	    	WriteRegDword( LCD_TVD_PITCH_YC_REG, (2*h_active) );
	    } else if( source_format == PF_RGB888P ) {
	    	WriteRegDword( LCD_TVD_PITCH_YC_REG, (3*h_active) );
	    } else if( (source_format == PF_RGB888UP) || (source_format == PF_RGBA888) || (source_format == PF_RGB888A) ) {
	    	WriteRegDword( LCD_TVD_PITCH_YC_REG, (4*h_active) );
	    } else if( (source_format == PF_YUV422PL) || (source_format == PF_YUV420PL) || (source_format == PF_PALETTE8) ) {
	    	WriteRegDword( LCD_TVD_PITCH_YC_REG, h_active );
	    	if( (source_format == PF_YUV422PL) || (source_format == PF_YUV420PL) ) {
	        	WriteRegDword( LCD_TVD_PITCH_UV_REG, (((h_active/2)<<16)|(h_active/2)) );
	    	}
	   	} else if( source_format == PF_PALETTE4 ) {
	    	WriteRegDword( LCD_TVD_PITCH_YC_REG, (h_active/2) );
	   	}

	    /* set screen starting point, source size and destination size */
	    WriteRegDword( LCD_TVD_OVSA_HPXL_VLN_REG, 0x00000000 );
	    WriteRegDword( LCD_TVD_HPXL_VLN_REG, ((v_active<<16)|h_active) );
	    WriteRegDword( LCD_TVDZM_HPXL_VLN_REG, ((v_active<<16)|h_active) );

	}

	/* set vsync input trigger mode and alpha mode */
    WriteRegDwordAnd( LCD_TV_CTRL1_REG, ~(CFG_TV_SYNC_MODE_MASK | CFG_TV_ALPHA_MODE_MASK | CFG_TV_ALPHA_MASK ) );
	WriteRegDwordOr( LCD_TV_CTRL1_REG, CFG_TV_SYNC_MODE(2) | CFG_TV_ALPHA_MODE(2) | CFG_TV_ALPHA(0xFF) );

	/* set interleave mode if applicable */
    WriteRegDwordAnd( LCD_TV_CTRL1_REG, ~( CFG_TV_INTERLACE_EN | CFG_TV_NIB) );
    if( interleave_mode ) {
		WriteRegDwordOr( LCD_TV_CTRL1_REG, CFG_TV_INTERLACE_EN | CFG_TV_NIB );
	    WriteRegDword( LCD_TV_V_H_TOTAL_FLD_REG, (((v_blank+1+v_active)<<16)|v_active) );
	    WriteRegDword( LCD_TV_V_PORCH_FLD_REG, ((v_back<<16)|(v_front+1)) );
	    WriteRegDword( LCD_TV_SEPXLCNT_FLD_REG, ((h_active+h_front)<<16)|(h_active+h_front) );
	}

	/* set timing parameters */
    WriteRegDword( LCD_TV_V_H_TOTAL_REG, (((v_blank+v_active)<<16)|(h_blank+h_active)) );
    WriteRegDword( LCD_TV_V_H_ACTIVE_REG, ((v_active<<16)|h_active) );
    WriteRegDword( LCD_TV_H_PORCH_REG, ((h_back<<16)|h_front) );
    WriteRegDword( LCD_TV_V_PORCH_REG, ((v_back<<16)|v_front) );
    WriteRegDword( LCD_TV_SEPXLCNT_REG, ((h_active+h_front)<<16)|(h_active+h_front) );


    /* set blank color to blue */
    WriteRegDword( LCD_TV_BLANKCOLOR_REG, 0x000000CC );

    /* reset RGB/YUV color keys */
    WriteRegDword( LCD_TV_COLORKEY_Y_REG, 0x00000000 );
    WriteRegDword( LCD_TV_COLORKEY_U_REG, 0x00000000 );
    WriteRegDword( LCD_TV_COLORKEY_V_REG, 0x00000000 );

    /* reset SRAM control parameters */
    WriteRegDword( LCD_SRAM_CTRL_REG, 0x00000000 );
    WriteRegDword( LCD_SRAM_WRDAT_REG, 0x00000000 );
    WriteRegDword( LCD_SRAM_PARA0_REG, 0x00000000 );
    WriteRegDword( LCD_SRAM_PARA1_REG, 0x0000E000 );

    /* set contrast, saturation and hue */
    WriteRegDword( LCD_TV_CONTRAST_REG, 0x00004000 );
    WriteRegDword( LCD_TV_SATURATION_REG, 0x20004000 );
    WriteRegDword( LCD_TV_CBSH_HUE_REG, 0x00004000 );

	/* set destination object color format to 24-Bit RGB*/
    WriteRegDwordAnd( LCD_TVIF_CTRL_REG, ~CFG_HDMIMODE_MASK );
    WriteRegDwordOr( LCD_TVIF_CTRL_REG, CFG_HDMIMODE_RGB888 );

	/* select hdmi pll for tv path clock */
	WriteRegDwordAnd( LCD_TCLK_DIV_REG, ~TCLK_SOURCE_MASK );
	WriteRegDwordOr( LCD_TCLK_DIV_REG, TCLK_SOURCE_HDMI_PLL );

	/* select tv path for color management unit */
	WriteRegDwordOr( LCD_TOP_CTRL_REG, CFG_TVG_4_BURSTS | CFG_TVD_4_BURSTS | CFG_CMU_TV_PATH );

	/* select hdmi pll for color management unit clock */
	WriteRegDwordOr( CMU_CLK_CTRL_REG, CMU_CLK_HDMI_PLL );

    /* disable display interrupts */
    WriteRegDword( SPU_IRQ_ENA_REG, 0x00000000 );

	/* disable display controller and bus clocks */
//	WriteRegDwordAnd( PMUA_DISP1_CTRL_REG, ~(DISP_AXI_CLK_EN | DISP_AXI_CLK_RST) );
//	WriteRegDwordAnd( PMUA_DISP1_CTRL_REG, ~(DISP1_CLK_EN | DISP1_CLK_RST) );

	return (DRIVER_NO_ERROR);
}

/*
 * DispHwTransmit
 *
 * Starts or stops signal transmission of the underlying
 * Display hardware controller.
 *
 * @param transmit - a boolean flag which determines if the
 * 				     hardware will start or will stop signal
 * 					 transmission.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int DispHwTransmit(BOOL transmit)
{
	if( transmit ) {

		/* enable display controller and bus clocks */
//		WriteRegDwordOr( PMUA_DISP1_CTRL_REG, DISP_AXI_CLK_EN | DISP_AXI_CLK_RST );
//		WriteRegDwordOr( PMUA_DISP1_CTRL_REG, DISP1_CLK_EN | DISP1_CLK_RST );

		/* enable all hdmi-related clocks */
		WriteRegDwordOr( CMU_CLK_CTRL_REG, CMU_CLK_EN );
		WriteRegDwordOr( LCD_TOP_CTRL_REG, CFG_CMU_ENA );

		/* enable TV path */
		WriteRegDwordOr( LCD_TVIF_CTRL_REG, CFG_TVIF_ENA );

		/* enable TV path DMA transfer */
		if( (ReadRegDword( LCD_TV_CTRL0_REG ) & CFG_TVG_HSMOOTH) == CFG_TVG_HSMOOTH ) {
		    WriteRegDwordAnd( LCD_TV_CTRL0_REG, ~CFG_TVD_ENA );
		    WriteRegDwordOr( LCD_TV_CTRL0_REG, CFG_TVG_ENA );
		} else {
		    WriteRegDwordAnd( LCD_TV_CTRL0_REG, ~CFG_TVG_ENA );
		    WriteRegDwordOr( LCD_TV_CTRL0_REG, CFG_TVD_ENA );
		}

	} else {

		/* disable TV path DMA transfer */
	    WriteRegDwordAnd( LCD_TV_CTRL0_REG, ~(CFG_TVG_ENA | CFG_TVD_ENA) );

	    /* disable TV path */
		WriteRegDwordAnd( LCD_TVIF_CTRL_REG, ~CFG_TVIF_ENA );

		/* disable all hdmi-related clocks */
		WriteRegDwordAnd( LCD_TOP_CTRL_REG, ~CFG_CMU_ENA );
		WriteRegDwordAnd( CMU_CLK_CTRL_REG, ~CMU_CLK_EN );

		/* disable display controller and bus clocks */
//		WriteRegDwordAnd( PMUA_DISP1_CTRL_REG, ~(DISP_AXI_CLK_EN | DISP_AXI_CLK_RST) );
//		WriteRegDwordAnd( PMUA_DISP1_CTRL_REG, ~(DISP1_CLK_EN | DISP1_CLK_RST) );
	}

	return (DRIVER_NO_ERROR);
}

#define ICU_DMA_IRQ_MSK	0x003FFFFF
#define ADMA_CHANNEL_0	18
#define ADMA_CHANNEL_1	19

#define DMA_DESC_START	0xD1030000
#define DMA_SRC_ADDR	0xD1032000
#define DMA_DST_ADDR	0xD1038000
#define DMA_SIZE		0x00006000

typedef struct {
	DWORD ByteCount;
	DWORD SrcAddr;
	DWORD DstAddr;
	DWORD Next;
} DmaDesc_t;

DWORD admaCh0Ctr = 0, admaCh1Ctr = 0, sspaTxCtr = 0, sspaRxCtr = 0;

void AdmaIsr(void)
{
	if( (ReadRegDword(ADMA_CHAN_0_INT_STATUS_REG) & ADMA_COMPLETE) == ADMA_COMPLETE ) {
		admaCh0Ctr++;
		WriteRegDword( ADMA_CHAN_0_INT_STATUS_REG, 0x00000000 );
	}

	if( (ReadRegDword(ADMA_CHAN_1_INT_STATUS_REG) & ADMA_COMPLETE) == ADMA_COMPLETE ) {
		admaCh1Ctr++;
		WriteRegDword( ADMA_CHAN_1_INT_STATUS_REG, 0x00000000 );
	}

	return;
}

/*
 * AudioHwConfigure
 *
 * Configures the underlying auddio hardware subsystem.
 *
 * @param channel_count - the number of audio channels.
 * @param sample_frequency - the audio sampling frequency.
 * @param sample_size - the audio bits per channel.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int AudioHwConfigure(
	AudioChannel_t channel_count,
	SampleFreq_t sample_freq,
	SampleSize_t sample_size)
{
	DWORD bitClkDiv;
	DWORD sysClkDiv;
	DWORD numerator;
	DWORD framePeriod;
	DWORD frameWidth;
	DmaDesc_t *dmaDesc;
	BOOL pmuClock;
	DWORD *bufp;
	DWORD i;

    static const RE_PadConfiguration I2SPins = {
	    1,                			// altFunctionMode
	    PAD_DIRECTION_OUTPUT,       // initialDirection
	    FALSE,                      // initialLevel
	    FALSE,                      // pullUpEnable
	    FALSE,                      // pullDownEnable
	    PAD_INTERRUPT_NONE,         // interruptType
	    NULL,                       // interruptHandler
	    FALSE,                      // interruptsInitiallyEnabled
	    FALSE                       // interruptDebounceEnabled
	};

	bufp = (DWORD *)(DMA_DESC_START + (2 * sizeof(DmaDesc_t)));
	for( i=0; i < DMA_SIZE/4; i += 4 ) {
		*(bufp+i) = 0xAAAAAAAA;
		*(bufp+i+1) = 0xBBBBBBBB;
		*(bufp+i+2) = 0xCCCCCCCC;
		*(bufp+i+3) = 0xDDDDDDDD;
	}

	/* determine appropriate pll parameters: sysclk = 128Fs, bitclk = 64Fs */
	switch( sample_freq ) {
		case SF_32KHZ:
			/* set PMU M/N PLL to 24.9165MHz (M to 4) */
			pmuClock = TRUE;
			bitClkDiv = 12;
			sysClkDiv = 1;
			numerator = 4;
			break;
		case SF_44P1KHZ:
			/* set Audio PLL to 33.8688MHz */
			pmuClock = FALSE;
			bitClkDiv = 12;
			sysClkDiv = 1;
			break;
		case SF_48KHZ:
			/* set PMU M/N PLL to 49.833MHz (M to 2) */
			pmuClock = TRUE;
			bitClkDiv = 16;
			sysClkDiv = 1;
			numerator = 2;
			break;
		case SF_88P2KHZ:
			/* set Audio PLL clock to 33.8688MHz */
			pmuClock = FALSE;
			bitClkDiv = 6;
			sysClkDiv = 1;
			break;
		case SF_96KHZ:
			/* set PMU M/N PLL to 49.833MHz (M to 2) */
			pmuClock = TRUE;
			bitClkDiv = 8;
			sysClkDiv = 1;
			numerator = 2;
			break;
		case SF_176P4KHZ:
			/* set Audio PLL clock to 33.8688MHz */
			pmuClock = FALSE;
			bitClkDiv = 3;
			sysClkDiv = 1;
			break;
		case SF_192KHZ:
			/* set PMU M/N PLL to 49.833MHz (M to 2) */
			pmuClock = TRUE;
			bitClkDiv = 4;
			sysClkDiv = 1;
			numerator = 2;
			break;
		default:
			PRINTN( "Unsupported sampling rate: %d", sample_freq );
			return (-1);
	}

	/* setup frame period and frame sync based on audio channel configuration */
	if( channel_count == AC_2CH ) {
		/* stereo 2 channels: 64 bits frame-period, 32 bit frame-sync */
		framePeriod = 64;
		frameWidth = 32;
	} else if( (channel_count == AC_8CH) && (sample_freq != SF_88P2KHZ) && (sample_freq != SF_176P4KHZ) ) {
		PRINT( "8 Channel Layout ");
		/* TDM 7+1: 256 bits frame-period, 1 bit frame-sync */
		framePeriod = 256;
		frameWidth = 1;
		/* adjust clock dividers: sysclk = 256Fs, bitclk = 256Fs */
		bitClkDiv /= 4;
		sysClkDiv = bitClkDiv;
	} else {
		PRINT2N( "Unsupported number of audio channels: %d (Fs=%d)", channel_count, sample_freq );
		return (-1);
	}

	/* setup audio dma descriptor chains for channel 1 */
	dmaDesc = (DmaDesc_t *) DMA_DESC_START;
	dmaDesc->ByteCount = DMA_SIZE / 2;
	dmaDesc->SrcAddr = SSPA_RX_DATA_REG;
	dmaDesc->DstAddr = DMA_DST_ADDR;
	dmaDesc->Next = (DWORD)(DMA_DESC_START + sizeof(DmaDesc_t));
	dmaDesc = (DmaDesc_t *) dmaDesc->Next;
	dmaDesc->ByteCount = DMA_SIZE / 2;
	dmaDesc->SrcAddr = SSPA_RX_DATA_REG;
	dmaDesc->DstAddr = DMA_DST_ADDR + (DMA_SIZE / 2);
	dmaDesc->Next = (DWORD) DMA_DESC_START;

	/* setup audio dma descriptor chains for channel 0 */
	dmaDesc = (DmaDesc_t *)(DMA_DESC_START + (2 * sizeof(DmaDesc_t)));
	dmaDesc->ByteCount = DMA_SIZE / 2;
	dmaDesc->SrcAddr = DMA_SRC_ADDR;
	dmaDesc->DstAddr = SSPA_TX_DATA_REG;
	dmaDesc->Next = (DWORD)(dmaDesc + sizeof(DmaDesc_t));
	dmaDesc = (DmaDesc_t *) dmaDesc->Next;
	dmaDesc->ByteCount = DMA_SIZE / 2;
	dmaDesc->SrcAddr = DMA_SRC_ADDR + (DMA_SIZE / 2);
	dmaDesc->DstAddr = SSPA_TX_DATA_REG;
	dmaDesc->Next = (DWORD)(DMA_DESC_START + (2 * sizeof(DmaDesc_t)));

    /* configure SSPA I/O pads */
    RE_PadConfigure( I2S_SYSCLK, &I2SPins, NULL );
    RE_PadConfigure( I2S_BITCLK, &I2SPins, NULL );
    RE_PadConfigure( I2S_SYNC, &I2SPins, NULL );
    RE_PadConfigure( I2S_DATA_OUT, &I2SPins, NULL );
    RE_PadConfigure( I2S_DATA_IN, &I2SPins, NULL );
    RE_PadSet( I2S_DATA_IN, PAD_INPUT_ENABLE );

    /* enable the SSPA module clock */
	WriteRegDwordOr( PMUA_AUDIO_CTRL_REG, AUDIO_PWR_UP );
	WriteRegDwordOr( PMUA_AUDIO_CTRL_REG, AUDIO_CLK_EN );
	WriteRegDwordOr( PMUA_AUDIO_CTRL_REG, AUDIO_ISB_DIS );
	WriteRegDwordOr( PMUA_AUDIO_CTRL_REG, AUDIO_CLK_RST );

	/* configure pll */
	if( pmuClock ) {
		/* disable audio PLL */
		WriteRegDword( SSPA_AUD_PLL_CTRL0_REG, 0x00000000 );
		WriteRegDword( SSPA_AUD_PLL_CTRL1_REG, 0x00000000 );
		/* enable I2S clock output of the PMU M/N to SSPA1 */
		WriteRegDwordOr( PMUM_CGR_SP_REG, PMUM_I2SCLK1_EN );
		WriteRegDword( PMUM_ISSCR1_REG, PMUM_SYSCLK_EN | PMUM_SYSCLK_PLL1DIV2 | PMUM_DENOM(1) | PMUM_NOM(numerator) );
		/* configure audio sampling frequency */
		WriteRegDword( SSPA_AUD_CTRL0_REG, SSPA1_CLK_I2S1 );
		WriteRegDwordOr( SSPA_AUD_CTRL0_REG, SSPA1_CLK_DIV2_PREE(bitClkDiv) | SSPA1_CLK_DIV1_PREE(sysClkDiv) );
		WriteRegDwordOr( SSPA_AUD_CTRL0_REG, SSPA1_CLK_DIV2_EN | SSPA1_CLK_DIV1_EN );
	} else {
		/* disable I2S clock output of the PMU M/N to SSPA1 */
		WriteRegDword( PMUM_ISSCR1_REG, 0x00000000 );
		WriteRegDwordAnd( PMUM_CGR_SP_REG, ~PMUM_I2SCLK1_EN );
		/* enable audio PLL @33.8688 MHz */
		WriteRegDword( SSPA_AUD_PLL_CTRL0_REG, DIV_OCLK_MODULO(0) | FRACT(0x08A18) | ENA_DITHER | PLL_PWR_UP );
		WriteRegDword( SSPA_AUD_PLL_CTRL1_REG, AUD_CLK_SEL | DIV_OCLK_PATTERN(0) );
		while( (ReadRegDword( SSPA_AUD_PLL_CTRL1_REG ) & AUD_PLL_LOCK) != AUD_PLL_LOCK )
			PRINT( "Waiting for Audio PLL to Lock ..." );
		/* configure audio sampling frequency */
		WriteRegDword( SSPA_AUD_CTRL0_REG, SSPA1_CLK_DIV2_PREE(bitClkDiv) | SSPA1_CLK_DIV1_PREE(sysClkDiv) );
		WriteRegDwordOr( SSPA_AUD_CTRL0_REG, SSPA1_CLK_DIV2_EN | SSPA1_CLK_DIV1_EN );
	}

	/* configure audio ssp for i2s: 1 bit data delay, 32 bits per channel, left justified */
	WriteRegDword( SSPA_TX_SP_CTRL_REG, SSPA_FFLUSH | SSPA_S_RST );
	WriteRegDword( SSPA_TX_SP_CTRL_REG, SSPA_WEN | SSPA_FPER(framePeriod) | SSPA_FWID(frameWidth) );
	WriteRegDword( SSPA_TX_CTRL_REG, SSPA_DATDLY_1BIT | SSPA_WDLEN_32BITS | SSPA_FRLEN(channel_count) | SSPA_SSZ(sample_size) );
	WriteRegDword( SSPA_TX_FIFO_LL_REG, 0x00000001 );
//	WriteRegDword( SSPA_TX_INT_MSK_REG, SSPA_FF_INT_MASK );
	WriteRegDword( SSPA_RX_SP_CTRL_REG, SSPA_FFLUSH | SSPA_S_RST );
	WriteRegDword( SSPA_RX_SP_CTRL_REG, SSPA_WEN | SSPA_MSL_INTERNAL | SSPA_FPER(framePeriod) | SSPA_FWID(frameWidth) );
	WriteRegDword( SSPA_RX_CTRL_REG, SSPA_DATDLY_1BIT | SSPA_WDLEN_32BITS | SSPA_FRLEN(channel_count) | SSPA_SSZ(sample_size) );
	WriteRegDword( SSPA_RX_FIFO_UL_REG, 0x00000000 );
//	WriteRegDword( SSPA_RX_INT_MSK_REG, SSPA_FF_INT_MASK );

	/* configure transmitter to use audio dma channel 0 */
	WriteRegDword( ADMA_CHAN_0_NEXT_PTR_REG, DMA_DESC_START + (2 * sizeof(DmaDesc_t)) );
	WriteRegDword( ADMA_CHAN_0_INT_MASK_REG, ADMA_COMPLETE );
	WriteRegDword( ADMA_CHAN_0_CTRL_REG, ADMA_SSZ(sample_size) | ADMA_PACKMOD | ADMA_FETCHND | ADMA_DESTDIR_HOLD );
	WriteRegDword( ADMA_CHAN_0_INT_STATUS_REG, 0x00000000 );

	/* configure receiver to use audio dma channel 1 */
	WriteRegDword( ADMA_CHAN_1_NEXT_PTR_REG, DMA_DESC_START );
	WriteRegDword( ADMA_CHAN_1_INT_MASK_REG, ADMA_COMPLETE );
	WriteRegDword( ADMA_CHAN_1_CTRL_REG, ADMA_SSZ(sample_size) | ADMA_PACKMOD | ADMA_FETCHND | ADMA_SRCDIR_HOLD );
	WriteRegDword( ADMA_CHAN_1_INT_STATUS_REG, 0x00000000 );

	/* register interrupts */
//    RE_RegisterIntHandler( HWIRQ_SSPA1, SspaIsr );
    RE_RegisterIntHandler( HWIRQ_ADMA_CHANNEL0, AdmaIsr );
    RE_RegisterIntHandler( HWIRQ_ADMA_CHANNEL1, AdmaIsr );

	return (DRIVER_NO_ERROR);
}

/*
 * AudioHwTransmit
 *
 * Starts or stops signal transmission of the underlying
 * Audio hardware controller.
 *
 * @param transmit - a boolean flag which determines if the
 * 				     hardware will start or will stop signal
 * 					 transmission.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int AudioHwTransmit(BOOL transmit)
{
	if( transmit ) {

		/* start audio reception and transmission */
		WriteRegDwordOr( SSPA_RX_SP_CTRL_REG, SSPA_S_EN );
		WriteRegDwordOr( SSPA_TX_SP_CTRL_REG, SSPA_S_EN );

		/* enable ADMA channels: 1 for receiver, 0 for transmitter */
		WriteRegDwordOr( ADMA_CHAN_0_CTRL_REG, ADMA_CHANEN );
		WriteRegDwordOr( ADMA_CHAN_1_CTRL_REG, ADMA_CHANEN );

		/* enable audio dma interrupts for channel 0 and 1 */
	    RE_EnableInterrupt( HWIRQ_ADMA_CHANNEL0 );
	    RE_EnableInterrupt( HWIRQ_ADMA_CHANNEL1 );

	} else {

		/* stop audio reception and transmission */
		WriteRegDwordAnd( SSPA_RX_SP_CTRL_REG, ~SSPA_S_EN );
		WriteRegDwordAnd( SSPA_TX_SP_CTRL_REG, ~SSPA_S_EN );
		WriteRegDword( SSPA_RX_SP_CTRL_REG, SSPA_FFLUSH | SSPA_S_RST );
		WriteRegDword( SSPA_TX_SP_CTRL_REG, SSPA_FFLUSH | SSPA_S_RST );

		/* abort transfers of ADMA channels: 1 for receiver, 0 for transmitter */
		WriteRegDwordOr( ADMA_CHAN_0_CTRL_REG, ADMA_ABR );
		WriteRegDwordOr( ADMA_CHAN_1_CTRL_REG, ADMA_ABR );
		WriteRegDwordAnd( ADMA_CHAN_0_CTRL_REG, ~ADMA_CHANEN );
		WriteRegDwordAnd( ADMA_CHAN_1_CTRL_REG, ~ADMA_CHANEN );
		while( ((ReadRegDword( ADMA_CHAN_0_CTRL_REG ) & ADMA_CHANACT) == ADMA_CHANACT) ||
			   ((ReadRegDword( ADMA_CHAN_1_CTRL_REG ) & ADMA_CHANACT) == ADMA_CHANACT) ) {
			PRINT( "Waiting for Audio DMA channels to abort transfers" );
		}

		/* disable audio dma interrupts for channel 0 and 1 */
	    RE_DisableInterrupt( HWIRQ_ADMA_CHANNEL0 );
	    RE_DisableInterrupt( HWIRQ_ADMA_CHANNEL1 );
		WriteRegDword( ADMA_CHAN_0_INT_STATUS_REG, 0x00000000 );
		WriteRegDword( ADMA_CHAN_1_INT_STATUS_REG, 0x00000000 );

		/* disable audio PLL */
		WriteRegDword( SSPA_AUD_PLL_CTRL0_REG, 0x00000000 );
		WriteRegDword( SSPA_AUD_PLL_CTRL1_REG, 0x00000000 );

		/* disable I2S clock output of the PMU M/N to SSPA1 */
		WriteRegDword( PMUM_ISSCR1_REG, 0x00000000 );
		WriteRegDwordAnd( PMUM_CGR_SP_REG, ~PMUM_I2SCLK1_EN );
	}

	return (DRIVER_NO_ERROR);
}
