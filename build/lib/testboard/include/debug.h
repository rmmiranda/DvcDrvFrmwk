#ifndef __DEBUG__H__
#define __DEBUG__H__

#include "basetype.h"
#include "arm7tdmi.h"
//#include <stdarg.h>

//void PrintMessage(const char *msg,...);


//#define DEBUG(z)                         
#if 0

#define PRINT(msg)                       PrintMessage(msg)
#define PRINTN(msg, n1)                  PrintMessage(msg, n1)
#define PRINT2N(msg, n1, n2)             PrintMessage(msg, n1, n2)
#define PRINT3N(msg, n1, n2, n3)         PrintMessage(msg, n1, n2, n3)
#define PRINT4N(msg, n1, n2, n3, n4)     PrintMessage(msg, n1, n2, n3, n4)
#define PRINT5N(msg, n1, n2, n3, n4, n5) PrintMessage(msg, n1, n2, n3, n4, n5)




#define WARN(msg)                        PrintMessage(msg)
#define WARNN(msg, n1)                   PrintMessage(msg, n1)
#define WARN2N(msg, n1, n2)              PrintMessage(msg, n1, n2)
#define WARN3N(msg, n1, n2, n3)          PrintMessage(msg, n1, n2, n3)
#define WARN4N(msg, n1, n2, n3, n4)      PrintMessage(msg, n1, n2, n3, n4)
#define WARN5N(msg, n1, n2, n3, n4, n5)  PrintMessage(msg, n1, n2, n3, n4, n5)


#define ASSERTF(x)
#define ASSERTFN( x,y )
#define ASSERTFAILURE(x)
#define ASSERT(x,y)
#define ASSERTC(x,y)
#define ASSERTF4N(x,y,z,w,p)
#define ASSERTCDEV(string, condition)
#define ASSERTF3N(string, n1, n2, n3)
#define ASSERTF2N(string, n1, n2)
#define BUF_PRINT2N( x,y,z, m )
#define BUF_PRINT( m, n )
#define PrintCopyFun( a, b )
#define WARNCOPY(format,str);

#endif



void CatastrophicFailureWithCode( const char *failureMessage, int code );
void RimCatastrophicFailure( const char *failureMessage );

void CatastrophicFailureWithContext( const char *failureMessage, int code,
                                     void *regContext );
extern RegisterContext storageForRegisters_data;

extern RegisterContext *storageForRegisters;

int IsHeadInSand(void);

typedef enum {
    NV_FLASH_DENIED = -17,      ///< Access denied
    NV_FLASH_FAILED = -16,      ///< Operation failed
    NV_FLASH_FULL,              ///< Operation failed because NVRAM is full
    NV_FLASH_RETRY,             ///< Internal only, used to ensure thread safety
    NV_FLASH_MULTIPLE_UPDATE,   ///< Multiple simultaneous updates to the same record (user error)
    NV_FLASH_NOT_SHARED_MEMORY, ///< Attempt to use cross core API without shared memory
    NV_PASS,
    NV_FAIL,
    PACK_REQUIRED,
    PACK_SUCCESSFUL,
    PACK_FAILED,
    NV_HASH_EXISTS,
    NV_FLASH_OK = 0             ///< Success
} NvFlashStatus;


DWORD NvGetRecordCopy( WORD recordNumber, void *data_p, DWORD maxSize );
NvFlashStatus NvUpdateRecord( WORD recordNumber, const void *data_p, DWORD length );
DWORD NvDeleteRecord( WORD recordNumber );


extern BOOL CatastrophicFailure;


#define LOGWORTHY_EVENT_USB             3

#define NV_OSSTORE_TavorPVUsbCal_NUM 1
#define NV_OSSTORE_USB_NUM 2
#define NV_OSSTORE_USB_COPY_NUM 3


//void HitWatachDog(void);
#define HitWatachDog    WDT_HIT



#endif
