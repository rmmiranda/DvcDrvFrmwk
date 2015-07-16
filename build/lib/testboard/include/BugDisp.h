/*****************************************************************************
 * * Filename:    BugDisp.h
 * *
 * * Copyright 2000, Research In Motion Ltd
 * *
 * * Author:      Sherif Abdel-Kader
 * *
 * * Created:     Oct 2000
 * *
 * * Description: This file contains macros and constants for ASSERTS, PRINTS,
 * *              WARNS, etc.
 * *              as well as prototypes for the Print, PrintN..etc. functions
 * *
 * *****************************************************************************/
#ifndef __BUGDISP_H__
#define __BUGDISP_H__

/* Don't #include anything here.  This file is used by many different build
 * processes, and some of them don't seem to have the "standard" header
 * directories (such as common\inc_pub) on their include paths.  An unfortunate
 * consequence of this is that RimCatastrophicFailure must be declared here,
 * as well as in common\inc_pub\device.h.
 */

#ifdef __cplusplus

    #ifdef RIM_FLEDGE
        #include <string>
        #include <srcfile.h>
    #endif

    #if !defined( RIM_BOOTROM ) && !(defined( RIM_CFP_RAMIMAGE ) && !defined( RAMIMAGE_DEBUG ))
        extern "C" void RimCatastrophicFailure(const char *);
        extern "C"  int IsHeadInSand(void); // for ASSERTCDEV()
    #endif

#else // ifdef __cplusplus

    #if !defined( RIM_BOOTROM ) && !(defined( RIM_CFP_RAMIMAGE ) && !defined( RAMIMAGE_DEBUG ))
        extern void RimCatastrophicFailure(const char *);
        extern  int IsHeadInSand(void); // for ASSERTCDEV()
    #endif

#endif

#if !defined( NO_DEBUG ) && !defined( RIM_FLEDGE )
    #define DEBUG 1
#endif

/*  Module identifiers for Bugdisp

 IMPORTANT NOTE:
 Bugdisp gives us 12 bits for file IDs.  Here's how they're allocated:
    Range (inclusive)       Used by
    -----------------       -------
    0x000                   Reserved by Bugdisp
    0x001-0x002             Debugger (usually FILE_CATFAIL and FILE_CATFAIL_UI)
    0x003-0x003             Used by whoever generates the thunk file (ie FILE_APIFAIL)
    0x004-0x02F             Common code
    0x030-0x1FF             Radio stack-specific files (may be broken up
                             further by the OS/radio code)
    0x200-0x207             Reserved by Bugdisp
    0x208-0x4FF             Radio stack-specific files (may be broken up
                             further by the radio code)
    0x500-0x5FF             Common code
    0x600-0x6FF             Radio stack-specific files (may be broken up
                             further by the radio code)
    0x700-0x7FF             Radio stack-specific files (may be broken up
                             further by the radio code)
    0x800-0x9FF             Radio stack-specific files for 12 bit file IDs
                            (may be broken up further by the radio code)
    0xA00-0xAFF             Common code
    0xB00-0xB7F             Reserved for Bluetooth
    0xB80-0xC7F             Files in the MWAF build under common/radio.
    0xC80-0xDFF             Radio stack-specific files (may be broken up
                             further by the radio code)
    0xE00-0xFFE             Common code
    0xFFF                   RESERVED BY BUGDISP

EXTENDED (14-bit) BUGDISP FILE IDS

    0x1000-0x17FF           Common code
    0x1800-0x1FFF           Radio stack-specific files.
    0x2000-0x3FFF           Not yet allocated
*/

#define BUGBUILD_ENUM "fileid"
// This entry is here because it is used by the radiocode when they generate thunks.
// The rest of the common entries now defined in bugdispc.h and are not released.
#define FILE_APIFAIL        0x03
#undef BUGBUILD_ENUM

#if defined( BUILD_STEP_NESSUS ) || defined( QUALCOMM_OS ) || defined( RIM_FLEDGE ) || defined( RIM_CFP_RAMIMAGE )
    #include <bugdispc.h>
#endif

#define BUGBUILD_ENUM "groupid"
#define GROUP_APP_INTERFACE 0x0F65    // Dark red-ish grey
#define GROUP_STREAMING     0x01AB    // Bluish
#define GROUP_FLASH         0x0584    // Greenish (like my eyes)
#define GROUP_USB           0x080F    // Purple (like Rob's teeth)
#define GROUP_POWER         0x0FA2    // Brown (like Max's... nm)
#define GROUP_LCDBKLGT      0x02FE    // Cyan
#define GROUP_VIDEO         0x009A    // Dark-cyan
#define GROUP_OMX           0x009F    // Dark-cyan
#define GROUP_MEDIA         0x0A62    // dark brown
#define GROUP_MFG           0x0C28    // dark red
#define GROUP_VAD           0x007D    // Light Blue
#define GROUP_GPS           0x00FB    // Bright Green
#define GROUP_CAMERA        0x04FC    // greenish cyan
#define GROUP_BLUETOOTH     0x0EE0    // Yellow
#define GROUP_TASKS         0x09EF    // Some colour, likely teale
#define GROUP_FPS           0x0ABF    // Purplish
#define GROUP_SMARTCARD     0x0F77    // Red
#define GROUP_HEAP          0x039F    // Bluish
#define GROUP_GRAPHICS      0x08F1    // Neon green
#define GROUP_QUINCY        0x0A2F    // Purplesque
#define GROUP_INPUT         0x0F74    // yellowish
//#define GROUP_OUTPUT        0x02FE    // Cyan //Sujoy:- GROUP_OUTPUT is creating some warning in generating
                                                //        debug.dat file. So, just to get rid of that warning
                                                //        I have commented out this.
#undef BUGBUILD_ENUM /*  "groupid" */

/* Ids for the supergroups. Only one occurance of this Enum. */
#define BUGBUILD_ENUM "supergroupid"
#define SUPERGROUP_OS           0x01
#define SUPERGROUP_WLAN         0x02
#define SUPERGROUP_MULTIMEDIA   0x03
#define SUPERGROUP_WLANCHIPSET  0x04
#define SUPERGROUP_MWAF         0x05
#define SUPERGROUP_GPRS         0x06
#define SUPERGROUP_UMTS         0x07
#undef BUGBUILD_ENUM

/* A special Enum for mapping groups (or supergroups) to supergroups.
 * Occurs in any bugdisp header that defines groups.
 * For example, this Enum also occurs in nessus\include\bugdispi.h */
#define BUGBUILD_ENUM "groupmappings"
/* SUPERGROUP_OS */
#define GROUP_GPS_MAP               0x01
#define GROUP_BLUETOOTH_MAP         0x01
#define GROUP_APP_INTERFACE_MAP     0x01
#define GROUP_DEBUGGER_MAP          0x01
#define GROUP_MFG_MAP               0x01
#define GROUP_USB_MAP               0x01
#define GROUP_FLASH_MAP             0x01
#define GROUP_LCDBKLGT_MAP          0x01
#define GROUP_POWER_MAP             0x01
#define GROUP_FPS_MAP               0x01
#define GROUP_HEAP_MAP              0x01
#define GROUP_SMARTCARD_MAP         0x01
#define SUPERGROUP_MULTIMEDIA_MAP   0x01
#define GROUP_QUINCY_MAP            0x01
#define GROUP_INPUT_MAP             0x01

