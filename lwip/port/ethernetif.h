#ifndef ETHERNETIF_H
#define ETHERNETIF_H

#include "lwip/err.h"
#include "fsl_enet_driver.h"
#include "lwip/netif.h"


#define ENET_RXBD_NUM                 (8)
#define ENET_TXBD_NUM                 (4)
#define ENET_EXTRXBD_NUM              (4)
#define ENET_RXBUFF_SIZE        (kEnetMaxFrameSize)
#define ENET_TXBUFF_SIZE        (kEnetMaxFrameSize)
#define ENET_RXLARGEBUFF_NUM    (4)
#define ENET_RX_BUFFER_ALIGNMENT     (16)  
#define ENET_TX_BUFFER_ALIGNMENT     (16)
#define ENET_BD_ALIGNMENT            (16)
#define ENET_RXBuffSizeAlign(n)      ENET_ALIGN(n, ENET_RX_BUFFER_ALIGNMENT)    
#define ENET_TXBuffSizeAlign(n)      ENET_ALIGN(n, ENET_TX_BUFFER_ALIGNMENT) 

#if defined   (__GNUC__)        /* GNU Compiler */
  #ifndef __ALIGN_END
    #define __ALIGN_END    __attribute__ ((aligned (ENET_BD_ALIGNMENT)))
  #endif 
  #ifndef __ALIGN_BEGIN 
    #define __ALIGN_BEGIN
  #endif 
#else
  #ifndef __ALIGN_END
    #define __ALIGN_END
  #endif 
  #ifndef __ALIGN_BEGIN      
    #if defined   (__CC_ARM)      /* ARM Compiler */
      #define __ALIGN_BEGIN    __align(ENET_BD_ALIGNMENT)  
    #elif defined (__ICCARM__)    /* IAR Compiler */
      #define __ALIGN_BEGIN 
    #endif
  #endif
#endif

 
#if FSL_FEATURE_ENET_SUPPORT_PTP
#define ENET_PTP_TXTS_RING_LEN           (25)
#define ENET_PTP_RXTS_RING_LEN           (25)
#define ENET_PTP_L2RING_LEN            (15)
#define ENET_PTP_SYNC_MSG              (0)
#define ENET_PTP_DELAYREQ_MSG          (1)
#define ENET_PTP_ETHERNET_LAYER2_TEST  (1)
#define ENET_IPV4_VERSION              (4)
#endif
/* MAC address configuration. */
#define configMAC_ADDR0	0x00
#define configMAC_ADDR1	0xCF
#define configMAC_ADDR2	0x52
#define configMAC_ADDR3	0x35
#define configMAC_ADDR4	0x00
#define configMAC_ADDR5	0x01

#define ENET_OK 0U
#define ENET_ERROR 0xffU
#if !ENET_RECEIVE_ALL_INTERRUPT
#define RECV_TASK_STACK_SIZE  (800)
#define ENET_RECV_TASK_PRIO    1
#endif


extern 		enet_dev_if_t enetDevIf[];
err_t 		ethernetif_init(struct netif *netif);
uint32_t 	ethernetif_input(void *enetPtr, enet_mac_packet_buffer_t *packetBuffer);
void 		ENET_receive(task_param_t param);

#endif
