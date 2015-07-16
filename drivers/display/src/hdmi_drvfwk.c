/*****************************************************************************
 * Filename:    hdmi_drvfwk.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     May 2010
 *
 * Description: This file implements the common hardware independent code and
 * 				interface to the driver framework for the HDMI driver.
 *
 *****************************************************************************/

#include <string.h>

#include "rei.h"
#include "framework.h"
#include "driver.h"
#include "hdmi.h"

/*
 ***************************************************
 ***************************************************
 *
 * Macro definitions
 *
 ***************************************************
 ***************************************************
 */

#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE  FILE_HDMI_FWK

/*
 ***************************************************
 ***************************************************
 *
 * Function forward declarations
 *
 ***************************************************
 ***************************************************
 */

void DrvFwkHdmiTxDebug(char *cmd, DWORD arg1, DWORD arg2);
void HdmiTxShowRegs(void);


/*
 ***************************************************
 ***************************************************
 *
 * Data structure definitions
 *
 ***************************************************
 ***************************************************
 */

/* Driver internal data structure */
typedef struct {
    /* driver interface data structure instance */
    Driver_t Driver;
    /* required data structure instances for the driver interface */
    HdmiTxStatus_t  Status;
    HdmiTxStats_t   Statistics;
    HdmiTxConfig_t  Config;
    /* driver specific (private) fields */
} HdmiTxDriver_t;

/*
 ***************************************************
 ***************************************************
 *
 * Private global variable declarations
 *
 ***************************************************
 ***************************************************
 */

/* HDMI TX device driver instance */
static HdmiTxDriver_t HdmiTxDriver;

/*
 ***************************************************
 ***************************************************
 *
 * Public global variable declarations
 *
 ***************************************************
 ***************************************************
 */

/*
 ***************************************************
 ***************************************************
 *
 * Private function definitions
 *
 ***************************************************
 ***************************************************
 */

/*
 ***************************************************
 ***************************************************
 *
 * Driver framework interface function definitions
 *
 ***************************************************
 ***************************************************
 */

/*
 * DrvHdmiTxGetStatus
 *
 * Retrieves a pointer to the HDMI transmitter device
 * driver's status information memory location.
 *
 * @param driver - Pointer to the driver framework
 * 				   interface instance.
 *
 * @return The pointer to the driver's status data
 *         structure.
 */
void *DrvHdmiTxGetStatus(Driver_t *driver)
{
    Driver_t *hdmitx_driver;

    /* get handle to the driver interface instance */
    hdmitx_driver = &(HdmiTxDriver.Driver);

    if( hdmitx_driver == driver ) {
        /* return a void pointer to the status structure */
        return (void *)&(HdmiTxDriver.Status);
    }

    /* invalid driver instance found */
    return NULL;
}

/*
 * DrvHdmiTxGetStats
 *
 * Retrieves a pointer to the HDMI transmitter device
 * driver's statistical information memory location.
 *
 * @param driver - Pointer to the driver framework
 * 				   interface instance.
 *
 * @return The pointer to the driver's statistics data
 *         structure.
 */
void *DrvHdmiTxGetStats(Driver_t *driver)
{
    Driver_t *hdmitx_driver;

    /* get handle to the driver interface instance */
    hdmitx_driver = &(HdmiTxDriver.Driver);

    if( hdmitx_driver == driver ) {
        /* return a void pointer to the status structure */
        return (void *)&(HdmiTxDriver.Statistics);
    }

    /* invalid driver instance found */
    return NULL;
}

/*
 * DrvHdmiTxGetConfig
 *
 * Retrieves a pointer to the HDMI transmitter device
 * driver's configuration data memory location.
 *
 * @param driver - Pointer to the driver framework
 * 				   interface instance.
 *
 * @return The pointer to the driver's configuration
 * 		   data structure.
 */
void *DrvHdmiTxGetConfig(Driver_t *driver)
{
    Driver_t *hdmitx_driver;

    /* get handle to the driver interface instance */
    hdmitx_driver = &(HdmiTxDriver.Driver);

    if( hdmitx_driver == driver ) {
        /* return a void pointer to the status structure */
        return (void *)&(HdmiTxDriver.Config);
    }

    /* invalid driver instance found */
    return NULL;
}