/* SUPERGROUP_MULTIMEDIA */
#define GROUP_VIDEO_MAP     0x03
#define GROUP_MEDIA_MAP     0x03
#define GROUP_CAMERA_MAP    0x03
#define GROUP_VAD_MAP       0x03
#define GROUP_STREAMING_MAP 0x03

/* SUPERGROUP_WLAN */
#define GROUP_TASKS_MAP     0x02
#undef BUGBUILD_ENUM

#define DBG_LINE_SHIFT 8   // 12 bit file ID, 12 bit line number
#define DBG_SRC_SHIFT  20
#define LINE_OVERFLOWS_TO_MODULE_BITS   1

#define BUGDISP_VERSION_MAJOR   4
#define BUGDISP_VERSION_MINOR   0
#if defined( BUGDISP_EXTENDED_FILE_ID )
    #define BD_SRC_SHIFT  18  // 14 bit file ID, 18 bit line number
#endif

#if defined( BUGDISP_EXTENDED_FILE_ID )
    #define BD_FILE_LINE    unsigned int File_Line
    #define BD_FLL(f,l,x)  (((unsigned int)((f) << BD_SRC_SHIFT)) + \
                            ((unsigned int)(l)))
    #define BD_NAME(fn)     fn##Ext
#else
    #define BD_FILE_LINE    unsigned int File_Line_Len
    #define BD_NAME(fn)     fn
    #ifdef LINE_OVERFLOWS_TO_MODULE_BITS
        #ifndef LINEOVERFLOW
            // To support humongous SDL-generated source files
            // on compilers which limit __LINE__ to 16 bits.
            #define LINEOVERFLOW 0
        #endif
        #define BD_FLL(f,l,x)   ((x) | ((unsigned int)(f) << DBG_SRC_SHIFT) + \
                                (((unsigned int)(l) | LINEOVERFLOW) << DBG_LINE_SHIFT))
        #define DBG_LINE    (((unsigned int)(__LINE__) | LINEOVERFLOW) << DBG_LINE_SHIFT)
    #else
        #define BD_FLL(f,l,x)   ((x) | ((unsigned int)(f) << DBG_SRC_SHIFT) + \
                                ((unsigned int)((l) & 0x1fff) << DBG_LINE_SHIFT))
        #define DBG_LINE    ((unsigned int)(__LINE__ & 0x1fff) << DBG_LINE_SHIFT)
    #endif
    #define DBG_SRC     ((unsigned int)SRCFILE << DBG_SRC_SHIFT)
#endif

#define BUGBUILD_ENUM "fileid"
#define FILE_BLOCKS       0x200  // Special 'file' for bugdisp to handle when displaying PDUs
#define MODULE_SIGNAL_RIM 0x202
#define MODULE_BIG_SHOW   0x203
#define MODULE_EXT        0xFFF
#undef BUGBUILD_ENUM

/*****************************************************************************
 * Big Memory Show item
 * --------------------
 *
 * +------------+----+--------+--------+------------+------------+--------+
 * | 0x203      |Ext | SeqNo  | Length | Module     | LineNo     | Blocks |
 * +------------+----+--------+--------+------------+------------+--------+
 *
 * EXT (4 bits) - Allow interleaving of up to 16 different structures.
 * SEQ (8 bits) - Block sequence number.
 * LEN (8 bits) - Item length.
 *
 * The module identifier and line number from where BIG_SHOW_MEMORY was
 * actually called has been pushed into the item data payload.  Additionally,
 * a field was added to indicate how many blocks this multi-block item spans.
 *
 *****************************************************************************/
#define DBG_EXT_SHIFT       (16)
#define DBG_SEQ_SHIFT       (8)
#define DBG_LEN_SHIFT       (0)

#define DBG_EXT_MASK        (0xF << DBG_EXT_SHIFT)
#define DBG_SEQ_MASK        (0xFF << DBG_SEQ_SHIFT)
#define DBG_LEN_MASK        (0xFF << DBG_LEN_SHIFT)

#define DBG_BS_LLF(ext, seq, len)                       \
    ((DWORD) ((MODULE_BIG_SHOW << DBG_SRC_SHIFT)        \
        | (((ext) << DBG_EXT_SHIFT) & DBG_EXT_MASK)     \
        | (((seq) << DBG_SEQ_SHIFT) & DBG_SEQ_MASK)     \
        | (((len) << DBG_LEN_SHIFT) & DBG_LEN_MASK)))

#ifdef RIM_FLEDGE
    #define MAX_BUGDISP_ITEM_SIZE 84
#else
    #define MAX_BUGDISP_ITEM_SIZE 42
#endif

typedef unsigned long BDPARM;

/*  Reserved file mappings for debugging. */
/*  NOTE: If the following #defines are changed, corresponding updates */
/*  to the list built into the bugdisp program are required */
#define FILE_RESERVED   0x00
#define DBG_RESERVED(x)     ((FILE_RESERVED<<24) | ((x)&0x1FFF)<<DBG_LINE_SHIFT)

