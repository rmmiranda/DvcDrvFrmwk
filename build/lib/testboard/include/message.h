/*****************************************************************************
 * * Filename:    message.h
 * *
 * * Copyright 2000, Research In Motion Ltd
 * *
 * * Description: Rim message definitions.
 * ****************************************************************************/
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "basetype.h"

#ifdef _MSC_VER
#pragma pack( push, 1 )
#endif

/**
 * Message structure.
 *
 * Any system or application message is encoded by a message
 * structure.
 *
 */
typedef struct _MESSAGE
{
  DWORD   Device;   ///< Indicates sender of the message: one of the DEVICE_* constants.
  DWORD   Event;    ///< Event that prompted sending this message.
  DWORD   SubMsg;   ///< Extra data further categorizing this event.
  DWORD   Length;   ///< Length of the included data blob.
  char   *DataPtr;  ///< Pointer to a data blob for extra event-specific information.
  DWORD   Data[2];  ///< Fixed length data array for extra event-specific information.
} MESSAGE;


/**
 * @name Miscellanous system constants
 */
//@{
#define MAX_MESSAGES    128          ///< Maximum number of messages that can fit in the message queue.
#define TASK_NOT_FOUND ((TASK)0xffff)///< Task not found error event.
//@}


/**
 * @name Device event senders.
 *
 * These values encompass the various event types that an
 * application can receive from the system.
 */
//@{
#define BUGBUILD_ENUM "DEVICE"
#define DEVICE_NONE             0x00  ///< No event.
#define DEVICE_SYSTEM           0x01  ///< System event.
#define DEVICE_KEYPAD           0x02  ///< Keypad event.
#define DEVICE_RTC              0x03  ///< Real-time clock event.
#define DEVICE_TIMER            0x04  ///< Timer event.
#define DEVICE_SERIAL           0x05  ///< Serial port event.
#define DEVICE_RADIO            0x06  ///< Radio event.
#define DEVICE_HOLSTER          0x07  ///< Holster event.
#define DEVICE_HEADSET          0x08  ///< Headset event.
#define DEVICE_SIM              0x09  ///< SIM card event.
#define DEVICE_ALERT            0x0A  ///< Alert event.
#define DEVICE_PERIPHERAL       0x0B  ///< Peripheral attachment event.
#define DEVICE_IRDA             0x0C  ///< IRDA port event.
#define DEVICE_STREAMING        0x0D  ///< Audio streaming event
#define DEVICE_USB              0x0E  ///< USB event.
#define DEVICE_EXPANSION        0x0F  ///< Device expansion event.
#define DEVICE_SMARTCARD        0x10  ///< Smartcard event.
#define DEVICE_RTP_RTCP         0x11  ///< RTP RTCP event.
#define DEVICE_WLAN             0x12  ///< WLANLink device.
#define DEVICE_FILE             0x13  ///< File System event.
#define DEVICE_CURSOR           0x14  ///< EOTD positioning system.
#define DEVICE_AUDIO            0x15  ///< Audio event, including TTY.
#define DEVICE_SIMULATOR        0x16  ///< Simulator-only event.
#define DEVICE_GPS              0x17  ///< Global positioning system event.
#define DEVICE_MELODY           0x18  ///< Melody/audio system.
#define DEVICE_BLUETOOTH        0x19  ///< Bluetooth event.
#define DEVICE_STYLUS           0x1A  ///< Stylus event.
#define DEVICE_TRACKBALL        0x1B  ///< Trackball event.
#define DEVICE_VIDEO            0x1D  ///< Video player event.
#define DEVICE_BBCLIENT         0x1E  ///< BlackBerry Client event.
#define DEVICE_VAD              0x1F  ///< Voice activated dialing event.
#define DEVICE_MEDIA            0x20  ///< Media player event.
#define DEVICE_TCP              0x21  ///< TCP event.
#define DEVICE_INET             0x22  ///< INET : <generic> Internet protocols (DNS, NTP) event
#define DEVICE_VPN              0x23  ///< VPN event.
#define DEVICE_CAMERA           0x24  ///< Camera event.
#define DEVICE_RTSP             0x25  ///< RTSP event.
#define DEVICE_DISPLAY          0x26  ///< Display event.
#define DEVICE_SOCKET           0x27  ///< Socket event.
#define DEVICE_WAF_3GPP         0x28  ///< 3GPP Wireless Access Family
#define DEVICE_WAF_WLAN         0x29  ///< WLAN Wireless Access Family
#define DEVICE_WAF_CDMA         0x30  ///< CDMA Wireless Access Family
#define DEVICE_WAF              0x31  ///< Generic Wireless Access Family
#define DEVICE_WAF_VPN          0x32  ///< VPN Wireless Access Family
#define DEVICE_SENSOR           0x33  ///< Sensor event.
#define DEVICE_FPS              0x34  ///< Fingerprint Scanner (FPS) event.
#define DEVICE_TOUCHSCREEN      0x35  ///< Touchscreen event.
#define DEVICE_ACCELEROMETER    0x36  ///< Accelerometer event.
#define DEVICE_WAF_IDEN         0x37  ///< IDEN Wireless Access Family
#define DEVICE_FXP              0x38  ///< File Xfer Protocol event
#define DEVICE_DRM              0x39  ///< DRM event.
#define DEVICE_SYSPROF          0x3A  ///< System profiling event. (see sysprof.h for message description)
#define DEVICE_FLASH            0x3B  ///< Flash storage system event
#define DEVICE_PTX              0x3C  ///< PTX event
#define DEVICE_TTS              0x3D  ///< TTS event
#define DEVICE_IMAGE            0x3E  ///< Image event.
#define DEVICE_ASSD             0x3F  ///< Advanced Security SD (ASSD) event.
#define DEVICE_SW_CMPTT         0x40  ///< Core mobility PTT software event.
#define DEVICE_USER             0x80  ///< User defined event.
#undef BUGBUILD_ENUM
//@}

