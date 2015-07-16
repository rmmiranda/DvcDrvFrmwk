/*****************************************************************************
* Filename:    disphal_tavorpv2.c
*
* Copyright 2009, Research In Motion Ltd
*
* Author:      Victor Chen
*
* Created:     May 26th, 2009
*
* Description: This file implements the HW routines (HW specific code) to 
*              support the DSI/Display driver.
*
*
*****************************************************************************/
/* include header files */

// Register Definitions
#include "disphal_tavorpv2.h"

#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE FILE_DISP_HAL

// GPIOs for LCD subsystem (move to the beginning)
static const PadConfiguration dispGpioOutput = {
            0,                            // altFunctionMode: GPIO
            PAD_DIRECTION_OUTPUT,        // initialDirection
            FALSE,                        // initialLevel
            FALSE,                        // pullUpEnable
            FALSE,                        // pullDownEnable
            PAD_INTERRUPT_NONE,         // interruptType
            NULL,                        // interruptHandler
            FALSE,                        // interruptsInitiallyEnabled
            FALSE                        // interruptDebounceEnabled
};
    

/* Externs */
extern WORD BqHugeLdoVoltage( WORD num, WORD value );
extern void BqHugeLdoEnable( WORD ldo );
extern void BqHugeLdoDisable( WORD ldo );


// Set LCD (Display and DSI controllers) functional/interface clock 
void tavorpv2_lcdSetFIClock(BYTE dsi0LinkClock, BYTE dsi1LinkClock)
{
    DWORD regVal = 0;

    // Display Controller functional clock
    WriteRegDwordOr(D0CKEN_A_REG32, D0CKEN_A_LCD);

    // Display Controller functional clock
    regVal = D0CKENB_VIDEO_ACC | D0CKENB_GCU | DOCKENB_WIRE | D0CKENB_HSIO;
    //WriteRegDwordOr(D0CKEN_B, 0x0FC0);
    WriteRegDwordOr(D0CKEN_B_REG32, regVal);
    
    // Enable DSI0/DSI1, System Bus Clock fucntional clocks
    regVal = D0CKENC_DSITX2_ESC | D0CKENC_DSITX1_ESC 
             |D0CKENC_DISPLAY | D0CKENC_PIXEL 
              | D0CKENC_DSI_BIT2 | D0CKENC_DSI_BIT1 
             | D0CKENC_SYS_BUS;

    if (dsi0LinkClock == DSI_LINK_CLK_UNDEF)
        regVal = regVal & (~D0CKENC_DSI_BIT1);
    if (dsi1LinkClock == DSI_LINK_CLK_UNDEF)
        regVal = regVal & (~D0CKENC_DSI_BIT2);
        
    WriteRegDwordOr(D0CKEN_C_REG32, regVal);

    regVal = 0;
     // ACCR1 for DSIs link clock frequnecy - set to 624/3 = 208 Mhz for Z0 board, A0 board canbe set to 312Mhz
     if (dsi0LinkClock != DSI_LINK_CLK_UNDEF)
         regVal |= (ACCR1_DSI_BIT1(dsi0LinkClock));
     if (dsi1LinkClock != DSI_LINK_CLK_UNDEF)
         regVal |= (ACCR1_DSI_BIT2(dsi1LinkClock));

     WriteRegDwordOr(ACCR1_REG32, regVal);

     // Enable AXI32 and get DSI/Display out of reset state before access any other registers 
     WriteRegDword(LCD_CTL,LCD_AXI32_EN);
}

// Set DSI controllers interface clock 
void tavorpv2_dsiSetIntClock(DWORD dsiId, BYTE dsiLinkClock)
{
    DWORD regVal = 0;

    if ( dsiId > 1)
        return;         

    if (dsiLinkClock == DSI_LINK_CLK_UNDEF) 
        return;

     // Mask original clock settings
     if (dsiId == 0)
         regVal = ~(ACCR1_DSI_BIT1(0x07));
     else
         regVal = ~(ACCR1_DSI_BIT2(0x07));
     WriteRegDwordAnd(ACCR1_REG32, regVal);
     
     // ACCR1 for DSIs link clock frequnecy - set to 624/3 = 208 Mhz for Z0 board, A0 board canbe set to 312Mhz
     regVal = 0;
     if (dsiId == 0)
         regVal |= (ACCR1_DSI_BIT1(dsiLinkClock));
     else
         regVal |= (ACCR1_DSI_BIT2(dsiLinkClock));

     WriteRegDwordOr(ACCR1_REG32, regVal);
}


void tavorpv2_lcdSetGpios()
{
    /* DSI reference voltage level workaround for Caprock REV2 board: Caprock REV2 board is using BqHuge LDO3 as
          MIPI reference voltage and the default value is 1800mv.  So adjust LDO3 output to be 1250 mv to make it falls 
          into [1100mv 1300mv] as MIPI spec requires.
       */

    //LdoEnable(LDO_AUDDIG);
    //LdoSetLevel(LDO_AUDDIG, 1250);
    // Disable LDO3
    BqHugeLdoDisable(3);
    RE_RimSleep(3);
    // Set to 1250mv
    BqHugeLdoVoltage(3,1250);
    // Enable LDO3
    BqHugeLdoEnable(3);
    RE_RimSleep(3);

    //Reset Signal required for TPO LCD Panel           
    PadConfigure( DISP_LCD_RST_N, & dispGpioOutput,  NULL );
    // Set to HI and keeps 120ms
    PadSet(DISP_LCD_RST_N, PAD_OUTPUT);
    RE_HwDelay(120*RE_MILLISECOND);
    // Set to LO and keeps 6us-10us
    PadClear(DISP_LCD_RST_N, PAD_OUTPUT);
    RE_HwDelay(10*RE_MICROSECOND);

    // Finally set to HI
    PadSet(DISP_LCD_RST_N, PAD_OUTPUT);
    RE_HwDelay(120*RE_MILLISECOND);
	
    //Toggle GPIO_74      
    PadConfigure( GPIO_74, & dispGpioOutput,  NULL );
    // Set to HI and keeps 120ms
    PadSet(GPIO_74, PAD_OUTPUT);

    // Toggle GPIO_75          
    PadConfigure( GPIO_75, & dispGpioOutput,  NULL );
    // Set to HI and keeps 120ms
    PadSet(GPIO_75, PAD_OUTPUT);

    return;
}    

void tavorpv2_lcdResx(int ms)
{
    //Reset Signal required for TPO LCD Panel           
    PadConfigure( DISP_LCD_RST_N, & dispGpioOutput,  NULL );
    // Set to HI and keeps 120ms
    PadSet(DISP_LCD_RST_N, PAD_OUTPUT);
    RE_HwDelay(10*RE_MILLISECOND);
    // Set to LO and keeps 6us-10us
    PadClear(DISP_LCD_RST_N, PAD_OUTPUT);
    //RE_HwDelay(20*RE_MICROSECOND);
    RE_HwDelay(3*RE_MILLISECOND);

    // Finally set to HI and keeps 5 millisecond
    PadSet(DISP_LCD_RST_N, PAD_OUTPUT);
    RE_HwDelay(5*RE_MILLISECOND);

}


//Minimal Initialization for Marvell DSI controller
void tavorpv2_lcdInitDsi(DWORD dsiId)
{

    if ( dsiId > 1)
        return;         

    // Write "1" to conv_en bit in DxSCR0
    WriteRegDwordOr(DSI_DxSCR0(dsiId), DSI_DxSCR0_SP_RD_INT_EN | DSI_DxSCR0_CONV_EN);

    // Clear interrupt status
    WriteRegDword(DSI_DxINST0(0), 0xffffffff);
    WriteRegDword(DSI_DxINST0(1), 0xffffffff);
    WriteRegDword(DSI_DxINST1(0), 0xffffffff);
    WriteRegDword(DSI_DxINST1(1), 0xffffffff);

    // Disable all DSI interrupts     
    WriteRegDword(DSI_DxINEN(dsiId), 0x00000000);

    //  Enable "smart panel read" interupt
    WriteRegDwordOr(DSI_DxINEN(0), DSI_DxINEN_SP_RD_INT_EN);
    WriteRegDwordOr(DSI_DxINEN(1), DSI_DxINEN_SP_RD_INT_EN);
     
    // Set to be bi-directional panel and set active lane number to 1 by default. 
    WriteRegDwordOr(DSI_DxSCR1(dsiId), DSI_DxSCR1_PT | (DSI_DxSCR1_NOL(0x00)));

    // Enable DSI phy with DxSCR1[DSI_EN]=1. 
    WriteRegDwordOr(DSI_DxSCR1(dsiId), DSI_DxSCR1_DSI_EN );

    // Read sp_unndrun_int_sts bit from DxINST0, write back the value so that if it was set from before enable, it will be cleared.
    WriteRegDwordOr(DSI_DxINST0(dsiId), SP_UNDRUN_INT_STS & (ReadRegDword(DSI_DxINST0(1))));
    
    // Enable the "Smart Panel Under Run" and "Smart Panel Read", "Start of Frame" "End of Frame"  interrupt 
    WriteRegDwordOr(DSI_DxSCR0(dsiId), DSI_DxSCR0_SP_UNDRUN_INT_EN | DSI_DxSCR0_SP_RD_INT_EN 
                                       | DSI_DxSCR0_DISP_SOF_INT_EN | DSI_DxSCR0_DISP_EOF_INT_EN);

    // Enable ACK_ERR_EN and DSI under Run Interrupt
    WriteRegDwordOr(DSI_DxINEN(dsiId), DSI_DxINEN_ACK_ERR_EN);

    // Always set to asserte a 50uA reference to generate a 200mv reference
    WriteRegDwordOr(DSI_DxPHY_CAL(dsiId),DSI_DxPHY_CAL_MIPI_BG_VREF_EN);

    //Set GEN_REG4 (0x4240_4078)(10:8 -0b101) as Marvell suggested
    WriteRegDwordOr(0x42404078, 0x00000300);

    return; 
}