#define DBG_STREAM_PADDING              0x0000  /*  overrides __LINE__ entry */
#define DBG_DISPLAY_MEMORY_BYTEWISE     0x0001
#define DBG_UNASSEMBLE                  0x0002
#define DBG_DISPLAY_LCD                 0x0003
#define DBG_DOWNLOAD                    0x0004
#define DBG_APP_INFO                    0x0005
#define DBG_TIMESTAMP                   0x0006
#define DBG_NEED_PASSWORD               0x0010
#define DBG_BAD_PASSWORD                0x0011
#define DBG_START_SAVED                 0x0018
#define DBG_END_SAVED                   0x0019
#define DBG_BUILD_INFO                  0x0020
/* 21, 22, 23 used for internal wbugdisp commands */
#define DBG_SEND_ME_FILTER              0x0024
#define DBG_MARKER                      0x0025
#define DBG_FILTER                      0x0026
#define DBG_DEVICE_TIMESTAMP            0x0027
#define DBG_BACKLOG_INFO                0x0028
#define DBG_AUTH_CHALLENGE              0x0029
#define DBG_DISPLAY_MEMORY_WORDWISE     0x002B
#define DBG_DISPLAY_MEMORY_DWORDWISE    0x002C
#define DBG_DEVICE_CORE                 0x002D
#define DBG_BUILD_INFO2                 0x002E
#define DBG_BUFFER_INFO                 0x002F
/* 30 used for internal wbugdisp command */
#define DBG_DEVICE_TASK                 0x0031
#define DBG_ENCRYPTED_SYNC              0x0032
#define DBG_LONG_STRING                 0x0033
#if defined( PROCESSOR_NEVO ) && defined( UART_DEBUG )
extern void putchr(char c);
extern void putstr(const char* s);
#endif
#if ( defined( RIM_FLEDGE ) || defined( DEBUG ) ) \
    && !defined( RIM_FLEDGE_OSX ) \
    && !defined( RIM_BOOTROM ) \
    && ( !defined( RIM_CFP_RAMIMAGE ) || defined( RAMIMAGE_DEBUG ) ) \
    && !defined( RIM_ICE_LOADER ) \
    && !defined( RIM_CFP_TOOLS )

    #ifdef __cplusplus
        extern "C" {
    #endif

    void ClearMessageMask( unsigned int cls );
    void SetMessageMask( unsigned int cls );

    void BD_NAME(PrintFun) ( BD_FILE_LINE );
    void BD_NAME(PrintNFun)( BD_FILE_LINE, BDPARM n1 );
    void BD_NAME(Print2NFun)( BD_FILE_LINE, BDPARM n1, BDPARM n2 );
    void BD_NAME(Print3NFun)( BD_FILE_LINE, BDPARM n1, BDPARM n2, BDPARM n3 );
    void BD_NAME(Print4NFun)( BD_FILE_LINE, BDPARM n1, BDPARM n2, BDPARM n3, BDPARM n4 );
    void BD_NAME(Print5NFun)( BD_FILE_LINE, BDPARM n1, BDPARM n2, BDPARM n3, BDPARM n4, BDPARM n5 );
    void BD_NAME(PrintCopyFun)( BD_FILE_LINE, const char * String );
    #if defined( RIM_FLEDGE ) && defined( __cplusplus )
        } // extern "C"
        extern "C++" {
            inline void PrintCopyFunCpp( unsigned int File_Line_Len, const char* str )
                { PrintCopyFun( File_Line_Len, str ); }
            inline void PrintCopyFunCpp( unsigned int File_Line_Len, const std::string& str )
                { PrintCopyFun( File_Line_Len, str.c_str( ) ); }
            #define PrintCopyFun PrintCopyFunCpp
        }
        extern "C" {
    #endif

    #if defined (RIM_LOG_WITH_DEDICATED_BUFFER)
        // one param, or no param setting param1 to 0
        void LogSpecialDebugLine( unsigned long fileLineLength, unsigned long param1 );
    #endif

    void BD_NAME(ShowMemory)( BD_FILE_LINE, void * p, unsigned n );
    void BD_NAME(ShowMemoryAddress)( BD_FILE_LINE, void * p, unsigned n );
    void BD_NAME(BigShowMemory)( BD_FILE_LINE, void * p, unsigned n );

    void ShowRimMessage(void *msg, int sender, int dest);

    void PossiblyEnterDebugMode(void);

    #ifdef __cplusplus
        }
    #endif


    #define PRINT(string)                   BD_NAME(PrintFun)  (BD_FLL(SRCFILE,__LINE__,4))
    #define PRINTN(string,n1)               BD_NAME(PrintNFun) (BD_FLL(SRCFILE,__LINE__,5), (unsigned long)(n1))
    #define PRINT2N(string,n1,n2)           BD_NAME(Print2NFun)(BD_FLL(SRCFILE,__LINE__,5), (unsigned long)(n1), (unsigned long)(n2))
    #define PRINT3N(string,n1,n2,n3)        BD_NAME(Print3NFun)(BD_FLL(SRCFILE,__LINE__,5), (unsigned long)(n1), (unsigned long)(n2), (unsigned long)(n3))
    #define PRINT4N(string,n1,n2,n3,n4)     BD_NAME(Print4NFun)(BD_FLL(SRCFILE,__LINE__,5), (unsigned long)(n1), (unsigned long)(n2), (unsigned long)(n3), (unsigned long)(n4))
    #define PRINT5N(string,n1,n2,n3,n4,n5)  BD_NAME(Print5NFun)(BD_FLL(SRCFILE,__LINE__,5), (unsigned long)(n1), (unsigned long)(n2), (unsigned long)(n3), (unsigned long)(n4), (unsigned long)(n5))
    #define PRINTCOPY(string, str)          BD_NAME(PrintCopyFun)(BD_FLL(SRCFILE,__LINE__,5), str)
    #if defined (RIM_LOG_WITH_DEDICATED_BUFFER)
        // 4, 5, 6, 8 bytes per entry
        #define PRINTNLOG(string,n1)            LogSpecialDebugLine (BD_FLL(SRCFILE,__LINE__,4), n1)
    #endif
    #define BUGTEXT(x)  BD_FLL(SRCFILE,__LINE__,4)

    #define PRINT_H(hs,string)                      BD_NAME(PrintFun)  (BD_FLL(SRCFILE,hs,4))
    #define PRINTN_H(hs,string,n1)                  BD_NAME(PrintNFun) (BD_FLL(SRCFILE,hs,5), (unsigned long)(n1))
    #define PRINT2N_H(hs,string,n1,n2)              BD_NAME(Print2NFun)(BD_FLL(SRCFILE,hs,5), (unsigned long)(n1), (unsigned long)(n2))
    #define PRINT3N_H(hs,string,n1,n2,n3)           BD_NAME(Print3NFun)(BD_FLL(SRCFILE,hs,5), (unsigned long)(n1), (unsigned long)(n2), (unsigned long)(n3))
    #define PRINT4N_H(hs,string,n1,n2,n3,n4)        BD_NAME(Print4NFun)(BD_FLL(SRCFILE,hs,5), (unsigned long)(n1), (unsigned long)(n2), (unsigned long)(n3), (unsigned long)(n4))
    #define PRINT5N_H(hs,string,n1,n2,n3,n4,n5)     BD_NAME(Print5NFun)(BD_FLL(SRCFILE,hs,5), (unsigned long)(n1), (unsigned long)(n2), (unsigned long)(n3), (unsigned long)(n4), (unsigned long)(n5))
    #define PRINTCOPY_H(hs,string, str)             BD_NAME(PrintCopyFun)(BD_FLL(SRCFILE,hs,4), str)
    #if defined (RIM_LOG_WITH_DEDICATED_BUFFER)
        // 4, 5, 6, 8 bytes per entry
        #define PRINTNLOG_H(hs,string,n1)               LogSpecialDebugLine (BD_FLL(SRCFILE,hs,5), n1)
    #endif

    #define WARN(string)                    BD_NAME(PrintFun)  (BD_FLL(SRCFILE,__LINE__,4))
    #define WARNN(string,n1)                BD_NAME(PrintNFun) (BD_FLL(SRCFILE,__LINE__,5), (unsigned long)(n1))
    #define WARN2N(string,n1,n2)            BD_NAME(Print2NFun)(BD_FLL(SRCFILE,__LINE__,5), (unsigned long)(n1), (unsigned long)(n2))
    #define WARN3N(string,n1,n2,n3)         BD_NAME(Print3NFun)(BD_FLL(SRCFILE,__LINE__,5), (unsigned long)(n1), (unsigned long)(n2), (unsigned long)(n3))
    #define WARN4N(string,n1,n2,n3,n4)      BD_NAME(Print4NFun)(BD_FLL(SRCFILE,__LINE__,5), (unsigned long)(n1), (unsigned long)(n2), (unsigned long)(n3), (unsigned long)(n4))
    #define WARN5N(string,n1,n2,n3,n4,n5)   BD_NAME(Print5NFun)(BD_FLL(SRCFILE,__LINE__,5), (unsigned long)(n1), (unsigned long)(n2), (unsigned long)(n3), (unsigned long)(n4), (unsigned long)(n5))
    #define WARNCOPY(string, str)           BD_NAME(PrintCopyFun)(BD_FLL(SRCFILE,__LINE__,5), str)

    #define WARN_H(hs,string)                       BD_NAME(PrintFun)  (BD_FLL(SRCFILE,hs,4))
    #define WARNN_H(hs,string,n1)                   BD_NAME(PrintNFun) (BD_FLL(SRCFILE,hs,5), (unsigned long)(n1))
    #define WARN2N_H(hs,string,n1,n2)               BD_NAME(Print2NFun)(BD_FLL(SRCFILE,hs,5), (unsigned long)(n1), (unsigned long)(n2))
    #define WARN3N_H(hs,string,n1,n2,n3)            BD_NAME(Print3NFun)(BD_FLL(SRCFILE,hs,5), (unsigned long)(n1), (unsigned long)(n2), (unsigned long)(n3))
    #define WARN4N_H(hs,string,n1,n2,n3,n4)         BD_NAME(Print4NFun)(BD_FLL(SRCFILE,hs,5), (unsigned long)(n1), (unsigned long)(n2), (unsigned long)(n3), (unsigned long)(n4))
    #define WARN5N_H(hs,string,n1,n2,n3,n4,n5)      BD_NAME(Print5NFun)(BD_FLL(SRCFILE,hs,5), (unsigned long)(n1), (unsigned long)(n2), (unsigned long)(n3), (unsigned long)(n4), (unsigned long)(n5))
    #define WARNCOPY_H(hs,string, str)              BD_NAME(PrintCopyFun)(BD_FLL(SRCFILE,hs,4), str)

    #if defined( BUGDISP_LONG_PERSISTENCE_BUFFERS )