/*
 * DrvHdmiTxEnable
 *
 * Activates the HDMI transmitter driver and the HDMI
 * transmitter controller for Platform Software use.
 *
 * @param driver - Pointer to the driver framework
 * 				   interface instance.
 * @param flag   - Currently not used.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int DrvHdmiTxEnable(Driver_t *driver, int flag)
{
    Driver_t *hdmitx_driver;

    /* get handle to the driver interface instance */
    hdmitx_driver = &(HdmiTxDriver.Driver);

    if( hdmitx_driver == driver ) {
        /* enable the HDMI TX task */
        if( HdmiTxEnable(&(HdmiTxDriver.Config)) != DRIVER_NO_ERROR ) {
        	WARN( "Driver could not be enabled." );
        	return (-1);
        }
        /* return successfully */
        return (DRIVER_NO_ERROR);
    }

    /* invalid driver instance found */
    return (-DRIVER_NO_INSTANCE);
}

/*
 * DrvHdmiTxDisable
 *
 * Disables the HDMI transmitter driver and the HDMI
 * transmitter controller.
 *
 * @param driver - Pointer to the driver framework
 * 				   interface instance.
 * @param flag   - Currently not used.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int DrvHdmiTxDisable(Driver_t *driver, int normal)
{
    Driver_t *hdmitx_driver;

    /* get handle to the driver interface instance */
    hdmitx_driver = &(HdmiTxDriver.Driver);

    if( hdmitx_driver == driver ) {
        /* disable the HDMI TX task */
        if( HdmiTxDisable() != DRIVER_NO_ERROR ) {
        	WARN( "Driver could not be enabled." );
        	return (-1);
        }
        /* return successfully */
        return (DRIVER_NO_ERROR);
    }

    /* invalid driver instance found */
    return (-DRIVER_NO_INSTANCE);
}

/*
 * DrvHdmiTxControl
 *
 * Provides an entry point to invoke HDMI transmitter driver
 * specific functions.
 *
 * @param driver  - Pointer to the driver framework
 * 				    interface instance.
 * @param control - Numeric code of a particular driver function.
 * @param args    - Variable list of arguments specific to the
 *                  particular driver function.
 *
 * @return Zero if the operation is successful. Otherwise
 *         returns a negative value whose absolute refers
 *         to a particular error code.
 */
int DrvHdmiTxControl(Driver_t *driver, int control, va_list args)
{
    Driver_t *hdmitx_driver;

    /* get handle to the driver interface instance */
    hdmitx_driver = &(HdmiTxDriver.Driver);

    if( hdmitx_driver == driver ) {
    	switch( control ) {
			case HDMITX_CMD_START_TRANSMIT:
				if( HdmiTxStartTransmit() == DRIVER_NO_ERROR ) {
			        /* return successfully */
			        return (DRIVER_NO_ERROR);
				}
				break;
			case HDMITX_CMD_STOP_TRANSMIT:
				if( HdmiTxStopTransmit() == DRIVER_NO_ERROR ) {
			        /* return successfully */
			        return (DRIVER_NO_ERROR);
				}
				break;
			case HDMITX_CMD_SET_AVMUTE:
				if( HdmiTxAvMute( TRUE ) == DRIVER_NO_ERROR ) {
			        /* return successfully */
			        return (DRIVER_NO_ERROR);
				}
				break;
			case HDMITX_CMD_CLEAR_AVMUTE:
				if( HdmiTxAvMute( FALSE ) == DRIVER_NO_ERROR ) {
			        /* return successfully */
			        return (DRIVER_NO_ERROR);
				}
				break;
			case HDMITX_CMD_CONNECT:
				HdmiTxConnect();
		        /* return successfully */
		        return (DRIVER_NO_ERROR);
			case HDMITX_CMD_DISCONNECT:
				HdmiTxDisconnect();
		        /* return successfully */
		        return (DRIVER_NO_ERROR);
			case HDMITX_CMD_SHOW_REGS:
				HdmiTxHwShowRegs();
		        /* return successfully */
		        return (DRIVER_NO_ERROR);
			default:
				WARNN( "Invalid HDMI TX driver command: %d", control );
				break;
    	}

    	/* invalid driver command */
        return (-1);
    }

    /* invalid driver instance found */
    return (-DRIVER_NO_INSTANCE);
}

/*
 * DrvHdmiTxPollEvents
 *
 * Polls for platform software events and platform software
 * interrupts associated to the HDMI transmitter device driver.
 *
 * @param driver - Pointer to the driver framework
 * 				   interface instance.
 *
 * @return None.
 */
void DrvHdmiTxPollEvents(Driver_t *driver)
{
    Driver_t *hdmitx_driver;

    /* get handle to the driver interface instance */
    hdmitx_driver = &(HdmiTxDriver.Driver);

    if( hdmitx_driver == driver ) {
        /* return successfully */
        return;
    }

    /* return regardless */
    return;
}