// Eanble/Disable one of DSI controllers 
BOOL tavorpv2_dsiCtlEnable(DWORD dsiId, DWORD enFlag)
{
    BOOL rc = TRUE;
    DWORD regVal = 0;
    DWORD regTimeOut = 0;

    if ( dsiId > 1)
        return FALSE;

    // Get satus
    regVal = ReadRegDword(DSI_DxSSR(dsiId));  

    switch (enFlag)
    {
        // Eanble LCD controller
        case DSI_CTL_ENABLE:
            // Determine if enabled or not
            if ( !(regVal & DSI_DxSSR_DSI_EN_STAT))
            {
                WriteRegDwordOr(DSI_DxSCR1(dsiId), DSI_DxSCR1_DSI_EN);    
                // Enable the corresponding converter
                WriteRegDwordAnd(DSI_DxSCR0(dsiId), ~DSI_DxSCR0_CONV_EN);     
            }
            break;
        // Fully Disable DSIs including Disable LCD controller
        case DSI_CTL_DISABLE:
            // Determine if enabled or not
            if ( regVal & DSI_DxSSR_DSI_EN_STAT)
            {
                // 1. Break Out of DSI infinite loop
                WriteRegDwordOr(DSI_DxSCR0(dsiId), DSI_DxSCR0_BREAK);
                // 2. Wait for interrupt
                do {
                    regVal = BREAK_INT_STS & (ReadRegDword(DSI_DxINST0(dsiId)));
                    if (regVal)
                        break;
                    RE_HwDelay(RE_MICROSECOND);
                    regTimeOut += RE_MICROSECOND;
                } while ( regTimeOut <= TAVORPV2_REG_TIMEOUT);

                // 3. Disable the converter and clear "break"
                WriteRegDwordAnd(DSI_DxSCR0(dsiId), ~(DSI_DxSCR0_CONV_EN | DSI_DxSCR0_BREAK));     

                // Clear SP/DISP under flow interrupt
                WriteRegDwordOr(DSI_DxINST0(dsiId), SP_UNDRUN_INT_STS );
                WriteRegDwordOr(DSI_DxINST0(dsiId), DISP_URUN_INT_STS );

                // 4. Disable DSI controller
                WriteRegDwordAnd(DSI_DxSCR1(dsiId), ~DSI_DxSCR1_DSI_EN);    
            }    
            break;
        default:
            rc = FALSE;
            break;
    }
        
    return rc;
    
}


// Enable/Disable LCD controller : disable LCD controller will also disable DSI controller
BOOL tavorpv2_dispCtlEnable(DWORD enFlag)
{
    BOOL rc = TRUE;
    DWORD regVal = 0;
    DWORD regTimeOut = 0;

	regVal = (ReadRegDword(LCD_CTL));
	PRINT2N("tavorpv2_dispCtlEnable: enFlag=0x%08x, reg=0x%08x",enFlag, regVal);
	

    // Clear Global Interrupt Status Register 
    regVal = LCD_EN_INT_STS | LCD_DIS_INT_STS | LCD_QD_INT_STS;
    WriteRegDwordOr(LCD_CTL_INT_STS, regVal);
    
    switch (enFlag)
    {
        case DISP_CTL_ENABLE:
            // Check if already in the required state
            regVal = (ReadRegDword(LCD_CTL));
            if ((regVal & LCD_EN) && (!(regVal & LCD_QD)))
                goto cleanUp;

            // Enable interrupt if not
            // if (!(regVal & LCD_EN_INT_EN))
            //    WriteRegDwordOr(LCD_CTL, LCD_EN_INT_EN);
            // Enable LCD_CTL
            // WriteRegDwordOr(LCD_CTL, LCD_EN);

            // Clear "LCD_EN_INT_STS"  interrupt
            WriteRegDwordOr(LCD_CTL_INT_STS, LCD_EN_INT_STS);
			
            // Set All interrupts enabled by default
            regVal = LCD_EN_INT_EN | LCD_QD_INT_EN | LCD_DIS_INT_EN | LCD_AXI32_EN | LCD_GMIX_INT_EN | LCD_GWIN_INT_EN | LCD_GFETCH_INT_EN;
            // Enable Lcd
            regVal |= LCD_EN;
            WriteRegDword(LCD_CTL, regVal);
            // Wait for EN interrupt
            do {
                regVal = LCD_EN_INT_STS & (ReadRegDword(LCD_CTL_INT_STS));
                if (regVal)
                    break;
                RE_HwDelay(RE_MICROSECOND);
                regTimeOut += RE_MICROSECOND;
            } while ( regTimeOut <= TAVORPV2_REG_TIMEOUT);
                
            if (regTimeOut > TAVORPV2_REG_TIMEOUT) 
            {
                WARN("Enable Display Controller Timed Out");
            }    
            break;

        case DISP_CTL_DISABLE:

            // Disable all mixers
            tavorpv2_dispCtlMixerEnable(0, FALSE);
            tavorpv2_dispCtlMixerEnable(1, FALSE);

            // Check if already in the required state
            regVal = LCD_EN & (ReadRegDword(LCD_CTL));
            if (!regVal)
                goto cleanUp;

            // Clear "LCD_DIS_INT_STS"  interrupt before setting "disable" 
            WriteRegDwordOr(LCD_CTL_INT_STS, LCD_DIS_INT_STS);
            
            // Set All interrupts enabled by default
            // regVal = LCD_EN_INT_EN | LCD_QD_INT_EN | LCD_DIS_INT_EN | LCD_AXI32_EN | LCD_GMIX_INT_EN | LCD_GWIN_INT_EN | LCD_GFETCH_INT_EN;
            // WriteRegDword(LCD_CTL, regVal);
            WriteRegDwordOr(LCD_CTL, LCD_DIS_INT_EN);
            // Clear LCD_EN bit
            WriteRegDwordAnd(LCD_CTL, ~(LCD_EN | LCD_QD));
            // Wait for LCD_CTL_DIS interrupt
            do {
                regVal = LCD_DIS_INT_STS & (ReadRegDword(LCD_CTL_INT_STS));
                if (regVal)
                    break;
                RE_HwDelay(RE_MICROSECOND);
                regTimeOut += RE_MICROSECOND;
            } while ( regTimeOut <= TAVORPV2_REG_TIMEOUT);

            if (regTimeOut > TAVORPV2_REG_TIMEOUT) 
            {
                WARN("Disable Display controller INT Timed Out");
            }    

            regTimeOut = 0;
            break;
        // Less systematic shutdown - for entry into deep low-power modes
        case DISP_CTL_QDISABLE:
           // Check if already in the required state
           regVal = (ReadRegDword(LCD_CTL));
           // Already disabled?
           if (!(regVal & LCD_EN))
               goto cleanUp;

            // Clear "LCD_DIS_INT_STS"  interrupt before setting "disable" 
            WriteRegDwordOr(LCD_CTL_INT_STS, LCD_QD_INT_STS);

            // Set All interrupts enabled by default
            regVal = LCD_QD | LCD_QD_INT_EN;
            //regVal &= ~LCD_EN;
            
            // regVal = LCD_EN_INT_EN | LCD_QD_INT_EN | LCD_DIS_INT_EN | LCD_AXI32_EN | LCD_GMIX_INT_EN | LCD_GWIN_INT_EN | LCD_GFETCH_INT_EN;
            // Quick Disable
            WriteRegDwordOr(LCD_CTL, regVal);

            // Wait for LCD_CTL_DIS interrupt
            do {
                regVal = LCD_QD_INT_STS & (ReadRegDword(LCD_CTL_INT_STS));
                if (regVal)
                    break;
                RE_HwDelay(RE_MICROSECOND);
                regTimeOut += RE_MICROSECOND;
            } while ( regTimeOut <= TAVORPV2_REG_TIMEOUT);

            if (regTimeOut > TAVORPV2_REG_TIMEOUT) 
            {
                WARN("Quick Disable Display controller INT Timed Out");
                // Clear LCD_CTL[LCD_QD] bit for Z1
                WriteRegDwordAnd(LCD_CTL, ~(LCD_QD|LCD_EN));
            }

			// Clear LCD_CTL[LCD_QD] bit for Z1
			//WriteRegDwordAnd(LCD_CTL, ~LCD_QD);

            break;
        default:
            rc = FALSE;
            break;
    }

cleanUp:
    return rc;
}



