/*****************************************************************************
 * Filename:    camTest.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:     Tai Yu 
 *
 * Created:     July 2009
 *
 * Description: Verification code for PV2 camera driver using driver framework
 *
 *****************************************************************************/
/* include header files */

/* standard library header files */
#include <string.h>

#include "rei.h"
#include "framework.h"
#include "camera.h"
//#include "rtc.h"
//#include "threadservices.h"

#define SRCGROUP GROUP_PLT_APP
#define SRCFILE  FILE_TST_CAM
#define ISP_ADDR_NUM 1 //# of address to checi
//#define PV2_ULPS_ADDR 0x50020014
#define FIVE_SECONDS 5
typedef enum hwI2cTestResultTagFIVE_SECONDS
{
	I2C_TEST_FAILED,
	I2C_TEST_PASSED,
	I2C_TEST_ERROR,
	I2C_NO_RESULT_AVAILABLE
} hwI2cTestResult;

typedef enum hwCSITestResultTag
{
	CSI_TEST_FAILED,
	CSI_TEST_PASSED,
	CSI_TEST_ERROR,
	CSI_NO_RESULT_AVAILABLE
} hwCSITestResult;
typedef enum {
    CAM_MSG_TEST_TIMER_EXPIRED = 0,
    CAM_MSG_TEST_TIMER_START=11,
    CAM_MSG_TEST_TIMER_STOP=12,		
	CAM_MSG_TEST_START=13,
	CAM_MSG_ULPS_START=14		
}CsiTestMsg;
typedef enum IspModeTag
{
	ISP_VF,
	ISP_MV
} hwRunningMode;
static BOOL heartbeat=TRUE;
static hwI2cTestResult I2CtestResult = I2C_NO_RESULT_AVAILABLE;
static hwCSITestResult CSItestResult = CSI_NO_RESULT_AVAILABLE;
static hwRunningMode CurrentRunningMode =ISP_MV;
static Driver_t * drv;
static CamConfig * cfgPtr;
static CamStatus * staPtr;
static CamStatis * cntPtr;
static RE_MESSAGE camMsg;
static UINT I2c_TestCycle = 1000;
static UINT CSI_BER_Duration_minutes=1;
static UINT I2c_Opt_Addr =0x2025;

//#ifdef RIM_BQHUGE
static UINT I2c_Test_Addr =0x2025;
//#endif
static UINT I2c_Value = 0;
static UINT Err_Ctrl =2; //real time stop for errors
/* Forward function declaration */
static void camTestDebug(char * cmd, DWORD arg1, DWORD arg2);
static QWORD rtcTicksBefore, Throughput ;
static QWORD totalTick,tickUsed;
static DWORD test_duration;
static DWORD ULPS_cnt,Csi_tmr;
static DWORD Counter_5s=0, Counter_1m=0;
void VfCsiTestProcessMsg(MESSAGE * msg);
static BOOL ULPS_RUN;
static RE_MESSAGE testMsg;
static int test_result = 0; 
void StartULPS()
{
IODWORD csiStatus;		
IODWORD CurrentStat;
//if(staPtr->csiStatusRegPtr)           
		//csiStatus = ReadRegDword(staPtr->csiStatusRegPtr);
		csiStatus =ReadRegDword(0x50020014);

PRINTN("init status of ULPS is %8x", csiStatus); 
CurrentStat=csiStatus & 0x07000000;
	while(ULPS_RUN==TRUE)
	{		
	RimSleep(5);
	csiStatus =ReadRegDword(0x50020014); // CSI status register CSxSR
	if((csiStatus!=0x1f000000)&&\
					(csiStatus!=0x1f000001))
		PRINTN("new status of ULPS is %8x", csiStatus); 
			
	if(CurrentStat!=(csiStatus & 0x07000000))
		{
  		CurrentStat=csiStatus & 0x07000000;
		PRINT("ULPS toggles once ");
		ULPS_cnt++;
		}
	}
}



