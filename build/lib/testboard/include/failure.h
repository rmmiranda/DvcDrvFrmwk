/******************************************************************************
 * Copyright (c) 2000 Research In Motion Ltd
 ******************************************************************************
 *
 *      FILENAME : falure.h
 * CREATION DATE : 2000/10/20
 *   DESCRIPTION : Failure codes for catastrophic failure screen
 *
 ******************************************************************************/

#ifndef _FAILURE_H_
#define _FAILURE_H_

#include <basetype.h>

#if defined( RIM_CFP_RAMIMAGE ) || defined( RIM_BOOTROM )
 #error use ErrorMessages.h instead
#endif

/******************************************************************************
 * FAILURE CODES
 *
 * Failure codes start at 300 to avoid any confusion with Intel failure codes
 ******************************************************************************/

/** @name Exception Handlers */
//@{
#define FAILURE_UNDEFINED_EXCEPTION 241
#define FAILURE_SWI_EXCEPTION       242
#define FAILURE_PREFETCH_EXCEPTION  243
#define FAILURE_ABORT_EXCEPTION     244
#define FAILURE_IRQ_EXCEPTION       245
#define FAILURE_HWERROR_EXCEPTION   246
#define FAILURE_UNDEFINEDHANDLER_EXCEPTION 247
#define FAILURE_FIQ_EXCEPTION       248
#define FAILURE_JUMPTOZERO_EXCEPTION 249
//@}

/** @name 300 - 309: ? */
//@{
#define FAILURE_USER_HALT           300
    /**<
     * User requested halt.
     * User pressed ALT-SHIFT-BACKSPACE (or equivalent 3-key sequence).
     */
#define FAILURE_DESCRIBED           302
    /**<
     * Generic error described by failure message.
     * Used by general-purpose functions to display a failure message directly
     * to user.  Failure message in question is required to debug this problem.
     * Submit an SDR to devtrack.
     * TODO: Create a table of failure messages.
     */
#define FAILURE_TIMERHW             303
    /**<
     * Failure with the timer hw API.
     * Submit an SDR to devtrack.
     */
#define FAILURE_BOOT                305
    /**<
     * Generic boot failure.
     * Submit an SDR to devtrack.
     */
#define FAILURE_EXCEPTION           306
    /**<
     * Exit from exception failed.
     * Submit an SDR to devtrack.
     */
#define FAILURE_BAD_PASSWORD        307
    /**<
     * Incorrect password entered in communication with PC; device wiped.
     */
//@}

/** @name 310 - 319: Hardware Failure Codes */
//@{
#define FAILURE_HARDWARE            310
    /**<
     * Generic unrecoverable hardware failure.
     * Submit an SDR to devtrack.
     */
#define FAILURE_RTC_FREQUENCY       311
    /**<
     * RTC hardware set to wrong frequency.
     */
#define FAILURE_FREQUENCY_CHANGE    314
    /**<
     * Failure when trying a frequency change.
     */
#define FAILURE_FIQ_UNREGISTERED    315
    /**<
     * FIQ unregistered.
     */
#define FAILURE_LANDFILL            316
    /**<
     * This device belongs in the trash.
     * This error is only seen when we drop support for older revisions of
     * hardware, and can only be seen if you loaded your device in some
     * clever non-standard way.  You likely need a replacement device, or
     * an old (or custom-built) OS.
     */
#define FAILURE_INVALID_HWV         317
    /**<
     * The HWV table is corrupt.
     */
#define FAILURE_RTC_HALTED          318
    /**<
     * The HW RTC and/or slow clock has stopped running.
     */
#define FAILURE_PMIC                319
    /**<
     * Failure communicating to PMIC.
     */
//@}

/** @name 320 - 329: AMX Failure Codes */
//@{
#define FAILURE_AMX_FATAL_EXIT      320
    /**<
     * Fatal exit failure detected by AMX.
     */
#define FAILURE_AMX_FATAL_EXCEPTION 321
    /**<
     * Fatal exception error detected by AMX.
     */
#define FAILURE_AMX_SERIOUS_ERROR   322
    /**<
     * Serious error detected by AMX.
     */
//@}

/** @name 330 - 339: Kernel Failure Codes 1 */
//@{
#define FAILURE_TASKING_BAD_DEST    331
    /**<
     * Failure related to app task switcher.
     * Submit an SDR to devtrack.
     */