// Issue/Clear a Bus-Turn-Around request
BOOL tavorpv2_dsiIssueBTA(DWORD dsiId, DWORD enFlag)
{
    DWORD regVal = 0;
    
    if (enFlag == TRUE)
        WriteRegDwordOr(DSI_DxSCR1(dsiId), DSI_DxSCR1_BTA);
    else
        WriteRegDwordAnd(DSI_DxSCR1(dsiId), ~DSI_DxSCR1_BTA);

    // Check if DSI link direction reset
    RE_HwDelay(10*RE_MICROSECOND);
    regVal = DSI_DxSSR_DIR & (ReadRegDword(DSI_DxSSR(dsiId)));
    if (!regVal)
        return TRUE;

    // Wait one more time for direction to reverse for 100 microseconds if not reset
    RE_HwDelay(100*RE_MICROSECOND);
    // Check if DSI link direction reset
    regVal = DSI_DxSSR_DIR & (ReadRegDword(DSI_DxSSR(dsiId)));
    if (!regVal)
        return TRUE;
    else
    {
        WARN("BTA(Bus-Turn-Around) time Out");
    }     
    return (FALSE);
}


//Get current DSI link direction: Forword (0) or Receiver (1)
DWORD tavorpv2_dsiGetLinkDirect(DWORD dsiId)
{
   DWORD regVal = 0;
   regVal = ReadRegDword(DSI_DxSSR(dsiId));
   if (regVal & DSI_DxSSR_DIR)
           return 1;
   else
           return 0;
}


// Issue/Clear a Bus-Turn-Around request
DWORD tavorpv2_dsiPerformBTA(DWORD dsiId)
{
    
#define DSI_BTA_TRIG   (0x1u<<30)
#define DSI_BTA_ACK    (0x1u<<31)

    DWORD regVal, regTimeOut = 0;
    regVal = ReadRegDword(DSI_DxPRSR(dsiId));
    // WriteRegDwordOr(DSI_DxPRSR(dsiId), DSI_DxPRSR_CON_NT | DSI_DxPRSR_ST_OK);

    // Wait for EN interrupt
    do {
       regVal = SP_RD_INT_STS & (ReadRegDword(DSI_DxINST0(dsiId)));
       if (regVal)
           break;
       RE_HwDelay(RE_MICROSECOND);
       regTimeOut += RE_MICROSECOND;
   } while ( regTimeOut <= TAVORPV2_REG_TIMEOUT);
       
   if (regTimeOut > TAVORPV2_REG_TIMEOUT) 
   {
       WARN("Wait for Smart Panel Read command interupt Timed Out");
       return 0;
   }    


    // Check if system hang here because of BTA
    // Check if a "read command" has been executed
    if (SP_RD_INT_STS & ReadRegDword(DSI_DxINST0(dsiId)))
    {
         WARN("SP_RD_INT triggered.");
        // Clear interrupt
        WriteRegDwordOr(DSI_DxINST0(dsiId), SP_RD_INT_STS);
        if ( DSI_DxSSR_STOP_ST & ReadRegDword(DSI_DxSSR(dsiId)))
        {
            // System hang, issue a BTA manually
            WARN("Manually BTA issued.");
            WriteRegDwordOr(DSI_DxSCR1(dsiId),DSI_DxSCR1_BTA);
        }
        else
        WriteRegDwordOr(DSI_DxPRSR(dsiId), DSI_DxPRSR_CON_NT | DSI_DxPRSR_ST_OK);
    }

    WriteRegDwordOr(DSI_DxPRSR(dsiId), DSI_DxPRSR_CON_NT | DSI_DxPRSR_ST_OK);
    
    return (regVal);


}




/* Set DSI to one of Power Modes - HS/LP/ULPS */
BOOL tavorpv2_dsiCtlSetPowerMode(DWORD dsiId, DWORD powerMode)
{
    BOOL rc = FALSE;
    if (dsiId > 1) return rc;

    switch (powerMode) 
    {
    case DSI_POWERMODE_LPDT:
        WriteRegDwordOr(DSI_DxSCR1(dsiId), DSI_DxSCR1_LPDT);
        break;
    case DSI_POWERMODE_HS:
        WriteRegDwordAnd(DSI_DxSCR1(dsiId), ~(DSI_DxSCR1_LPDT));              
        break;
    case DSI_POWERMODE_HS_RIHS:
        WriteRegDwordAnd(DSI_DxSCR1(dsiId), ~(DSI_DxSCR1_LPDT));
        WriteRegDwordOr(DSI_DxSCR1(dsiId), DSI_DxSCR1_RIHS);
        break;
    case DSI_POWERMODE_HS_NORIHS:
        WriteRegDwordAnd(DSI_DxSCR1(dsiId), ~(DSI_DxSCR1_LPDT));
        WriteRegDwordAnd(DSI_DxSCR1(dsiId), ~DSI_DxSCR1_RIHS);
        break;
    case DSI_POWERMODE_ENTER_ULPS:
        WriteRegDwordOr(DSI_DxSCR1(dsiId), DSI_DxSCR1_ULPS);            
        break;
    case DSI_POWERMODE_EXIT_ULPS:
        WriteRegDwordAnd(DSI_DxSCR1(dsiId), ~DSI_DxSCR1_ULPS);             
        break;
    default:
        return rc;
    }
    rc = TRUE;
    return rc;
}


/* DSI PHY Timer setttings by Marvell recommended starting value (See Application note "PXA950 CSI and DSI D-PHY Tuning Guidelines")*/
BOOL tavorpv2_dsiCtlSetPhyTimer(DWORD dsiId, DWORD dsiLinkClock)
{
    BOOL rc = FALSE;

    // Same settings for G-Timer and BTA 
    //DxTIM0
    WriteRegDword(DSI_DxTIM0(dsiId), 0x1000FFFF);
    //DxTIM1: G-TIMER and BTA timeout value
    WriteRegDword(DSI_DxTIM1(dsiId), 0xFFFFFFFF);
    
    // DSI D-PHY Timing settings based on DSI bit clock
    if (dsiLinkClock == DSI_LINK_CLK_624M)
    {
        // Starting value from Marvell
        WriteRegDword(DSI_DxPHY_TIM0(dsiId), 0x0803060F);
        WriteRegDword(DSI_DxPHY_TIM1(dsiId), 0x14101105);
        WriteRegDword(DSI_DxPHY_TIM2(dsiId), 0x00180100);

    }
    // 312 Mbps
    if (dsiLinkClock == DSI_LINK_CLK_312M)
    {
        WriteRegDword(DSI_DxPHY_TIM0(dsiId), 0x0803060F);
        WriteRegDword(DSI_DxPHY_TIM1(dsiId), 0x14101105);
        WriteRegDword(DSI_DxPHY_TIM2(dsiId), 0x00180100);

    }
    // 208 Mbps
    else if (dsiLinkClock == DSI_LINK_CLK_208M)
    {
        WriteRegDword(DSI_DxPHY_TIM0(dsiId), 0x08030709);
        WriteRegDword(DSI_DxPHY_TIM1(dsiId), 0x14101105);
        WriteRegDword(DSI_DxPHY_TIM2(dsiId), 0x00180100);

    }
    // 156 Mbps
    else if (dsiLinkClock == DSI_LINK_CLK_156M)
    {
        WriteRegDword(DSI_DxPHY_TIM0(dsiId), 0x0804080A);
        WriteRegDword(DSI_DxPHY_TIM1(dsiId), 0x14101105);
        WriteRegDword(DSI_DxPHY_TIM2(dsiId), 0x00180100);

    }

    // 104 Mbps
    else if (dsiLinkClock == DSI_LINK_CLK_104M)
    {
        WriteRegDword(DSI_DxPHY_TIM0(dsiId), 0x08050905);
        WriteRegDword(DSI_DxPHY_TIM1(dsiId), 0x1410030A);
        WriteRegDword(DSI_DxPHY_TIM2(dsiId), 0x00180100);

    }
    // 52Mhz    
    else if (dsiLinkClock == DSI_LINK_CLK_52M)
    {
        WriteRegDword(DSI_DxPHY_TIM0(dsiId), 0x08070C08);
        WriteRegDword(DSI_DxPHY_TIM1(dsiId), 0x14101105);
        WriteRegDword(DSI_DxPHY_TIM2(dsiId), 0x00180100);

    }
    else
        return rc;

    rc = TRUE;
    return rc;

}

