/*****************************************************************************
 * Filename:    disTest.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:     Tai Yu 
 *
 * Created:     Nov 2009
 *
 * Description: Verification code for PV2 display DSI driver using driver framework
 *
 *****************************************************************************/
/* include header files */

/* standard library header files */
#include <string.h>

#include "rei.h"
#include "framework.h"
#include "display.h"
#include "driver.h"
#include "display_cmd.h"
#include "stdlib.h" // rand()
#define SRCGROUP GROUP_PLT_APP
#define SRCFILE  FILE_TST_DIS
#define PATTERN_TYPE 2
#define PATTERN_SIZE 8
#define FIVE_SECONDS 5
typedef enum hwDSITestResultTag
{
	DSI_TEST_FAILED,
	DSI_TEST_PASSED,
	DSI_TEST_ERROR,
	DSI_NO_RESULT_AVAILABLE
} hwDSITestResult;
typedef enum {
    DIS_MSG_TEST_TIMER_EXPIRED = 0,
    DIS_MSG_TEST_TIMER_START=21,
    DIS_MSG_TEST_TIMER_STOP=22,		
	DIS_MSG_TEST_START=23,
	//DSI_MSG_TEST_START=12,
	DSI_MSG_LC_START=24	
}DsiTestMsg;

typedef enum {
	TPO,
	eDISCO,
	Proteus,
	National = 0xff	
}DSIdevice;

typedef enum {
	DSI_CHN_0=0,
	DSI_CHN_1=1	
}DsiId;

typedef enum DispModeTag
{
	Disp_Pipeline,
	Disp_Loopbuf
} RunningMode;

typedef enum PattenTag
{
	WorstCase,
	BestCase
} RunningPattern;


static BOOL heartbeat=TRUE;
static hwDSITestResult DsitestResult = DSI_NO_RESULT_AVAILABLE;
static DsiId CurrentDsiId=DSI_CHN_1;
static RunningMode testRunningMode=Disp_Pipeline;
static DSIdevice TestDevice=TPO;
static RunningPattern testPattern=WorstCase;
static Driver_t * drv;
static lcdConfig * cfgPtr;
static lcdStatus * staPtr;
static lcdStats * cntPtr;
static RE_MESSAGE disMsg;
static UINT TestCycle = 10;
static UINT DSI_BER_Duration_minutes=1;
static Data4Lcd_t lcdData,lcdDataBTA;

static UINT Err_Ctrl =2; //real time stop for errors
static BOOL DisplayTraffic=TRUE;
static DWORD Counter_5s=0, Counter_1m=0,Dsi_tmr;
static BOOL ULPS_TEST=FALSE;
static BOOL LPDT_TEST=FALSE;
static DWORD SoF,EoF;

/* Forward function declaration */
static void disTestDebug(char * cmd, DWORD arg1, DWORD arg2);
static QWORD rtcTicksBefore,Throughput;
//static QWORD totalTick,tickUsed;
static DWORD test_duration;
static BYTE PatternSeed[PATTERN_TYPE][PATTERN_SIZE]={\
		{0xAA,0x33,0xF0,0x7F,0x55,0xCC,0x0F,0x80},\
		{0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00}};
static BYTE Pilot[512];
static BYTE PilotReturn[512];
void VfDsiTestProcessMsg(MESSAGE * msg);
static int test_result = 0; 
static DWORD repeat=3;
static BYTE DispVirtChan = 0;
static void ShowRxErrors()
{
		PRINTN("DB_HW_TEST: Error Status for DSI port[%d].... ",CurrentDsiId);
		PRINTN("DB_HW_TEST: CRC error number is %d", cntPtr->dsiStats[CurrentDsiId].dsiRtnChecksumErr); 
		PRINTN("DB_HW_TEST: 1b ECC error number is %d", cntPtr->dsiStats[CurrentDsiId].dsiRtnEcc1BitErr); 
		PRINTN("DB_HW_TEST: 2b ECC error number is %d", cntPtr->dsiStats[CurrentDsiId].dsiRtnEccBitsErr); 
		PRINTN("DB_HW_TEST: SoT error number is %d", cntPtr->dsiStats[CurrentDsiId].dsiRtnAckSotErr);
	   	PRINTN("DB_HW_TEST: SoT Sync error number is %d", cntPtr->dsiStats[CurrentDsiId].dsiRtnAckSotSyncErr);
 		PRINTN("DB_HW_TEST: EoT error number is %d", cntPtr->dsiStats[CurrentDsiId].dsiRtnAckEotSyncErr);
 		PRINTN("DB_HW_TEST: HS Rx timeout error number is %d", cntPtr->dsiStats[CurrentDsiId].dsiRtnHsRcvTimeoutErr); 		
 		PRINTN("DB_HW_TEST: Escape mode entry error number is %d", cntPtr->dsiStats[CurrentDsiId].dsiRtnEscModEntryCmdErr); 
		PRINTN("DB_HW_TEST: LP transmit error number is %d", cntPtr->dsiStats[CurrentDsiId].dsiRtnLpTransSyncErr);
 		PRINTN("DB_HW_TEST: Flase control error number is %d", cntPtr->dsiStats[CurrentDsiId].dsiRtnFalseCtlErr);
 		PRINTN("DB_HW_TEST: Invalid transmission length error number is %d", cntPtr->dsiStats[CurrentDsiId].dsiRtnTransLenErr);
 		PRINTN("DB_HW_TEST: DSI protocol violation error number is %d", cntPtr->dsiStats[CurrentDsiId].dsiRtnDsiProErr);

}