// DEVICE_DNS is has been obsoleted by DEVICE_INET, but it is still defined for
// backward compatibility in the OS API.
#define DEVICE_DNS              DEVICE_INET


/**
 * @name System event types.
 *
 * These values encompass the various events that can be passed in a
 * {@link #DEVICE_SYSTEM } event message.
 */
//{@
#define BUGBUILD_ENUM "EVENT"
/**
 * Low battery warning.
 *
 * When the internal battery voltage falls below a critical level, the system
 * sends this message to all applications. The applications should then save any
 * necessary information to the flash file system. To start recharging the
 * battery, the user must plug in the external power adapter.
 */
#define BATTERY_LOW          0x0102
/**
 * Device is powering off.
 *
 * The system sends this message to all applications; the user is eputting the
 * device into a power-off state. Although power is not actually off, the
 * applications do not receive any messages, the LCD display is blank in
 * power-save mode, and the radio is off. An application will turn the power off
 * by calling {@link #RimRequestPowerOff }.
 *
 * <em>Note</em>: If the COM port is open at power down, the system closes it
 * automatically.
 */
#define POWER_OFF            0x0103
/**
 * Device is powering up.
 *
 * The system sends this message to all applications; the device has left the
 * power-off state. This may be caused by a key or trackwheel event, a real-time
 * clock alarm event, or a change of state on the COM control lines indicating
 * the communications cable has been plugged in.
 */
#define POWER_UP             0x0104
#define POWER_OFF_REQUEST    0x0105  ///< Request to power off via silver-key press.
/**
 * Battery at normal power levels.
 *
 * The system sends this message to all applications.
 */
#define BATTERY_GOOD         0x0107
#define MEMORY_LOW           0x0108  ///< Low memory warning.
#define BATTERY_UPDATE       0x0109  ///< Change in battery state.
/**
 * Tell BugDispTask to grab the COM port.
 *
 * The system sends this message only to the BugDisp task.
 */
#define BUGDISP_INIT         0x010A
#define CAL_UPDATE           0x0120  ///< Tell calibration screen if we are in console mode.
/**
 * A device has been placed in the wrong cradle.
 *
 * Messages informs apps that a device has been inserted into the wrong cradle
 * (i.e. a Tachyon 2+ being put into a Tachyon 2 cradle)
 */
#define CRADLE_MISMATCH         0x0121
#define CRADLE_MISMATCH_CLEAR   0x0122  ///< Cradle mismatch error has been cleared.
/**
 * Enough current available to do a flash erase (<strong>Baryon</strong>).
 *
 * The charging task sends a message to the application task
 * when there is enough current available to do a flash erase.
 */
#define ENOUGH_CURRENT_AVAILABLE    0x0130
/**
 * Battery put into device (<strong>Baryon</strong>).
 *
 * The system sends a message to the charging task when the user puts a battery
 * into the device.
 */
#define BATTERY_IN                  0x0131
/**
 * Battery removed from device (<strong>Baryon</strong>).
 *
 * The system sends a message to the charging task when the user removes a
 * battery from the device.
 */
#define BATTERY_OUT                 0x0132
/**
 * Battery door opened.
 *
 * When hardware supports it, the system sends a message to the application
 * when the user opens the battery door.  This is used to display appropriate
 * messages to the user.
 */
#define BATTERY_DOOR_OPENED         0x0133
/**
 * Battery door closed.
 *
 * When hardware supports it, the system sends a message to the application
 * when the user closes the battery door.  This is used to display appropriate
 * messages to the user.
 */
