/*****************************************************************************
 * Filename:    utility_message.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 2008
 *
 * Description: Contains the platform software driver framework message
 *              ulitity code.
 *
 *****************************************************************************/

#include "framework_utilities.h" 

static MsgHandlerMap msgMap[PLATFORM_THREAD_TOTAL][PLATFORM_THREAD_MAX_MSG];

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
int PfMsgRegister(DWORD threadId, DWORD msgEvt, MsgCallbackFunc cbFunc)
{
    int i = 0;
    int idx = threadId - PLATFORM_THREAD_ID_DRV;
    while(i <= PLATFORM_THREAD_MAX_MSG)
    {
        if(msgMap[idx][i].EventId == 0)
        {
            msgMap[idx][i].EventId   = msgEvt;
            msgMap[idx][i].cbFunc    = cbFunc;
            
            return 0;
        }
        i++;        
    }
    
    //WARN2N("Platform thread %d: Failed to register message - %d", threadId, msgEvt);    
    return -1;
}

/**
 * Calls explicitly a Nessus message handler specific to a thread.
 *
 * @param threadId - The numeric code associated with the thread.
 * @param msg - A pointer to the data stucture associated with the message.
 *
 * @return none.
 *
 */
void PfMsgHandler(DWORD threadId, RE_MESSAGE * msg)
{
    int i = 0;        
    int idx = threadId - PLATFORM_THREAD_ID_DRV;   
    while(i <= PLATFORM_THREAD_MAX_MSG)
    {
        if(msgMap[idx][i].EventId == msg->Event)
        {
            //PRINT2N("Platform thread %d: Received message - %d", threadId, msg->Event);
            msgMap[idx][i].cbFunc(msg);
            return;
        }
        i++;
    }
    
    //WARN2N("Platform thread %d: Unknown message - %d", threadId, msg->Event);
}
