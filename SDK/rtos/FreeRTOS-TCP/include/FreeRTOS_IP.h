#ifndef FREERTOS_IP_H
#define FREERTOS_IP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Application level configuration options. */
#include "FreeRTOSIPConfig.h"
#include "FreeRTOSIPConfigDefaults.h"
#include "IPTraceMacroDefaults.h"

/* Some constants defining the sizes of several parts of a packet */
#define ipSIZE_OF_ETH_HEADER			14
#define ipSIZE_OF_IP_HEADER				20
#define ipSIZE_OF_IGMP_HEADER			8
#define ipSIZE_OF_ICMP_HEADER			8
#define ipSIZE_OF_UDP_HEADER			8
#define ipSIZE_OF_TCP_HEADER			20


/* The number of octets in the MAC and IP addresses respectively. */
#define ipMAC_ADDRESS_LENGTH_BYTES ( 6 )
#define ipIP_ADDRESS_LENGTH_BYTES ( 4 )

/* IP protocol definitions. */
#define ipPROTOCOL_ICMP			( 1 )
#define ipPROTOCOL_IGMP         ( 2 )
#define ipPROTOCOL_TCP			( 6 )
#define ipPROTOCOL_UDP			( 17 )

/* Dimensions the buffers that are filled by received Ethernet frames. */
#define ipSIZE_OF_ETH_CRC_BYTES					( 4UL )
#define ipSIZE_OF_ETH_OPTIONAL_802_1Q_TAG_BYTES	( 4UL )
#define ipTOTAL_ETHERNET_FRAME_SIZE				( ( ( uint32_t ) ipconfigNETWORK_MTU ) + ( ( uint32_t ) ipSIZE_OF_ETH_HEADER ) + ipSIZE_OF_ETH_CRC_BYTES + ipSIZE_OF_ETH_OPTIONAL_802_1Q_TAG_BYTES )


/*

Space left at the beginning of a network buffer storage area to store a
pointer back to the network buffer.  Should be a multiple of 8 to ensure 8 byte
alignment is maintained on architectures that require it.

In order to get a 32-bit alignment of network packets, an offset of 2 bytes
would be desirable, as defined by ipconfigPACKET_FILLER_SIZE.  So the malloc'd
buffer will have the following contents:
	uint32_t pointer;	// word-aligned
	uchar_8 filler[6];
	<< ETH-header >>	// half-word-aligned
	uchar_8 dest[6];    // start of pucEthernetBuffer
	uchar_8 dest[6];
	uchar16_t type;
	<< IP-header >>		// word-aligned
	uint8_t ucVersionHeaderLength;
	etc

*/
#define ipBUFFER_PADDING		( 8 + ipconfigPACKET_FILLER_SIZE )

/*
	The structure used to store buffers and pass them around the network stack.
	Buffers can be in use by the stack, in use by the network interface hardware
	driver, or free (not in use).
*/

//struct pbuf {
//  /** next pbuf in singly linked pbuf chain */
//  struct pbuf *next;
//
//  /** pointer to the actual data in the buffer */
//  void *payload;
//
//  /**
//   * total length of this buffer and all next buffers in chain
//   * belonging to the same packet.
//   *
//   * For non-queue packet chains this is the invariant:
//   * p->tot_len == p->len + (p->next? p->next->tot_len: 0)
//   */
//  u16_t tot_len;
//
//  /** length of this buffer */
//  u16_t len;
//
//  /** pbuf_type as u8_t instead of enum to save space */
//  u8_t /*pbuf_type*/ type;
//
//  /** misc flags */
//  u8_t flags;
//
//  /**
//   * the reference count always equals the number of pointers
//   * that refer to this pbuf. This can be pointers from an application,
//   * the stack itself, or pbuf->next pointers from a chain.
//   */
//  u16_t ref;
//};


typedef struct xNETWORK_BUFFER
{
	/* Used to reference the buffer from the free buffer list or a socket. */
	ListItem_t xBufferListItem;

	/* Source or destination IP address, depending on usage scenario. */
	uint32_t ulIPAddress;

	/* Pointer to the start of the Ethernet frame. */
	uint8_t *pucEthernetBuffer;

	/* Starts by holding the total Ethernet frame length, then the UDP/TCP payload length. */
	size_t xDataLength;

	/* Source or destination port, depending on usage scenario. */
	uint16_t usPort;

	/* The port to which a transmitting socket is bound. */
	uint16_t usBoundPort;

	/* Possible optimisation for expert users - requires network driver support. */
	#if( ipconfigUSE_LINKED_RX_MESSAGES != 0 )
		struct xNETWORK_BUFFER *pxNextBuffer;
	#endif

} NetworkBufferDescriptor_t;



