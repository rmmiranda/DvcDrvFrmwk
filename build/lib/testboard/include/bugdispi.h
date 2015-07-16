/******************************************************************************
 * Filename:      bugdispi.h
 *
 * Copyright 2002-2009, Research In Motion Ltd
 *
 * Author:        David Kennedy
 *
 * Created:       March 2002
 *
 * Description:   Bugdisp defines for Nessus.
 *****************************************************************************/
#ifndef __BUGDISPI_H__
#define __BUGDISPI_H__

#include "basetype.h"

/*----------------------------------------------------------------------------------*/
// Groups for determining colours.
#define BUGBUILD_ENUM "groupid"
#define GROUP_SYSTEM      0xDEE     // Grey
#define GROUP_OS_INTERNAL 0xFE4     // Yellow
#define GROUP_OS_API      0xF88     // Red
#define GROUP_HW_IO       0x88F     // Blue
#define GROUP_SERIAL      0xFFF     // White
#define GROUP_DEBUGGER    0xFAF     // Purple
#define GROUP_MUSIC       0xA7D     // Magenta
#define GROUP_AUDIOHW     0x06F     // Blue
#define GROUP_DSP         0x090     // Dark Green
#define GROUP_IPC         0x3CC     // Bluish greenish

#undef BUGBUILD_ENUM

/* A special Enum for mapping groups (or supergroups) to supergroups.
 * Occurs in any bugdisp header that defines groups.
 * For example, this Enum also occurs in common\include\BugDisp.h
 * For a complete list of supergroups, or to add another supergroup,
 *  please see common\include\BugDisp.h */
#define BUGBUILD_ENUM "groupmappings"
/* SUPERGROUP_OS */
#define GROUP_OS_INTERNAL_MAP   0x01
#define GROUP_OS_API_MAP        0x01
#define GROUP_SYSTEM_MAP        0x01
#define GROUP_SERIAL_MAP        0x01
#define GROUP_HW_IO_MAP         0x01

/* SUPERGROUP_MULTIMEDIA */
#define GROUP_MUSIC_MAP     0x03

#undef BUGBUILD_ENUM

// MESSAGE.Events are now made into bugbuild enums in the header files that
// define them; MESSAGE.Devices are now in inc_pub/message.h

/*****************************************************************/

/*  Module identifiers for Bugdisp

 IMPORTANT NOTE:
 common\include\BugDisp.h defines the ranges 0x030-0x1FF, 0x208-0x4FF, 0x600-0x9FF
 and 0xC80-0xD7F (inclusive) for our use.  We impose the following sub-ranges, which
 must be followed by the Nessus-based platforms:
    Range (inclusive)       Used by
    -----------------       -------
    0x030-0x0FF             Nessus
    0x100-0x1FF             Radio stack-specific files (may be broken up
                             further by the radio code)
    0x208-0x4FF             Radio stack-specific files (may be broken up
                             further by the radio code)
    0x600-0x9FF             Radio stack-specific files (may be broken up
                             further by the radio code)
    0xC80-0xD7F             Radio stack-specific files (may be broken up
                             further by the radio code)

 If we need additional file ID ranges, we should first email OS API Changes,
 and then change that file.
*/
#define BUGBUILD_ENUM "fileid"
// Debug Goes First
#define FILE_CATFAIL        0x01
#define FILE_CATFAIL_UI     0x02

#define FILE_PROC_UTC       0x30
#define FILE_RADIODEBUGTASK 0x31
#define FILE_INIT           0x32
#define FILE_DVFM           0x33
#define FILE_COMMS_GPIO     0x34
#define FILE_DEBUG          0x35
#define FILE_IPC            0x36
#define FILE_THREAD         0x37
#define FILE_CLOCKS_IPC     0x38
#define FILE_PROC_INTCTRL   0x39
#define FILE_COMMON_STUBS   0x3A
#define FILE_TAVOR_DDRCTRL  0x3B
#define FILE_FLAGS          0x3C
#define FILE_ENTRYPOINT     0x3D
#define FILE_CRITICAL       0x3E
#define FILE_ARGONLV_SPBA   0x3F

#define FILE_ARM7_HANDLER   0x40
#define FILE_BF_HANDLER     0x41
#define FILE_PROC_TIMER     0x42
#define FILE_OSINITTASK     0x43
#define FILE_ABC            0x44
#define FILE_RTC            0x45
#define FILE_AUDIODEBUGTASK 0x46
#define FILE_TAVOR_GB_TIMER 0x47
#define FILE_JVMTIMESLICE   0x48
#define FILE_KEYPAD         0x49
#define FILE_LDO            0x4A
#define FILE_TAVORPV2_COMM_TIMERS 0x4B
#define FILE_KERNELHEAP     0x4C
#define FILE_ALERT          0x4D
#define FILE_SEMAPHORE      0x4E
#define FILE_WATCHDOGTASK   0x4F