#ifdef __cplusplus
extern  "C" {
#endif // __cplusplus
        void BD_NAME(LpPrintFun)( BD_FILE_LINE );
        void BD_NAME(LpPrintNFun)( BD_FILE_LINE, BDPARM n1 );
        void BD_NAME(LpPrint2NFun)( BD_FILE_LINE, BDPARM n1, BDPARM n2 );
        void BD_NAME(LpPrint3NFun)( BD_FILE_LINE, BDPARM n1, BDPARM n2, BDPARM n3 );
        void BD_NAME(LpPrint4NFun)( BD_FILE_LINE, BDPARM n1, BDPARM n2, BDPARM n3, BDPARM n4 );
        void BD_NAME(LpPrint5NFun)( BD_FILE_LINE, BDPARM n1, BDPARM n2, BDPARM n3, BDPARM n4, BDPARM n5 );
        void BD_NAME(LpPrintCopyFun)( BD_FILE_LINE, const char * String );
        void BD_NAME(LpPrintStreamFun)(BD_FILE_LINE, unsigned char *stream, unsigned int len );
        void BD_NAME(LpShowMemory)( BD_FILE_LINE, void * p, unsigned n );
        void BD_NAME(LpShowMemoryAddress)( BD_FILE_LINE, void * p, unsigned n );
        void BD_NAME(LpBigShowMemory)( BD_FILE_LINE, void * p, unsigned n );
#ifdef __cplusplus
}
#endif // __cplusplus
    #else

        #define LpPrintFun          PrintFun
        #define LpPrintNFun         PrintNFun
        #define LpPrint2NFun        Print2NFun
        #define LpPrint3NFun        Print3NFun
        #define LpPrint4NFun        Print4NFun
        #define LpPrint5NFun        Print5NFun
        #define LpPrintCopyFun      PrintCopyFun
        #define LpPrintStreamFun
        #define LpShowMemory        ShowMemory
        #define LpShowMemoryAddress ShowMemoryAddress
        #define LpBigShowMemory     BigShowMemory

        #define LpPrintFunExt       PrintFunExt
        #define LpPrintNFunExt      PrintNFunExt
        #define LpPrint2NFunExt     Print2NFunExt
        #define LpPrint3NFunExt     Print3NFunExt
        #define LpPrint4NFunExt     Print4NFunExt
        #define LpPrint5NFunExt     Print5NFunExt
        #define LpPrintCopyFunExt   PrintCopyFunExt
        #define LpPrintStreamFunExt
        #define LpShowMemoryExt        ShowMemory
        #define LpShowMemoryAddressExt ShowMemoryAddress
        #define LpBigShowMemoryExt     BigShowMemoryExt
    #endif

    #define LP_PRINT( string )                        BD_NAME(LpPrintFun)  (BD_FLL(SRCFILE,__LINE__,4))
    #define LP_PRINTN( format, n1 )                   BD_NAME(LpPrintNFun) (BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1))
    #define LP_PRINT2N( format, n1, n2 )              BD_NAME(LpPrint2NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (BDPARM)(n2))
    #define LP_PRINT3N( format, n1, n2, n3 )          BD_NAME(LpPrint3NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3))
    #define LP_PRINT4N( format, n1, n2, n3, n4 )      BD_NAME(LpPrint4NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3), (BDPARM)(n4))
    #define LP_PRINT5N( format, n1, n2, n3, n4, n5 )  BD_NAME(LpPrint5NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3), (BDPARM)(n4), (BDPARM)(n5))
    #define LP_PRINTCOPY( format, string )            BD_NAME(LpPrintCopyFun)(BD_FLL(SRCFILE,__LINE__,4), string)

    #define LP_PRINT_H( hs, string )                        BD_NAME(LpPrintFun)  (BD_FLL(SRCFILE,hs,4))
    #define LP_PRINTN_H( hs, format, n1 )                   BD_NAME(LpPrintNFun) (BD_FLL(SRCFILE,hs,5), (BDPARM)(n1))
    #define LP_PRINT2N_H( hs, format, n1, n2 )              BD_NAME(LpPrint2NFun)(BD_FLL(SRCFILE,hs,5), (BDPARM)(n1), (BDPARM)(n2))
    #define LP_PRINT3N_H( hs, format, n1, n2, n3 )          BD_NAME(LpPrint3NFun)(BD_FLL(SRCFILE,hs,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3))
    #define LP_PRINT4N_H( hs, format, n1, n2, n3, n4 )      BD_NAME(LpPrint4NFun)(BD_FLL(SRCFILE,hs,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3), (BDPARM)(n4))
    #define LP_PRINT5N_H( hs, format, n1, n2, n3, n4, n5 )  BD_NAME(LpPrint5NFun)(BD_FLL(SRCFILE,hs,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3), (BDPARM)(n4), (BDPARM)(n5))
    #define LP_PRINTCOPY_H( hs, format, string )            BD_NAME(LpPrintCopyFun)(BD_FLL(SRCFILE,hs,4), string)

    #define LP_WARN( string )                         BD_NAME(LpPrintFun)  (BD_FLL(SRCFILE,__LINE__,4))
    #define LP_WARNN( format, n1 )                    BD_NAME(LpPrintNFun) (BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1))
    #define LP_WARN2N( format, n1, n2 )               BD_NAME(LpPrint2NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (BDPARM)(n2))
    #define LP_WARN3N( format, n1, n2, n3 )           BD_NAME(LpPrint3NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3))
    #define LP_WARN4N( format, n1, n2, n3, n4 )       BD_NAME(LpPrint4NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3), (BDPARM)(n4))
    #define LP_WARN5N( format, n1, n2, n3, n4, n5 )   BD_NAME(LpPrint5NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3), (BDPARM)(n4), (BDPARM)(n5))
    #define LP_WARNCOPY( format, string )             BD_NAME(LpPrintCopyFun)(BD_FLL(SRCFILE,__LINE__,4), string)

    #define LP_WARN_H( hs, string )                         BD_NAME(LpPrintFun)  (BD_FLL(SRCFILE,hs,4))
    #define LP_WARNN_H( hs, format, n1 )                    BD_NAME(LpPrintNFun) (BD_FLL(SRCFILE,hs,5), (BDPARM)(n1))
    #define LP_WARN2N_H( hs, format, n1, n2 )               BD_NAME(LpPrint2NFun)(BD_FLL(SRCFILE,hs,5), (BDPARM)(n1), (BDPARM)(n2))
    #define LP_WARN3N_H( hs, format, n1, n2, n3 )           BD_NAME(LpPrint3NFun)(BD_FLL(SRCFILE,hs,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3))
    #define LP_WARN4N_H( hs, format, n1, n2, n3, n4 )       BD_NAME(LpPrint4NFun)(BD_FLL(SRCFILE,hs,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3), (BDPARM)(n4))
    #define LP_WARN5N_H( hs, format, n1, n2, n3, n4, n5 )   BD_NAME(LpPrint5NFun)(BD_FLL(SRCFILE,hs,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3), (BDPARM)(n4), (BDPARM)(n5))
    #define LP_WARNCOPY_H( hs, format, string )             BD_NAME(LpPrintCopyFun)(BD_FLL(SRCFILE,hs,4), string)

    #if defined( OS_SUPPORTS_JVM )
        #include "CircBuffer.h"