#define BATTERY_DOOR_CLOSED         0x0134
/**
 * Battery log.
 *
 * The system sends a message to the application when Battery Log should add
 * a new entry.  This message will be sent periodically, such as during normal
 * battery updates.
 */
#define BATTERY_LOG                 0x0135

/**
 * The backlight domain state has changed.
 *
 * The system sends this message to the apps when the state of one or more
 * backlights have changed.
 *
 * BACKLIGHT_UPDATE message fields have the following format if the backlight
 * API major version is 2 (returned by {@link RimBkltApiVersionGet}.
 * <ul>
 * <li>SubMsg:  Active Backlight Domain
 * <li>Length:  Active Backlight Domain State; TRUE == awake; FALSE == asleep
 * <li>DataPtr: Active Backlight Domain brightness
 * <li>Data[0]: Value of RimGetNativeTicks() at the time of the backlight state
 *              change
 * </ul>
 *
 * BACKLIGHT_UPDATE message fields have the following format if the backlight
 * API major version is 1 (returned by {@link RimBkltApiVersionGet}.
 * <ul>
 * <li>SubMsg:  Mask of all backlight types that have changed
 * <li>Length:  New state of the specified backlights
 * <li>DataPtr: Active Backlight Domain brightness
 * <li>Data[0]: Value of RimGetNativeTicks() at the time of the backlight state
 *              change
 * </ul>
 *
 * Note that the new BACKLIGHT_UPDATE message format is backwards compatible
 * with the old BACKLIGHT_UPDATE format, since only the LCD_BACKLIGHT flag
 * was important/useful, which has the same value as the Primary Backlight Domain
 * mask in the newer SubMsg field.  As well, the new Length value that
 * corresponds with the Active Backlight Domain sleep value is identical to
 * the value that would have been received for an LCD backlight on/off using
 * the old 1.0 backlight API.  Finally, the Data[0] field is also the same.
 *
 * Situations where backlights are turned on by the OS:
 * <p> Key press or roller wheel
 * <p> Out of holster
 * <p> Trackball press (but not roll)
 * <p> Flip phone open/close
 *
 * <em>Note</em>:  If a keypress or rollerwheel event causes the
 * backlight to turn on, then the KEY_TURNED_BACKLIGHT_ON flag
 * is set in the corresponding KEY_DOWN message sent to the apps
 * (see keypad.h).
 *
 * <em>Note</em> (for backlight API 1.0 only): If HW_CAP_LIGHT_SENSOR is
 * present on the device, then changes in KEYPAD_BACKLIGHT state do not trigger
 * BACKLIGHT_UPDATE messages (though the KEYPAD_BACKLIGHT mask
 * will be set in a BACKLIGHT_UPDATE message if both the LCD_BACKLIGHT
 * and KEYPAD_BACKLIGHT are turned on or off at the same time).
 */
#define BACKLIGHT_UPDATE            0x0140

/**
 * The maximum amount of failed password attempts has been exceeded
 *
 * The system sends this message to the apps when the amount of failed
 * password attempts exceeds the maximum amount of attempts allowed.
 */
#define FAILED_MAX_PWD_ATTEMPTS     0x0150


/**
 * The worker task should execute the requested task.
 *
 * The apps send this message to the appropriate worker task in order to
 * execute tasks in the background.  DataPtr is is a pointer to a function
 * to be called by the worker task, and the rest is user defined.  See
 * worker_task.h for details.
 */
#define WORKER_TASK_EXECUTE       0x0165
/**
 * The worker task has completed the requested task.
 *
 * The system sends this message to the apps when a worker task completes
 * a task.  See worker_task.h for more details.
 */
#define WORKER_TASK_DONE          0x0166


#undef BUGBUILD_ENUM
//@}