void StopCsiTest()
{
		PRINT("shut down ISP from test app.... ");
		RE_TimerStop(Csi_tmr);
        RE_TimerDelete(Csi_tmr);
        PRINT("CSI timer deleted");     
	  	if(CurrentRunningMode ==ISP_VF)
		{	
			//CamifStreamStop();
			DrvControl(drv, CAM_CMD_STOP_VF);
		}
	  	if(CurrentRunningMode ==ISP_MV)
		{	
		 	DrvControl(drv, CAM_CMD_STOP_MV);
		}
		//tickUsed=ReadRtcWhole()-rtcTicksBefore;
		//SHOW_MEMORY("tickUsed is:", &tickUsed, 8); //@@@@
		//SHOW_MEMORY("totalTick is:", &totalTick, 8); //@@@@	
		//SHOW_MEMORY("rtcTicksBefore is:", &rtcTicksBefore, 8); //@@@@
		PRINTN("Totoal CSI SoF is %d",cntPtr->csiStatis.csiSof); //total frame received
		PRINTN("Totoal CSI EoF is %d",cntPtr->csiStatis.csiEof); //total frame received
	  	if(CurrentRunningMode ==ISP_MV)
		{
		  PRINT2N("Movie Mode Image Resolution is %d x %d",cfgPtr->resolution.reslX, cfgPtr->resolution.reslY); 
		  Throughput=(((cfgPtr->resolution.reslX*cfgPtr->resolution.reslY>>10)*cntPtr->csiStatis.csiEof)>>10)*8*2/cfgPtr->csiConfig.CsiLaneNum; //RBG16
		  PRINTN("Traffic throughput in this test case is %d Mbits per lane ", Throughput);			  
		}
		if((cntPtr->csiStatis.csiSof-cntPtr->csiStatis.csiEof)>3)
		{
			test_result = 1;
			WARN("CSI SoF and EoF mismatch!!");
		}
		if(cntPtr->csiStatis.csiErrCrc!=0 || cntPtr->csiStatis.csiErr1Bit!=0 || cntPtr->csiStatis.csiErr2Bit!=0 || cntPtr->csiStatis.csiErrSoTSync!=0 || cntPtr->csiStatis.csiErrSoT!=0 || cntPtr->csiStatis.csiErrCtrl!=0 )
		{
			test_result = 1;
			WARN("Error detected!!");
		}		
        //devCamIspPowerDown();
		DrvControl(drv, CAM_CMD_ISP_PWR_DN);		
		//test_duration=(ReadRtcWhole()-rtcTicksBefore)/32760/60;
		PRINTN("DB_HW_TEST: Current Link Speed setting is %d (1 - 200Mbps, 2 - 480Mbps, 3 - 600Mbps, 4 - 700Mbps, 5 - 800Mbps)", cfgPtr->csiConfig.DataRate);	
 		PRINTN("DB_HW_TEST: Current Lane Number is %d", cfgPtr->csiConfig.CsiLaneNum);
		PRINTN("DB_HW_TEST: Totoal test duration for this test case is %d minutes", Counter_1m); //real test duaration
		PRINT("DB_HW_TEST: Error Status .... ");
		PRINTN("DB_HW_TEST: CRC error number is %d", cntPtr->csiStatis.csiErrCrc); 
		PRINTN("DB_HW_TEST: 1b ECC error number is %d", cntPtr->csiStatis.csiErr1Bit); 
		PRINTN("DB_HW_TEST: 2b ECC error number is %d", cntPtr->csiStatis.csiErr2Bit); 
		PRINTN("DB_HW_TEST: SoT Sync error number is %d", cntPtr->csiStatis.csiErrSoTSync); 
		PRINTN("DB_HW_TEST: SoT error number is %d", cntPtr->csiStatis.csiErrSoT); 
		PRINTN("DB_HW_TEST: Control error number is %d", cntPtr->csiStatis.csiErrCtrl); 
    	if (test_result == 0) {
        	PRINT("DB_HW_TEST: ###################CSI test PASS ############################");
			CSItestResult = CSI_TEST_PASSED;
		}
    	else {
        	WARN("DB_HW_TEST: ####################CSI test FAIL###########################");
			CSItestResult = CSI_TEST_FAILED;
		}		
	
}