#ifdef __cplusplus
extern  "C" {
#endif // __cplusplus
        void BD_NAME(BufPrintFun)( BD_FILE_LINE, CircularBuffer *buf );
        void BD_NAME(BufPrintNFun)( BD_FILE_LINE, BDPARM n1, CircularBuffer *buf );
        void BD_NAME(BufPrint2NFun)( BD_FILE_LINE, BDPARM n1, BDPARM n2, CircularBuffer *buf );
        void BD_NAME(BufPrint3NFun)( BD_FILE_LINE, BDPARM n1, BDPARM n2, BDPARM n3, CircularBuffer *buf );
        void BD_NAME(BufPrint4NFun)( BD_FILE_LINE, BDPARM n1, BDPARM n2, BDPARM n3, BDPARM n4, CircularBuffer *buf );
        void BD_NAME(BufPrint5NFun)( BD_FILE_LINE, BDPARM n1, BDPARM n2, BDPARM n3, BDPARM n4, BDPARM n5, CircularBuffer *buf );
#ifdef __cplusplus
}
#endif // __cplusplus

        #define BUF_PRINT( buf, string )                        BD_NAME(BufPrintFun)  (BD_FLL(SRCFILE,__LINE__,4), (CircularBuffer*) buf)
        #define BUF_PRINTN( buf, format, n1 )                   BD_NAME(BufPrintNFun) (BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (CircularBuffer*) buf)
        #define BUF_PRINT2N( buf, format, n1, n2 )              BD_NAME(BufPrint2NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (BDPARM)(n2), (CircularBuffer*) buf)
        #define BUF_PRINT3N( buf, format, n1, n2, n3 )          BD_NAME(BufPrint3NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3), (CircularBuffer*) buf)
        #define BUF_PRINT4N( buf, format, n1, n2, n3, n4 )      BD_NAME(BufPrint4NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3), (BDPARM)(n4), (CircularBuffer*) buf)
        #define BUF_PRINT5N( buf, format, n1, n2, n3, n4, n5 )  BD_NAME(BufPrint5NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3), (BDPARM)(n4), (BDPARM)(n5), (CircularBuffer*) buf)

        #define BUF_WARN( buf, string )                         BD_NAME(BufPrintFun)  (BD_FLL(SRCFILE,__LINE__,4), (CircularBuffer*) buf)
        #define BUF_WARNN( buf, format, n1 )                    BD_NAME(BufPrintNFun) (BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (CircularBuffer*) buf)
        #define BUF_WARN2N( buf, format, n1, n2 )               BD_NAME(BufPrint2NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (BDPARM)(n2), (CircularBuffer*) buf)
        #define BUF_WARN3N( buf, format, n1, n2, n3 )           BD_NAME(BufPrint3NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3), (CircularBuffer*) buf)
        #define BUF_WARN4N( buf, format, n1, n2, n3, n4 )       BD_NAME(BufPrint4NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3), (BDPARM)(n4), (CircularBuffer*) buf)
        #define BUF_WARN5N( buf, format, n1, n2, n3, n4, n5 )   BD_NAME(BufPrint5NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3), (BDPARM)(n4), (BDPARM)(n5), (CircularBuffer*) buf)
    #else
        #define BUF_PRINT( buf, string )                        PRINT( string )
        #define BUF_PRINTN( buf, format, n1 )                   PRINTN( format, n1 )
        #define BUF_PRINT2N( buf, format, n1, n2 )              PRINT2N( format, n1, n2 )
        #define BUF_PRINT3N( buf, format, n1, n2, n3 )          PRINT3N( format, n1, n2, n3 )
        #define BUF_PRINT4N( buf, format, n1, n2, n3, n4 )      PRINT4N( format, n1, n2, n3, n4 )
        #define BUF_PRINT5N( buf, format, n1, n2, n3, n4, n5 )  PRINT5N( format, n1, n2, n3, n4, n5 )

        #define BUF_WARN( buf, string )                         WARN( string )
        #define BUF_WARNN( buf, format, n1 )                    WARNN( format, n1 )
        #define BUF_WARN2N( buf, format, n1, n2 )               WARN2N( format, n1, n2 )
        #define BUF_WARN3N( buf, format, n1, n2, n3 )           WARN3N( format, n1, n2, n3 )
        #define BUF_WARN4N( buf, format, n1, n2, n3, n4 )       WARN4N( format, n1, n2, n3, n4 )
        #define BUF_WARN5N( buf, format, n1, n2, n3, n4, n5 )   WARN5N( format, n1, n2, n3, n4, n5 )
    #endif

    #ifndef NO_ASSERT

        #define ASSERT(string,condition)                                \
            do {                                                        \
                if( !(condition) ) {                                    \
                    BD_NAME(LpPrintFun)( BD_FLL(SRCFILE,__LINE__,4) );  \
                }                                                       \
            } while( 0 )

        #define ASSERTN(string, n1, condition)                              \
            do {                                                            \
                if( !(condition) ) {                                        \
                    BD_NAME(LpPrintNFun)( BD_FLL(SRCFILE,__LINE__,5), n1 ); \
                }                                                           \
            } while( 0 )

    #endif

    #define ASSERTF(string)                     BD_NAME(LpPrintFun)(BD_FLL(SRCFILE,__LINE__,4))
    #define ASSERTFAILURE(string)               BD_NAME(LpPrintFun)(BD_FLL(SRCFILE,__LINE__,4))
    #define ASSERTFN(string, n1)                BD_NAME(LpPrintNFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1))
    #define ASSERTF2N(string, n1, n2)           BD_NAME(LpPrint2NFun)(BD_FLL(SRCFILE,__LINE__,5),(BDPARM)(n1), (BDPARM)(n2))
    #define ASSERTF3N(string, n1, n2, n3)       BD_NAME(LpPrint3NFun)(BD_FLL(SRCFILE,__LINE__,5),(BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3))
    #define ASSERTF4N(string, n1, n2, n3, n4)   BD_NAME(LpPrint4NFun)(BD_FLL(SRCFILE,__LINE__,5),(BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3), (BDPARM)(n4))
    #define ASSERTFAILUREN(string, n1)          BD_NAME(LpPrintNFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)(n1))
    #define ASSERTFCOPY(string, str)            BD_NAME(LpPrintCopyFun)(BD_FLL(SRCFILE,__LINE__,4), str)

    #ifndef NO_ASSERT

        #define ASSERT_H(hs, string, condition)                     \
            do {                                                    \
                if( !(condition) ) {                                \
                    BD_NAME(LpPrintFun)( BD_FLL(SRCFILE,hs,4) );    \
                }                                                   \
            } while( 0 )

        #define ASSERTN_H(hs, string, n1, condition)                \
            do {                                                    \
                if( !(condition) ) {                                \
                    BD_NAME(LpPrintNFun)( BD_FLL(SRCFILE,hs,5), n1 );\
                }                                                   \
            } while( 0 )

    #endif

    #define ASSERTF_H(hs, string)                           BD_NAME(LpPrintFun)(BD_FLL(SRCFILE,hs,4))
    #define ASSERTFAILURE_H(hs, string)                     BD_NAME(LpPrintFun)(BD_FLL(SRCFILE,hs,4))
    #define ASSERTFN_H(hs, string, n1)                      BD_NAME(LpPrintNFun)(BD_FLL(SRCFILE,hs,5),  (BDPARM)(n1))
    #define ASSERTF2N_H(hs, string, n1, n2)                 BD_NAME(LpPrint2NFun)(BD_FLL(SRCFILE,hs,5), (BDPARM)(n1), (BDPARM)(n2))
    #define ASSERTF3N_H(hs, string, n1, n2, n3)             BD_NAME(LpPrint3NFun)(BD_FLL(SRCFILE,hs,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3))
    #define ASSERTF4N_H(hs, string, n1, n2, n3, n4)         BD_NAME(LpPrint4NFun)(BD_FLL(SRCFILE,hs,5), (BDPARM)(n1), (BDPARM)(n2), (BDPARM)(n3), (BDPARM)(n4))
    #define ASSERTFAILUREN_H(hs, string, n1)                BD_NAME(LpPrintNFun)(BD_FLL(SRCFILE,hs,5),  (BDPARM)(n1))
    #define ASSERTFCOPY_H(hs, string, str)                  BD_NAME(LpPrintCopyFun)(BD_FLL(SRCFILE,hs,4), str)

    #ifndef ASSERTC

        #ifdef FLEDGE_DEBUG

            #ifdef __cplusplus
                extern "C"
            #endif
            void CatastrophicAssertionFailure( const char* file, int line, const char* reason );
            #define DoCatastrophicAssertionFailure( string ) \
                CatastrophicAssertionFailure( __FILE__, __LINE__, string )

        #else   // ifdef FLEDGE_DEBUG

            #define DoCatastrophicAssertionFailure( string ) \
                RimCatastrophicFailure("Catastrophic Assertion Failure")

        #endif

        #define ASSERTC(string,condition)                                       \
            do {                                                                \
                if( !(condition) ) {                                            \
                    BD_NAME(LpPrintFun)( BD_FLL(SRCFILE,__LINE__,4) );          \
                    DoCatastrophicAssertionFailure( string );                   \
                }                                                               \
            } while( 0 )

        #define ASSERTFC(string)                                                \
            do {                                                                \
                BD_NAME(LpPrintFun)( BD_FLL(SRCFILE,__LINE__,4) );              \
                DoCatastrophicAssertionFailure( string );                       \
            } while( 0 )

        #define ASSERTCDEV(string, condition)                                   \
            do {                                                                \
                if( !(condition) ) {                                            \
                    BD_NAME(LpPrintFun)( BD_FLL(SRCFILE,__LINE__,4) );          \
                    if( !IsHeadInSand() ) {                                     \
                        DoCatastrophicAssertionFailure( string );               \
                    }                                                           \
                }                                                               \
            } while( 0 )

        #define ASSERTFCDEV(string)                                             \
            do {                                                                \
                BD_NAME(LpPrintFun)( BD_FLL(SRCFILE,__LINE__,4) );              \
                if ( !IsHeadInSand() ) {                                        \
                    DoCatastrophicAssertionFailure( string );                   \
                }                                                               \
            } while( 0 )

        #define ASSERTC_H(hs, string, condition)                                \
            if( !(condition) ) {                                                \
                BD_NAME(LpPrintFun)( BD_FLL(SRCFILE,hs,4) );                    \
                DoCatastrophicAssertionFailure( string );                       \
            }

        #define ASSERTFC_H(hs, string)                                          \
            do {                                                                \
                BD_NAME(LpPrintFun)( BD_FLL(SRCFILE,hs,4) );                    \
                DoCatastrophicAssertionFailure( string );                       \
            } while( 0 )

        #define ASSERTCDEV_H(hs, string, condition)                             \
            do {                                                                \
                if( !(condition) ) {                                            \
                    BD_NAME(LpPrintFun)( BD_FLL(SRCFILE,hs,4) );                \
                    if( !IsHeadInSand() ) {                                     \
                        DoCatastrophicAssertionFailure( string );               \
                    }                                                           \
                }                                                               \
            } while( 0 )

        #define ASSERTFCDEV_H(hs, string)                                       \
            do {                                                                \
                BD_NAME(LpPrintFun)( BD_FLL(SRCFILE,hs,4) );                    \
                if ( !IsHeadInSand() ) {                                        \
                    DoCatastrophicAssertionFailure( string );                   \
                }                                                               \
            } while( 0 )

    #endif  // ifndef ASSERTC