static void StopDsiTest()
{
		PRINT("shut down DSI traffic from test app.... ");
		RE_TimerStop(Dsi_tmr);
        RE_TimerDelete(Dsi_tmr);
        PRINT("DSI timer deleted");    
		//tickUsed=ReadRtcWhole()-rtcTicksBefore;
		//SHOW_MEMORY("tickUsed is:", &tickUsed, 8); //@@@@
	
		//SHOW_MEMORY("totalTick is:", &totalTick, 8); //@@@@	
		//SHOW_MEMORY("rtcTicksBefore is:", &rtcTicksBefore, 8); //@@@@
		PRINTN("Totoal DSI Tx SoF is %d",cntPtr->dsiStats[CurrentDsiId].dsiDispSof); //total frame received
		PRINTN("Totoal DSI Tx EoF is %d",cntPtr->dsiStats[CurrentDsiId].dsiDispEof); //total frame received

		PRINT2N("Display Image Resolution is %d x %d",lcdData.height, lcdData.width); 			

		Throughput=(((lcdData.height*lcdData.width>>10)*cntPtr->dsiStats[CurrentDsiId].dsiDispEof)>>10)*8*3/cfgPtr->dsiConfig[CurrentDsiId].activeLanes; //RBG24
		PRINTN("Traffic throughput in this test case is %d Mbits per lane ", Throughput);			
		if((cntPtr->dsiStats[CurrentDsiId].dsiDispSof-cntPtr->dsiStats[CurrentDsiId].dsiDispEof)>10)
		{
			//test_result = 1;
			WARN("DSI SoF and EoF mismatch!!");
		}
		if(cntPtr->dsiStats[CurrentDsiId].dsiRtnAckSotErr!=0 || cntPtr->dsiStats[CurrentDsiId].dsiRtnAckSotSyncErr!=0 || cntPtr->dsiStats[CurrentDsiId].dsiRtnEcc1BitErr!=0 || cntPtr->dsiStats[CurrentDsiId].dsiRtnEccBitsErr!=0 || cntPtr->dsiStats[CurrentDsiId].dsiRtnChecksumErr!=0 )
		{
			test_result = 1;
			WARN("Error detected!!");
		}		
        //DrvDisable( drv, 0 ); //disable drv and stop traffic
		DrvControl( drv, DISP_CNTRL_DSI_STOP_TRAFFIC, CurrentDsiId);		
		//test_duration=(ReadRtcWhole()-rtcTicksBefore)/32760/60;


		PRINT2N("DB_HW_TEST: DSI[%d] Link Speed setting is %d (0->52Mbps;1->104Mbps;2->156Mbps;3->208Mbps;4->312Mbps;5->400Mbps;6->624Mbps)",CurrentDsiId,cfgPtr->dsiConfig[CurrentDsiId].dsiLinkClock);		
		PRINT2N("DB_HW_TEST: DSI[%d] active Lane Number is %d", CurrentDsiId, cfgPtr->dsiConfig[CurrentDsiId].activeLanes);		
		PRINTN("DB_HW_TEST: Totoal test duration for this test case is %d minutes", Counter_1m); //real test duaration
		ShowRxErrors();
		//PRINTN("DB_HW_TEST: SoT error number is %d", cntPtr->csiStatis.csiErrSoT); 
    	if (test_result == 0) {
        	PRINT("DB_HW_TEST: ###################DSI test PASS ############################");
			DsitestResult = DSI_TEST_PASSED;
		}
    	else {
        	WARN("DB_HW_TEST: ####################DSI test FAIL###########################");
			DsitestResult = DSI_TEST_FAILED;
		}	
}




BOOL LinkCheck()
{

		//DWORD SeedSize=sizeof(PatternSeed);
		DWORD i=0; //repeat should <64
		//BYTE *p=&Pilot[0];
		//BYTE *pr=&PilotReturn[0];
		//PRINTN("SeedSize is %d bytes",sizeof(PatternSeed));
		BOOL test=FALSE;		
		for(i=0;i<PATTERN_SIZE*repeat;i++)
		{
			Pilot[i]=PatternSeed[0][i/repeat];
			//PRINT2N("Pilot[%d] is 0x%x ",i,Pilot[i]);
		}
		for(i=0;i<PATTERN_SIZE*repeat;i++)
		{
				PilotReturn[i]=0;
		}		
        cfgPtr->dsiConfig[CurrentDsiId].powerMode = DSI_POWERMODE_HS_NORIHS;

        //lcdData.lcdPanelId = DispVirtChan;
        lcdData.dataType = DISP_DATA_PIXELS;
        lcdData.pixelFormat = DISP_PIXELFRT_RGB24;
        lcdData.dataAddr = (DWORD)&Pilot[0];
		if(TestDevice==TPO)
		{		
        lcdData.height = 480;  //TPO
        lcdData.width = 864; //TPO
		PRINT("MG LPDT Speed set to default 52M to match up TPO Tlpx");
		}
		else if (TestDevice==Proteus)
		{
		lcdData.height = 440;  //TPO
		lcdData.width = 480; //TPO
		PRINT("MG LPDT Speed set to default 52M to match up Proteus Tlpx");
		}
		//lcdData.length = sizeof(PatternSeed)*repeat/sizeof(DWORD); //cast to DWORD
		lcdData.length = PATTERN_SIZE*repeat; //cast to DWORD
        lcdData.yPos = 0;
        lcdData.xPos = 0;
        lcdData.repeatTimes = 0x0;
		PRINTN("lcdData.length is %d bytes",lcdData.length);
		DispSetDsiId( CurrentDsiId );
		//PRINTN("lcdData1 is %8x bytes",lcdData.dataAddr);
        //DrvEnable( drv, 0 );	
		DispSmartPanelInit( TestDevice );//enable TPO
		RE_HwDelay(1000*RE_MILLISECOND);
        //DrvEnable( drv, 0 );		
        DispSmartPanelTransmit( &lcdData );
		RE_HwDelay(1000*RE_MILLISECOND);
		if(ULPS_TEST==TRUE)
		{
			PRINT("Entering ULPS now...");
            DrvControl( drv, DISP_CNTRL_DSI_ENTER_ULPS, CurrentDsiId);
			RimSleep(1000);
			PRINT("Exiting ULPS now...");			
            DrvControl( drv, DISP_CNTRL_DSI_EXIT_ULPS, CurrentDsiId);
		    //RE_HwDelay(1000*RE_MILLISECOND);

		}

        lcdData.dataAddr = (DWORD)&PilotReturn[0];
        DispSmartPanelReceive( &lcdData );
		RE_HwDelay(1000*RE_MILLISECOND);
        //DrvDisable( drv, 0 );
		DrvControl( drv, DISP_CNTRL_DSI_STOP_TRAFFIC, CurrentDsiId);
		if(DisplayTraffic==TRUE)
		{	
			for(i=0;i<PATTERN_SIZE*repeat;i++)
			{
			PRINT2N("Pilot[%d] is 0x%x ",i,Pilot[i]);
			}		
			for(i=0;i<PATTERN_SIZE*repeat;i++)
			{
			PRINT2N("PilotReturn[%d] is 0x%x ",i,PilotReturn[i]);
			}
		}
		for(i=0;i<(PATTERN_SIZE*repeat);i++)
		{
			if(Pilot[i]!=PilotReturn[i])
			{
			PRINT3N("Pilot[%d] is 0x%x, but PilotReturn is 0x%x ",i,Pilot[i],PilotReturn[i]);
			test=FALSE;
			break;
			}
			test=TRUE;
		}
		if(test==TRUE)
				return TRUE;
		else
				return FALSE;
		
}		