void VfCsiTestProcessMsg(MESSAGE * msg) //test thread
{
    

 	DWORD subMsg, eventId;
	//QWORD rtcTicksBefore;
	//QWORD totalTick;

	//rtcTicksBefore = ReadRtcWhole();
	subMsg = msg->SubMsg;
	eventId = msg->Event; 
	if(heartbeat==TRUE)
	PRINT2N("****** Received message - %d, %d", msg->Event, msg->SubMsg);

    if(((CamMsg) subMsg == CAM_MSG_CSI_ERR)&&(eventId==PLATFORM_MSG_CAM_DRV))
    {
        WARN("CSI Error captured");
        //drvCamShow(0, 0); 
		test_result = 1;
        if(Err_Ctrl==2)
		{	
		WARN("Packet Error captured, stop streaming");
		//test_result = 1;		
 		StopCsiTest();
        //return;
		}
    }
    else if(((CsiTestMsg)subMsg ==CAM_MSG_TEST_TIMER_START)&&(eventId==PLATFORM_MSG_CAM_TEST_APP))
    {
		
		//rtcTicksBefore = ReadRtcWhole();
		Csi_tmr = RE_TimerCreate();
        RE_TimerStart(Csi_tmr, msg->Data[0] * RE_TicksPerSecond(), 
                          (msg->Data[1] == 0) ? TIMER_ONE_SHOT : TIMER_PERIODIC, 
                          PLATFORM_MSG_CAM_TEST_APP); 
        PRINT2N("timer started: %d, %d", msg->Data[0], msg->Data[1]);   
		Counter_5s=0;
		Counter_1m=0;
		Throughput=0;	
		//totalTick=60*32768*CSI_BER_Duration_minutes;
		//PRINTN("totoalTick is %16x", totalTick);
		//SHOW_MEMORY("totalTick is:", &totalTick,sizeof(totalTick));
		//SHOW_MEMORY("totalTick is:", &totalTick, 8); //@@@@	
		//SHOW_MEMORY("rtcTicksBefore is:", &rtcTicksBefore, 8); //@@@@			
		test_result = 0; 

		if(staPtr->drvEnabled == TRUE)
    	{
			WARN("Camera driver already enabled, this test is not running..");
			test_result = 1;
        	return;
    	}		
		DrvControl(drv, CAM_CMD_CLEAR_STATIS);  	
		DrvControl(drv, CAM_CMD_ISP_PWR_UP);
	  	if(CurrentRunningMode ==ISP_VF)
		{
			DrvControl(drv, CAM_CMD_START_VF);
		}
		else if(CurrentRunningMode ==ISP_MV)
		{
			DrvControl(drv, CAM_CMD_START_MV);
		}
		else
		PRINT("wrong running mode");

		//PRINTN("rtcTickBefore is %16x", rtcTicksBefore);
		//PRINTN("totalTick is %16x", totalTick);
		RimSleep(4000);
		//PRINTN("rtcTickNow is %16x", ReadRtcWhole());
		//PRINTN("ticks gone is %16x", ReadRtcWhole()-rtcTicksBefore);
		if(staPtr->drvEnabled == TRUE)
    	{
			PRINT("ISP powered, you should see me");
    	}
		//pf.cam.show 0 1 for counters
		PRINTN("Number of frames received at CSI now is %d", cntPtr->csiStatis.csiEof); //total frame received
		/*if(cntPtr->csiStatis.csiEof<2)
			{
				PRINTN("Number of frames received at CSI now is %d", cntPtr->csiStatis.csiEof); //total frame received
				WARN("No Frame received in current setting, this test is not running");
				test_result = 1;
				PRINTN("Current Link Speed setting is %d (1 - 200Mbps, 2 - 480Mbps, 3 - 600Mbps, 4 - 700Mbps, 5 - 800Mbps)", cfgPtr->csiConfig.DataRate);
			}		PRINT("5s timer expired");   
		else
		{*/
	}
   else if(((CsiTestMsg)subMsg ==CAM_MSG_TEST_TIMER_EXPIRED)&&(eventId==PLATFORM_MSG_CAM_TEST_APP))
	{
		//PRINT("5s timer expired"); 
		Counter_5s++;
	  	if(Counter_5s%12==0)
		{
			Counter_1m++;
			PRINTN("%d Minutes elapsed..",Counter_1m);
		}	
		//if(((ReadRtcWhole()-rtcTicksBefore) < totalTick) && (staPtr->drvEnabled == TRUE)) //test on-going
		if((Counter_1m <CSI_BER_Duration_minutes) && (staPtr->drvEnabled == TRUE))
		{
			if(heartbeat==TRUE)
			{		
			//PRINTN("rtcTickBefore is %16x", rtcTicksBefore);
		    //PRINTN("totalTick is %16x", totalTick);		
			//PRINTN("rtcTickNow is %16x", ReadRtcWhole());
			PRINT("5s timer expired");
			}
		}
		else // test duration reached
		{
		PRINT("stop testing");   
		StopCsiTest();		
		}		
		
	}
	else if(((CsiTestMsg)subMsg ==CAM_MSG_ULPS_START)&&(eventId==PLATFORM_MSG_CAM_TEST_APP))
	{
		StartULPS();
	}
	else
		WARN("undefiend message");	

	RimSleep(500);



}

