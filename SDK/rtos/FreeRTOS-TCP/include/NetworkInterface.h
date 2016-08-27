#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include "FreeRTOS_IP.h"

#ifdef __cplusplus
extern "C" {
#endif

BaseType_t xNetworkInterfaceInitialise( void );

BaseType_t xNetworkInterfaceOutput( NetworkBufferDescriptor_t * const pxNetworkBuffer, BaseType_t xReleaseAfterSend );

void vNetworkInterfaceAllocateRAMToBuffers( NetworkBufferDescriptor_t pxNetworkBuffers[ ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS ] );

BaseType_t xGetPhyLinkStatus( void );

UBaseType_t uxRand();

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* NETWORK_INTERFACE_H */

