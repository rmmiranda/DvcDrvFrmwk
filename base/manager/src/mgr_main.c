/*****************************************************************************
 * Filename:    mgr_main.c
 *
 * Copyright 2005, Research In Motion Ltd
 *
 * Author:      Platform SW Group
 *
 * Created:     January 2005
 *
 * Description: Platform SW Manager related functions
 *****************************************************************************/

/* standard library header files */
#include <string.h>

/* application header files */
#include  "rei.h"
#include  "framework.h"
#include  "driver.h"

#define SRCGROUP GROUP_PLT_FRMWRK
#define SRCFILE  FILE_MGR_MAIN

#ifdef BOOTROM
/**
 * This is the Platform Software entry point in Bootrom.
 *
 * @param void.
 *
 * @return Zero if the call is successful. Otherwise, returns a non zero value.
 *
 */
char tmpbuf[64];

int platformMain()
{
    Driver_t *serialDrv;
    int i;

    /* initialize the drive framework */
    DrvFrameworkInit();
    
    /* Initialize the verification app */
    PRINT("call AppVerificationInit");
    AppVerificationInit();

    /* get a handle to the serial driver */
    serialDrv = DrvGetHandle( IODRV_ID_SERIAL );

    if( serialDrv == NULL ) {
        /* if we got here, either the
         * the framework initialization
         * failed or there is no serial
         * driver found.
         */
        RE_LED_Off(); 
        RE_HwDelay(RE_MILLISECOND*300);
        RE_KickWatchdog();
        return 1;
    }

    if( DrvEnable( serialDrv, 0 ) < 0 ) {
        RE_LED_Off(); 
        RE_HwDelay(RE_MILLISECOND*300);
        RE_KickWatchdog();
        return 1;
    }

    while(1)
    //for(i = 0; i < 6; i++)
    {
        /* Sample code */
        RE_LED_Off(); 
        RE_HwDelay(RE_MILLISECOND*300);
        RE_KickWatchdog();

        /* poll for serial events */        
        DrvPollEvents();

        if( DrvControl(serialDrv,
            DRIVER_PORT_READ,
            &tmpbuf,
            1) ) {

            DrvControl(serialDrv,
                DRIVER_PORT_WRITE,
                &tmpbuf,
                1);
        }

        RE_LED_On(); 
        RE_HwDelay(RE_MILLISECOND*300);
        RE_KickWatchdog();        
    }

    if( DrvDisable( serialDrv, 0 ) < 0 ) {
        RE_LED_Off(); 
        RE_HwDelay(RE_MILLISECOND*300);
        RE_KickWatchdog();
        return 1;
    }

   return 0; 
}
#endif


/**
 * This is the Platform Software entry point in Nessus.
 *
 * @param void.
 *
 * @return void.
 *
 */
void PlatformThreadEntryPoint(void)
{
    RE_MESSAGE msg;
    
    /* Add delay here to allow Nessus completes its initialization */
    RimSleep(5000);
    
    PRINT("Platform Sw thread started.");

    ReiInit();
    
    /* Initialize driver framework */ 
    DrvFrameworkInit();

    while( 1 ) {        
        /* Handle incoming messages */
        RE_RimGetMessage( &msg );
        
        PfMsgHandler(PLATFORM_THREAD_ID_DRV, &msg);
    }
}


