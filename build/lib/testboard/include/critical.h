/*****************************************************************************
 * Filename:    critical.h
 *
 * Copyright 2002, Research In Motion Ltd
 *
 * Author:      Ryan Marshall
 *
 * Created:     March 14, 2002
 *
 * Description: Critical Section Handling
 ****************************************************************************/
#ifndef __CRITICAL_H__
#define __CRITICAL_H__

#include <basetype.h>
#include <rimerr.h>
#include "debug.h"
#include "bugdispi.h"

#if defined(RIM_ELTRON5)
#include "RimRexWrap.h"
#elif defined( QUALCOMM_OS )
 #include <rex.h>
#endif
#if !defined( RIM_CFP_RAMIMAGE ) && !defined( RIM_BOOTROM )
 #include <failure.h>
#else
 #include <ErrorMessages.h>
 
 //Sujoy:- We have removed HaltAndDisplayErrorMessage from miniOS.
 //#define CatastrophicFailureWithCode( x, y )                                 \
 //                   HaltAndDisplayErrorMessage( FALSE, CACHE_FAILURE )
#endif
#if !defined( RIM_FLEDGE )
 #include <int_ops.h>
#endif

#if defined(QUALCOMM_OS)
 #include "queue.h"
#endif

/* Enable this flag to do profiling on semaphore */
#ifdef IMAGE_APPS_PROC
//#define PROFILE_SEMAPHORE
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Counting Semaphore structure.
 */
typedef struct CountingSemaphore_tag{
#ifdef RIM_FLEDGE
    DWORD   state;
#else
    SWORD   count;    ///< Number of free resource units ( < 0 => tasks waiting).
    SWORD   maxCount; ///< Count at which an unlock doesn't increment semaphore count.
    DWORD   initState;  ///< Cookie for semaphore init state

///< ID for task waiting first in line for the semaphore.
#if defined( QUALCOMM_OS )
    q_type    waitingList;       // queue to maintian wainting tasks
    WORD    nestedLockCnt; ///< Number of nested locks
    struct CountingSemaphore_tag *next;
#else
    WORD    firstWaiter; ///< Index of first task waiting for the semaphore (use
                         ///< MAKE_TASK_ID to get the task ID).
                         ///< @note The real type of this value is TaskIndex_t,
                         ///<       but this type is defined only inside the
                         ///<       kernel and cannot be used here.
#endif
#endif
#ifdef PROFILE_SEMAPHORE
   DWORD    totalLockCnts;
   DWORD    firstLockedTime;
   DWORD    totalLockedTime;
   DWORD    lockTimeSamples;
   DWORD    maxNestedLockCnts;
#endif
} CountingSemaphore;


/**
 * Mutex semaphore structure.
 */
typedef struct {
#if defined( QUALCOMM_OS )
    // Implement binary semaphores as counting semaphores with initial and
    // maximum value of 1.
    rex_tcb_type *owner;
    CountingSemaphore sema;
#elif defined( RIM_FLEDGE )
    DWORD   state;
#else
    WORD    state;      ///< Semaphore state (locked, unlocked).
    WORD    waiters;    ///< Number of tasks blocked on this semaphore.
    TASK    owner;      ///< ID for task that currently owns (has locked) the semaphore.
    DWORD   initState;
#endif
} Semaphore;


//Mutex semaphores....
//No access from FIQ handlers
//
//There is no complex deadlock detection.  Be careful
//These are not to be used for flow control, but data protection
//These are not counting semaphores
//The code will assert if the same task tries to lock the same semaphore
//  twice.

//There are some cool ways to cause priority inversion that Sye has come up with
//  don't do them.

/**
 * Locks a Mutex Semaphore
 *
 * @param sem Pointer to Semaphore structure to lock.
 */
IPIFUNC void    RimLockSemaphore( Semaphore *sem );

/**
 * Unlocks a Mutex Semaphore.
 *
 * @param sem Pointer to Semaphore structure to unlock.
 */
IPIFUNC void    RimUnlockSemaphore( Semaphore *sem );

/**
 * Initializes a Mutex Semaphore.
 *
 * Use this function to prepare a Semaphore for use, once you create the
 * structure.
 *
 * @param sem Pointer to Semaphore structure to initialize.
 */
IPIFUNC void    RimInitSemaphore( Semaphore *sem );



//Counting semaphores....
//
//There is no complex deadlock detection.  Be careful
//
//There are some cool ways to cause priority inversion that Sye has come up with
//  don't do them.