/* Set Active Data Lanes - 1 to 4 */
BOOL tavorpv2_dsiCtlSetActiveLane(DWORD dsiId, DWORD activeLanes)
{
    BOOL rc = FALSE;
    if (dsiId > 1 || activeLanes > 4 || activeLanes < 1) return rc;
    WriteRegDwordAnd(DSI_DxSCR1(dsiId), ~(DSI_DxSCR1_NOL_MASK));
    WriteRegDwordOr(DSI_DxSCR1(dsiId), ((DSI_DxSCR1_NOL(activeLanes - 1)) & DSI_DxSCR1_NOL_MASK));

    return TRUE;
}

#if defined(TAVORPV2_HWVER_MG1)        
/* Set Pixel Format of DSI when DSI DCS/Generic Long Wirte commands are used*/
BOOL tavorpv2_dsiCtlSetVcFomat(DWORD dsiId, DWORD vcNumber, DispPixelFormat srcFormat)
{
    BOOL rc = FALSE;
    BYTE pixelFormat;
    if (dsiId > 1 || vcNumber > 3) return rc;

    if (srcFormat ==  DISP_PIXELFRT_RGB24)
        pixelFormat = 0x03;
    else if (srcFormat ==  DISP_PIXELFRT_RGB24_unpacked)
        pixelFormat = 0x02;
    else if (srcFormat ==  DISP_PIXELFRT_RGB18)
        pixelFormat = 0x01;
    else if (srcFormat ==  DISP_PIXELFRT_RGB16)
        pixelFormat = 0x00;
    else
        return rc;
    
    // configure output pixel format based on VC number
    switch (vcNumber)
    {
        // VC3
        case 3:
            WriteRegDwordAnd(DSI_DxCONV_GEN_NULL_BLANK(dsiId), ~(DSI_DxCONV_GEN_PXL_FORMAT_3(DSI_DxCONV_PXL_FORMAT_MASK)));
            WriteRegDwordOr(DSI_DxCONV_GEN_NULL_BLANK(dsiId), DSI_DxCONV_GEN_PXL_FORMAT_3(pixelFormat));
            break;
        // VC2
        case 2:
            WriteRegDwordAnd(DSI_DxCONV_GEN_NULL_BLANK(dsiId), ~(DSI_DxCONV_GEN_PXL_FORMAT_2(DSI_DxCONV_PXL_FORMAT_MASK)));
            WriteRegDwordOr(DSI_DxCONV_GEN_NULL_BLANK(dsiId), DSI_DxCONV_GEN_PXL_FORMAT_2(pixelFormat));
            break;
        case 1:
        case 0:    
            WriteRegDwordAnd(DSI_DxCONV_GEN_NULL_BLANK(dsiId), ~(DSI_DxCONV_GEN_PXL_FORMAT(DSI_DxCONV_PXL_FORMAT_MASK)));
            WriteRegDwordOr(DSI_DxCONV_GEN_NULL_BLANK(dsiId), DSI_DxCONV_GEN_PXL_FORMAT(pixelFormat));
            break;
        default:
            return rc;
            break;
    }

    rc = TRUE;
    return rc;
}
#endif



// Set VC Mappings based on LCD panels connected with DSI
BOOL tavorpv2_dsiCtlSetVcMap(DWORD dsiId, DWORD panels)
{
    if (dsiId > 1) return FALSE;
    //Disable VC if only one panel connected
    switch (panels) {
        case 0:
            //do nothing here
            break;
        case 1:    
            WriteRegDwordAnd(DSI_DxSCR1(dsiId), ~(DSI_DxSCR1_VC_EN));
            WriteRegDwordOr(DSI_DxSCR0(dsiId), DSI_DxSCR0_PRIM_VC);
            break;
        case 2:
            // Enable VC - need to think about VC mapping more carefully based on LCD panel' number
            WriteRegDwordOr(DSI_DxSCR1(dsiId), DSI_DxSCR1_VC_EN);
            WriteRegDwordOr(DSI_DxSCR0(dsiId), DSI_DxSCR0_PRIM_VC);
            //Disable the other DSI's PRIM_VC if this DSI interface has 2 LCD panels connected     
            if (dsiId == 0)
                WriteRegDwordAnd(DSI_DxSCR0(dsiId + 1), ~DSI_DxSCR0_PRIM_VC);
            else
                WriteRegDwordAnd(DSI_DxSCR0(dsiId - 1), ~DSI_DxSCR0_PRIM_VC);

            // Set Primary converter as VC0 and Secondary converter as VC1 
            WriteRegDwordOr(DSI_DxSCR1(dsiId), DSI_DxSCR1_VC0(00));
            WriteRegDwordOr(DSI_DxSCR1(dsiId), DSI_DxSCR1_VC1(01));
            break;
            // One DSI with over 3 panels are not supported
        default:
            return FALSE;
    }


    // Always set Panel type to be bi-directional
    WriteRegDwordOr(DSI_DxSCR1(dsiId), DSI_DxSCR1_PT);
    
    return TRUE;
}


/* Enable/Disable Mixer and set Display Update Control Bit */
void tavorpv2_dispCtlMixerEnable(DWORD mixerId, BOOL enabled)
{
    //   pDisplayDriver    
    if ((mixerId != 0) && (mixerId != 1)) return;

    switch (enabled) {
        case TRUE:
            //Enable Mixer
            if (mixerId == 0)
                WriteRegDwordOr(LCD_MIXERx_CTL0(0), LCD_MIXERx_CTL0_MIX_EN | LCD_MIXERx_CTL0_DISP_UP);
            else 
                WriteRegDwordOr(LCD_MIXERx_CTL0(1), LCD_MIXERx_CTL0_MIX_EN | LCD_MIXERx_CTL0_DISP_UP);
            break;
        case FALSE:
            //Disable
            if (mixerId == 0)
                WriteRegDwordAnd(LCD_MIXERx_CTL0(0), ~((DWORD) LCD_MIXERx_CTL0_MIX_EN));
            else 
                WriteRegDwordAnd(LCD_MIXERx_CTL0(1), ~((DWORD) LCD_MIXERx_CTL0_MIX_EN));
            break;
        default:
            break;
    }
    return;
}


/* Set output device resolutions */
void tavorpv2_dispSetMixRes(DWORD mixerId, DWORD width, DWORD height)
{
    //   pDisplayDriver    
    if ((mixerId != 0) && (mixerId != 1)) return;

     WriteRegDwordOr(LCD_MIXERx_CTL1(mixerId),LCD_MIXERx_CTL1_DISP_XRES(width) | LCD_MIXERx_CTL1_DISP_YRES(height));

    return;
}

/* Set Mixer output device ID - converter ID (DSI0, DSI1, Paralell etc) */
void tavorpv2_dispSetMixConvId(DWORD mixerId, MixerConverterID converterId)
{
    // there are only 2 mixer units    
    if ((mixerId != 0) && (mixerId != 1)) 
        return;

    WriteRegDword(LCD_MIXERx_CTL2(mixerId), LCD_MIXERx_CTL2_CONV_ID(converterId));

    return;
}


/* Timing Configurations for Tavor PV2 Display subsystem */
void tavorpv2_dispConfigureInterfaceTiming(DWORD dsiId, DWORD dsiDxPhyTim0, DWORD dsiDxPhyTim1, DWORD dsiDxPhyTim2)
{

    // TODO - For DSI, need to configure timing for DxPHY_TIMEx (from SVP definition)
    WriteRegDword(DSI_DxPHY_TIM0(dsiId), dsiDxPhyTim0);
    WriteRegDword(DSI_DxPHY_TIM1(dsiId), dsiDxPhyTim1);
    WriteRegDword(DSI_DxPHY_TIM2(dsiId), dsiDxPhyTim2);

    // Marvell recommended starting value for LP_RX_TO/HS_TX_TO/GTIMER/BTA_TO timeout settings
    WriteRegDword(DSI_DxTIM0(dsiId), 0x1000FFFF);
    WriteRegDword(DSI_DxTIM1(dsiId), 0xFFFFFFFF);

    return;
}