#include "pack_struct_start.h"
struct xMAC_ADDRESS
{
	uint8_t ucBytes[ ipMAC_ADDRESS_LENGTH_BYTES ];
}
#include "pack_struct_end.h"


typedef struct xMAC_ADDRESS MACAddress_t;

typedef enum eNETWORK_EVENTS
{
	eNetworkUp,		/* The network is configured. */
	eNetworkDown	/* The network connection has been lost. */
} eIPCallbackEvent_t;


typedef enum ePING_REPLY_STATUS
{
	eSuccess = 0,		/* A correct reply has been received for an outgoing ping. */
	eInvalidChecksum,	/* A reply was received for an outgoing ping but the checksum of the reply was incorrect. */
	eInvalidData		/* A reply was received to an outgoing ping but the payload of the reply was not correct. */
} ePingReplyStatus_t;


/* Endian related definitions. */
#if( ipconfigBYTE_ORDER == pdFREERTOS_LITTLE_ENDIAN )

	/* FreeRTOS_htons / FreeRTOS_htonl: some platforms might have built-in versions
	using a single instruction so allow these versions to be overridden. */
	#ifndef FreeRTOS_htons
		#define FreeRTOS_htons( usIn ) ( (uint16_t) ( ( ( usIn ) << 8U ) | ( ( usIn ) >> 8U ) ) )
	#endif

	#ifndef	FreeRTOS_htonl
		#define FreeRTOS_htonl( ulIn ) 											\
			(																	\
				( uint32_t ) 													\
				( 																\
					( ( ( ( uint32_t ) ( ulIn ) )                ) << 24  ) | 	\
					( ( ( ( uint32_t ) ( ulIn ) ) & 0x0000ff00UL ) <<  8  ) | 	\
					( ( ( ( uint32_t ) ( ulIn ) ) & 0x00ff0000UL ) >>  8  ) | 	\
					( ( ( ( uint32_t ) ( ulIn ) )                ) >> 24  )  	\
				) 																\
			)
	#endif

#else /* ipconfigBYTE_ORDER */

	#define FreeRTOS_htons( x ) ( ( uint16_t ) ( x ) )
	#define FreeRTOS_htonl( x ) ( ( uint32_t ) ( x ) )

#endif /* ipconfigBYTE_ORDER == pdFREERTOS_LITTLE_ENDIAN */

#define FreeRTOS_ntohs( x ) FreeRTOS_htons( x )
#define FreeRTOS_ntohl( x ) FreeRTOS_htonl( x )

#if( ipconfigHAS_INLINE_FUNCTIONS == 1 )

	static portINLINE int32_t  FreeRTOS_max_int32  (int32_t  a, int32_t  b) { return a >= b ? a : b; }
	static portINLINE uint32_t FreeRTOS_max_uint32 (uint32_t a, uint32_t b) { return a >= b ? a : b; }
	static portINLINE int32_t  FreeRTOS_min_int32  (int32_t  a, int32_t  b) { return a <= b ? a : b; }
	static portINLINE uint32_t FreeRTOS_min_uint32 (uint32_t a, uint32_t b) { return a <= b ? a : b; }
	static portINLINE uint32_t FreeRTOS_round_up   (uint32_t a, uint32_t d) { return d * ( ( a + d - 1 ) / d ); }
	static portINLINE uint32_t FreeRTOS_round_down (uint32_t a, uint32_t d) { return d * ( a / d ); }

#else

	#define FreeRTOS_max_int32(a,b)  ( ( ( int32_t  ) ( a ) ) >= ( ( int32_t  ) ( b ) ) ? ( ( int32_t  ) ( a ) ) : ( ( int32_t  ) ( b ) ) )
	#define FreeRTOS_max_uint32(a,b) ( ( ( uint32_t ) ( a ) ) >= ( ( uint32_t ) ( b ) ) ? ( ( uint32_t ) ( a ) ) : ( ( uint32_t ) ( b ) ) )

	#define FreeRTOS_min_int32(a,b)  ( ( ( int32_t  ) a ) <= ( ( int32_t  ) b ) ? ( ( int32_t  ) a ) : ( ( int32_t  ) b ) )
	#define FreeRTOS_min_uint32(a,b) ( ( ( uint32_t ) a ) <= ( ( uint32_t ) b ) ? ( ( uint32_t ) a ) : ( ( uint32_t ) b ) )

	/*  Round-up: a = d * ( ( a + d - 1 ) / d ) */
	#define FreeRTOS_round_up(a,d)   ( ( ( uint32_t ) ( d ) ) * ( ( ( ( uint32_t ) ( a ) ) + ( ( uint32_t ) ( d ) ) - 1UL ) / ( ( uint32_t ) ( d ) ) ) )
	#define FreeRTOS_round_down(a,d) ( ( ( uint32_t ) ( d ) ) * ( ( ( uint32_t ) ( a ) ) / ( ( uint32_t ) ( d ) ) ) )

	#define FreeRTOS_ms_to_tick(ms)  ( ( ms * configTICK_RATE_HZ + 500 ) / 1000 )