#define FILE_IOTASK         0x50
#define FILE_MUTEX          0x51
#define FILE_POWER          0x52
#define FILE_HWME           0x53
#define FILE_PROC_CLOCKS    0x54
#define FILE_SRV_THREAD     0x55
#define FILE_DATAQUEUE      0x56
#define FILE_UNIT_TEST      0x57
#define FILE_UTC            0x58
#define FILE_CALTASK        0x59
#define FILE_APPTASK        0x5A
#define FILE_SMARTCARD      0x5B
#define FILE_PROC_SERIAL_TEST 0x5C
#define FILE_PROC_SERIAL    0x5D
#define FILE_PROFILE        0x5E
#define FILE_PROC_CBSERIAL  0x5F
#define FILE_DSP_FREQCHANGE 0x60
#define FILE_RADIO_FREQCHANGE 0x61
#define FILE_SLEEPCYCLE     0x62
#define FILE_FREQCHANGE     0x63
#define FILE_BUGDISPXCMDS   0x64
#define FILE_FREQCHANGE_COMMON 0x65
#define FILE_NOBATT_ADC     0x66
#define FILE_LED            0x67
#define FILE_HNB0_INTERPROCESSOR 0x68
#define FILE_MAX8856        0x69
#define FILE_IDLETASK       0x6A
#define FILE_PROC_SLOWCLOCK_ARM 0x6B
#define FILE_PROC_SLOWCLOCK 0x6C
#define FILE_BQTINY_SWITCHER 0x6D
#define FILE_BQTINY         0x6E
#define FILE_APP            0x6F
// overflow                 0x70

#define FILE_PROC_SYSPLL    0x71
#define FILE_DEVICEINFO     0x72
#define FILE_AUDIO_AUX      0x73
#define FILE_AUDIOTASK      0x74
#define FILE_TEST_COMMS_PERIPH 0x75
#define FILE_PROC_MISC      0x76
#define FILE_GSPSERIAL      0x77
#define FILE_VOCODERTHREAD  0x78
#define FILE_PROC_I2C       0x79
#define FILE_MIXERDSP       0x7A
#define FILE_HNB0_IRAM      0x7B
#define FILE_PROC_PLL       0x7C
#define FILE_LIGHTSENSOR_OS 0x7D
#define FILE_PM7540         0x7E
#define FILE_ARM9_MISC      0x7F

#define FILE_COREMISC_ARM7  0x80
#define FILE_COREMISC_BF    0x81
#define FILE_HUGE_INT       0x82
#define FILE_HUGE_PMIC      0x83
#define FILE_A2DPTHREAD     0x84
#define FILE_MIGHTY_PMIC    0x85
#define FILE_PMIC_TASK      0x86
#define FILE_BLUETOOTH      0x87
#define FILE_BLUETOOTHTHREAD 0x88
#define FILE_PROC_BTSERIAL  0x89
#define FILE_CLOCKS         0x8A
#define FILE_ADCSCHEDTASK   0x8B
#define FILE_PMUTIL         0x8C
#define FILE_TAVOR_PWRCNSMP 0x8D
#define FILE_PMIC           0x8E
#define FILE_TPS62356       0x8F

#define FILE_PERFMON        0x90
#define FILE_PROC_DEBUG     0x91
#define FILE_HNB0_PERFMON   0x92
#define FILE_PROC_KEYPAD    0x93
#define FILE_HARBELL_PERFMON 0x94
#define FILE_DIRECTKEY      0x95
#define FILE_DEADLOCK       0x96
#define FILE_ARGONLV_PERFMON 0x97
#define FILE_HNB0_SSP       0x98
#define FILE_LARGEMSG       0x99
#define FILE_HUGE_CHRG      0x9A
#define FILE_MIGHTY_CHRG    0x9B
#define FILE_TLS            0x9C
#define FILE_SRV_MESSAGE    0x9D
#define FILE_MIGHTY_INT     0x9E
#define FILE_NOBATT_INT     0x9F

#define FILE_PROC_RTC       0xA0
#define FILE_TAVOR_MSL      0xA1
#define FILE_PMJET_PMIC     0xA2
#define FILE_PMJET_CHRG     0xA3
#define FILE_BFMEMINIT      0xA4
#define FILE_SLOWCLOCKING_MSA 0xA5
#define FILE_IPCPROFILE     0xA6
#define FILE_CATFAIL_IPC    0xA7
// unused                   0xA8
// unused                   0xA9
#define FILE_GRAPHICS_TASK  0xAA
#define FILE_SCHEDULER_TASK 0xAB
// unused                   0xAC
#define FILE_COREMISC_HIFI2 0xAD
#define FILE_WM1830_HIFI2_INTCTRL 0xAE
#define FILE_CAMERA_TASK    0xAF

