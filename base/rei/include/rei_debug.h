/*****************************************************************************
 * Filename:    rei_debug.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Jan 07, 2009
 *
 * Description: This file defines the REI(runtime environment interface) for 
 *              critical section and semaphores. The original data structures 
 *              and functions are defined in file armos/common/include/critical.h. 
 *              Please refer to it for the usage of message queue.
 *              The REI provides an abstraction layer between the platform 
 *              software and nessus/bootrom.
 *
 ****************************************************************************/
#ifndef __REI_DEBUG_H__
#define __REI_DEBUG_H__

#include "rei_basetype.h"

#undef RIM_FLEDGE
#undef RIM_CFP_RAMIMAGE
#undef RIM_LOG_WITH_DEDICATED_BUFFER
#undef OS_SUPPORTS_JVM
#undef NON_BUGDISP_LCD_LOGGING_TPLGSM
#undef NON_BUGDISP_LCD_LOGGING_NESSUS
#undef NON_BUGDISP_LCD_LOGGING_ELTRON

#include "BugDisp.h" 

/* Define the Bugdisp groups here */
#define BUGBUILD_ENUM "groupid"
#define GROUP_PLT_FRMWRK        0x0E00
#define GROUP_PLT_DRV           0x0070
#define GROUP_PLT_APP           0x009E
#undef BUGBUILD_ENUM

/* Define the Bugdisp file IDs here */
#define BUGBUILD_ENUM "fileid"
#define FILE_ID_PLATFORM       0xF00  

#define FILE_MGR_MAIN          0xF01
#define FILE_ID_FRAMEWORK      0xF02
#define FILE_FWK_BUGDISP       0xF03
#define FILE_REI_GPIO          0xF04
#define FILE_REI_CLK           0xF05

#define FILE_CAM_IF            0xF10
#define FILE_CAM_FWK           0xF11
#define FILE_CAM_DEV           0xF12
#define FILE_CAM_I2C           0xF13
#define FILE_CAM_ISP           0xF14
#define FILE_CAM_TST           0xF15
#define FILE_CAM_TST_ISP       0xF16

#define FILE_UART_FWK          0xF20
#define FILE_SERIAL_FWK        0xF21

#define FILE_I2C     		   0xF2A
#define FILE_I2C_FWK 		   0xF2B

#define FILE_DISP_IF           0xF30
#define FILE_DISP_FWK          0xF31
#define FILE_DISP_DEV          0xF32
#define FILE_DISP_HAL	       0xF33
#define FILE_DISP_CMD          0xF34
#define FILE_DISP_IF_STUB      0xF35
#define FILE_DISP_HDMI	       0xF36
#define FILE_DISP_HDMI_HW      0xF37

#define FILE_TST_CAM	       0xF40
#define FILE_TST_DIS		   0xF41
#define FILE_TST_GPIO          0xF42

#undef BUGBUILD_ENUM

#if defined( DRV_SERIAL ) && defined( DRV_UART )

#undef PRINT
#undef PRINTN
#undef PRINT2N
#undef PRINT3N
#undef PRINT4N
#undef PRINTCOPY

#define PRINT(a)                {DrvPrint((DBG_FLG)0,DBG_LVL_ALL,(a));DrvPrint((DBG_FLG)0,DBG_LVL_ALL,"\n");}
#define PRINTN(a,b)             {DrvPrint((DBG_FLG)0,DBG_LVL_ALL,(a),(b));DrvPrint((DBG_FLG)0,DBG_LVL_ALL,"\n");}
#define PRINT2N(a,b,c)          {DrvPrint((DBG_FLG)0,DBG_LVL_ALL,(a),(b),(c));DrvPrint((DBG_FLG)0,DBG_LVL_ALL,"\n");}
#define PRINT3N(a,b,c,d)        {DrvPrint((DBG_FLG)0,DBG_LVL_ALL,(a),(b),(c),(d));DrvPrint((DBG_FLG)0,DBG_LVL_ALL,"\n");}
#define PRINT4N(a,b,c,d,e)      {DrvPrint((DBG_FLG)0,DBG_LVL_ALL,(a),(b),(c),(d),(e));DrvPrint((DBG_FLG)0,DBG_LVL_ALL,"\n");}
#define PRINTCOPY(a,b)          {DrvPrint((DBG_FLG)0,DBG_LVL_ALL,(a),(b));DrvPrint((DBG_FLG)0,DBG_LVL_ALL,"\n");}

#undef WARN
#undef WARNN
#undef WARN2N
#undef WARN3N
#undef WARN4N

#define WARN(a)                {DrvPrint((DBG_FLG)0,DBG_LVL_ALL,(a));DrvPrint((DBG_FLG)0,DBG_LVL_ALL,"\n");}
#define WARNN(a,b)             {DrvPrint((DBG_FLG)0,DBG_LVL_ALL,(a),(b));DrvPrint((DBG_FLG)0,DBG_LVL_ALL,"\n");}
#define WARN2N(a,b,c)          {DrvPrint((DBG_FLG)0,DBG_LVL_ALL,(a),(b),(c));DrvPrint((DBG_FLG)0,DBG_LVL_ALL,"\n");}
#define WARN3N(a,b,c,d)        {DrvPrint((DBG_FLG)0,DBG_LVL_ALL,(a),(b),(c),(d));DrvPrint((DBG_FLG)0,DBG_LVL_ALL,"\n");}
#define WARN4N(a,b,c,d,e)      {DrvPrint((DBG_FLG)0,DBG_LVL_ALL,(a),(b),(c),(d),(e));DrvPrint((DBG_FLG)0,DBG_LVL_ALL,"\n");}

#endif

#endif //__REI_DEBUG_H__