// Gamma Correction Configuration here is bypass mode by default for all 3 mixers, i.e. pixels pass mixer won't be changed
void tavorpv2_dispCtlSetDefaultGamma()
{
    DWORD regVal, i = 0;    
    DWORD RedLinearLUT[] = {0x30201000, 0x70605040, 0xb0a09080,0xf0e0d0c0,0x000000ff};
    DWORD GreenLinearLUT[] = {0x30201000, 0x70605040, 0xb0a09080, 0xf0e0d0c0,0x000000ff};    
    DWORD BlueLinearLUT[] = {0x30201000, 0x70605040, 0xb0a09080, 0xf0e0d0c0,0x000000ff}; 

    //set GAMMA correction to linear
    for(i=0;i<5;i++)
    { 
        regVal = (MIXERx_G_CTL_ADD_PTR_R(i))
            |(MIXERx_G_CTL_ADD_PTR_G(i))
            |(MIXERx_G_CTL_ADD_PTR_B(i));
        WriteRegDword(LCD_MIXER0_G_CTL, regVal);
        WriteRegDword(DSI_Dx_G_CTL(0), regVal);
        WriteRegDword(DSI_Dx_G_CTL(1), regVal);
        //Red
        WriteRegDword(LCD_MIXER0_G_DAT_RED, RedLinearLUT[i]);
        WriteRegDword(DSI_Dx_G_DAT_RED(0), RedLinearLUT[i]);
        WriteRegDword(DSI_Dx_G_DAT_RED(1), RedLinearLUT[i]);
        //Green
        WriteRegDword(LCD_MIXER0_G_DAT_GREEN, GreenLinearLUT[i]);
        WriteRegDword(DSI_Dx_G_DAT_GREEN(0), GreenLinearLUT[i]);
        WriteRegDword(DSI_Dx_G_DAT_GREEN(1), GreenLinearLUT[i]);
        //Blue
        WriteRegDword(LCD_MIXER0_G_DAT_BLUE, BlueLinearLUT[i]);
        WriteRegDword(DSI_Dx_G_DAT_BLUE(0), BlueLinearLUT[i]);
        WriteRegDword(DSI_Dx_G_DAT_BLUE(1), BlueLinearLUT[i]);
    }

    // Set to entires per quadrant
    regVal = (MIXERx_G_CTL_Q4_C(0))
        |(MIXERx_G_CTL_Q3_C(0))
        |(MIXERx_G_CTL_Q2_C(0))
        |(MIXERx_G_CTL_Q1_C(0));
    WriteRegDwordOr(LCD_MIXER0_G_CTL, regVal);
    WriteRegDwordOr(DSI_Dx_G_CTL(0), regVal);
    WriteRegDwordOr(DSI_Dx_G_CTL(1), regVal);

    return;
}

BOOL tavorpv2_dispCtlSetFetchUnit(DispDmaChannel dmaCh, DWORD dmaAddr, DispPixelFormat srcFormat)
{
    DWORD timeOut = 0;
    DWORD regVal = 0;
    
    // Fetch Unit Register address is based on fectch DMA channel
    // Set DMA fectch address
    WriteRegDword(LCD_NXT_DESC_ADDR(dmaCh), (dmaAddr & 0xFFFFFFF0));

    // Enable channel, configure as Smart Panel Mode and SRC_FOR - (only support Smart Panel now)
    regVal = CHAN_EN | SP_MODE | SRC_FOR(srcFormat) 
             | BUS_ERR_INT_EN | START_FR_INT_EN | END_FR_INT_EN;

    //regVal = CHAN_EN | SRC_FOR(srcFormat) 
    //         | BUS_ERR_INT_EN | START_FR_INT_EN | END_FR_INT_EN;
    
    WriteRegDword(LCD_FETCH_CTL(dmaCh), regVal);

    // Wait for status bit to be set
    do {
        regVal = CHAN_STS & (ReadRegDword(LCD_FETCH_STS(dmaCh)));
        if (regVal)
            break;
        HwDelay(RE_MICROSECOND);    
        timeOut += RE_MICROSECOND;
    } while (timeOut <= TAVORPV2_REG_TIMEOUT);
    
    if (timeOut > TAVORPV2_REG_TIMEOUT) 
    {
        WARN("TavorPV2: Set DMA Fetch Timed Out");
    }    
        
    return TRUE;
}

/* Set Display controller to use Smart Panel mode */
void tavorpv2_dispCtlSetSpMode(DispDmaChannel dmaCh, BOOL enFLag)
{
    DWORD regVal = 0;
    
    if (enFLag)
		WriteRegDwordOr(LCD_FETCH_CTL(dmaCh), SP_MODE);
	else
		WriteRegDwordAnd(LCD_FETCH_CTL(dmaCh), ~SP_MODE);

}

// Start fetching a new frame from system frame buffer by setting update flag
void tavorpv2_dispCtlStartFetch(DWORD dmaCh, BOOL enFlag)
{
    if (enFlag)
    {
        // Enable channel and Start fetching new frame
        WriteRegDwordOr(LCD_FETCH_CTL(dmaCh), CHAN_EN | SP_MODIFIED_FRAME);
    }
    else
        // Disable channel
        WriteRegDwordAnd(LCD_FETCH_CTL(dmaCh), ~(CHAN_EN));
}


// Set Windows Generation Unit (Window Size, Alpha Value)
BOOL tavorpv2_dispCtlSetWindowUnit(DispOverlay winGeId, WORD width, WORD height)
{
    DWORD regVal = 0;

    // Set resolution of source image (divided by 4 here)
    regVal = WIN_XRES(width/4) | WIN_YRES(height/4);

    // Enable all interrupt
    regVal |= WIN_SOF_INT_EN | WIN_EOF_INT_EN | WIN_URUN_INT_EN;
    
    //TODO:
    WriteRegDword(LCD_WINx_CTL(winGeId),regVal);

    // Enable Windows-Specific alpha
    if (!winGeId)
        // WinGen 0 is used as baseplane
        WriteRegDword(LCD_CHx_ALPHA(winGeId), 0x800000ff);
    else
        //Others used as  Overlay
        WriteRegDword(LCD_CHx_ALPHA(winGeId), 0x800000ff);

    return TRUE;
}


// Set Mixer Unit (output device resolution, baseplace or overlay)
BOOL tavorpv2_dispCtlSetMixerUnit(DWORD mixerId, WORD width, WORD height, DWORD x, DWORD y, DWORD overlayId)
{
    DWORD regVal = 0;
    
    // Overlay ID : 0 - baseplane, n - overlay n (1 - 3)
    if (overlayId > 3 ) return FALSE;

    // Maximum 3 mixers     
    if (mixerId > 1) return FALSE;

    PRINT4N( "tavorpv2_dispCtlSetMixerUnit: mixerId=0x%04x width=0x%04x heigth=0x%04x overlayId=0x%02x",
            mixerId, width, height, overlayId );

    switch (overlayId)
    {
        // Baseplane
        case 0:
            // Display Resolution
            WriteRegDword(LCD_MIXERx_CTL1(mixerId),LCD_MIXERx_CTL1_DISP_XRES(width) | LCD_MIXERx_CTL1_DISP_YRES(height));
            // Position
            WriteRegDword(LCD_MIXERx_BP_CFG0(mixerId), LCD_MIXERx_CFG0_XPOS(x) | LCD_MIXERx_CFG0_YPOS(y));
            // Resolution of Baseplane (make it as same as dispaly resolution)
            WriteRegDword(LCD_MIXERx_BP_CFG1(mixerId),LCD_MIXERx_CFG1_XRES(width) | LCD_MIXERx_CFG1_YRES(height));
            // Mask BP_ID and test mode
            WriteRegDwordAnd(LCD_MIXERx_CTL0(mixerId),~(LCD_MIXERx_CTL0_BP_ID_MASK | LCD_MIXERx_CTL0_BP_COLOR_USER));
            regVal = LCD_MIXERx_CTL0_BP_ID(MIXER_CONV_ID_WGE0);
            // Set Mixer output device ID - converter ID (DSI0, DSI1, Paralell etc)
            if (mixerId == 1)
                WriteRegDword(LCD_MIXERx_CTL2(mixerId), LCD_MIXERx_CTL2_CONV_ID(MIXER_CONV_ID_S_CONV1));
            else
                WriteRegDword(LCD_MIXERx_CTL2(mixerId), LCD_MIXERx_CTL2_CONV_ID(MIXER_CONV_ID_S_CONV0));
            break;
        case 1:
            // Position
            WriteRegDword(LCD_MIXERx_OL1_CFG0(mixerId), LCD_MIXERx_CFG0_XPOS(x) | LCD_MIXERx_CFG0_YPOS(y));
            // Resolution of Baseplane (make it as same as dispaly resolution)
            WriteRegDword(LCD_MIXERx_OL1_CFG1(mixerId),LCD_MIXERx_CFG1_XRES(width) | LCD_MIXERx_CFG1_YRES(height));
            // Mask OL1_ID
            WriteRegDwordAnd(LCD_MIXERx_CTL0(mixerId),~(LCD_MIXERx_CTL0_OL1_ID_MASK | LCD_MIXERx_CTL0_BP_COLOR_USER));
            regVal = LCD_MIXERx_CTL0_OL1_ID(MIXER_CONV_ID_WGE1);
            break;
        case 2:
            // Position
            WriteRegDword(LCD_MIXERx_OL2_CFG0(mixerId), LCD_MIXERx_CFG0_XPOS(x) | LCD_MIXERx_CFG0_YPOS(y));
            // Resolution of Baseplane (make it as same as dispaly resolution)
            WriteRegDword(LCD_MIXERx_OL2_CFG1(mixerId),LCD_MIXERx_CFG1_XRES(width) | LCD_MIXERx_CFG1_YRES(height));
            // Mask OL1_ID
            WriteRegDword(LCD_MIXERx_CTL0(mixerId),~(LCD_MIXERx_CTL0_OL2_ID_MASK | LCD_MIXERx_CTL0_BP_COLOR_USER));
            regVal = LCD_MIXERx_CTL0_OL1_ID(MIXER_CONV_ID_WGE2);
            break;
        case 3:
            // Position
            WriteRegDword(LCD_MIXERx_OL3_CFG0(mixerId), LCD_MIXERx_CFG0_XPOS(x) | LCD_MIXERx_CFG0_YPOS(y));
            // Resolution of Baseplane (make it as same as dispaly resolution)
            WriteRegDword(LCD_MIXERx_OL3_CFG1(mixerId),LCD_MIXERx_CFG1_XRES(width) | LCD_MIXERx_CFG1_YRES(height));
            // Mask OL1_ID
            WriteRegDword(LCD_MIXERx_CTL0(mixerId),~(LCD_MIXERx_CTL0_OL3_ID_MASK | LCD_MIXERx_CTL0_BP_COLOR_USER));
            regVal = LCD_MIXERx_CTL0_OL1_ID(MIXER_CONV_ID_WGE3);
            break;
        default:
            return FALSE;
            break;
    }

    // Color    : Yellow, Alpha: 0xFF
    // WriteRegDwordOr(LCD_MIXERx_BP_CLR(mixerId), 0xFFFFFF00);

    // Set client ID, enable mixer and update flag
    WriteRegDwordOr(LCD_MIXERx_CTL0(mixerId), regVal | LCD_MIXERx_CTL0_MIX_EN | LCD_MIXERx_CTL0_DISP_UP);
   
    return TRUE;
}