/**
 * Locks a Counting Semaphore
 *
 * @param sem Pointer to CountingSemaphore structure to lock.
 */
IPIFUNC void    RimLockCountingSemaphore( CountingSemaphore *sem );

/**
 * Unlocks a Counting Semaphore.
 *
 * @param sem Pointer to CountingSemaphore structure to unlock.
 */
IPIFUNC void    RimUnlockCountingSemaphore( CountingSemaphore *sem );

#if !defined( RIM_CFP_RAMIMAGE )
/**
 * Return the current count of the counting semaphore
 *
 * @param sem Pointer to CountingSemaphore structure
 */
#if defined( QUALCOMM_OS )
SWORD RimCheckSemaphoreCount( CountingSemaphore *sem );
#elif defined( RIM_FLEDGE )
IPIFUNC SDWORD  RimCheckSemaphoreCount( CountingSemaphore *sem );
#else
static __inline SWORD RimCheckSemaphoreCount( CountingSemaphore *sem ) {
    if( sem == NULL ) {
        CatastrophicFailureWithCode( "Null semaphore found", FAILURE_CRITICALSECTION );
    }
    return sem->count;
}
#endif
#endif

/**
 * Initializes a Counting Semaphore.
 *
 * Use this function to prepare a CountingSemaphore for use, once you create the
 * structure.
 *
 * @param sem   Pointer to CountingSemaphore structure to initialize.
 * @param count Initial count.  The first 'count' Locks will not block.
 * @param maxCount  Maximum count.  count will never be greater then maxCount.
 */
IPIFUNC void    RimInitCountingSemaphore( CountingSemaphore *sem, SWORD count, SWORD maxCount );

#ifdef RIM_FLEDGE

/**
 * Direct Fledge to track usage of the semaphore in bugdisp.
 */
IPIFUNC void    RimLogSemaphoreUsage( CountingSemaphore *sem );

#endif

/**
 * Maximum number of RimEnterCriticalSection without RimLeaveCriticalSection
 */
#define MAX_CRITICAL_SECTION_NEST   5

/**
 * Marks start of a critical section.
 */
IPIFUNC void    RimEnterCriticalSection(void);

/**
 * Marks end of a critical section
 */
IPIFUNC void    RimLeaveCriticalSection(void);

/**
 * Checks if currently in critical section.
 *
 * @return 0 if not currently executing within a critical section; otherwise, if
 * positive, number of critical sections currently nested.
 */
IPIFUNC BYTE    RimCheckCriticalSection(void);

/**
 * Define to unconditionally enable interrupts
 */
#if defined ( CORE_BLACKFIN )
#if defined ( HERMON_DSP_DEBUGGER )
#define ENABLE_ALL_INTS   0xFFC0
#else
#define ENABLE_ALL_INTS   0xFFE0
#endif
#else
#define ENABLE_ALL_INTS   0
#endif

/**
 * Disable interrupts.
 *
 * Use this function to disble hardware interrupts for a short piece of code
 * that must operate on data atomically. (This can prevent a piece of critical
 * data from being handled by the interrupt handler when the current task
 * requires exclusive use of it.)
 *
 * Make no system calls while interrupts are disabled using this function.
 * If it is necessary to make a system call (e.g., RimPostMessage) while
 * interrupts are disabled, use {@link #OsDisableInterrupts }
 *
 * @return Disable-marker value to be passed to {@link #CoreIntEnable } for
 * conditional enabling. This API will return the previous interrupt mask
 * value on that specific core.  On the XSCALE core, the first bit of the mask
 * indicates the status of FIQ interrupts ( 1 = disabled, 0 = enabled ),
 * while the second bit indicates the status of IRQ interrupts.
 * On the blackfin core, the previous IMASK value is returned.
 */
#ifdef QUALCOMM_OS
#define         CoreIntDisable() INTLOCK_RET()
#else
IPIFUNC DWORD   CoreIntDisable(void);
#endif

/**
 * Re-enable interrupts.
 *
 * Use this function to re-enable disabled interrupts.
 *
 * If you provide the disable-marker value returned to you by
 * {@link #CoreIntDisable }, this function restores the interrupt state to what it
 * was before you called CoreIntDisable.
 *
 * If you want to unconditionally re-enable interrupts, then pass ENABLE_ALL_INTS
 * into the function
 *
 * @param oldValue Disable-marker value passed back by {@link #CoreIntDisable } if
 * called from thread; zero to unconditionally force an enable.
 */