#endif /* ipconfigHAS_INLINE_FUNCTIONS */

/*
 * FULL, UP-TO-DATE AND MAINTAINED REFERENCE DOCUMENTATION FOR ALL THESE
 * FUNCTIONS IS AVAILABLE ON THE FOLLOWING URL:
 * http://www.FreeRTOS.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/FreeRTOS_TCP_API_Functions.html
 */
BaseType_t FreeRTOS_IPInit( const uint8_t ucIPAddress[ ipIP_ADDRESS_LENGTH_BYTES ],
	const uint8_t ucNetMask[ ipIP_ADDRESS_LENGTH_BYTES ],
	const uint8_t ucGatewayAddress[ ipIP_ADDRESS_LENGTH_BYTES ],
	const uint8_t ucDNSServerAddress[ ipIP_ADDRESS_LENGTH_BYTES ],
	const uint8_t ucMACAddress[ ipMAC_ADDRESS_LENGTH_BYTES ] );


void * FreeRTOS_GetUDPPayloadBuffer( size_t xRequestedSizeBytes, TickType_t xBlockTimeTicks );

void FreeRTOS_GetAddressConfiguration( uint32_t *pulIPAddress, uint32_t *pulNetMask, uint32_t *pulGatewayAddress, uint32_t *pulDNSServerAddress );

void FreeRTOS_SetAddressConfiguration( const uint32_t *pulIPAddress, const uint32_t *pulNetMask, const uint32_t *pulGatewayAddress, const uint32_t *pulDNSServerAddress );

BaseType_t FreeRTOS_SendPingRequest( uint32_t ulIPAddress, size_t xNumberOfBytesToSend, TickType_t xBlockTimeTicks );

void FreeRTOS_ReleaseUDPPayloadBuffer( void *pvBuffer );

const uint8_t * FreeRTOS_GetMACAddress( void );

void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent );

void vApplicationPingReplyHook( ePingReplyStatus_t eStatus, uint16_t usIdentifier );

const uint8_t * FreeRTOS_GetMACAddress( void );

uint32_t FreeRTOS_GetIPAddress( void );

void FreeRTOS_SetIPAddress( uint32_t ulIPAddress );

void FreeRTOS_SetNetmask( uint32_t ulNetmask );

void FreeRTOS_SetGatewayAddress( uint32_t ulGatewayAddress );

uint32_t FreeRTOS_GetGatewayAddress( void );

uint32_t FreeRTOS_GetDNSServerAddress( void );

uint32_t FreeRTOS_GetNetmask( void );

void FreeRTOS_OutputARPRequest( uint32_t ulIPAddress );

BaseType_t FreeRTOS_IsNetworkUp( void );



#if( ipconfigCHECK_IP_QUEUE_SPACE != 0 )
	UBaseType_t uxGetMinimumIPQueueSpace( void );
#endif

/*
 * Defined in FreeRTOS_Sockets.c
 * //_RB_ Don't think this comment is correct.  If this is for internal use only it should appear after all the public API functions and not start with FreeRTOS_.
 * Socket has had activity, reset the timer so it will not be closed
 * because of inactivity
 */
const char *FreeRTOS_GetTCPStateName( UBaseType_t ulState);

/* _HT_ Temporary: show all valid ARP entries
 */
void FreeRTOS_PrintARPCache( void );

void FreeRTOS_ClearARP( void );

#if( ipconfigDHCP_REGISTER_HOSTNAME == 1 )

	/* DHCP has an option for clients to register their hostname.  It doesn't
	have much use, except that a device can be found in a router along with its
	name. If this option is used the callback below must be provided by the
	application	writer to return a const string, denoting the device's name. */
	const char *pcApplicationHostnameHook( void );

#endif /* ipconfigDHCP_REGISTER_HOSTNAME */


/*
	For backward compatibility define old structure names to the newer equivalent
	structure name.
*/

#ifndef ipconfigENABLE_BACKWARD_COMPATIBILITY
	#define ipconfigENABLE_BACKWARD_COMPATIBILITY	1
#endif

#if( ipconfigENABLE_BACKWARD_COMPATIBILITY == 1 )
	#define xIPStackEvent_t 			IPStackEvent_t
	#define xNetworkBufferDescriptor_t 	NetworkBufferDescriptor_t
	#define xMACAddress_t 				MACAddress_t
	#define xWinProperties_t 			WinProperties_t
	#define xSocket_t 					Socket_t
	#define xSocketSet_t 				SocketSet_t
#endif /* ipconfigENABLE_BACKWARD_COMPATIBILITY */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FREERTOS_IP_H */