#define FAILURE_TASKING_OUT_OF_MSG  332
    /**<
     * Out of free pool messages.
     * Submit an SDR to devtrack.
     */
#define FAILURE_DEADLOCK            333
    /**<
     * Deadlock detected by the kernel.
     * Submit an SDR to devtrack.
     */
#define FAILURE_SENDMESSAGE         334
    /**<
     * System Thread called blocking send.
     * Submit an SDR to devtrack.
     */
#define FAILURE_QUEUE_PRIORITY      335
    /**<
     * Queue priority error.
     * Submit an SDR to devtrack.
     */
#define FAILURE_QUEUE_CODE          336
    /**<
     * Queue code error.
     * Submit an SDR to devtrack.
     */
#define FAILURE_ENTRYPOINT          337
    /**<
     * Bad mode before entry.
     * Submit an SDR to devtrack.
     */
#define FAILURE_SWI                 338
    /**<
     * Unknown or Bad Secure SWI called.
     * Submit an SDR to devtrack.
     */
#define FAILURE_CREATETHREAD        339
    /**<
     * Can't create Thread!.
     * Submit an SDR to devtrack.
     */
//@}

/** @name 340 - 343: Memory Failure Codes */
//@{
#define FAILURE_DSTACK              342
    /**<
     * Stack overflow.
     */
#define FAILURE_MEMORY_CORRUPT      343
    /**<
     * Memory corruption has been detected.
     */
//@}

/** @name 344 - 349: Unusual/Deprecated Failure Codes */
//@{
#define FAILURE_CATAFAIL            344
    /**<
     * Catastrophic Fail Called Twice.
     * Submit an SDR to devtrack.
     */
#define FAILURE_SMALL_HEAP          345
    /**<
     * App heap too small.
     */
#define FAILURE_TTP_DEVASSERT       346
    /**<
     * TTP DevAsssert went off (if in developer mode).
     */
#define FAILURE_TTP_DEVCHECK        347
    /**<
     * TTP DevCheck went off (if in developer mode).
     */
#define FAILURE_QCOM_DEVFAIL        348
    /**<
     * Qualcomm's ERR_FATAL error.
     */
#define FAILURE_TTP_DEVFAIL         349
    /**<
     * TTP DevFail error (if in developer mode).
     */
//@}

/** @name 350 - 359: Software Failure Codes 1 */
//@{
#define FAILURE_SOFTWARE            350
    /**<
     * Generic unrecoverable software error.
     */
#define FAILURE_MEM_INIT            351
    /**<
     * Memory initialization problem.
     */
#define FAILURE_SOFTWARE_ERASE_ADDR 352
    /**<
     * Bad address on call to erase.
     */
#define FAILURE_SOFTWARE_WRITE_ADDR 353
    /**<
     * Bad address on call to write.
     */
#define FAILURE_USB_DRIVER          354
    /**<
     * Failure in USB driver or hardware.
     */
#define FAILURE_IOTASK              355
    /**<
     * Failure related to iotask services.
     */
#define FAILURE_SLOWCLOCK           356
    /**<
     * Failure related to slow clocking code.
     */
#define FAILURE_SOFTWARE_COMPILER   357
    /**<
     * Software failures raised by compiler.
     */
#define FAILURE_INT_CONTROLLER      358
    /**<
     * Interrupt controller software.
     */
#define FAILURE_CACHE_MMU           359
    /**<
     * Various cache / mmu problems.
     */
//@}

/** @name 360 - 364: Flash Failure Codes 1 */
//@{
#define FAILURE_FLASH_ERASE         360
    /**<
     * Flash erase failed.
     */
#define FAILURE_FLASH_WRITE         361
    /**<
     * Flash programming failed.
     */
#define FAILURE_FLASH_ID            362
    /**<
     * Unsupported flash type.
     */
#define FAILURE_FLASH_SIZE          363
    /**<
     * Unexpected flash size.
     */
//@}

/** @name 365 - 369: OSStore & CalSector failure codes */
//@{
#define FAILURE_DEVICE_UNCALIBRATED 364
    /**<
     * Device's band support is not set.
     */
#define FAILURE_SECTOR_NOT_INIT     365
    /**<
     * OSStore or calsector address not initialized.
     */
#define FAILURE_OSSTORE_BAD_KEY     366
    /**<
     * OSStore's magic key different from OS's.
     */