void LinkCheckColorBar() //visual inspection on coloar bar switching
{

		//DWORD SeedSize=sizeof(PatternSeed);
		DWORD i=0; //repeat should <64
		//BYTE *p=&Pilot[0];
		//BYTE *pr=&PilotReturn[0];
		//PRINTN("SeedSize is %d bytes",sizeof(PatternSeed));
		BOOL test=FALSE;		
	
        cfgPtr->dsiConfig[CurrentDsiId].powerMode = DSI_POWERMODE_HS_NORIHS;

        //lcdData.lcdPanelId = DispVirtChan;
        //lcdData.dataType = DISP_TEST_COLOR_BAR;
        lcdData.pixelFormat = DISP_PIXELFRT_RGB24;
        lcdDataBTA.dataAddr = (DWORD) NULL;
		if(TestDevice==TPO)
		{		
        lcdData.height = 480;  //TPO
        lcdData.width = 864; //TPO
		PRINT("MG LPDT Speed set to default 52M to match up TPO Tlpx");
		}
		else if (TestDevice==Proteus)
		{
		lcdData.height = 440;  //TPO
		lcdData.width = 480; //TPO
		PRINT("MG LPDT Speed set to default 52M to match up Proteus Tlpx");
		}
		//lcdData.length = sizeof(PatternSeed)*repeat/sizeof(DWORD); //cast to DWORD
		lcdData.length = PATTERN_SIZE*repeat; //cast to DWORD
        lcdData.yPos = 0;
        lcdData.xPos = 0;
        lcdData.repeatTimes = 0x0;
		PRINTN("lcdData.length is %d bytes",lcdData.length);
		//PRINTN("lcdData1 is %8x bytes",lcdData.dataAddr);
		DispSmartPanelInit( TestDevice );//enable TPO
		RE_HwDelay(1000*RE_MILLISECOND);		
        //DrvEnable( drv, 0 );		
        DispSmartPanelTransmit( &lcdData );
		RE_HwDelay(1000*RE_MILLISECOND);

        //DrvDisable( drv, 0 );
		DrvControl( drv, DISP_CNTRL_DSI_STOP_TRAFFIC, CurrentDsiId);
}	


BOOL LPDT_LinkCheck()
{
		DWORD i=0; //repeat should <64
		BOOL test=FALSE;		
		for(i=0;i<PATTERN_SIZE*repeat;i++)
		{
			Pilot[i]=PatternSeed[0][i/repeat];
		}
		for(i=0;i<PATTERN_SIZE*repeat;i++)
		{
				PilotReturn[i]=0;
		}
        cfgPtr->dsiConfig[CurrentDsiId].powerMode = DSI_POWERMODE_LPDT;
		if(TestDevice==TPO)
		{		
        lcdData.height = 480;  //TPO
        lcdData.width = 864; //TPO
		PRINT("MG LPDT Speed set to default 52M to match up TPO Tlpx");
		}
		else if (TestDevice==Proteus)
		{
		lcdData.height = 440;  //TPO
		lcdData.width = 480; //TPO
		PRINT("MG LPDT Speed set to default 52M to match up Proteus Tlpx");
		}
			
        lcdData.dataType = DISP_DATA_PIXELS;
        lcdData.pixelFormat = DISP_PIXELFRT_RGB24;
        lcdData.dataAddr = (DWORD)&Pilot[0];
		lcdData.length = PATTERN_SIZE*repeat; //cast to DWORD
        lcdData.yPos = 0;
        lcdData.xPos = 0;
        lcdData.repeatTimes = 0x0;
		PRINTN("lcdData.length is %d bytes",lcdData.length);

        DispSetDsiId( CurrentDsiId );
		DispSmartPanelInit( TestDevice );//enable TPO
        DispSmartPanelTransmit( &lcdData );
		RimSleep(2000);

        lcdData.dataAddr = (DWORD)&PilotReturn[0];
        DispSmartPanelReceive( &lcdData );
		RimSleep(2000);

		if(ULPS_TEST==TRUE)
		{
			PRINT("Entering ULPS...");
            DrvControl( drv, DISP_CNTRL_DSI_ENTER_ULPS, CurrentDsiId);
			RimSleep(1000);
			PRINT("Exiting ULPS...");			
            DrvControl( drv, DISP_CNTRL_DSI_EXIT_ULPS, CurrentDsiId);
		    //RE_HwDelay(1000*RE_MILLISECOND);

		}
        //DrvDisable( drv, 0 );
        
		if(DisplayTraffic==TRUE)
		{	
			for(i=0;i<PATTERN_SIZE*repeat;i++)
			{
			PRINT2N("Pilot[%d] is 0x%x ",i,Pilot[i]);
			}		
			for(i=0;i<PATTERN_SIZE*repeat;i++)
			{
			PRINT2N("PilotReturn[%d] is 0x%x ",i,PilotReturn[i]);
			}
		}
		for(i=0;i<(PATTERN_SIZE*repeat);i++)
		{
			if(Pilot[i]!=PilotReturn[i])
			{
			PRINT3N("Pilot[%d] is 0x%x, but PilotReturn is 0x%x ",i,Pilot[i],PilotReturn[i]);
			test=FALSE;
			break;
			}
			test=TRUE;
		}
		if(test==TRUE)
				return TRUE;
		else
				return FALSE;

}		