/*****************************************************************************
 * Function:    camTestInit 
 * Description: Initialization function for the camera test code. This function
 *              is called by platformMain during the Platform Software
 *              initialization
 * Parameters:  Void
 * Return:      Void
 *
 *****************************************************************************/
void camTestInit(void)
{  
   	PRINT(" camTestInit");
   /* Test code initialization - memory, static variables, bugdisp etc. */ 
    
   /* Register Bugdisp commands */
   	VfCmdRegister("cam", "Camera Verification Code", &camTestDebug);
  	//devCamIspPowerOn(); 
    /* register message handler */
    //PfMsgRegister(PLATFORM_THREAD_ID_APP0, PLATFORM_MSG_CAM_DRV,  cbProcessMsg);  
    PfMsgRegister(PLATFORM_THREAD_ID_APP0, PLATFORM_MSG_CAM_TEST_APP, VfCsiTestProcessMsg);
    PfMsgRegister(PLATFORM_THREAD_ID_APP0, PLATFORM_MSG_CAM_DRV, VfCsiTestProcessMsg);	
	drv = DrvGetHandle(IODRV_ID_CAMERA);
    if(drv == NULL)
    {
        WARN("Invalid camera driver pointer");
        return;
    }
	else
	{		
	cfgPtr = DrvGetConfig(drv);
	    if(cfgPtr == NULL)
    	{
        WARN("Invalid camera cfg pointer");
        return;
		}
	//CamStatus * statPtr = (CamStatus * )DrvGetStatus(driver);
	staPtr = DrvGetStatus(drv);
	    if(staPtr == NULL)
    	{
        WARN("Invalid camera status pointer");
        return;
		}
	cntPtr = DrvGetStats(drv);
	    if(cntPtr == NULL)
    	{
        WARN("Invalid camera statis pointer");
        return;
		}

	}
}

/*****************************************************************************
 * Function:    Isp_I2cTest 
 * Description: Function to generate random data to stress test ISP I2C
 * Parameters:  
 * Return:      
 *
 *****************************************************************************/
void Isp_I2cTest()
{

    int i,j,retValue;
    BYTE pI2c_gold[ISP_ADDR_NUM ];
    BYTE pI2c_read[ISP_ADDR_NUM ];
    BYTE IspI2cRestore[ISP_ADDR_NUM ];
	DrvControl(drv, CAM_CMD_ISP_PWR_UP);  //power up ISP 	
	PRINTN("Test Cycle was set to %d", I2c_TestCycle);	
    for (i = 0; i < ISP_ADDR_NUM; i++) {
	    	 DrvControl(drv, CAM_CMD_ISP_REG_RD, (WORD)(I2c_Test_Addr+i), &IspI2cRestore[i]);     
    }

	for(j = 0; j < I2c_TestCycle; j++) {
        	//RimTaskYield();
        	if (j % 100 == 0) 
            	PRINTN("ISP Test Cycle is %d", j);
        	for (i = 0; i < ISP_ADDR_NUM ; i++) {
	        	pI2c_gold[i] =(rand() % 0xFF) ; //MODIFY
    			//PRINT2N("gold[%d]=%x", i, pI2c_gold[i]);
            	DrvControl(drv, CAM_CMD_ISP_REG_WR, (WORD)(I2c_Test_Addr+i), pI2c_gold[i],&retValue);  
	    	}

          	for (i = 0; i < ISP_ADDR_NUM; i++) {
	        	DrvControl(drv, CAM_CMD_ISP_REG_RD, (WORD)(I2c_Test_Addr+i), &pI2c_read[i]);       
            	if(pI2c_read[i] != pI2c_gold[i]) {
    		    	PRINT3N("ISP Wrong at addr 0x%x data read is 0x%x and gold 0x%x",(I2c_Test_Addr+i),pI2c_read[i],pI2c_gold[i]);
    		    	test_result = 1;
            	}
	    	}
    	}
    
	for (i = 0; i < ISP_ADDR_NUM; i++) 
		DrvControl(drv, CAM_CMD_ISP_REG_WR, (WORD)(I2c_Test_Addr+i),IspI2cRestore[i],&retValue);  


    	if (test_result == 0) {
        	PRINT("DB_HW_TEST: ISP I2C test PASS");
			I2CtestResult = I2C_TEST_PASSED;
		}
    	else {
        	WARN("DB_HW_TEST: ISP I2C test FAIL");
			I2CtestResult = I2C_TEST_FAILED;
		}
		RimSleep(500);
        DrvControl(drv, CAM_CMD_ISP_PWR_DN);  //power down ISP

}

