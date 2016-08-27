#ifndef FREERTOS_STREAM_BUFFER_H
#define	FREERTOS_STREAM_BUFFER_H

#if	defined( __cplusplus )
extern "C" {
#endif

#include "FreeRTOS_IP.h"


typedef struct xSTREAM_BUFFER {
	volatile size_t uxTail;		/* next item to read */
	volatile size_t uxMid;		/* iterator within the valid items */
	volatile size_t uxHead;		/* next position store a new item */
	volatile size_t uxFront;	/* iterator within the free space */
	size_t LENGTH;				/* const value: number of reserved elements */
	uint8_t ucArray[ sizeof( size_t ) ];
} StreamBuffer_t;




static portINLINE void vStreamBufferClear( StreamBuffer_t *pxBuffer )
{
	/* Make the circular buffer empty */
	pxBuffer->uxHead = 0;
	pxBuffer->uxTail = 0;
	pxBuffer->uxFront = 0;
	pxBuffer->uxMid = 0;
}
/*-----------------------------------------------------------*/




static portINLINE size_t uxStreamBufferSpace( const StreamBuffer_t *pxBuffer, size_t uxLower, size_t uxUpper )
{
/* Returns the space between lLower and lUpper, which equals to the distance minus 1 */
size_t uxCount;

	uxCount = pxBuffer->LENGTH + uxUpper - uxLower - 1;
	if( uxCount >= pxBuffer->LENGTH )
	{
		uxCount -= pxBuffer->LENGTH;
	}

	return uxCount;
}
/*-----------------------------------------------------------*/

static portINLINE size_t uxStreamBufferDistance( const StreamBuffer_t *pxBuffer, size_t uxLower, size_t uxUpper )
{
/* Returns the distance between lLower and lUpper */
size_t uxCount;

	uxCount = pxBuffer->LENGTH + uxUpper - uxLower;
	if ( uxCount >= pxBuffer->LENGTH )
	{
		uxCount -= pxBuffer->LENGTH;
	}

	return uxCount;
}
/*-----------------------------------------------------------*/



static portINLINE size_t uxStreamBufferGetSpace( const StreamBuffer_t *pxBuffer )
{
	/* Returns the number of items which can still be added to lHead
	before hitting on lTail */
	return uxStreamBufferSpace( pxBuffer, pxBuffer->uxHead, pxBuffer->uxTail );
}
/*-----------------------------------------------------------*/



static portINLINE size_t uxStreamBufferFrontSpace( const StreamBuffer_t *pxBuffer )
{
	/* Distance between lFront and lTail
	or the number of items which can still be added to lFront,
	before hitting on lTail */
	return uxStreamBufferSpace( pxBuffer, pxBuffer->uxFront, pxBuffer->uxTail );
}
/*-----------------------------------------------------------*/



static portINLINE size_t uxStreamBufferGetSize( const StreamBuffer_t *pxBuffer )
{
	/* Returns the number of items which can be read from lTail
	before reaching lHead */
	return uxStreamBufferDistance( pxBuffer, pxBuffer->uxTail, pxBuffer->uxHead );
}
/*-----------------------------------------------------------*/



static portINLINE size_t uxStreamBufferMidSpace( const StreamBuffer_t *pxBuffer )
{
	/* Returns the distance between lHead and lMid */
	return uxStreamBufferDistance( pxBuffer, pxBuffer->uxMid, pxBuffer->uxHead );
}
/*-----------------------------------------------------------*/



static portINLINE void vStreamBufferMoveMid( StreamBuffer_t *pxBuffer, size_t uxCount )
{
/* Increment lMid, but no further than lHead */
size_t uxSize = uxStreamBufferMidSpace( pxBuffer );

	if( uxCount > uxSize )
	{
		uxCount = uxSize;
	}
	pxBuffer->uxMid += uxCount;
	if( pxBuffer->uxMid >= pxBuffer->LENGTH )
	{
		pxBuffer->uxMid -= pxBuffer->LENGTH;
	}
}
/*-----------------------------------------------------------*/



static portINLINE BaseType_t xStreamBufferIsEmpty( const StreamBuffer_t *pxBuffer )
{
BaseType_t xReturn;

	/* True if no item is available */
	if( pxBuffer->uxHead == pxBuffer->uxTail )
	{
		xReturn = pdTRUE;
	}
	else
	{
		xReturn = pdFALSE;
	}
	return xReturn;
}
/*-----------------------------------------------------------*/



static portINLINE BaseType_t xStreamBufferIsFull( const StreamBuffer_t *pxBuffer )
{
	/* True if the available space equals zero. */
	return uxStreamBufferGetSpace( pxBuffer ) == 0;
}
/*-----------------------------------------------------------*/




static portINLINE BaseType_t xStreamBufferLessThenEqual( const StreamBuffer_t *pxBuffer, size_t uxLeft, size_t uxRight )
{
BaseType_t xReturn;
size_t uxTail = pxBuffer->uxTail;

	/* Returns true if ( uxLeft < uxRight ) */
	if( ( uxLeft < uxTail ) ^ ( uxRight < uxTail ) )
	{
		if( uxRight < uxTail )
		{
			xReturn = pdTRUE;
		}
		else
		{
			xReturn = pdFALSE;
		}
	}
	else
	{
		if( uxLeft <= uxRight )
		{
			xReturn = pdTRUE;
		}
		else
		{
			xReturn = pdFALSE;
		}
	}
	return xReturn;
}
/*-----------------------------------------------------------*/




static portINLINE size_t uxStreamBufferGetPtr( StreamBuffer_t *pxBuffer, uint8_t **ppucData )
{
size_t uxNextTail = pxBuffer->uxTail;
size_t uxSize = uxStreamBufferGetSize( pxBuffer );

	*ppucData = pxBuffer->ucArray + uxNextTail;

	return FreeRTOS_min_uint32( uxSize, pxBuffer->LENGTH - uxNextTail );
}



/*
 * Add bytes to a stream buffer.
 *
 * pxBuffer -	The buffer to which the bytes will be added.
 * lOffset -	If lOffset > 0, data will be written at an offset from lHead
 *				while lHead will not be moved yet.
 * pucData -	A pointer to the data to be added.
 * lCount -		The number of bytes to add.
 */
size_t uxStreamBufferAdd( StreamBuffer_t *pxBuffer, size_t uxOffset, const uint8_t *pucData, size_t uxCount );



/*
 * Read bytes from a stream buffer.
 *
 * pxBuffer -	The buffer from which the bytes will be read.
 * lOffset -	Can be used to read data located at a certain offset from 'lTail'.
 * pucData -	A pointer to the buffer into which data will be read.
 * lMaxCount -	The number of bytes to read.
 * xPeek -		If set to pdTRUE the data will remain in the buffer.
 */
size_t uxStreamBufferGet( StreamBuffer_t *pxBuffer, size_t uxOffset, uint8_t *pucData, size_t uxMaxCount, BaseType_t xPeek );




#if	defined( __cplusplus )
} /* extern "C" */
#endif

#endif	/* !defined( FREERTOS_STREAM_BUFFER_H ) */
