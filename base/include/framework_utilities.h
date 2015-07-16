/*****************************************************************************
 * Filename:    framework_utilities.h
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 2008
 *
 * Description: Contains definitions of the platform software driver framework
 *              utilities module's data structures, enumerated types and macros.
 *
 *****************************************************************************/

#ifndef __FRAMEWORK_UTILITIES_H__
#define __FRAMEWORK_UTILITIES_H__

#include "rei.h" 

typedef void (*PfCmdFunction)( char*, DWORD, DWORD);

/**
 * Registers a BugDisp command so that it can be recognized and handled by
 * the driver.
 *
 * @param driver / app  - The name string of the driver or the application.
 * @param help          - The help string associated with the BugDisp command.
 * @param PfCmdFunction - A pointer to the callback function associated with
 *                        the BugDisp command. 
 *
 * @return void.
 *
 */
void PfCmdRegister(const char *driver, const char *help, PfCmdFunction func);
void VfCmdRegister(const char *app, const char *help, PfCmdFunction func);

/**
 * Registers a Nessus message to a thread. The callback function will be called
 * once a message is received by the thread.
 *
 * @param threadId - The numeric code associated with the thread.
 * @param msgEvt - The event code associated with the thread's message.
 * @param MsgCallbackFunc - The callback function associated with the thread. 
 *
 * @return none.
 *
 */
int  PfMsgRegister(DWORD threadId, DWORD msgEvt, MsgCallbackFunc cbFunc);

/**
 * Calls explicitly a Nessus message handler specific to a thread.
 *
 * @param threadId - The numeric code associated with the thread.
 * @param msg - A pointer to the data stucture associated with the message.
 *
 * @return none.
 *
 */
void PfMsgHandler(DWORD threadId, RE_MESSAGE * msg);

#endif // __FRAMEWOK_UTILITIES_H__