#ifdef __cplusplus
extern "C" {
#endif


  /**
   * Posts a message with the application server.
   *
   * Use this function to post a message to the application server, to then be
   * delivered to other applications. This function returns immediately without
   * waiting for the message to be processed by the other
   * application. Attempting to send messages to tasks that do not exist
   * generates an exception.
   *
   * @param hTask Task handle for the application to receive the message.
   * @param msg Pointer to the structure containing the message particulars.
   *
   * @return True if message successfully posted to app server; otherwise,
   * false.
   */
APIFUNC BOOL RimPostMessage( TASK hTask, const MESSAGE *msg );


  /**
   * Pulls the next message from its message queue.
   *
   * Use this function to obtain the next message for your application from your
   * message queue. If no messages are available to the calling task, the task
   * is suspended until a message becomes available.
   *
   * It is important that a task calls this function to obtain events, as this
   * allows the application server to determine which tasks are currently not
   * using the CPU. The processor is then put into low power mode.
   *
   * @param msg Pointer to the structure to contain the message data.
   *
   * @return Task handle of the task that sent or posted the retrieved message.
   */
APIFUNC TASK RimGetMessage( MESSAGE *msg );


  /**
   * Determines whether any messages are available.
   *
   * Use this function to determine whether there are any messages available for
   * your application. You should call this function to handle keypad or radio
   * messages while performing any long operation.
   *
   * @return True if there is a message waiting on the event queue for the
   * calling task; otherwise, false.
   */
APIFUNC BOOL RimPeekMessage( void );


  /**
   * Yields control to the system, allowing other applications to run.
   *
   * Use this function so your application can yield control allowing other
   * applications to run. You should do this primarily during CPU-intensive
   * operations so that other applications can run.
   *
   * If there are no other applications with messages pending, this function
   * returns immediately; otherwise, this function blocks until the system
   * passes control back to the calling application.
   *
   * @deprecated There is no point in having a task yield in a
   * strict-priority RTOS, so the only true purpose of this function is to
   * keep watchdog timers at bay. This is better accomplished with
   * {@link #RimWatchdogHit}.
   */
APIFUNC void RimTaskYield( void );

  /**
   * Prevents watchdog failures from occurring.
   *
   * Resets any applicable software and hardware watchdog timers to indicate
   * that the system is responsive, avoiding a watchdog interrupt and the
   * resulting catastrophic failure.
   *
   * Call this function periodically during long CPU-intensive operations.
   *
   * Note that this function can be called from any context, even from an ISR
   * such as the timeslice handler; however, it will only reset the watchdog
   * if the current thread (or the interrupted thread, if called from an
   * ISR) is APP_THREAD.
   */
APIFUNC void RimWatchdogHit( void );

  /**
   * Retrieves number of priority messages in the JVM message queue.
   *
   * Priority messages are messages of the following types:
   * {@link #DEVICE_KEYPAD }, {@link #DEVICE_SERIAL },
   * {@link #DEVICE_TRACKBALL}, {@link #DEVICE_USB },
   * {@link #DEVICE_TOUCHSCREEN }, {@link #DEVICE_ACCELEROMETER },
   * {@link #DEVICE_HEADSET } with the {@link MESSAGE#Event Event } field set to
   * {@link #HOOKSWITCH_CLICK } or {@link #HOOKSWITCH_UNCLICK }, and
   * {@link #DEVICE_RADIO } with the {@link MESSAGE#Event Event } field set to
   * {@link #RADIO_CALL_INCOMING }, {@link #RADIO_DC_CALL_CONNECTED },
   * {@link #RADIO_DC_CALL_ALERT }, {@link #RADIO_CALL_DISCONNECTED }, or
   * {@link #RADIO_CALL_FAILED }
   *
   * @return Number of priority messages in the JVM message queue.
   */
APIFUNC DWORD RimPriorityMessageCount( void );

#ifdef __cplusplus
}
#endif

#ifdef _MSC_VER
#pragma pack( pop )
#endif

#endif


/**
 * @file
 *
 * Provides access to the device's event messaging system.
 *
 * Applications receive all external notification through events sent to them by
 * the underlying operating system. Applications, upon having completed
 * processing fo an event, call {@link #RimGetMessage } to receive the next
 * event. If no event is available, the application blocks the send process,
 * allowing other applications to run. If no other applications have events to
 * process, the application puts the CPU in a standby state until the next event
 * (such as the expiration of a timer) initiates the application again.
 *
 * You may call {@link #RimGetMessage } from anywhere within an
 * application. Thus, while a certain application, implementing a certain
 * screen, may call {@link #RimGetMessage } to process messages, another
 * application, in another screen and context, might process those messages
 * differently.
 *
 * Currently, messages can only be sent asynchronously.
 *
 * <h3>
 * Asynchronous send
 * </h3>
 * For asynchronous communications (non-blocking send), applications post
 * messages to another application's message queue by calling
 * {@link #RimPostMessage }. The destination application receives the message
 * after previous events in its message queue are processed.
 *
 * The sending process continues execution immediately after the call to
 * {@link #RimPostMessage }.
 *
 * <h3>
 * Message structure
 * </h3>

 * Any system or application message is encoded in a {@link #MESSAGE }
 * structure. When you send a message, you should set the
 * {@link MESSAGE#Device } and {@link MESSAGE#Event } fields
 * appropriately. However, the system itself does not bother to examine the
 * contents of message objects, so the precise use of the structure's fields is
 * up to you (and the applications intenteded to receive the event messages that
 * you send).
 */
