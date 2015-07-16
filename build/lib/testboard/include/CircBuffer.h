/*****************************************************************************
 * * Filename:    CircBuffer.h
 * *
 * * Copyright 2007, Research In Motion Ltd
 * *
 * * Author:      Vic Kulikauskas
 * *
 * * Created:     Feb 2007
 * *
 * * Description: A data type and a few functions that use it
 * *
 * ***************************************************************************/

#ifndef __CIRCBUFFER_H__
#define __CIRCBUFFER_H__

#include "basetype.h"

/**
 * A structure that allows some code to deal with different kinds of buffers
 * in a general way.
 */
typedef struct {
    volatile DWORD *pReadIndex;
    volatile DWORD *pWriteIndex;
    DWORD  SizeMask;
    BYTE  *pBuffer;
} CircularBuffer;


#ifdef __cplusplus
extern  "C" {
#endif // __cplusplus
// Wrapping memcpy - copy any new data from one circular buffer into another.
// Will return the number of bytes still to be written if destination buffer is full, zero if the destination buffer is not full
DWORD CircBufferCopy( const CircularBuffer *dest, const CircularBuffer *src );

// Same as CircBufferCopy, but if the destination buffer fills up, continue to overwrite old data until the copy is finished
void CircBufferOverwrite( const CircularBuffer *dest, const CircularBuffer *src );
#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __CIRCBUFFER_H__ */