// TODO: Port the use of the BugBuild configuration file to eltron4 and Nessus
//       so that these macros can be used in those code streams.
#ifdef RIM_ELTRON5
    // For now, the following are defined only for RIM_ELTRON5, becuase that is
    // the only code stream that uses the BugBuild configuration file utility.

    // Macros for BugDisp printing of double or float values.

    // PRINT[[2|3]N]D(format, [n1, [n2, [n3,]]] d1 )
    // n1, n2, n3 are DWORDs and can use any BugDisp integer format specifier.
    // d1 is either a double-precision (double) or floating-point (float) value.  Use %f in the format string.
    //
    #define PRINTD(format, d1) do { \
        double __a = (double)d1; \
        BD_NAME(Print2NFun)(BD_FLL(SRCFILE,__LINE__,5), *(BDPARM *)&__a, *(1+(BDPARM *)&__a)); \
    } while(0)

    #define PRINTND(format, n1, d1) do { \
        double __a = (double)d1; \
        BD_NAME(Print3NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)n1, *(BDPARM *)&__a, *(1+(BDPARM *)&__a)); \
    } while(0)

    #define PRINT2ND(format, n1, n2, d1) do { \
        double __a = (double)d1; \
        BD_NAME(Print4NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)n1, (BDPARM)n2, *(BDPARM *)&__a, *(1+(BDPARM *)&__a)); \
    } while(0)

    #define PRINT3ND(format, n1, n2, n3, d1) do { \
        double __a = (double)d1; \
        BD_NAME(Print5NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)n1, (BDPARM)n2, (BDPARM)n3, *(BDPARM *)&__a, *(1+(BDPARM *)&__a)); \
    } while(0)

    // PRINT[N]2D(format, [n1,] d1, d2 )
    // n1 is a DWORD and can use any BugDisp integer format specifier.
    // Each of d1 and d2 is either a double-precision (double) or floating-point (float) value.  Use two %f's in the format string.
    //
    #define PRINT2D(format, d1, d2) do { \
        double __a = (double)d1; \
        double __b = (double)d2; \
        BD_NAME(Print4NFun)(BD_FLL(SRCFILE,__LINE__,5), *(BDPARM *)&__a, *(1+(BDPARM *)&__a), *(BDPARM *)&__b, *(1+(BDPARM *)&__b)); \
    } while(0)

    #define PRINTN2D(format, n1, d1, d2) do { \
        double __a = (double)d1; \
        double __b = (double)d2; \
        BD_NAME(Print5NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)n1, *(BDPARM *)&__a, *(1+(BDPARM *)&__a), *(BDPARM *)&__b, *(1+(BDPARM *)&__b)); \
    } while(0)

    // WARN[[2|3]N]D(format, [n1, [n2, [n3,]]] d1)
    // n1, n2, n3 are DWORDs and can use any BugDisp integer format specifier.
    // d1 is either a double-precision (double) or floating-point (float) value.  Use %f in the format string.
    //
    #define WARND(format, d1) do { \
        double __a = (double)d1; \
        BD_NAME(Print2NFun)(BD_FLL(SRCFILE,__LINE__,5), *(BDPARM *)&__a, *(1+(BDPARM *)&__a)); \
    } while(0)

    #define WARNND(format, n1, d1) do { \
        double __a = (double)d1; \
        BD_NAME(Print3NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)n1, *(BDPARM *)&__a, *(1+(BDPARM *)&__a)); \
    } while(0)

    #define WARN2ND(format, n1, n2, d1) do { \
        double __a = (double)d1; \
        BD_NAME(Print4NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)n1, (BDPARM)n2, *(BDPARM *)&__a, *(1+(BDPARM *)&__a)); \
    } while(0)

    #define WARN3ND(format, n1, n2, n3, d1) do { \
        double __a = (double)d1; \
        BD_NAME(Print5NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)n1, (BDPARM)n2, (BDPARM)n3, *(BDPARM *)&__a, *(1+(BDPARM *)&__a)); \
    } while(0)

    // WARN[N]2D(format, [n1,] d1, d2)
    // n1 is a DWORD and can use any BugDisp integer format specifier.
    // Each of d1 and d2 is either a double-precision (double) or floating-point (float) value.  Use two %f's in the format string.
    //
    #define WARN2D(format, d1, d2) do { \
        double __a = (double)d1; \
        double __b = (double)d2; \
        BD_NAME(Print4NFun)(BD_FLL(SRCFILE,__LINE__,5), *(BDPARM *)&__a, *(1+(BDPARM *)&__a), *(BDPARM *)&__b, *(1+(BDPARM *)&__b)); \
    } while(0)

    #define WARNN2D(format, n1, d1, d2) do { \
        double __a = (double)d1; \
        double __b = (double)d2; \
        BD_NAME(Print5NFun)(BD_FLL(SRCFILE,__LINE__,5), (BDPARM)n1, *(BDPARM *)&__a, *(1+(BDPARM *)&__a), *(BDPARM *)&__b, *(1+(BDPARM *)&__b)); \
    } while(0)

