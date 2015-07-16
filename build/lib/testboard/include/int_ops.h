/*****************************************************************************
 * Filename:    int_ops.h
 *
 * Copyright 2006-2009 Research In Motion Ltd
 *
 * Author:      Jerry Mailloux
 *
 * Created:     July 23, 2006
 *
 * Description: Inline definition for interrupt operations
 ****************************************************************************/
#ifndef _INT_OPS_H_
#define _INT_OPS_H_

#include <basetype.h>

#if defined( CORE_BLACKFIN )

#define INT_DISABLE_DEF()                                                   \
do {                                                                        \
    DWORD status;                                                           \
    asm volatile( "CLI %0;"                                                 \
                  : "=d" (status) );                                        \
    return status;                                                          \
} while( 0 )

#define INT_ENABLE_DEF( x ) asm volatile( "STI %0;" : : "d" (x) )

#elif defined( CORE_STARCORE )

#include <prototype.h>

#define INT_DISABLE_DEF()                                                   \
do {                                                                        \
    DWORD status = !(readSR() & (1<<19));                                   \
    di();                                                                   \
    return status;                                                          \
} while( 0 )

#define INT_ENABLE_DEF( x )                                                 \
do {                                                                        \
    if( (x) ) {                                                             \
        ei();                                                               \
    }                                                                       \
} while( 0 )

#elif defined( __XTENSA__ )
#include <xtensa/xtruntime.h>
#define INT_DISABLE_DEF()   return XTOS_SET_INTLEVEL( 15 )
#define INT_ENABLE_DEF( x ) XTOS_RESTORE_INTLEVEL( x )

#else
// on ARM cores, we can't do an inline version of this because we have to
// switch to ARM mode.
extern DWORD FastCoreIntDisable_real( void );
extern void  FastCoreIntEnable_real( DWORD oldValue );

#define INT_DISABLE_DEF()  return FastCoreIntDisable_real()

#define INT_ENABLE_DEF( x )  FastCoreIntEnable_real( x )

#endif
#endif