#ifdef QUALCOMM_OS
#define         CoreIntEnable( mask ) INTFREE_SAV( mask )
#else
IPIFUNC void    CoreIntEnable( DWORD mask );
#endif

/**
 * Determines whether interrupts are enabled or not.
 *
 * @return True if interrupts are enabled; otherwise, false.
 */
IPIFUNC BOOL    InterruptsAreEnabled( void );

/**
 * This function should only be called by code that know's what it is doing.
 * Basically this function does everything CoreIntDisable
 * does, except, it doesn't include the time interrupts are disabled for with
 * this call as part of its profiling.  As such, it should only be called by
 * code that:
 * (1) is only run at initialization, or
 * (2) constantly checks for interrupts while interrupts are disabled, or
 * (3) is only a few lines long, containing no loops or function calls.
 *
 */
#if defined( RIM_FLEDGE ) || defined( WIN32 )
IPIFUNC DWORD   FastCoreIntDisable( void );
#elif defined( QUALCOMM_OS )
#define         FastCoreIntDisable() INTLOCK_RET()
#else
static __inline DWORD FastCoreIntDisable( void )
{
    INT_DISABLE_DEF();
}
#endif

/**
 * This function should only be called by code that know's what it is doing.
 * Basically this function does everything CoreIntEnable
 * does, except, it doesn't include interrupt disable profiling.  It should
 * only be called if IntDisableNoProfile was called to disable interrupts.
 */
#if defined( RIM_FLEDGE ) || defined( WIN32 )
IPIFUNC void    FastCoreIntEnable( DWORD oldValue );
#elif defined( QUALCOMM_OS )
#define         FastCoreIntEnable( oldValue ) INTFREE_SAV( oldValue )
#else
static __inline void FastCoreIntEnable( DWORD oldValue )
{
    INT_ENABLE_DEF( oldValue );
}
#endif

/**
 * Disable Interrupts
 * Protect a region of code from being interrupted by ISRs and preempted
 * by other tasks. This call essentially combines disabling processor
 * interrupts with entering a critical section.
 *
 * It is safe to make non-blocking system calls (e.g. RimPostMessage,
 * RimUnlockCountingSemaphore) while interrupts have been disabled using
 * this function, but blocking system calls will fail.
 *
 * Interrupts should be disabled only when necessary, and only for as long
 * as necessary. Disabling interrupts for too long will result in the
 * failure of critical system components.
 *
 */
IPIFUNC void OsDisableInterrupts( void );

/**
 * Enable Interrupts
 * Re-enable interrupts after they were disabled by
 * {@link OsDisableInterrupts }.
 *
 * Calls to {@link OsDisableInterrupts } may be made when interrupts have
 * already been disabled. This function will only reenable interrupts when
 * it has been called as many times as {@link OsDisableInterrupts } was
 * previously called.
 *
 */
IPIFUNC void OsEnableInterrupts( void );

/*=============================================================================

  FUNCTION RimLockCountingSemaphoreWithTimeout

  DESCRIPTION
    Locks a counting semaphore w/ a timer, i.e. a timed semaphore.  This is
    essentially identical to a regular semaphore, except it will automatically
    unlock after expire, causing this function to return FALSE.  This will
    use a timed semaphore tag already allocated for this semaphore if present,
    otherwise it will dynamically allocate one temporarily.

  PARAMETERS
    sem         The counting semaphore to be locked w/ expiry.
    expiry_ms   Milliseconds until the calling thread will get the lock and
                return a FALSE value, corresponding to a timeout.

  RETURN VALUE
    TRUE if the semaphore was unlocked by another task, FALSE if the semaphore
    expired.

  NOTES

=============================================================================*/
IPIFUNC BOOL RimLockCountingSemaphoreWithTimeout( CountingSemaphore *semAddr, DWORD expiry_ms );

#if defined( QUALCOMM_OS )
/*=============================================================================

  FUNCTION RimDumpCountingSemaphoreDebugInfo

  DESCRIPTION
    Dump detail info about each semaphore.

  PARAMETERS
    dumpAll: TRUE  - Dump the debug info for each semaphore
                  FALSE - Dump the debug info for active semaphores

  RETURN VALUE
    None.

  NOTES

=============================================================================*/
void RimDumpCountingSemaphoreDebugInfo(boolean dumpAll);
#endif /*defined( QUALCOMM_OS )*/

#ifdef __cplusplus
}
#endif

#endif
/**
 * @file
 *
 * Provides access to APIs for semaphore and critical section handling.
 */