void CSI_BER_Test()
{
   	//post msg to trigger test
	camMsg.Event   = (DWORD) PLATFORM_MSG_CAM_TEST_APP;
    camMsg.SubMsg  = (DWORD) CAM_MSG_TEST_TIMER_START;
    camMsg.Data[0] = FIVE_SECONDS;  //5 sec delay
    camMsg.Data[1] = 1; // 0 - one-shot; otherwise periodic
    RE_RimPostMessage( PLATFORM_THREAD_ID_APP0, &camMsg );       

	//camMsg.Event   = (DWORD) PLATFORM_MSG_CAM_TEST_APP;
    //camMsg.SubMsg  = (DWORD) CAM_MSG_TEST_START;
    //RE_RimPostMessage( PLATFORM_THREAD_ID_APP0, &camMsg );           	
}


/*****************************************************************************
 * Function:    camTestDebug 
 * Description: Function to handle the bugdisp commands. Note both arg1 and arg2 
 *              are only 16-bit wide (they really shouldn't be DWORD).
 * Parameters:  
 * Return:      
 *
 *****************************************************************************/

static void camTestDebug(char * cmd, DWORD arg1, DWORD arg2)
{
    //Driver_t * drv = DrvGetHandle(IODRV_ID_CAMERA);

        
    if(strcmp(cmd, "cfg") == 0)
    {
        //CamConfig * cfgPtr = DrvGetConfig(drv);
        if(cfgPtr == NULL)
        {
            WARN("Invalid camera driver config pointer");
            return;
        }
            
        if(arg1 == 1)
        {
            /* I2C test cycle # */
			if(arg2!=0)				
            I2c_TestCycle = arg2;
            PRINTN("Camera VerifApp: ISP I2C Test Cycle is set to %d", I2c_TestCycle);
  
        }

       else if(arg1 == 2)   
        {
            //change i2c access address
			if(arg2!=0)	//
			I2c_Opt_Addr=arg2;
            PRINTN("Camera VerifApp: ISP I2C Test operating address is set to 0x%x",I2c_Opt_Addr);
        }
        else if(arg1 == 3)
        {
            
			if(arg2!=0)	
			/* Set control option for CSI errors: 1 -continue or 2 - stop */
			{
			if(arg2==1)//continue
			Err_Ctrl=1;
			else
			Err_Ctrl=2;//stop
			
            cfgPtr->csiConfig.errControl = (CamErrControl)arg2;
            PRINTN("Verfication: Error control is set to %d", cfgPtr->csiConfig.errControl);
			}

        }
        else if(arg1 == 4)
        {
 			if(arg2!=0)	           
			/* Set CSI data rate: */
            cfgPtr->csiConfig.DataRate = (CamCsiDataRate)arg2;
            PRINTN("Verfication: Data rate is set to %d (1 - 200Mbps, 2 - 480Mbps, 3 - 600Mbps, 4 - 700Mbps, 5 - 800Mbps)", cfgPtr->csiConfig.DataRate);
        }
        else if(arg1 == 5)
        {
 			if(arg2!=0)	 
            /* Set test mode */
            cfgPtr->ispTestMode = (int)arg2;
            PRINTN("Verfication: Test mode is set to %d (1 - disabled, 2 - enabled)", cfgPtr->ispTestMode);
        }
        else if(arg1 == 6)
        {
            
			if(arg2!=0)	 
			/* Set number of lanes */
            cfgPtr->csiConfig.CsiLaneNum = (int)arg2;
            PRINTN("Verfication: Number of lanes is set to %d", cfgPtr->csiConfig.CsiLaneNum);
        }    
        else if(arg1 == 7)
        {
            DrvControl(drv, CAM_CMD_ISP_PWR_UP);       
        }
        else if(arg1 == 8)
        {
            DrvControl(drv, CAM_CMD_ISP_PWR_DN);       
        }
        else if(arg1 == 9)
        {
            DrvControl(drv, CAM_CMD_START_VF);
        }
        else if(arg1 == 10)
        {
            DrvControl(drv, CAM_CMD_STOP_VF);
        }  
		else if(arg1 == 11)
        {
            /* CSI BER test duration # */
			if(arg2!=0)				
            CSI_BER_Duration_minutes = arg2;
            PRINTN("Camera VerifApp: ISP BER Test Duration is set to to %d minutes", CSI_BER_Duration_minutes);
  
        }
        else if(arg1 == 12)
        {
			if(arg2!=0)	 
			{
				if(arg2==1)
				heartbeat=TRUE;
				if(arg2==2)
				heartbeat=FALSE;						
			}

        } 
        else if(arg1 == 13)
        {
			if(arg2!=0)	 
			{
				if(arg2==1)
				{		
				CurrentRunningMode =ISP_MV;
				PRINT("Verfication: Set to Movie Mode");
				}
				if(arg2==2)
				{		
				CurrentRunningMode =ISP_VF;	
				PRINT("Verfication: Set to Viewfinder Mode");
				}				
			}

        }
        else if(arg1 == 14)
        {
			if(arg2!=0)	 
			{
				if(arg2==1)
				{		
				DrvControl(drv, CAM_CMD_CHANGE_RESOLUTION, 1);
				PRINT("Verfication: Set to resolution 640x480");
				}
				if(arg2==2)
				{		
				DrvControl(drv, CAM_CMD_CHANGE_RESOLUTION, 2);
				PRINT("Verfication: Set to resolution 800x600");
				}
				if(arg2==3)
				{		
				DrvControl(drv, CAM_CMD_CHANGE_RESOLUTION, 3);
				PRINT("Verfication: Set to resolution 1280x720");
				}
				if(arg2==4)
				{		
				DrvControl(drv, CAM_CMD_CHANGE_RESOLUTION, 4);
				PRINT("Verfication: Set to resolution 1280x1024");
				}

			}

        }  

        else
        {

            PRINT("===== Camera VerifApp Help =====");
			PRINT("including test and MIPI link configuration");			
            PRINTN("x vf.cam.cfg 1 N: I2C Stress Test Cycle changed to N, current test cycle is %d",I2c_TestCycle);
            PRINTN("x vf.cam.cfg 2 N: change I2C Rd/Wr operating address, current address is 0x%x",I2c_Opt_Addr);
            PRINT("x vf.cam.cfg 3 N: Set control option for CSI errors where N = 1(continue) or 2(stop)");
            PRINT("x vf.cam.cfg 4 N: Set CSI date rate N to 1(200Mbps), 2(480Mbps), 3(600Mbps), 4(700Mbps) or 5(800bps)");
            PRINT("x vf.cam.cfg 5 N: Set ISP colorbar test mode, where N is 1(disable) or 2(enable)");
            PRINT("x vf.cam.cfg 6 N: Set number of CSI data lanes, N = 1(single lane) or 2(dual lane)");
            PRINT("x vf.cam.cfg 7 : Power on ISP");
            PRINT("x vf.cam.cfg 8 : Power down ISP");
            PRINT("x vf.cam.cfg 9 : Start viewfinder");
            PRINT("x vf.cam.cfg 10 : Stop viewfinder");
            PRINT("x vf.cam.cfg 11 N : Specify test duration for CSI interface N=number of minutes");
            PRINT("x vf.cam.cfg 12 N : 1 enable heartbeat 2: disable heartbeat");
            PRINT("x vf.cam.cfg 13 N : 1 movie mode 2: viewfinder mode");
            PRINT("x vf.cam.cfg 14 N : set movie mode resolution 1:640x480, 2:800x600, 3:1280x720, 4:1280_1024");
        }        
    }
    else if(strcmp(cmd, "para") == 0)
    {    
        if(arg1 == 1)
        {
            DrvControl(drv, CAM_CMD_ISP_REG_RD, (WORD)I2c_Opt_Addr, &I2c_Value);       
            PRINT2N("Camera VerifApp: Read ISP Register[0x%x] = 0x%x", I2c_Opt_Addr, I2c_Value);  

        }
        else if(arg1 == 2)
        {
            int retVal;
            DrvControl(drv, CAM_CMD_ISP_REG_WR, (WORD)I2c_Opt_Addr, arg2, &retVal);       
            PRINT3N("Camera VerifApp: Write ISP Register[0x%x] = 0x%x, return %d", I2c_Opt_Addr, arg2, retVal);  
        }
        else if(arg1 == 3)
        {
			if(arg2!=0)	 
			{
           		if(arg2==1)
				{
				PRINT("Verfication: Start ULPS counter");
				ULPS_cnt=0;
				ULPS_RUN=TRUE;
				camMsg.Event   = (DWORD) PLATFORM_MSG_CAM_TEST_APP;
    			camMsg.SubMsg  = (DWORD) CAM_MSG_ULPS_START;
    			RE_RimPostMessage( PLATFORM_THREAD_ID_APP0, &camMsg );  				
		
				}
				if(arg2==2)
				{
				ULPS_RUN=FALSE;		
				PRINT("Verfication: Disable ULPS counter");
			    PRINTN("Verfication: Detected %d times of toggle at all lanes", ULPS_cnt);
				}
			}	
        }
        else        
        {
            PRINT("===== Camera VerifApp Help =====");
            PRINT("x vf.cam.para 1 : Read ISP I2C as specified in cfg 2");
            PRINT("x vf.cam.para 2 N: Write ISP I2C with value N. Example x vf.cam.para 2 0x2025");
            PRINT("x vf.cam.para 3 N: N=1 enable ULPS counter, N=2 Disable and report result ");	
        }
    }
    else if(strcmp(cmd, "func") == 0)
    {    
        if(arg1 == 1)
        {
            PRINT("Camera VerifApp: ISP I2C Functional Stress Test starts..."); 
		   	Isp_I2cTest();	
        }
        else if(arg1 == 2)
        {
              PRINT("Camera VerifApp: ISP CSI Functional Stress Test starts...");
		   	  CSI_BER_Test();

        }
		else if(arg1 == 3)
        {
            testMsg.Event   = (DWORD) PLATFORM_MSG_CAM_TEST_APP;
            testMsg.SubMsg  = (DWORD) 3;
            RE_RimPostMessage( PLATFORM_THREAD_ID_APP0, &testMsg );      
        }          
      
        else        
        {
            PRINT("===== Camera VerifApp Help =====");
            PRINT("x vf.cam.func 1 : ISP I2C Functional Stress Test");
            PRINT("x vf.cam.func 2 : ISP CSI Functional Stress Test");
            PRINT("x vf.cam.func 3 : sample to generate msg");
            PRINT("place holder");	
        }
    }
    else if(strcmp(cmd, "stat") == 0)
    {    
        //CamConfig * cfgPtr = DrvGetConfig(drv);
		if(arg1 == 1)
        {
            PRINT("Listing CSI Registers");  
			DrvControl(drv, CAM_CMD_DUMP_REG);
        }
        else if(arg1 == 2)
        {
			DrvControl(drv, CAM_CMD_DRIVER_SHOW);
        }
        else if(arg1 == 3)
        {
            PRINT("CSI Link Configuration Status");
			//PRINTN("Camera VerifApp: ISP I2C Test Cycle is set %d", I2c_TestCycle);
			//PRINTN("Camera VerifApp: ISP I2C Test operating address is set to 0x%x",I2c_Opt_Addr);
			PRINTN("Verfication: Error control is set to %d", cfgPtr->csiConfig.errControl);
			PRINTN("Verfication: Number of lanes is set to %d", cfgPtr->csiConfig.CsiLaneNum);
			PRINTN("Verfication: Data rate is set to %d (1 - 200Mbps, 2 - 480Mbps, 3 - 600Mbps, 4 - 700Mbps, 5 - 800Mbps)", cfgPtr->csiConfig.DataRate);
			PRINTN("Verfication: Test mode (Colorbar) is set to %d (1 - disabled, 2 - enabled)", cfgPtr->ispTestMode);
        }		
        else        
        {
            PRINT("===== Camera VerifApp Help =====");
            PRINT("x vf.cam.stat 1 : Show CSI amd SCI Registers");
            PRINT("x vf.cam.stat 2 : Show driver statistics");
            PRINT("x vf.cam.stat 3 : Show Current Configuration of CSI link");

            PRINT("place holder");	
        }
    }	
    //else if(strcmp(cmd, "help") == 0)
	else
    {
        PRINT("===== Camera VerifApp Help =====");
        PRINT("x vf.cam.cfg: configuration command help ");
        PRINT("x vf.cam.para: parametric measurement command help");
        PRINT("x vf.cam.func: functional test command help");
        PRINT("x vf.cam.stat: status check command help");		
    }
}
