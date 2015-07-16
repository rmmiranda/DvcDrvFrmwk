/*****************************************************************************
 * Filename:    camif_test.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Dec 9th, 2009
 *
 * Description: Camera driver test code *
 *****************************************************************************/
/* standard library header files */
#include <string.h>

/* driver header files */
#include "driver.h"
#include "camera.h"
#include "camif_drvfwk.h"
#define SRCGROUP GROUP_PLT_DRV
#define SRCFILE  FILE_CAM_TST

extern void ResetNow( void );
/*****************************************************************************
 * Function:    DrvFwkCamDebug 
 * Description: Drv Framework API - driver local debugging and testing commands
 * Parameters:  instance
 * Return:      void
 *
 *****************************************************************************/
void DrvFwkCamDebug(char * cmd, DWORD arg1, DWORD arg2)
{
   /* Bugdisp commands:
       show: display driver state, statistics, and registers;
       cmd: enable, disable, take picture, etc... */
	Driver_t * drv = DrvGetHandle(IODRV_ID_CAMERA);	
    
    if(strcmp(cmd, "show") == 0)
    {
        /* arg1 - category, arg2 - verbose level */
        if(arg1 == 0)
            CamifShowStats(arg1, arg2);
        else if(arg1 == 4)
            CamifShowReg(arg1, arg2);
    }
    else if(strcmp(cmd, "cmd") == 0)
    {
        /* arg1 - action */
        if(arg1 == 1)
        {
            PRINT("Enable ISP");
            CamifIspPowerOn();
        }
        else if(arg1 == 2)
        {
            PRINT("Disable ISP");
            CamifIspPowerDown();
        }
        else if(arg1 == 3)
        {
            /* Start viewfinder */
            CamifStartViewfinder();       
        }
        else if(arg1 == 4)
        {
            CamifStreamStop();
        }
        else if(arg1 == 5)
        {
            /* Start movie mode */
            if(arg2 == 0)
            {
                PRINT2N("Starting movie mode (RGB565): %d, %d", camConfig.resolution.reslX, camConfig.resolution.reslY);
                CamifStartMovie(CAM_PIXEL_FMT_RGB_565);       
            }
            else
            {
                PRINT2N("Starting movie mode (YUV422): %d, %d", camConfig.resolution.reslX, camConfig.resolution.reslY);
                CamifStartMovie(CAM_PIXEL_FMT_YCBCR_422);                       
            }
        }
        else if(arg1 == 6)
        {
            /* take a picture*/
            PRINT2N("Starting stills mode: %d, %d", camConfig.resolution.reslX, camConfig.resolution.reslY);            
            CamifStartPicture(); 
        }        
        else if(arg1 == 7)
        {
            camConfig.ispTestMode = (CamTestMode) arg2;
            PRINTN("Set ISP test mode to %d(0 = Disabled, [1-7] = Patterns )", camConfig.ispTestMode);
            /* Enable test mode */
            CamDrv.IspSvpFunc(SVPCF_SETUP_TEST_MODE);

        }
        else if(arg1 == 8)
        {
            camConfig.ispFrameRate = (DWORD)arg2;
            PRINTN("Set ISP frame rate to %d", camConfig.ispFrameRate);            
        }        
        else if(arg1 == 9)
        {
            if((int)arg2 < 1)
            {
                WARN("Invalid number of data lanes");
                return;
            }
            
            csiConfig.CsiLaneNum = arg2;
            PRINTN("Set # of lanes to %d", csiConfig.CsiLaneNum);
        }
        else if(arg1 == 10)
        {
            csiConfig.DataRate = (CamCsiDataRate)arg2;
            PRINTN("Set CSI data rate to %d", csiConfig.DataRate);            
        }
        else if(arg1 == 11)
        {
            csiConfig.errControl = (CamErrControl)arg2;
            PRINTN("Set CSI errControl to %d", csiConfig.errControl);            
        }
        else if(arg1 == 12)
        {
            CamifClearStats();
        }
        else if(arg1 == 13)
        {
            camConfig.lcdRefreshRate = arg2;
            PRINTN("Set Camera display refresh rate to %d", camConfig.lcdRefreshRate);                        
        }
        else if(arg1 == 14)
        {
            DrvFwkSetCamResolution((CamResolutionId) arg2);

        }else if(arg1 == 15)
        {
            if(arg2 == 0)
            {
                PRINT("Selected Camel 2 with ISP cut1.0");
                CamDrv.IspReset = IspReset_ST987_10;
                CamDrv.IspLoadPatch = IspLoadPatch_ST987_10;
                CamDrv.IspSvpFunc = IspSvpFunc_ST987_10;
            }
            else
            {
                PRINT("Selected Camel 1 with ISP cut0.9");
                CamDrv.IspReset = IspReset_ST987_09;
                CamDrv.IspLoadPatch = IspLoadPatch_ST987_09;
                CamDrv.IspSvpFunc = IspSvpFunc_ST987_09;                                
            }                        
        }        
        else if(arg1 == 16)
        {
            CamifEnableInterleaving((CamInterleavingMode) arg2);
            PRINTN("Enable interleaving mode - %d", arg2);
        }
        else if(arg1 == 17)
        {
            PRINTN("test freq - %d", arg2);
            WriteRegDword(0x5002000C, (1u << 16 )| ((arg2) & 0xFFF));
        }
        else if(arg1 == 18)
        {
            PRINTN("VAUF frequency set to - %d ( 0 - 104Mhz, 1 - 156Mhz, 3 - 78Mhz) ", arg2);
            WriteRegDword(0x41340000,   (ReadRegDword(0x41340000) & ~0x30000000) | (arg2 << 28));
        }
        else
            WARNN("Invalid camera command ID - %d", arg1);            
    }
    else if(strncmp(cmd, "isp", 3) == 0)
    {
        if(strcmp(cmd, "ispr") == 0)
        {
            BYTE ret = CamSensorReadReg((WORD)arg1);
            PRINT3N("Read ISP reg(0x%x) - %d (0x%x)",arg1, ret, ret);      
        }
        else if(strcmp(cmd, "ispw") == 0)
        {
            int ret = CamSensorWriteReg((WORD)arg1, (BYTE)arg2);
            PRINT3N("Write ISP reg(0x%x) with value %d (0x%x); return %d",arg1, arg2, ret);
        }        
        else if(strcmp(cmd, "ispc") == 0)
        {
            DrvFwkCamIspTest();
        }
    }
    else if(strcmp(cmd, "help") == 0)
    {
        /* Show bugdisp commands for camera driver */
        PRINT("***************************************************************");
        PRINT("Camera Platform Driver Bugdisp Commands:");
        PRINT("[1]pf.cam.show                   0 - Driver info");
        PRINT("                                 1 - (NI) Driver Status ");
        PRINT("                                 2 - (NI) Driver Statisitcs");
        PRINT("                                 3 - (NI) Driver Config");
        PRINT("                                 4 - Device Register Info");
        PRINT("                                 5 - (NI) CSI Registers");
        PRINT("                                 6 - (NI) CI Registers");
        PRINT("                                 7 - (NI) I2C Registers");
        PRINT("[2]pf.cam.cmd                    0 - Not used");
        PRINT("                                 1 - ISP Power On");
        PRINT("                                 2 - ISP Power Down");
        PRINT("                                 3 - Start Viewfinder");
        PRINT("                                 4 - Stop Stream");
        PRINT("                                 5 - Start movie mode (0 - RGB 565, 0' - YUV 4:2:2");
        PRINT("                                 6 - Take Picture");
		PRINT("                                 7 - Set ISP test mode");
        PRINT("                                 8 - Set framerate (only for Camel2)");
        PRINT("                                 9 - Set number of CSI lanes");        
        PRINT("                                 10 - Set CSI Data rate(1 - 200Mbps, 2- 480Mbps, 3 - 600Mbps, 4 - 700Mbps,  5 - 800Mbps");
        PRINT("                                 11 - Set error control");         
        PRINT("                                 12 - Clear driver statistics");  
        PRINT("                                 13 - Change display refresh rate");  
        PRINT("                                 14 - Change sensor resolution(1 - 640x480, 2 - 800x600, 3 - 1280x720, 4 - 1280_1024)");
        PRINT("                                 15 - Choose between Camel2 or Camel1 (0 - cut1.0, 0' = cut0.9");
        PRINT("                                 16 - Interleaving Mode options (0 - disable, 1 - picture, 2 - movie");
        PRINT("                                 18 - set VAUF frequency ( 0 - 104Mhz, 1 - 156Mhz, 3 - 78Mhz)");
        PRINT("[3]pf.cam.ispr  [Reg Addr]       Read ISP register");        
        PRINT("[4]pf.cam.ispw  [Reg Addr]       Write ISP register");
        PRINT("[5]pf.cam.ispc                   Compare ISP register values with ST result");
        PRINT("[6]pf.cam.reset                  Reset the device");
    }
    else if (strcmp(cmd, "reset") == 0)
    {
    	PRINT("System is going for reboot now...");
    	ResetNow();
    }
    else
    {
        WARN("Invalid command");
    }        
}