void VfDsiTestProcessMsg(MESSAGE * msg) //test thread
{
    
 	DWORD subMsg, eventId;
	DWORD i=0;
	subMsg = msg->SubMsg;
	eventId = msg->Event;
   	//data1=msg->Data[1];	
	if(heartbeat==TRUE)
	PRINT2N("DSI Testapp: ****** Received message - %d, %d", msg->Event, msg->SubMsg);
  
    if(((lcdMsg) subMsg ==LCD_MSG_DSI_ERR)&&(eventId==PLATFORM_MSG_DISP_DRV))
    {
        
		if(	(cntPtr->dsiStats[CurrentDsiId].dsiRtnChecksumErr==0) && \
		(cntPtr->dsiStats[CurrentDsiId].dsiRtnEcc1BitErr==0 ) && \
		//(cntPtr->dsiStats[CurrentDsiId].dsiRtnEccBitsErr==0) && \ 
		(cntPtr->dsiStats[CurrentDsiId].dsiRtnAckSotErr==0) && \
	   	(cntPtr->dsiStats[CurrentDsiId].dsiRtnAckSotSyncErr==0) && \
 		(cntPtr->dsiStats[CurrentDsiId].dsiRtnAckEotSyncErr==0) && \
 		//(cntPtr->dsiStats[CurrentDsiId].dsiRtnHsRcvTimeoutErr==0) && \ 		
 		//(cntPtr->dsiStats[CurrentDsiId].dsiRtnEscModEntryCmdErr==0) && \ 
		//(cntPtr->dsiStats[CurrentDsiId].dsiRtnLpTransSyncErr==0) && \
 		//(cntPtr->dsiStats[CurrentDsiId].dsiRtnFalseCtlErr ==0) && \
 		//(cntPtr->dsiStats[CurrentDsiId].dsiRtnTransLenErr==0) && 
 		(cntPtr->dsiStats[CurrentDsiId].dsiRtnDsiProErr==1))
		{		
		WARN("DSI Protocol Violation Error captured"); // suspect only read two bytes from TPO causing protocol violaiton, remove this once Marvell got this issue fixed on MG B0
		}
		else
		{		
		WARN("DSI Error captured");
		test_result = 1;
        if(Err_Ctrl==2)
		{	
		WARN("Packet Error captured, stop streaming");
		//test_result = 1;		
 		StopDsiTest();
        //return;
		}
		}
    }
    else if(((DsiTestMsg)subMsg ==DIS_MSG_TEST_TIMER_START)&&(eventId==PLATFORM_MSG_DIS_TEST_APP))
    {
		
		PRINT("enter start msg...");	
		Dsi_tmr = RE_TimerCreate();
        RE_TimerStart(Dsi_tmr, msg->Data[0] * RE_TicksPerSecond(), 
                          (msg->Data[1] == 0) ? TIMER_ONE_SHOT : TIMER_PERIODIC, 
                          PLATFORM_MSG_DIS_TEST_APP); 
        PRINT2N("timer started: %d, %d", msg->Data[0], msg->Data[1]);  		
		Counter_5s=0;
		Counter_1m=0;
		Throughput=0;		
		test_result = 0; 
		SoF=0;
		EoF=0;
        //DrvDisable( drv, 0 );
		DrvControl( drv, DISP_CNTRL_DSI_STOP_TRAFFIC, CurrentDsiId);	
		if(staPtr ->dsiStatus[CurrentDsiId].enabled==TRUE)   
    	{
			WARN("display driver already enabled...");
			//WARN("display driver already enabled, this test is not running..");
			//test_result = 1;
        	//return;
    	}
		DrvControl(drv, DISP_CNTRL_DSI_RESET_STATISTICS,CurrentDsiId);
	    PRINTN("Init Number of frames transmitted at DSI panel now is %d", cntPtr->dsiStats[CurrentDsiId].dsiDispEof); 		
		PRINT( "DSI Testapp: Initialize Samsung Chip" );
		DispSetDsiId(CurrentDsiId);
       

	  	if(testRunningMode ==Disp_Pipeline)
		{
			lcdData.dataType = DISP_DATA_PIXELS;
			PRINT( "DSI Testapp: Running at pipeline mode" );
		}
		else if(testRunningMode ==Disp_Loopbuf)
		{
			lcdData.dataType = DISP_TEST_COLOR_BAR;
			PRINT( "DSI Testapp: Running at loop buffer mode" );
		}
		
			if(LinkCheck()==FALSE)
    		{
			WARN("PilotCheck fails, quitting this test..");
			test_result = 1;
			StopDsiTest();
        	return;
    		}
		
        DispSetDsiId(CurrentDsiId);
        //DrvDisable( drv, 0 );
		//
		//BTA lcdData init
		lcdDataBTA.dataAddr = (DWORD) NULL;
        lcdDataBTA.dataType = DISP_DATA_DSI_BTA;


        cfgPtr->dsiConfig[CurrentDsiId].powerMode = DSI_POWERMODE_HS_NORIHS;

        lcdData.lcdPanelId = DispVirtChan;
        //lcdData.dataType = DISP_DATA_PIXELS;
        lcdData.pixelFormat = DISP_PIXELFRT_RGB24;
		if(testPattern==WorstCase)
		{
			for(i=0;i<PATTERN_SIZE*repeat;i++)
			{
			//Pilot[i]=PatternSeed[0][i/repeat];
			Pilot[i]=PatternSeed[0][rand()%(PATTERN_SIZE)];
			PRINT2N("Pilot[%d] is 0x%x ",i,Pilot[i]);
			}				
		}
		else if(testPattern==BestCase)
		{

			for(i=0;i<PATTERN_SIZE*repeat;i++)
			{
			Pilot[i]=PatternSeed[1][i/repeat];
			PRINT2N("Pilot[%d] is 0x%x ",i,Pilot[i]);
			}	

		}
        //lcdData.dataAddr = (DWORD) NULL;
		if(lcdData.dataType == DISP_TEST_COLOR_BAR)
		lcdData.dataAddr = (DWORD) NULL;
		else		
		lcdData.dataAddr = (DWORD)&Pilot[0];

		lcdData.length = PATTERN_SIZE*repeat; 		
		if(TestDevice==TPO)
		{		
        lcdData.height = 480;  //TPO
        lcdData.width = 864; //TPO
		}
		else if (TestDevice==eDISCO)
		{
        lcdData.height = 480;  //eDisco
        lcdData.width = 640; //eDisco
		}
		else if (TestDevice==Proteus)
		{
        lcdData.height = 440;  //eDisco
        lcdData.width = 480; //eDisco
		}

        lcdData.yPos = 0;
        lcdData.xPos = 0;
        lcdData.repeatTimes = (0xFFFFFFFF-10);
		//DrvEnable( drv, 0 );
 		// DispSmartPanelInit( 0 );//enable TPO
		//RE_HwDelay(100*RE_MILLISECOND);		
        //DrvEnable( drv, 0 );
		RE_HwDelay(100*RE_MILLISECOND);		
		DispSmartPanelTransmit( &lcdData ); //start transmitting..
		RimSleep(4000);
		if(staPtr ->dsiStatus[CurrentDsiId].enabled==TRUE)
		//staPtr ->dsiStatus[CurrentDsiId].enabled==DSI_ENABLED
    	{
			PRINT("DSI enabled, you should see me");
    	}
		//pf.cam.show 0 1 for counters
		PRINTN("Number of frames transmitted at DSI panel SoF now is %d",cntPtr->dsiStats[CurrentDsiId].dsiDispSof); //total frame received
		PRINTN("Number of frames transmitted at DSI panel now is %d",cntPtr->dsiStats[CurrentDsiId].dsiDispEof); //total frame received
		if(cntPtr->dsiStats[CurrentDsiId].dsiDispEof==0)
		{
				//WARNN("Number of frames transmitted at DSI panel now is %d", cntPtr->dsiStats[CurrentDsiId].dsiDispEof); //total frame received
				WARN("No Frame transmitted in current setting, this test is not running");
				test_result = 1;
				PRINT2N("DSI[%d] Link Speed setting is %d 0->52Mbps;1->104Mbps;2->156Mbps;3->208Mbps;4->312Mbps;6->624Mbps",CurrentDsiId,cfgPtr->dsiConfig[CurrentDsiId].dsiLinkClock);
				return;
		}
	}
    else if(((DsiTestMsg)subMsg ==DIS_MSG_TEST_TIMER_EXPIRED)&&(eventId==PLATFORM_MSG_DIS_TEST_APP))	
     	
	{			
			Counter_5s++;
	  		if(Counter_5s%12==0)
			{
				Counter_1m++;
				PRINTN("%d Minutes elapsed..",Counter_1m);
				PRINTN("Totoal DSI Tx SoF is %d",cntPtr->dsiStats[CurrentDsiId].dsiDispSof); //total frame received
				PRINTN("Totoal DSI Tx EoF is %d",cntPtr->dsiStats[CurrentDsiId].dsiDispEof); //total frame received	
				if((SoF==cntPtr->dsiStats[CurrentDsiId].dsiDispSof)||(EoF==cntPtr->dsiStats[CurrentDsiId].dsiDispEof))
				{
						WARN("Frame freezing");
						test_result = 1;
						StopDsiTest();
						return;						

				}
				SoF=cntPtr->dsiStats[CurrentDsiId].dsiDispSof;
				cntPtr->dsiStats[CurrentDsiId].dsiDispEof;

							
			}	
			
			if((Counter_1m <DSI_BER_Duration_minutes) && (cfgPtr->dsiConfig[CurrentDsiId].status==DSI_ENABLED))			
	 		{
	 	
			//RimSleep(5000);
			//if(heartbeat==TRUE)
        	//DrvDisable( drv, 0 ); //stop traffic
        	//DrvControl( drv, DISP_CNTRL_DSI_STOP_TRAFFIC, CurrentDsiId);
			//RE_HwDelay(10*RE_MILLISECOND);
        	//DrvEnable( drv, 0 ); //start bta

			if(heartbeat==TRUE) {
			    PRINT("Checking Rx error");
			}
			//RE_HwDelay(50*RE_MILLISECOND);
			//RimSleep(100);
			//RE_HwDelay(10*RE_MILLISECOND);
        	   DrvControl( drv, DISP_CNTRL_DSI_STOP_TRAFFIC, CurrentDsiId);
        	// DrvDisable( drv, 0 );
			 RimSleep(100);
        	lcdData.repeatTimes = (0xFFFFFFFF-10);
			//DrvEnable( drv, 0 );
			//RE_HwDelay(10*RE_MILLISECOND);		
			DispSmartPanelTransmit( &lcdData ); //start transmitting..
	 		}
		    else
			{	
			StopDsiTest();
			}
		
	}
    else if(((DsiTestMsg)subMsg ==DSI_MSG_LC_START)&&(eventId==PLATFORM_MSG_DIS_TEST_APP))
	{

			/*
			if(TestDevice==TPO&&LinkCheckTPO()==TRUE)
					PRINTN("TPO DSI port[%d] passed link check",CurrentDsiId);
			else if(TestDevice==eDISCO&&LinkCheck()==TRUE)
					PRINTN("DSI port[%d] passed link check",CurrentDsiId);
			else
					WARNN("DSI port[%d] failed link check",CurrentDsiId);
					*/
			if(LinkCheck()==FALSE)
    		{
			WARNN("DSI port[%d] failed link check",CurrentDsiId);
    		}
			else
			PRINTN("DSI port[%d] passed link check",CurrentDsiId);
	}
			
	else
		WARN("undefiend message");	

	RimSleep(500);



}