#define FAILURE_OSSTORE_BAD_VERSION 367
    /**<
     * Device contains higher OSStore major version.
     */
#define FAILURE_CAL_SECTOR_CORRUPTED 368
    /**<
     * Device's checksum different from computed value.
     */
#define FAILURE_IMEI_MISMATCH       369
    /**<
     * OSStore's IMEI does not match Patriot's.
     */
//@}

/** @name 370 - 371: Flash Failure Codes 2 */
//@{
#define FAILURE_NAND_FLASH_READ     370
    /**<
     * NAND flash failure.
     */
#define FAILURE_NAND_FLASH          371
    /**<
     * NAND flash failure.
     */
//@}

/** @name 372 - 379: Software Failure Codes 2
     */
//@{
#define FAILURE_API                 372
    /**<
     * Incompatable OS API version or unsupported OS API call.
     * This is raised when the OS detects that it is incompatable with the VM
     * it is linked with, or when the VM calls a function that is unimplemented
     * in the OS.  If this error is seen in an SCM build, submit an SDR to
     * devtrack.  If this is seen in a developer build, you likely need
     * to recompile the OS and/or the VM with compatable API headers.
     */
#define FAILURE_TIMERS              373
    /**<
     * Timers corrupted.
     * Submit an SDR to devtrack.
     */
#define FAILURE_OTAFU               374
    /**<
     * Over the air firmware upgrade failure.
     * Submit an SDR to devtrack.
     */
#define FAILURE_API_PARAMS          375
    /**<
     * Some failure with API parameters
     */
#define FAILURE_OVERLAYS            377
    /**<
     * Overlay manager, mutual exclusion, corrupt overlays.
     */
#define FAILURE_I2C                 378
    /**<
     * Some failure in the I2C driver
     */
//@}

/** @name 390 - 399: Watchdog Failure Codes */
//@{
#define FAILURE_SW_WATCHDOG         394
    /**<
     * App software watchdog timeout.
     * Send a Bugdisp log to the VM Group.
     */
#define FAILURE_WATCHPUPPY          395
    /**<
     * Watchpuppy timeout.
     * Submit an SDR to devtrack.
     */
//@}

/** @name 400 - 409: Software Failure Codes 3 */
//@{
#define FAILURE_UNSPECIFIED         400
    /**<
     * Some unspecified failure.
     * Submit an SDR to devtrack.
     */
#define FAILURE_GPIO                401
    /**<
     * Failure due to misconfiguration of GPIO.
     * Submit an SDR to devtrack.
     */
//@}
#define FAILURE_BAD_HEAP_CALL       402
    /**<
     * Someone called a heap function with bad params.
     * Submit an SDR to devtrack.
     */
#define FAILURE_SDCARD              403
    /**<
     * SD card driver failure.
     */
#define FAILURE_BOOT_SECURITY       404
    /**<
     * Boot security failure.
     */
#define FAILURE_SOCKET              405
    /**<
     * Socket layer failure.
     * Send a Bugdisp log to the multi-WAF team.
     */
#define FAILURE_GENERIC_HEAP        406
    /**<
     * Generic Heap failure.
     * Submit an SDR to devtrack.
     */
#define FAILURE_NVSIG_SECURITY      407
    /**<
     * NVRAM signature verification failure.
     * Submit an SDR to devtrack.
     */
#define FAILURE_STACK_UTILS         408
    /**<
     * Stack utilities bug.  Includes alternate stack management.
     * Submit an SDR to devtrack.
     */

/** @name 410 - 419: Radio Failure Codes */
//@{
#define FAILURE_RADIO_SW            410
    /**<
     * Generic unrecoverable failure of the radio code.
     * Send a Bugdisp log to the appropriate Radio Group.
     */
#define FAILURE_RADIO_DSP           411
    /**<
     * Radio DSP failure.
     * Send a Bugdisp log to the appropriate Radio Group.
     */
#define FAILURE_SIM_CARD            412
    /**<
     * Failure related to SIM card.
     * Send a Bugdisp log to the appropriate Radio Group.
     */
#define FAILURE_DATAQUEUE           413
    /**<
     * Failure related to Priority Data Queues.
     * Submit an SDR to devtrack for the Kernel Services Group.
     */
//@}

/** @name 420 - 429: Kernel Failure Codes 2 */
//@{
#define FAILURE_REX_FATAL_EXCEPTION 420
    /**<
     * Fatal exception error detected by REX.
     * REX stands for Real Time Executive, as provided in Eltron by QUALCOMM Incorporated.
     * Any such error could be due to software error, power management or hardware issues.
     * Submit an SDR to devtrack.
     */