void tavorpv2_dsiWriteCmd2Fifo(DWORD dsiId, DWORD cmdEntry)
{
    if (dsiId >= TAVORPV2_DSI_CTL) 
    {
        WARN("Only 2 DSI controllers are supported!");
        return;
    }

    // PRINT2N("tavorpv2_dsiWriteCmd2Fifo:    Write command (0x%08x) to DSI(%d) Command FIFO (DSI_DxCFIF)", cmdEntry, dsiId);

    // Write to DSI Command FIFO
    WriteRegDword(DSI_DxCFIF(dsiId), cmdEntry);
}

BOOL tavorpv2_dsiInsertDly(DWORD dsiId, DWORD value)
{
    if (dsiId >= TAVORPV2_DSI_CTL) 
    {
        WARN("Only 2 DSI controllers are supported!");
        return FALSE;
    }

    WriteRegDword(DSI_DxCFIF(dsiId), value);    

    return TRUE;
}


void tavorpv2_dsiSetFlowCtronl(DWORD dsiId, BOOL enFlag)
{
    if (enFlag)
        WriteRegDwordAnd(DSI_DxSCR0(dsiId), ~DSI_DxSCR0_FLOW_DIS);    
    else
        WriteRegDwordOr(DSI_DxSCR0(dsiId), DSI_DxSCR0_FLOW_DIS);    
}

/* Set/Clear "break" feature of DSI start/goto lable */
void tavorpv2_dsiSetLoopBreak(DWORD dsiId, BOOL enFlag)
{
    if (enFlag)
        WriteRegDwordOr(DSI_DxSCR0(dsiId), DSI_DxSCR0_BREAK);    
    else
        WriteRegDwordAnd(DSI_DxSCR0(dsiId), ~DSI_DxSCR0_BREAK);    
}


/* Stop periodic traffic (with start/goto label) on DSI link (Jump out of loop buffer, flush DSI command FIFO) */
void tavorpv2_dsiStopTraffic(DWORD dsiId)
{
    DWORD regVal = 0;
    DWORD regTimeOut = 0;

    // Check if DSI link is still in running state
    regVal = DSI_DxSSR_STOP_ST & (ReadRegDword(DSI_DxSSR(dsiId)));
    // DSI_DxSSR_STOP_ST can't be used to determine DSI link activity
    //if ( !regVal)
    if (TRUE)
    {
        // Clear  Break interrupt status
        WriteRegDwordOr(DSI_DxINST0(dsiId), BREAK_INT_STS);
       
        regVal = DSI_DxSCR0_BREAK | DSI_DxSCR0_SP_BREAK_INT_EN;

        // Trigger a "break" out of DSI command FIFO  loop operation
        WriteRegDwordOr(DSI_DxSCR0(dsiId), regVal);

        // Wait for BREAK_INT_STS bit
        do {
            regVal = BREAK_INT_STS & (ReadRegDword(DSI_DxINST0(dsiId)));
            if (regVal)
                break;
            RE_HwDelay(RE_MICROSECOND);
            regTimeOut += RE_MICROSECOND;
        } while ( regTimeOut <= TAVORPV2_REG_TIMEOUT);

        if (regTimeOut > TAVORPV2_REG_TIMEOUT) 
        {
            WARN("Break out of DSI Command FIFO time Out");
        }

        else
        {
            PRINT("Beak out of loop and continue next command in DSI command FIFO");
            // Clear interrupt
            WriteRegDwordOr(DSI_DxINST0(dsiId), BREAK_INT_STS);
            //Execute next command in the  DSI command FIFO 
            WriteRegDwordOr(DSI_DxPRSR(dsiId), DSI_DxPRSR_ST_OK | DSI_DxPRSR_CON_NT);
        }    

    }
    return;
}


// Set Output Trigger Code
BOOL tavorpv2_dsiSetTrigCode(DWORD dsiId, DsiTrigCode dsiTrigCode)
{

    BYTE trigCode = 0x00;

    switch (dsiTrigCode)
    {
        case DSI_TRIG_CODE_RESET:
            trigCode = PV2_DSI_TRIGCODE_RESET;
            break;
        case DSI_TRIG_CODE_UNKNOWN5:
            trigCode = PV2_DSI_TRIGCODE_UNKNOWN5;
            break;
        case DSI_TRIG_CODE_TE:
            trigCode = PV2_DSI_TRIGCODE_TE;
            break;
        case DSI_TRIG_CODE_ACK:
            trigCode = PV2_DSI_TRIGCODE_ACK;
            break;
        default:
            break;
    }
    if (trigCode == 0)
        return (FALSE);
        
    // Set to DxTRIG
    WriteRegDwordAnd(DSI_DxTRIG(dsiId), 0xFF000000);
    WriteRegDwordOr(DSI_DxTRIG(dsiId), (0x00000000 | trigCode));

    // Send trigger message to the panel by setting DSCR0[TRIG] bit
    WriteRegDwordOr(DSI_DxSCR1(dsiId), DSI_DxSCR1_TRIG);

    return (TRUE);
}

// Get Input Trigger Code - Should be read when there is TRIG_RCVD interrupt happened
BYTE tavorpv2_dsiGetTrigCode(DWORD dsiId)
{

    DWORD regVal = 0;

    // Set to DxTRIG
    regVal = ReadRegDword(DSI_DxTRIG(dsiId));

    return ((regVal & 0xFF000000) >> 24);
}


// Workaround for sending DSI command sequence before enabling DSI
void tavorpv2_dsiStart(DWORD dsiId)
{
    DWORD regVal = 0;

    //PRINT("Enable DSI, Exit ULPS, Enable LCD, and wait Proc Int!");

    // Test if DSI enabled or not, if not, enable it first
    regVal = ReadRegDword(DSI_DxSCR1(dsiId));
    if (!(regVal & DSI_DxSCR1_DSI_EN)) {
		PRINT("tavorpv2_dsiStart: Enable DSI");
        WriteRegDwordOr(DSI_DxSCR1(dsiId), DSI_DxSCR1_DSI_EN);
    }	

    // 10 Read sp_unndrun_int_sts bit from DxINST0, write back the value so that if it was set from before enable, it will be cleared.
    regVal = SP_UNDRUN_INT_STS & (ReadRegDword(DSI_DxINST0(dsiId)));
    WriteRegDwordOr(DSI_DxINST0(dsiId), regVal);

    // Enable the SP_UNDRUN_INT interrupt and SP_RD_INT_EN (smart panel read operation generate an interrupt) 
    WriteRegDwordOr(DSI_DxSCR0(dsiId), DSI_DxSCR0_SP_UNDRUN_INT_EN | DSI_DxSCR0_SP_RD_INT_EN);

    // If ULPS Mode, exit ULPS (return back in "int proc" handler)
    regVal = ReadRegDword(DSI_DxSCR1(dsiId));
    if (regVal & DSI_DxSCR1_ULPS)
        // Clear to exit ULPS mode
        WriteRegDwordAnd(DSI_DxSCR1(dsiId), ~DSI_DxSCR1_ULPS);

    // 11.  Enable LCD engine if not enabled
	regVal = (ReadRegDword(LCD_CTL));
    if (!(regVal & LCD_EN)) 
	{ 
        PRINTN("tavorpv2_dsiStart: enable lcd ctl! 0x%08x",regVal);
        tavorpv2_dispCtlEnable(DISP_CTL_ENABLE);
	}

    return;
}