/*****************************************************************************
 * Function:    disTestInit 
 * Description: Initialization function for the display test code. This function
 *              is called by platformMain during the Platform Software
 *              initialization
 * Parameters:  Void
 * Return:      Void
 *
 *****************************************************************************/
void disTestInit(void)
{  
   	PRINT(" disTestInit");
   /* Test code initialization - memory, static variables, bugdisp etc. */ 
    
   /* Register Bugdisp commands */
   	VfCmdRegister("dis", "DSI Verification Code", &disTestDebug);
    /* register message handler */
    //PfMsgRegister(PLATFORM_THREAD_ID_APP0, PLATFORM_MSG_CAM_DRV,  cbProcessMsg);  
    PfMsgRegister(PLATFORM_THREAD_ID_APP0, PLATFORM_MSG_DIS_TEST_APP, VfDsiTestProcessMsg);
	PfMsgRegister(PLATFORM_THREAD_ID_APP0, PLATFORM_MSG_DISP_DRV, VfDsiTestProcessMsg);
	drv = DrvGetHandle(IODRV_ID_DISPLAY);
    if(drv == NULL)
    {
        WARN("Invalid display driver pointer");
        return;
    }
	else
	{		
	cfgPtr = DrvGetConfig(drv);
	    if(cfgPtr == NULL)
    	{
        WARN("Invalid display cfg pointer");
        return;
		}
		cfgPtr->dsiConfig[DSI_CHN_1].panelId = 0x00; //TPO panel id
		cfgPtr->dsiConfig[DSI_CHN_0].panelId = 0x00; //TPO panel id
	staPtr = DrvGetStatus(drv);
	    if(staPtr == NULL)
    	{
        WARN("Invalid display status pointer");
        return;
		}
	cntPtr = DrvGetStats(drv);
	    if(cntPtr == NULL)
    	{
        WARN("Invalid display statis pointer");
        return;
		}

	}
}