#endif // #ifdef RIM_ELTRON5

    #define SHOW_MEMORY(string, p, n)           BD_NAME(ShowMemory)(BD_FLL(SRCFILE,__LINE__,4),p,n)
    #define SHOW_MEMORY_ADDRESS(string, p, n)   BD_NAME(ShowMemoryAddress)(BD_FLL(SRCFILE,__LINE__,4),p,n)
    #define DEREF_MEMORY(r, p, n)               BD_NAME(ShowMemoryAddress)(BD_FLL(FILE_RESERVED,r,4),p,n)
    #ifndef RIM_CFP_RAMIMAGE
        #define BIG_SHOW_MEMORY(string, p, n)       BD_NAME(BigShowMemory)(BD_FLL(SRCFILE,__LINE__,0),p,n)
    #else
        #define BIG_SHOW_MEMORY(string, p, n)       BD_NAME(ShowMemory)(BD_FLL(SRCFILE,__LINE__,4),p,n)
    #endif

    #define SHOW_MEMORY_H(hs, string, p, n)             BD_NAME(ShowMemory)(BD_FLL(SRCFILE,hs,4),p,n)
    #define SHOW_MEMORY_ADDRESS_H(hs, string, p, n)     BD_NAME(ShowMemoryAddress)(BD_FLL(SRCFILE,hs,4),p,n)
    #ifndef RIM_CFP_RAMIMAGE
        #define BIG_SHOW_MEMORY_H(hs, string, p, n)     BD_NAME(BigShowMemory)(BD_FLL(SRCFILE,hs,0),p,n)
    #else
        #define BIG_SHOW_MEMORY(string, p, n)       BD_NAME(ShowMemory)(BD_FLL(SRCFILE,__LINE__,4),p,n)
    #endif

    #define LP_SHOW_MEMORY(string, p, n)           BD_NAME(LpShowMemory)(BD_FLL(SRCFILE,__LINE__,4),p,n)
    #define LP_SHOW_MEMORY_ADDRESS(string, p, n)   BD_NAME(LpShowMemoryAddress)(BD_FLL(SRCFILE,__LINE__,4),p,n)
    #ifndef RIM_CFP_RAMIMAGE
        #define LP_BIG_SHOW_MEMORY(string, p, n)       BD_NAME(LpBigShowMemory)(BD_FLL(SRCFILE,__LINE__,0),p,n)
    #else
        #define LP_BIG_SHOW_MEMORY(string, p, n)       BD_NAME(LpShowMemory)(BD_FLL(SRCFILE,__LINE__,4),p,n)
    #endif

    #define LP_SHOW_MEMORY_H(hs, string, p, n)             BD_NAME(LpShowMemory)(BD_FLL(SRCFILE,hs,4),p,n)
    #define LP_SHOW_MEMORY_ADDRESS_H(hs, string, p, n)     BD_NAME(LpShowMemoryAddress)(BD_FLL(SRCFILE,hs,4),p,n)
    #ifndef RIM_CFP_RAMIMAGE
        #define LP_BIG_SHOW_MEMORY_H(hs, string, p, n)     BD_NAME(LpBigShowMemory)(BD_FLL(SRCFILE,hs,0),p,n)
    #else
        #define LP_BIG_SHOW_MEMORY(string, p, n)       BD_NAME(LpShowMemory)(BD_FLL(SRCFILE,__LINE__,4),p,n)
    #endif

    #define PRINTTIMESTAMP()                    PrintTimeStamp()
    #define PRINTTIMESTAMPCOPY(ts)              PrintTimeStampCopy(ts)

    #if defined( NON_BUGDISP_LCD_LOGGING_TPLGSM )
        #include "..\..\..\common\include\lcd_logging.h"
    #elif defined( NON_BUGDISP_LCD_LOGGING_NESSUS ) || defined( NON_BUGDISP_LCD_LOGGING_ELTRON )
        #include "lcd_logging.h"
    #endif