/*
 * DrvFwkHdmiTxInit
 *
 * Performs initialization of the framework-specific
 * HDMI TX device driver instance.
 *
 * @param driver - Pointer to the driver framework
 * 				   interface instance.
 *
 * @return None.
 */
Driver_t *DrvFwkDispInit(void)
{
    HdmiTxDriver_t *hdmitx;

    /* get a handle to the driver instance */
    hdmitx = &HdmiTxDriver;

    /* set appropriately all driver status data */
    hdmitx->Status.Dummy = 0;
         
    /* reset all driver statistical data */
    hdmitx->Statistics.Dummy = 0;

    /* initialize driver configuration data */
    hdmitx->Config.DispMode = DM_GRAPHICS;
    hdmitx->Config.DestFormat = VF_720X480P_60HZ_ED;
    hdmitx->Config.SrcFormat = PF_RGB888P;
    hdmitx->Config.AudChanCount = AL_2CH;
    hdmitx->Config.SampleFreq = AF_48KHZ;
    hdmitx->Config.SampleSize = AS_16BIT;

    /* setup the driver interface functions */
    hdmitx->Driver.GetStatus  = DrvHdmiTxGetStatus;
    hdmitx->Driver.GetStats   = DrvHdmiTxGetStats;
    hdmitx->Driver.GetConfig  = DrvHdmiTxGetConfig;
    hdmitx->Driver.Enable     = DrvHdmiTxEnable;
    hdmitx->Driver.Disable    = DrvHdmiTxDisable;
    hdmitx->Driver.Control    = DrvHdmiTxControl;
    hdmitx->Driver.PollEvents = DrvHdmiTxPollEvents;

    /* initialize the HDMI TX task */
    if( HdmiTxInitialize() != DRIVER_NO_ERROR ) {
    	WARN( "Driver could not be initialized." );
    	return (NULL);
    }

    /* register debugger hook */
    PfCmdRegister("hdmi", "HDMI TX Driver", &DrvFwkHdmiTxDebug);

    /* return a pointer to the driver interface structure */
    return ((Driver_t *)&(hdmitx->Driver));
}

extern DWORD admaCh0Ctr, admaCh1Ctr, sspaTxCtr, sspaRxCtr;
/*
 * DrvFwkHdmiTxDebug
 *
 * Provides debugging hooks for use with either the BugDisp
 * or serial debugger.
 *
 * @param cmd  - Pointer to debug command string.
 * @param arg1 - The first argument to the debug command.
 * @param arg2 - The second argument to the debug command.
  *
 * @return None.
 */
