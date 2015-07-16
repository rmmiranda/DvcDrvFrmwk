/*****************************************************************************
 * Filename:    verifMain.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      
 *
 * Created:     
 *
 * Description: 
 *
 *****************************************************************************/
/* include header files */

/* standard library header files */

/* Other header files */
#include "verifApp.h"

void AppVerificationInit(void)
{
    /* Camera test code */
    #if defined(DRV_CAMERA)
    camTestInit();
    #endif
    
    /* Display test code */
    #if defined(DRV_DISPLAY)
    disTestInit();
    #endif
}

void PlatformApp0EntryPoint(void)
{
    RE_MESSAGE msg;
    
    /* Need to wait until the driver framework initialization is done */
    while(!DrvFrameworkInitialized())
    {
        RimSleep(100);
    }

    /* Initialize the verification app */
    AppVerificationInit();
       
    while( 1 ) {
        /* Handle incoming messages */
        RE_RimGetMessage( &msg );
        
        PfMsgHandler(PLATFORM_THREAD_ID_APP0, &msg);
    }    
    
}