#else  // if ( defined( RIM_FLEDGE ) || defined( DEBUG ) ) && ...

    #define PRINT(string)
    #define PRINTN(string,n1)
    #define PRINT2N(string,n1,n2)
    #define PRINT3N(string,n1,n2,n3)
    #define PRINT4N(string,n1,n2,n3,n4)
    #define PRINT5N(string,n1,n2,n3,n4,n5)
    #define PRINTCOPY(string, str)

    #define WARN(string)
    #define WARNN(string,n1)
    #define WARN2N(string,n1,n2)
    #define WARN3N(string,n1,n2,n3)
    #define WARN4N(string,n1,n2,n3,n4)
    #define WARN5N(string,n1,n2,n3,n4,n5)
    #define WARNCOPY(format,str)
    #ifndef NO_ASSERT
        #define ASSERT(string,condition)
    #endif
    #define ASSERTC(string,condition)
    #define ASSERTCDEV(string, condition)
    #define ASSERTFCDEV(string)
    #define ASSERTF(string)
    #define ASSERTFC(string)
    #define ASSERTFAILURE(string)
    #define ASSERTFN(string, n1)
    #define ASSERTF2N(string, n1, n2)
    #define ASSERTF3N(string, n1, n2, n3)
    #define ASSERTF4N(string, n1, n2, n3, n4)
    #define ASSERTFAILUREN(string, n1)
    #define ASSERTN(string, n1, condition)

    #define SHOW_MEMORY(string, p, n)
    #define SHOW_MEMORY_ADDRESS(string, p, n)
    #define DEREF_MEMORY(r, p, n)
    #define BIG_SHOW_MEMORY(string, p, n)
    #define PRINTTIMESTAMP()
    #define PRINTTIMESTAMPCOPY(ts)

    #define PRINT_H(hs, string)
    #define PRINTN_H(hs, string,n1)
    #define PRINT2N_H(hs, string,n1,n2)
    #define PRINT3N_H(hs, string,n1,n2,n3)
    #define PRINT4N_H(hs, string,n1,n2,n3,n4)
    #define PRINT5N_H(hs, string,n1,n2,n3,n4,n5)
    #define PRINTCOPY_H(hs, string, str)

    #define WARN_H(hs, string)
    #define WARNN_H(hs, string,n1)
    #define WARN2N_H(hs, string,n1,n2)
    #define WARN3N_H(hs, string,n1,n2,n3)
    #define WARN4N_H(hs, string,n1,n2,n3,n4)
    #define WARN5N_H(hs, string,n1,n2,n3,n4,n5)

    #define LP_PRINT( string )
    #define LP_PRINTN( format, n1 )
    #define LP_PRINT2N( format, n1, n2 )
    #define LP_PRINT3N( format, n1, n2, n3 )
    #define LP_PRINT4N( format, n1, n2, n3, n4 )
    #define LP_PRINT5N( format, n1, n2, n3, n4, n5 )
    #define LP_PRINTCOPY( format, string )

    #define LP_SHOW_MEMORY(string, p, n)
    #define LP_SHOW_MEMORY_ADDRESS(string, p, n)
    #define LP_BIG_SHOW_MEMORY(string, p, n)

    #define BUF_PRINT( buf, string )
    #define BUF_PRINTN( buf, format, n1 )
    #define BUF_PRINT2N( buf, format, n1, n2 )
    #define BUF_PRINT3N( buf, format, n1, n2, n3 )
    #define BUF_PRINT4N( buf, format, n1, n2, n3, n4 )
    #define BUF_PRINT5N( buf, format, n1, n2, n3, n4, n5 )

    #define BUF_WARN( buf, string )
    #define BUF_WARNN( buf, format, n1 )
    #define BUF_WARN2N( buf, format, n1, n2 )
    #define BUF_WARN3N( buf, format, n1, n2, n3 )
    #define BUF_WARN4N( buf, format, n1, n2, n3, n4 )
    #define BUF_WARN5N( buf, format, n1, n2, n3, n4, n5 )

    #ifndef NO_ASSERT
        #define ASSERT_H(hs, string,condition)
    #endif
    #define ASSERTC_H(hs, string,condition)
    #define ASSERTCDEV_H(hs, string, condition)
    #define ASSERTFCDEV_H(hs, string)
    #define ASSERTF_H(hs, string)
    #define ASSERTFC_H(hs, string)
    #define ASSERTFAILURE_H(hs, string)
    #define ASSERTFN_H(hs, string, n1)
    #define ASSERTF2N_H(hs, string, n1, n2)
    #define ASSERTF3N_H(hs, string, n1, n2, n3)
    #define ASSERTF4N_H(hs, string, n1, n2, n3, n4)
    #define ASSERTFAILUREN_H(hs, string, n1)
    #define ASSERTFCOPY(string, str)

    #define SHOW_MEMORY_H(hs, string, p, n)
    #define SHOW_MEMORY_ADDRESS_H(hs, string, p, n)
    #define DEREF_MEMORY_H(hs, r, p, n)
    #define BIG_SHOW_MEMORY_H(hs, string, p, n)

    #define BUGTEXT(string) 0

#endif // if ( defined( RIM_FLEDGE ) || defined( DEBUG ) ) && ... else ...

#endif  // ifndef __BUGDISP_H__