void DrvFwkHdmiTxDebug(char *cmd, DWORD arg1, DWORD arg2)
{
    char *dmStr[] = { "Graphics Mode", "Video Mode" };
    char *pfStr[] = { "RGB565", "RGB1555", "RGB888P", "RGB888UP", "RGBA888", "YUV422P", "YUV422PL", "YUV420PL", "", "PALETTE4", "PALETTE8", "RGB888A" };
    char *alStr[] = { "Stream Header", "2 Channels", "3 Channels", "4 Channels", "5 Channels", "6 Channels", "7 Channels", "8 Channels" };
    char *afStr[] = { "Stream Header", "32 KHz", "44.1 KHz", "48 KHz", "88.2 KHz", "96 KHz", "176.4 KHz", "192 KHz" };
    char *asStr[] = { "Stream Header", "16 Bits", "20 Bits", "24 Bits" };

    Driver_t *hdmitx = DrvGetHandle( IODRV_ID_DISPLAY );
    HdmiTxConfig_t *config = DrvGetConfig( hdmitx );

    if( strcmp(cmd, "cfg") == 0 ) {

        PRINT( "HDMI TX Driver Configuration Data:" );
    	PRINTN( " Display Mode        : %s", dmStr[config->DispMode] );
        PRINTN( " Video Format        : %d", config->DestFormat );
        PRINTN( " Source Pixel Format : %s", pfStr[config->SrcFormat] );
        PRINTN( " Audio Channel Count : %s", alStr[config->AudChanCount] );
        PRINTN( " Sampling Frequency  : %s", afStr[config->SampleFreq] );
        PRINTN( " Sampling Size       : %s", asStr[config->SampleSize] );

    } else if( strcmp(cmd, "dm") == 0 ) {

    	config->DispMode = (DisplayMode_t)arg1;
    	PRINTN( "Display %s configured.", dmStr[config->DispMode] );

    } else if( strcmp(cmd, "vf") == 0 ) {

    	config->DestFormat = (VideoFormat_t)arg1;
    	PRINTN( "Deep color mode %d configured.", config->DestFormat );

    } else if( strcmp(cmd, "pf") == 0 ) {

    	if ( arg1 == 0) config->SrcFormat = PF_RGB565;
    	else if ( arg1 == 1) config->SrcFormat = PF_RGB1555;
    	else if ( arg1 == 2) config->SrcFormat = PF_RGB888P;
    	else if ( arg1 == 3) config->SrcFormat = PF_RGB888UP;
    	else if ( arg1 == 4) config->SrcFormat = PF_RGBA888;
    	else if ( arg1 == 5) config->SrcFormat = PF_YUV422P;
    	else if ( arg1 == 6) config->SrcFormat = PF_YUV422PL;
    	else if ( arg1 == 7) config->SrcFormat = PF_YUV420PL;
    	else if ( arg1 == 9) config->SrcFormat = PF_PALETTE4;
    	else if ( arg1 == 10) config->SrcFormat = PF_PALETTE8;
    	else if ( arg1 == 11) config->SrcFormat = PF_RGB888A;
    	else WARNN( "Invalid source pixel format %d.", arg1 );
    	PRINTN( "Color Indicator %s configured.", pfStr[config->SrcFormat] );

    } else if( strcmp(cmd, "al") == 0 ) {

    	if ( arg1 == 0) config->AudChanCount = AL_STREAM;
    	else if ( arg1 == 2) config->AudChanCount = AL_2CH;
    	else if ( arg1 == 3) config->AudChanCount = AL_3CH;
    	else if ( arg1 == 4) config->AudChanCount = AL_4CH;
    	else if ( arg1 == 5) config->AudChanCount = AL_5CH;
    	else if ( arg1 == 6) config->AudChanCount = AL_6CH;
    	else if ( arg1 == 7) config->AudChanCount = AL_7CH;
    	else if ( arg1 == 8) config->AudChanCount = AL_8CH;
    	else WARNN( "Invalid audio channel count %d.", arg1 );
    	PRINTN( "Audio channel count %s configured.", alStr[config->AudChanCount] );

    } else if( strcmp(cmd, "af") == 0 ) {

    	if ( arg1 == 0) config->SampleFreq = AF_STREAM;
    	else if ( arg1 == 1) config->SampleFreq = AF_32KHZ;
    	else if ( arg1 == 2) config->SampleFreq = AF_44P1KHZ;
    	else if ( arg1 == 3) config->SampleFreq = AF_48KHZ;
    	else if ( arg1 == 4) config->SampleFreq = AF_88P2KHZ;
    	else if ( arg1 == 5) config->SampleFreq = AF_96KHZ;
    	else if ( arg1 == 6) config->SampleFreq = AF_176P4KHZ;
    	else if ( arg1 == 7) config->SampleFreq = AF_192KHZ;
    	else WARNN( "Invalid sampling frequency %d.", arg1 );
    	PRINTN( "Sampling frequency %s configured.", afStr[config->SampleFreq] );

    } else if( strcmp(cmd, "as") == 0 ) {

    	if ( arg1 == 0) config->SampleSize = AS_STREAM;
    	else if ( arg1 == 1) config->SampleSize = AS_16BIT;
    	else if ( arg1 == 2) config->SampleSize = AS_20BIT;
    	else if ( arg1 == 3) config->SampleSize = AS_24BIT;
    	else WARNN( "Invalid sampling size %d.", arg1 );
    	PRINTN( "Sampling size %s configured.", asStr[config->SampleSize] );

    } else if( strcmp(cmd, "con") == 0 ) {

        int ret;
        if( arg1 ) {
        	ret = DrvControl( hdmitx, HDMITX_CMD_CONNECT );
            PRINTN( "HDMI TX controller connect %s.", (ret==0)?"completed":"failed" );
        } else {
        	ret = DrvControl( hdmitx, HDMITX_CMD_DISCONNECT );
            PRINTN( "HDMI TX controller disconnect %s.", (ret==0)?"completed":"failed" );
        }

    } else if( strcmp(cmd, "en") == 0 ) {

        int ret;
        if( arg1 ) {
        	ret = DrvEnable( hdmitx, 0 );
            PRINTN( "HDMI TX controller enable operation %s.", (ret==0)?"completed":"failed" );
        } else {
        	ret = DrvDisable( hdmitx, 0 );
            PRINTN( "HDMI TX controller disable operation %s.", (ret==0)?"completed":"failed" );
        }

    } else if( strcmp(cmd, "st") == 0 ) {

        int ret;
        if( arg1 ) {
        	ret = DrvControl( hdmitx, HDMITX_CMD_START_TRANSMIT );
            PRINTN( "HDMI TX controller start transmission %s.", (ret==0)?"completed":"failed" );
        } else {
        	ret = DrvControl( hdmitx, HDMITX_CMD_STOP_TRANSMIT );
            PRINTN( "HDMI TX controller stop transmission %s.", (ret==0)?"completed":"failed" );
        }

    } else if( strcmp(cmd, "reg") == 0 ) {

    	DrvControl( hdmitx, HDMITX_CMD_SHOW_REGS );

    } else if( strcmp(cmd, "stat") == 0 ) {

    	PRINTN( "admaCh0Ctr		 = 0x%08x", admaCh0Ctr );
    	PRINTN( "admaCh1Ctr		 = 0x%08x", admaCh1Ctr );
    	PRINTN( "sspaTxCtr	 	 = 0x%08x", sspaTxCtr );
    	PRINTN( "sspaRxCtr	 	 = 0x%08x", sspaRxCtr );


    } else {
        PRINT("----------------------------------------------------------------------");
        PRINT( "HDMI TX Driver Debug Commands:");
        PRINT( "  x pf.hdmi.cfg      : Show current configuration data     " );
        PRINT( "  x pf.hdmi.dm X     : Configure display mode              " );
        PRINT( "                       [0] Graphics Mode                   " );
        PRINT( "                       [1] Video Mode                      " );
        PRINT( "  x pf.hdmi.vf X     : Configure video output format       " );
        PRINT( "                       (Please refer to CEA-861 for video  " );
        PRINT( "                        identification codes)              " );
        PRINT( "  x pf.hdmi.ci X     : Configure source pixel format       " );
        PRINT( "                       [0] RGB565                          " );
        PRINT( "                       [1] RGB1555                         " );
        PRINT( "                       [2] RGB888P                         " );
        PRINT( "                       [3] RGB888UP                        " );
        PRINT( "                       [4] RGBA888                         " );
        PRINT( "                       [5] YUV422P                         " );
        PRINT( "                       [6] YUV422PL                        " );
        PRINT( "                       [7] YUV420PL                        " );
        PRINT( "                       [9] PALETTE4                        " );
        PRINT( "                       [10] PALETTE8                       " );
        PRINT( "                       [11] RGB888A                        " );
        PRINT( "  x pf.hdmi.al X     : Configure audio channnel count      " );
        PRINT( "                       [0] Stream Header                   " );
        PRINT( "                       [2] 2 Channels                      " );
        PRINT( "                       [3] 3 Channels                      " );
        PRINT( "                       [4] 4 Channels                      " );
        PRINT( "                       [5] 5 Channels                      " );
        PRINT( "                       [6] 6 Channels                      " );
        PRINT( "                       [7] 7 Channels                      " );
        PRINT( "                       [8] 8 Channels                      " );
        PRINT( "  x pf.hdmi.af X     : Configure sampling frequency        " );
        PRINT( "                       [0] Stream Header                   " );
        PRINT( "                       [1] 32 KHz                          " );
        PRINT( "                       [2] 44.1 KHz                        " );
        PRINT( "                       [3] 48 KHz                          " );
        PRINT( "                       [4] 88.2 KHz                        " );
        PRINT( "                       [5] 96 KHz                          " );
        PRINT( "                       [6] 176.4 KHz                       " );
        PRINT( "                       [7] 192 KHz                         " );
        PRINT( "  x pf.hdmi.as X     : Configure sampling size             " );
        PRINT( "                       [0] Stream Header                   " );
        PRINT( "                       [1] 16 Bits                         " );
        PRINT( "                       [2] 20 Bits                         " );
        PRINT( "                       [3] 24 Bits                         " );
        PRINT( "  x pf.hdmi.con X    : Connect/disconnect an HDMI receiver " );
        PRINT( "                       [0] Disconnect                      " );
        PRINT( "                       [1] Connect                         " );
        PRINT( "  x pf.hdmi.en X     : Enable/disable the HDMI transmitter " );
        PRINT( "                       [0] Disable                         " );
        PRINT( "                       [1] Enable                          " );
        PRINT( "  x pf.hdmi.st X     : Start/stop the HDMI transmitter     " );
        PRINT( "                       [0] Start                           " );
        PRINT( "                       [1] Stop                            " );
        PRINT( "  x pf.hdmi.reg      : Dumps all HDMI transmitter registers" );
    }

}