#define FILE_TRACEBUF_BF    0xB0
// unused                   0xB1
#define FILE_DSPDEBUG       0xB2
// unused                   0xB3
#define FILE_HNB0_MSA_INTCTRL 0xB4
// unused                   0xB5
#define FILE_SAC_CTRL       0xB6
// unused                   0xB7
#define FILE_SAC_DSP        0xB8
#define FILE_AUDIO_OS       0xB9
#define FILE_SAC            0xBA
// overflow                 0xBB
// unused                   0xBC
// unused                   0xBD
#define FILE_VOICETEST      0xBE
// unused                   0xBF

// unused                   0xC0
// unused                   0xC1
// unused                   0xC2
#define FILE_LOWBATTERY     0xC3
#define FILE_PMINIT         0xC4
#define FILE_CRPTOBAT_OS    0xC5
#define FILE_IPC_SIG        0xC6
#define FILE_IPC_DATA       0xC7
#define FILE_IPC_COMMON     0xC8
#define FILE_IPC_TEST       0xC9
#define FILE_IPC_COPY       0xCA
// unused                   0xCB
#define FILE_CC_SAC         0xCC
// unused                   0xCD
#define FILE_DSPAUDIOTHREAD 0xCE
#define FILE_DTMF_TTY_DSP   0xCF

#define FILE_NOBATT_MIGHTY  0xD0
#define FILE_NOBATT_HUGE    0xD1
#define FILE_TAVOR_TEMPSENSE 0xD2
// unused                   0xD3
#define FILE_MIXER_CTRL     0xD4
// unused                   0xD5
#define FILE_GENERIC_DEBUG  0xD6
#define FILE_ENG_AVP        0xD7
#define FILE_KERNEL_PROF    0xD8
#define FILE_KERNEL_PROF2   0xD9
#define FILE_VOCODER_I      0xDA
#define FILE_VOCODER        0xDB
#define FILE_VIDEOTASK      0xDC
// unused                   0xDD
#define FILE_RENDERTASK     0xDE
// unused                   0xDF
#define FILE_EVENTFLAG_SERVICES 0XE0
#define FILE_EVENTFLAGS_NAME_SERVER_THREAD 0xE1
// unused                   0xE2
// unused                   0xE3
// unused                   0xE4
#define FILE_VRTHREAD       0xE5
#define FILE_VADTHREAD      0xE6
#define FILE_PROC_GPSSERIAL 0xE7
#define FILE_STARCORE_PERFMON 0xE8
#define FILE_BF_PERFMON     0xE9
#define FILE_MIXERCTRLTASK  0xEA
#define FILE_USBTASK        0xEB
#define FILE_HIFI2_HANDLER  0xEC
#define FILE_ARGONLV_MU     0xED
#define FILE_STARCORE_HANDLER 0xEF

#define FILE_ARGONLV_STARCORE_INTCTRL 0xF0
// unused                   0xF1
#define FILE_STARCORE_MISC  0xF2
// unused                   0xF3
#define FILE_ARGONLV_EPIT   0xF4
// unused                   0xF5
#define FILE_HNB0_BTSSP_DATA 0xF6
// unused                   0xF7
#define FILE_HNB0_BTSSP_CTRL 0xF8
// unused                   0xF9
// unused                   0xFA
// unused                   0xFB
// unused                   0xFC
// unused                   0xFD
#define FILE_DSP_SBC_PROC   0xFE
#define FILE_STARCORE_TRACEBUF 0xFF



#undef BUGBUILD_ENUM // "fileid"

#define BUGBUILD_ENUM "Dc_Id_Type"
    #define M_FLASH_ID            0
    #define M_PROCESSOR_ID        1
    #define M_BSN                 2
    #define M_IMEI                3
    #define M_BBPIN               4
    #define M_OS_VERSION          5
    #define M_HW_PLATFORM_ID      6
    #define M_HW_FREQ_MAP         7
    #define M_SIM_ICCID           8
    #define M_SIM_IMSI            9
    #define M_BSN_BARYON_RADIO    10
    #define M_MSN_BARYON          11
    #define M_PATRIOT_BINARY_VER  12
    #define M_SIM_ID_BARYON       13
    #define M_HW_BOARD_REV        14
    #define M_IMEI_BARYON_GW      15
    #define M_HW_STRING           16
    #define M_BOOTROM_ID          17
    #define M_BLUETOOTH_MAC       18
    #define M_WLAN_MAC            19
    #define M_RADIOCODE_LINKED_ID 20
    #define M_OS_METRICS_VERSION  21
#undef BUGBUILD_ENUM

//------------------------------------ STP debug end --------------------------

typedef struct DbgTimestamp {
    DWORD   DBG_TICK;
    WORD    DBG_LATCH;
} DbgTimeStamp;
#include "BugDisp.h"
#endif
