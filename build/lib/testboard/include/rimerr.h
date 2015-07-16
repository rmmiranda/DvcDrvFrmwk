/***************************************************************************
 * Filename:    rimerr.h
 *
 * Copyright 2008-2009, Research In Motion Ltd
 *
 * Author:      Chris Retterath
 *
 * Created:     December 18, 2008
 *
 * Description: Common error enumeration definition
 ***************************************************************************/
#ifndef __RIMERR_H__
#define __RIMERR_H__

#include "basetype.h"

#ifdef _MSC_VER
#pragma pack( push, 1 )
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef enum {
    E_OK           = 0,     //< @deprecated
    RIM_E_OK       = 0,     //< Success
    E_NOMEM        = 1,     //< @deprecated
    RIM_E_NOMEM    = 1,     //< No more memory
    E_ARG          = 2,     //< @deprecated
    RIM_E_ARG      = 2,     //< Bad arg
    E_DEADLCK      = 3,     //< @deprecated
    RIM_E_DEADLCK  = 3,     //< Requested operation would cause deadlock
    E_PERM         = 4,     //< @deprecated
    RIM_E_PERM     = 4,     //< Caller lacks permission to perform operation
    E_ISBUSY       = 5,     //< @deprecated
    RIM_E_ISBUSY   = 5,     //< Requested resource already in use
    E_TIMEDOUT     = 6,     //< @deprecated
    RIM_E_TIMEDOUT = 6,     //< Operation timed out before completion
    E_CRITSEC      = 7,     //< @deprecated
    RIM_E_CRITSEC  = 7,     //< Requested op. inappropriate in critical section
    E_CATFAIL      = 8,     //< @deprecated
    RIM_E_CATFAIL  = 8,     //< Catfail prevents operation
    E_NOTFOUND     = 9,     //< @deprecated
    RIM_E_NOTFOUND = 9,     //< Requested resource was not found.
    E_EXIST        = 10,    //< @deprecated
    RIM_E_EXIST    = 10,    //< Requested resource already exists.
    E_NODATA       = 11,    //< @deprecated
    RIM_E_NODATA   = 11,    //< Operation has no data to return.
    E_FULL         = 12,    //< @deprecated
    RIM_E_FULL     = 12,    //< Specified resource is full.
} _Error_t;
typedef DWORD Error_t;

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef _MSC_VER
#pragma pack( pop )
#endif

#endif // __RIMERR_H__

/**
 * @file
 *
 * Common error enumeration
 */