#define FAILURE_UNDESIRED_WATCHDOG_RESET 421
    /**<
     * Undesired watchdog reset.
     * Could be due to software error, clock problem, bad memory access, or
     * hardware issue.
     */
#define FAILURE_CRITICALSECTION     422
    /**<
     * Critical section violation.
     * Submit an SDR to devtrack.
     */
#define FAILURE_INTERRUPTS          423
    /**<
     * Some problem with interrupts.
     * They were disabled too long, they were enabled when they shouldn't be,
     * etc.
     */
#define FAILURE_UNHANDLED_RESET     424
    /**<
     * Device reset, but we don't know why.
     * Submit an SDR to devtrack.
     */
#define FAILURE_BAD_WAIT_MASK       425
    /**<
     * Someone passed in a bad mask in WaitForSpecificMessage.
     * Submit an SDR to devtrack.
     */
#define FAILURE_STOP_RESTART        426
    /**<
     * Task stop / restart API failure.
     * Submit an SDR to devtrack.
     */
#define FAILURE_MSG_QUEUE_FULL      427
    /**<
     * The kernel message queue is full.
     * Submit an SDR to devtrack.
     */
#define FAILURE_THREAD_RETURN       428
    /**<
     * A thread was returned from.
     * Submit an SDR to devtrack.
     */
#define FAILURE_SEMAPHORE           429
    /**<
     * Someone made an inappropriate call to the semaphore API.
     * Submit an SDR to devtrack.
     */
#define FAILURE_CRYPTO              430
    /**<
     * Cryptographic subsystem failure.
     * Submit an SDR to devtrack.
     */
//@}

/** @name 570 - 579: Kernel Failure Codes 3 */
//@{
#define FAILURE_RETURN              572
    /**<
     * Function return not expected.
     * Submit an SDR to devtrack.
     */
#define FAILURE_IPC                 573
    /**<
     * Intercore communication failure.
     * Submit an SDR to devtrack.
     */
//@}

/** @name 700 - 799: RESERVED for Manufacturing Error Codes */
//@{
#define FAILURE_MFG_FIRST           700
    /**<
     * The first error code reserved for manufacturing.
     */
#define FAILURE_MFG_LAST            799
    /**<
     * The last error code reserved for manufacturing.
     */
//@}


/**
 * Cookie used by the catfail code to signal when a catastrophic failure
 * occurs while in catfail mode.
 */
#define BAD_CATAFAIL        0x7A8B5C8F


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Raises a catastrophic failure with the given message and failure code, and
 * enters catfail mode; does not return.
 *
 * <em>NOTE</em>: Failure codes 32768 and above are not subject to Head-In-Sand
 * mode, meaning that a catfail halt (not a reset) is triggered when one of
 * these failure codes is used, regardless of the state of HIS mode.  All
 * codes below this value are bound by the HIS rules.  An easy way to make
 * one of the above codes unconditionally fatal is to add 40,000 to it.
 * Don't do this for a shipping build, though; fix the bug instead.  ;-)
 *
 * @param failureMessage String containing more detailed information about the
 * failure.
 * @param code An appropriate failure code to classify this failure.
 */
IPIFUNC void CatastrophicFailureWithCode( const char *failureMessage, int code );

/**
 * Determines if a catastrophic failure occured while in catfail mode.
 *
 * @return True if a failure occured while in catfail, false otherwise.
 */
BOOL  IsMultipleFailure(void);

/**
 * Flag that is set to {@link #BAD_CATAFAIL } when a catastrophic failure
 * occurs while in catfail mode.
 */
extern DWORD MultipleCatFail;

#ifdef __cplusplus
}
#endif

#endif

/**
 * @file
 *
 * Failure codes for catastrophic failure screen.
 *
 * This file contains the failure codes for the OS/Radio catastrophic failure
 * screen.  You may also be interested in the following:
 * - Failure codes for the bootrom are transmitted through the LED; you can
 *   find more information here: <a href="http://go/bootromcodes">http://go/bootromcodes</a>
 * - Failure codes for the JVM and Java Apps can be found here:
 *   <a href="http://indy/jvm/jvmerror.html">http://indy/jvm/jvmerror.html</a>
 *
 */