void DSI_BER_Test()
{
   	//post msg to trigger test
	//disMsg.Event   = (DWORD) PLATFORM_MSG_DIS_TEST_APP;
    //disMsg.SubMsg  = (DWORD) DSI_MSG_TEST_START;
    //RE_RimPostMessage( PLATFORM_THREAD_ID_APP0, &disMsg );    
   	//post msg to trigger test
	disMsg.Event   = (DWORD) PLATFORM_MSG_DIS_TEST_APP;
    disMsg.SubMsg  = (DWORD) DIS_MSG_TEST_TIMER_START;
    disMsg.Data[0] = FIVE_SECONDS;  //5 sec delay
    disMsg.Data[1] = 2; // 0 - one-shot; otherwise periodic
    RE_RimPostMessage( PLATFORM_THREAD_ID_APP0, &disMsg );      


}


/*****************************************************************************
 * Function:    disTestDebug 
 * Description: Function to handle the bugdisp commands. Note both arg1 and arg2 
 *              are only 16-bit wide (they really shouldn't be DWORD).
 * Parameters:  
 * Return:      
 *
 *****************************************************************************/

static void disTestDebug(char * cmd, DWORD arg1, DWORD arg2)
{
    //Driver_t * drv = DrvGetHandle(IODRV_ID_CAMERA);

        
    if(strcmp(cmd, "cfg") == 0)
    {
        //Config * cfgPtr = DrvGetConfig(drv);
        if(cfgPtr == NULL)
        {
            WARN("Invalid display driver config pointer");
            return;
        }
            
        if(arg1 == 1)
        {
            
             /* DSI BER test duration # */
			if(arg2!=0)				
            DSI_BER_Duration_minutes = arg2;
            PRINTN("Display VerifApp: BER Test Duration is set to to %d minutes", DSI_BER_Duration_minutes);
  
  
        }
        else if(arg1 == 2)   
        {

			if(arg2!=0)	 
			{
				if(arg2==1)
				{		
				CurrentDsiId=DSI_CHN_0;
				PRINT("Verfication: Set to DSI port 0");
				}
				if(arg2==2)
				{		
				CurrentDsiId=DSI_CHN_1;
				PRINT("Verfication: Set to DSI port 1");
				}
			}				

        }
        else if(arg1 == 3)   
        {

			if(arg2!=0)	 
			{
				if(arg2==1)
				{		
				cfgPtr->dsiConfig[CurrentDsiId].dsiLinkClock = DSI_LINK_CLK_52M;
				PRINTN("Verfication: Set DSI[%d] speed to 52M", CurrentDsiId);
				}
				else if(arg2==2)
				{		
				cfgPtr->dsiConfig[CurrentDsiId].dsiLinkClock = DSI_LINK_CLK_104M;
				PRINTN("Verfication: Set DSI[%d] speed to 104M", CurrentDsiId);
				}
				else if(arg2==3)
				{		
				cfgPtr->dsiConfig[CurrentDsiId].dsiLinkClock = DSI_LINK_CLK_156M;
				PRINTN("Verfication: Set DSI[%d] speed to 156M", CurrentDsiId);
				}
				else if(arg2==4)
				{		
				cfgPtr->dsiConfig[CurrentDsiId].dsiLinkClock = DSI_LINK_CLK_208M;
				PRINTN("Verfication: Set DSI[%d] speed to 208M", CurrentDsiId);
				}
				else if(arg2==5)
				{		
				cfgPtr->dsiConfig[CurrentDsiId].dsiLinkClock = DSI_LINK_CLK_312M;
				PRINTN("Verfication: Set DSI[%d] speed to 312M", CurrentDsiId);
				}
				else if(arg2==6)
				{		
				cfgPtr->dsiConfig[CurrentDsiId].dsiLinkClock = DSI_LINK_CLK_DDR2;
				PRINTN("Verfication: Set DSI[%d] speed to 400M(DDR2)", CurrentDsiId);
				}
				else if(arg2==7)
				{		
				cfgPtr->dsiConfig[CurrentDsiId].dsiLinkClock = DSI_LINK_CLK_624M;
				PRINTN("Verfication: Set DSI[%d] speed to 624M", CurrentDsiId);
				}
				else
            	PRINT("unknown speed setting");

			}				

        }

        else if(arg1 == 4)   
        {

			if(arg2!=0)	 
			{
				if(arg2==1)
				{		
				cfgPtr->dsiConfig[CurrentDsiId].activeLanes = 1;
				}
				else if(arg2==2)
				{		
				cfgPtr->dsiConfig[CurrentDsiId].activeLanes = 2;
				}
				else if(arg2==3)
				{		
				cfgPtr->dsiConfig[CurrentDsiId].activeLanes = 3;
				}
				else
            	PRINT("unknown active lane setting");
		 	PRINT2N("Verfication: Set DSI[%d] active lane number to %d", CurrentDsiId, cfgPtr->dsiConfig[CurrentDsiId].activeLanes);	
			}				

        }
        else if(arg1 == 5)
        {
            PRINT("current unavailabe");
		}		

        else if(arg1 == 6)
        {
            
			if(arg2!=0)	
			/* Set control option for DSI errors: 1 -continue or 2 - stop */
			{
			if(arg2==1)//continue
			Err_Ctrl=1;
			else
			Err_Ctrl=2;//stop
			
            cfgPtr->dsiConfig[CurrentDsiId].errControl = (DsiErrControl)arg2;
            PRINT2N("Verfication: Set DSI[%d] Error control to %d", CurrentDsiId, cfgPtr->dsiConfig[CurrentDsiId].errControl);
			}

		}
        else if(arg1 == 7)
        {
			if(arg2!=0)	 
			{
				if(arg2==1)
				heartbeat=TRUE;
				if(arg2==2)
				heartbeat=FALSE;						
			}
		}
        else if(arg1 == 8)
        {
            DrvDisable( drv, 0 );
			PRINT("DSI driver disabled");
		}
        else if(arg1 == 9)
        {
			if(arg2!=0)	 
			{
				if(arg2==1)
				{		
				testRunningMode=Disp_Loopbuf;
				PRINTN("Verfication: Set DSI[%d] to colorbar test", CurrentDsiId);
				}
				else if(arg2==2)
				{
				testRunningMode=Disp_Pipeline;
				testPattern=WorstCase;
				PRINTN("Verfication: Set DSI[%d] to worstcase pattern test", CurrentDsiId);
				}
				else if(arg2==3)
				{
				testRunningMode=Disp_Pipeline;
				testPattern=BestCase;
				PRINTN("Verfication: Set DSI[%d] to bestcase pattern test", CurrentDsiId);
				}				
			}
		}		
        else if(arg1 == 21)
        {
            
			if(arg2!=0)	
			{
			if(lcdData.pixelFormat == DISP_PIXELFRT_RGB24)			
			repeat=arg2*3;
			else if(lcdData.pixelFormat == DISP_PIXELFRT_RGB16)			
			repeat=arg2*2;
			else if(lcdData.pixelFormat == DISP_PIXELFRT_RGB32)			
			repeat=arg2*4;
			else
			PRINT("unknown pixel format to repeat");		
					

            PRINTN("Verfication: seed repeat set to %d", repeat);
			}

		}
        else if(arg1 == 22)
        {
			if(arg2!=0)	 
			{
				if(arg2==1)
				DisplayTraffic=TRUE;
				if(arg2==2)
				DisplayTraffic=FALSE;						
			}
		}
        else if(arg1 == 23)
        {
			if(arg2!=0)	 
			{
				if(arg2==1)
				{		
				TestDevice=TPO;
				PRINT("Verfication: to test TPO");
				}
				else if(arg2==2)
				{
				TestDevice=eDISCO;
				PRINT("Verfication: to test eDISCO");
				}
				else if(arg2==3)
				{
				TestDevice=Proteus;
				PRINT("Verfication: to test Proteus");
				}
				else if(arg2==0xff)
				{
				TestDevice=National;
				PRINT("Verfication: to test National");
				}
				else if(arg2 < 0xff)
				{
				TestDevice=arg2;
				PRINTN("Verfication: to test Panel: 0x%02x", TestDevice);
				}
				else
				    PRINT("unkonwn DSI device");
			}
		}		
        else
        {

            PRINT("===== Display VerifApp Help =====");
			PRINT("x vf.dis.cfg 1 N : Specify test duration for DSI interface");			
			PRINT("x vf.dis.cfg 2 N : Specify DSI port: 1->port0;2->port1(currently not availabe)");
			PRINT("x vf.dis.cfg 3 N : Specify DSI link speed: 1->52M;2->104M;3->156M;4->208M;5->312M;6->400M(DDR);7->624M");	
			PRINT("x vf.dis.cfg 4 N : Specify DSI active lane number: 1->1_lane; 2->2_lanes;3->3_lanes");
			PRINT("x vf.dis.cfg 5 N : Specify DSI test pattern");
 			PRINT("x vf.dis.cfg 6 N : Set control option for DSI errors where N = 1(continue) or 2(stop)");
            PRINT("x vf.dis.cfg 7 N : N=1 enable heartbeat 2: disable heartbeat");	
            PRINT("x vf.dis.cfg 8 : stop traffic");	
            PRINT("x vf.dis.cfg 9 N : N=1 cmd fifo test colorbar, pipeline N=2 load worstcase pattern, N=3 load bestcase pattern");				
            PRINT("x vf.dis.cfg 21 N: N link check pattern seed pixel repeat number");		
            PRINT("x vf.dis.cfg 22 N: N=1 display traffic N=2 no traffic display");
            PRINT("x vf.dis.cfg 23 N: N=1 TPO N=2 eDISCO");
        }        
    }
    else if(strcmp(cmd, "para") == 0)
    {  
		  	
        if(arg1 == 1)
        {
            PRINT("Display VerifApp: Running LPDT test");
			LPDT_TEST=TRUE;
			if(LPDT_LinkCheck()==FALSE)
    		{
			WARNN("DSI port[%d] failed LPDT check",CurrentDsiId);
    		}
			else
			PRINTN("DSI port[%d] passed LPDT check",CurrentDsiId);

        }
        else if(arg1 == 2)
        {
 			PRINT("Display VerifApp: BTA(with Data) tranactions through link check, setup scope for timing measurement");
			PRINT("Display VerifApp: manual BTAs were inserted every 5 seconds in DSI BER test");
			if(LinkCheck()==FALSE)
    		{
			WARNN("DSI port[%d] failed BTA check",CurrentDsiId);
    		}
			else
			PRINTN("DSI port[%d] passed BTA check",CurrentDsiId);
        }
        else if(arg1 == 3)
        {
			PRINT("Display VerifApp: Running ULPS testing in HS mode");
			ULPS_TEST=TRUE;
			if(LinkCheck()==FALSE)
    		{
			WARNN("DSI port[%d] failed HS ULPS check",CurrentDsiId);
    		}
			else
			PRINTN("DSI port[%d] passed HS ULPS check",CurrentDsiId);

			PRINT("Display VerifApp: Running ULPS testing in LPDT mode");
			ULPS_TEST=TRUE;
			if(LPDT_LinkCheck()==FALSE)
    		{
			WARNN("DSI port[%d] failed LPDT ULPS check",CurrentDsiId);
    		}
			else
			PRINTN("DSI port[%d] passed LPDT ULPS check",CurrentDsiId);

			ULPS_TEST=FALSE;

		}		
        else        
        {
            PRINT("===== Display VerifApp Help =====");
            PRINT("x vf.dis.para 1 : LPDT test");
            PRINT("x vf.dis.para 2 : BTAs test");			
            PRINT("x vf.dis.para 3 : ULPS test");
        }
    }
    else if(strcmp(cmd, "func") == 0)
    {    
        if(arg1 == 1)
        {
            PRINT("display VerifApp: DSI stress test started"); 
			DSI_BER_Test();

        }
        else if(arg1 == 2)
        {
            PRINT("display VerifApp: LinkCheck with current settings");

			disMsg.Event   = (DWORD) PLATFORM_MSG_DIS_TEST_APP;
    		disMsg.SubMsg  = (DWORD) DSI_MSG_LC_START;
    		RE_RimPostMessage( PLATFORM_THREAD_ID_APP0, &disMsg );     

        }
        else        
        {
            PRINT("===== Display VerifApp Help =====");
            PRINT("x vf.dis.func 1 : run DSI stress test");
            PRINT("x vf.dis.func 2 : run DSI link check test");
        }
    }
    else if(strcmp(cmd, "stat") == 0)
    {    
        //Config * cfgPtr = DrvGetConfig(drv);
		if(arg1 == 1)
        {
            PRINT("Listing DSI panel receiver errors"); 
		    ShowRxErrors();	

        }
	
        else        
        {
            PRINT("===== Display VerifApp Help =====");
            PRINT("x vf.dis.stat 1 : Show DSI Registers");


            PRINT("place holder");	
        }
    }	
    else if(strcmp(cmd, "help") == 0)
    {
        PRINT("===== Display VerifApp Help =====");
        PRINT("x vf.dis.cfg: configuration command help ");
        PRINT("x vf.dis.para: parametric measurement command help");
        PRINT("x vf.dis.func: functional test command help");
        PRINT("x vf.dis.stat: status check command help");		
    }
}