// Dump Display/DSI controllers and related Clock Configuration Registers
void tavorpv2_lcdRegsDump(DWORD regSet)
{
    DWORD i = 0;
    
    PRINT("================= TavorPV2 Debug: Dump Display/DSI/Clock Registers =================");
    if ((regSet == 1) || ( regSet == 0))
    {
        PRINT("================= Clock Setting Registers =================");
        PRINT2N("                    D0CKEN_A(0x%08X)           = %08X", 0x4134000C, (*((volatile DWORD *)0x4134000C)));
        PRINT2N("                    D0CKEN_B(0x%08X)           = %08X", 0x41340010, (*((volatile DWORD *)0x41340010)));
        PRINT2N("                    D0CKEN_C(0x%08X)           = %08X", 0x41340024, (*((volatile DWORD *)0x41340024)));
        PRINT2N("                    ACCR1(0x%08X)              = %08X", 0x41340020, (*((volatile DWORD *)0x41340020)));
    }
    // DSI Registers
    if ((regSet == 2) || (regSet == 0))
    {
        for (i = 0; i < TAVORPV2_DSI_CTL; i++)
        {
            PRINTN("DSI%d Registers:", i);
            PRINT2N("                     DxSCR0(0x%08X)             = %08X", DSI_DxSCR0(i), ReadRegDword(DSI_DxSCR0(i)));
#if defined(TAVORPV2_HWVER_MG1)            
            PRINT2N("                     DxCONV(0x%08X)             = %08X", DSI_DxCONV_GEN_NULL_BLANK(i), ReadRegDword(DSI_DxCONV_GEN_NULL_BLANK(i)));
#endif
            PRINT2N("                     DxSCR1(0x%08X)             = %08X", DSI_DxSCR1(i), ReadRegDword(DSI_DxSCR1(i)));
            PRINT2N("                     DxSSR(0x%08X)              = %08X", DSI_DxSSR(i), ReadRegDword(DSI_DxSSR(i)));
            PRINT2N("                     DxTRIG(0x%08X)             = %08X", DSI_DxTRIG(i), ReadRegDword(DSI_DxTRIG(i)));
            PRINT2N("                     DxINEN(0x%08X)             = %08X", DSI_DxINEN(i), ReadRegDword(DSI_DxINEN(i)));
            PRINT2N("                     DxINST0(0x%08X)            = %08X", DSI_DxINST0(i), ReadRegDword(DSI_DxINST0(i)));
            PRINT2N("                     DxINST1(0x%08X)            = %08X", DSI_DxINST1(i), ReadRegDword(DSI_DxINST1(i)));
            PRINT2N("                     DxTEINTCNT(0x%08X)         = %08X", DSI_DxTEINTCNT(i), ReadRegDword(DSI_DxTEINTCNT(i)));
            PRINT2N("                     Dx_G_CTL(0x%08X)           = %08X", DSI_Dx_G_CTL(i), ReadRegDword(DSI_Dx_G_CTL(i)));
            PRINT2N("                     Dx_G_DAT_RED(0x%08X)       = %08X", DSI_Dx_G_DAT_RED(i), ReadRegDword(DSI_Dx_G_DAT_RED(i)));
            PRINT2N("                     Dx_G_DAT_GREEN(0x%08X)     = %08X", DSI_Dx_G_DAT_GREEN(i), ReadRegDword(DSI_Dx_G_DAT_GREEN(i)));
            PRINT2N("                     Dx_G_DAT_BLUE(0x%08X)      = %08X", DSI_Dx_G_DAT_BLUE(i), ReadRegDword(DSI_Dx_G_DAT_BLUE(i)));
            PRINT2N("                     DxPRSR(0x%08X)             = %08X", DSI_DxPRSR(i), ReadRegDword(DSI_DxPRSR(i)));
            PRINT2N("                     DxADAT(0x%08X)             = %08X", DSI_DxADAT(i), ReadRegDword(DSI_DxADAT(i)));
            PRINT2N("                     DxTIM0(0x%08X)             = %08X", DSI_DxTIM0(i), ReadRegDword(DSI_DxTIM0(i)));
            PRINT2N("                     DxTIM1(0x%08X)             = %08X", DSI_DxTIM1(i), ReadRegDword(DSI_DxTIM1(i)));
            PRINT2N("                     DxPHY_TIM0(0x%08X)         = %08X", DSI_DxPHY_TIM0(i), ReadRegDword(DSI_DxPHY_TIM0(i)));
            PRINT2N("                     DxPHY_TIM1(0x%08X)         = %08X", DSI_DxPHY_TIM1(i), ReadRegDword(DSI_DxPHY_TIM1(i)));
            PRINT2N("                     DxPHY_TIM2(0x%08X)         = %08X", DSI_DxPHY_TIM2(i), ReadRegDword(DSI_DxPHY_TIM2(i)));
            PRINT2N("                     DxPHY_CAL(0x%08X)          = %08X", DSI_DxPHY_CAL(i), ReadRegDword(DSI_DxPHY_CAL(i)));
        }
    }
    if ((regSet == 3) || (regSet == 0))
    {
        // Display Controlelr Registers ((check address is corect later, still has issue))
        PRINT("Display Controller Global Registers:");
        PRINT2N("                   LCD_CTL(0x%08X)                = %08X", LCD_CTL, ReadRegDword(LCD_CTL));
        PRINT2N("                   LCD_CTL_STS(0x%08X)            = %08X", LCD_CTL_STS, ReadRegDword(LCD_CTL_STS));

        PRINT2N("                   LCD_CTL_INT_STS(0x%08X)        = %08X", LCD_CTL_INT_STS, ReadRegDword(LCD_CTL_INT_STS));
        PRINT2N("                   LCD_FETCH_INT_STS0(0x%08X)     = %08X", LCD_FETCH_INT_STS0, ReadRegDword(LCD_FETCH_INT_STS0));
        PRINT2N("                   LCD_FETCH_INT_STS1(0x%08X)     = %08X", LCD_FETCH_INT_STS1, ReadRegDword(LCD_FETCH_INT_STS1));
        PRINT2N("                   LCD_GWIN_INT_STS(0x%08X)       = %08X", LCD_GWIN_INT_STS, ReadRegDword(LCD_GWIN_INT_STS));
        PRINT2N("                   LCD_GMIX_INT_STS(0x%08X)       = %08X", LCD_GMIX_INT_STS, ReadRegDword(LCD_GMIX_INT_STS));
        PRINT2N("                   LCD_CRSR_INT_STS(0x%08X)       = %08X", LCD_CRSR_INT_STS, ReadRegDword(LCD_CRSR_INT_STS));

        // fetch unit channels (check address is corect later, still has issue)
        PRINT("Display Controller Fetch Unit Registers:");
        for ( i = 0; i < 7; i++)
        {
            PRINTN("Fetch Unit: %d", i);
            PRINT2N("                   LCD_FETCH_CTL(0x%08X)          = %08X", LCD_FETCH_CTL(i), ReadRegDword(LCD_FETCH_CTL(i)));
            PRINT2N("                   LCD_FETCH_STS(0x%08X)          = %08X", LCD_FETCH_STS(i), ReadRegDword(LCD_FETCH_STS(i)));
            PRINT2N("                   LCD_NXT_DESC_ADDR(0x%08X)      = %08X", LCD_NXT_DESC_ADDR(i), ReadRegDword(LCD_NXT_DESC_ADDR(i)));
            PRINT2N("                   LCD_FR_ID(0x%08X)              = %08X", LCD_FR_ID(i), ReadRegDword(LCD_FR_ID(i)));
            PRINT2N("                   LCD_FR_ADDR(0x%08X)            = %08X", LCD_FR_ADDR(i), ReadRegDword(LCD_FR_ADDR(i)));
            PRINT2N("                   LCD_CH_CMD(0x%08X)             = %08X", LCD_CH_CMD(i), ReadRegDword(LCD_CH_CMD(i)));
            PRINT2N("                   LCD_FR_BRANCH(0x%08X)          = %08X", LCD_FR_BRANCH(i), ReadRegDword(LCD_FR_BRANCH(i)));
        }
        PRINT2N("                   LCD_CH_ERR_ADDR(0x%08X)        = %08X", LCD_CH_ERR_ADDR, ReadRegDword(LCD_CH_ERR_ADDR));
        PRINT2N("                   LCD_CH_INT_ID(0x%08X)          = %08X", LCD_CH_INT_ID, ReadRegDword(LCD_CH_INT_ID));
        PRINT2N("                   LCD_CLRSPC_COEFF0(0x%08X)      = %08X", LCD_CLRSPC_COEFF0, ReadRegDword(LCD_CLRSPC_COEFF0));
        PRINT2N("                   LCD_CLRSPC_COEFF1(0x%08X)      = %08X", LCD_CLRSPC_COEFF1, ReadRegDword(LCD_CLRSPC_COEFF1));
        PRINT2N("                   LCD_CLRSPC_COEFF2(0x%08X)      = %08X", LCD_CLRSPC_COEFF2, ReadRegDword(LCD_CLRSPC_COEFF2));
        PRINT2N("                   LCD_CLRSPC_COEFF3(0x%08X)      = %08X", LCD_CLRSPC_COEFF3, ReadRegDword(LCD_CLRSPC_COEFF3));
        for ( i = 0; i < 5; i++)
        {
            PRINT3N("                   LCD_CHx_ALPHA%d(0x%08X)         = %08X", i, LCD_CHx_ALPHA(i), ReadRegDword(LCD_CHx_ALPHA(i)));
            PRINT3N("                   LCD_CHx_CLR_MATCH%d(0x%08X)     = %08X", i, LCD_CHx_CLR_MATCH(i), ReadRegDword(LCD_CHx_CLR_MATCH(i)));
           }

        //Windows Generator
        PRINT("Display Controller Window Generation Unit Registers:");
        for ( i = 0; i < 5; i++)
        {
            PRINTN("Window Generation Unit: %d", i);
            PRINT2N("                   LCD_WINx_CTL(0x%08X)           = %08X", LCD_WINx_CTL(i), ReadRegDword(LCD_WINx_CTL(i)));
            PRINT2N("                   LCD_WINx_INT_STS(0x%08X)       = %08X", LCD_WINx_INT_STS(i), ReadRegDword(LCD_WINx_INT_STS(i)));
            PRINT2N("                   LCD_WINx_CFG(0x%08X)           = %08X", LCD_WINx_CFG(i), ReadRegDword(LCD_WINx_CFG(i)));
            PRINT2N("                   LCD_WINx_CROP0(0x%08X)         = %08X", LCD_WINx_CROP0(i), ReadRegDword(LCD_WINx_CROP0(i)));
            PRINT2N("                   LCD_WINx_CROP1(0x%08X)         = %08X", LCD_WINx_CROP1(i), ReadRegDword(LCD_WINx_CROP1(i)));
            PRINT2N("                   LCD_WINx_SCALE_PTR(0x%08X)     = %08X", LCD_WINx_SCALE_PTR(i), ReadRegDword(LCD_WINx_SCALE_PTR(i)));
            PRINT2N("                   LCD_WINx_SCALE_RW(0x%08X)      = %08X", LCD_WINx_SCALE_RW(i), ReadRegDword(LCD_WINx_SCALE_RW(i)));
        }
        
        //Cursor Generator
        PRINT("Display Controller Cursor Generator Registers:");
        PRINT2N("                   LCD_CRSR0_CTRL(0x%08X)         = %08X", LCD_CRSR0_CTRL, ReadRegDword(LCD_CRSR0_CTRL));
        PRINT2N("                   LCD_CRSR0_CLR0(0x%08X)         = %08X", LCD_CRSR0_CLR0, ReadRegDword(LCD_CRSR0_CLR0));
        PRINT2N("                   LCD_CRSR0_CLR1(0x%08X)         = %08X", LCD_CRSR0_CLR1, ReadRegDword(LCD_CRSR0_CLR1));
        PRINT2N("                   LCD_CRSR0_CLR2(0x%08X)         = %08X", LCD_CRSR0_CLR2, ReadRegDword(LCD_CRSR0_CLR2));
        PRINT2N("                   LCD_CRSR0_CLR3(0x%08X)         = %08X", LCD_CRSR0_CLR3, ReadRegDword(LCD_CRSR0_CLR3));
        PRINT2N("                   LCD_CRSR0_PTR(0x%08X)          = %08X", LCD_CRSR0_PTR, ReadRegDword(LCD_CRSR0_PTR));
        PRINT2N("                   LCD_CRSR0_RW(0x%08X)           = %08X", LCD_CRSR0_RW, ReadRegDword(LCD_CRSR0_RW));
        
        // Mixer Unit 0/1/2        
        PRINT("Display Controller Mixer Unit Registers:");
        for ( i = 0; i < 2; i++)
        {
            PRINTN("Mixer Unit: %d", i);
            PRINT2N("                   LCD_MIXERx_CTL0(0x%08X)        = %08X", LCD_MIXERx_CTL0(i), ReadRegDword(LCD_MIXERx_CTL0(i)));
            PRINT2N("                   LCD_MIXERx_CTL1(0x%08X)        = %08X", LCD_MIXERx_CTL1(i), ReadRegDword(LCD_MIXERx_CTL1(i)));
            PRINT2N("                   LCD_MIXERx_CTL2(0x%08X)        = %08X", LCD_MIXERx_CTL2(i), ReadRegDword(LCD_MIXERx_CTL2(i)));
            PRINT2N("                   LCD_MIXERx_BP_CFG0(0x%08X)     = %08X", LCD_MIXERx_BP_CFG0(i), ReadRegDword(LCD_MIXERx_BP_CFG0(i)));
            PRINT2N("                   LCD_MIXERx_BP_CFG1(0x%08X)     = %08X", LCD_MIXERx_BP_CFG1(i), ReadRegDword(LCD_MIXERx_BP_CFG1(i)));
            PRINT2N("                   LCD_MIXERx_OL1_CFG0(0x%08X)    = %08X", LCD_MIXERx_OL1_CFG0(i), ReadRegDword(LCD_MIXERx_OL1_CFG0(i)));
            PRINT2N("                   LCD_MIXERx_OL1_CFG1(0x%08X)    = %08X", LCD_MIXERx_OL1_CFG1(i), ReadRegDword(LCD_MIXERx_OL1_CFG1(i)));
            PRINT2N("                   LCD_MIXERx_OL2_CFG0(0x%08X)    = %08X", LCD_MIXERx_OL2_CFG0(i), ReadRegDword(LCD_MIXERx_OL2_CFG0(i)));
            PRINT2N("                   LCD_MIXERx_OL2_CFG1(0x%08X)    = %08X", LCD_MIXERx_OL2_CFG1(i), ReadRegDword(LCD_MIXERx_OL2_CFG1(i)));
            PRINT2N("                   LCD_MIXERx_OL3_CFG0(0x%08X)    = %08X", LCD_MIXERx_OL3_CFG0(i), ReadRegDword(LCD_MIXERx_OL3_CFG0(i)));
            PRINT2N("                   LCD_MIXERx_OL3_CFG1(0x%08X)    = %08X", LCD_MIXERx_OL3_CFG1(i), ReadRegDword(LCD_MIXERx_OL3_CFG1(i)));
            PRINT2N("                   LCD_MIXERx_CRSR_CFG0(0x%08X)   = %08X", LCD_MIXERx_CRSR_CFG0(i), ReadRegDword(LCD_MIXERx_CRSR_CFG0(i)));
            PRINT2N("                   LCD_MIXERx_STS0(0x%08X)        = %08X", LCD_MIXERx_STS0(i), ReadRegDword(LCD_MIXERx_STS0(i)));
            PRINT2N("                   LCD_MIXERx_BP_CLR(0x%08X)      = %08X", LCD_MIXERx_BP_CLR(i), ReadRegDword(LCD_MIXERx_BP_CLR(i)));
            PRINT2N("                   LCD_MIXERx_OL1_CLR(0x%08X)     = %08X", LCD_MIXERx_OL1_CLR(i), ReadRegDword(LCD_MIXERx_OL1_CLR(i)));
            PRINT2N("                   LCD_MIXERx_OL2_CLR(0x%08X)     = %08X", LCD_MIXERx_OL2_CLR(i), ReadRegDword(LCD_MIXERx_OL2_CLR(i)));
            PRINT2N("                   LCD_MIXERx_OL3_CLR(0x%08X)     = %08X", LCD_MIXERx_OL3_CLR(i), ReadRegDword(LCD_MIXERx_OL3_CLR(i)));
            PRINT2N("                   LCD_MIXERx_INT_STS(0x%08X)     = %08X", LCD_MIXERx_INT_STS(i), ReadRegDword(LCD_MIXERx_INT_STS(i)));
            PRINT2N("                   LCD_MIXERx_STS1(0x%08X)        = %08X", LCD_MIXERx_STS1(i), ReadRegDword(LCD_MIXERx_STS1(i)));
        }
    
    }

}



