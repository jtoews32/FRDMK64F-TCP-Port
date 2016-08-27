#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_IP_Private.h"
#include "NetworkBufferManagement.h"
#include "NetworkInterface.h"

#include "ethernetif.h"

#include "fsl_enet_hal.h"
#include "fsl_phy_driver.h"

#include "lwip/udp.h"
#include "lwip/def.h"
#include "lwip/memp.h"
#include "lwip/inet_chksum.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/icmp.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "arch/perf.h"
#include "lwip/dhcp.h"
#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "lwip/udp.h"
#include "lwip/dhcp.h"
#include "netif/etharp.h"
#include "ethernetif.h"

#include "lwip/ip_addr.h"
#include "lwip/udp.h"


#include "lwip/def.h"
#include "lwip/memp.h"
#include "lwip/inet_chksum.h"

#include "lwip/netif.h"
#include "lwip/icmp.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "arch/perf.h"
#include "lwip/dhcp.h"
#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "lwip/udp.h"
#include "lwip/dhcp.h"
#include "netif/etharp.h"
#include "ethernetif.h"

char msg[] = " This is some test payload. ";
/*
sudo ifconfig eth0 192.168.1.222 netmask 255.255.255.0
sudo sysctl -w net.ipv4.ip_forward=1
sudo iptables -A FORWARD --in-interface eth0 -j ACCEPT
sudo iptables --table nat -A POSTROUTING --out-interface wlan0 -j MASQUERADE
 */

int got_udp = 0;
int sent_udp = 0;
int send_test = 0; //toggled by button
struct udp_pcb *pcb;

static void udp_test_recv(void *arg, struct udp_pcb *upcb, struct pbuf *p,
		struct ip_addr *addr, u16_t port) {

	printf("Received UDP Packet on port %d\r\n", port);
	// printf("Received UDP Packet from ip_addr %s\r\n",  inet_ntoa(*(struct in_addr*)&(ip_addr)));
	LWIP_UNUSED_ARG(arg);
	got_udp = 1;
	/* if packet is valid */
	if (p != NULL) {
		//Try to print out what we received
		printf("UDP Packet Received! Payload:\r\n");
		printf("-- %s --\r\n", (char *) (p->payload));
		err_t code = udp_sendto(upcb, p, IP_ADDR_BROADCAST, 5555); //send it back to port 5555
		printf("Echo'd packet, result code is %d\r\n", code);

		/* free the pbuf */
		pbuf_free(p);
		got_udp = 0;
		sent_udp = 0;
	}
}

void transmitToTx() {

	struct pbuf *p;

	err_t res;
	struct netif netif;
	ip_addr_t ipaddr, netmask, gw, out;

	tcpip_init(NULL, NULL);

	IP4_ADDR(&ipaddr, 192, 168, 1, 222);
	IP4_ADDR(&netmask, 255, 255, 255, 0);
	IP4_ADDR(&gw, 192, 168, 1, 255);
	IP4_ADDR(&out, 192,168,1,116);

	netif_add(&netif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, ethernet_input);

	netif_set_default(&netif);
	netif_set_ipaddr(&netif, &ipaddr);
	netif_set_netmask(&netif, &netmask);
	netif_set_gw(&netif, &gw);
	netif_set_up(&netif);	// Send ARP request

	pcb = udp_new();
	if (pcb != NULL) {
		pcb->so_options |= SOF_BROADCAST;
		udp_bind(pcb, IP_ADDR_ANY, 4444);
		udp_recv(pcb, udp_test_recv, NULL);
	}

	p = pbuf_alloc(PBUF_TRANSPORT, sizeof(msg), PBUF_RAM);
	memcpy(p->payload, msg, sizeof(msg));
	res = udp_sendto(pcb, p, IP_ADDR_BROADCAST, 5555); //broadcast test msg
	// res = udp_sendto(pcb, p, &out, 5555); //broadcast test msg
	pbuf_free(p); //De-allocate packet buffer

}

void sendBroadcast() {
	//broadcast something
	struct pbuf *p;
	char msg[] = "This is a test message";
	p = pbuf_alloc(PBUF_TRANSPORT, sizeof(msg), PBUF_RAM);
	memcpy(p->payload, msg, sizeof(msg));
	err_t res = udp_sendto(pcb, p, IP_ADDR_BROADCAST, 5555); //broadcast test msg
	pbuf_free(p); //De-allocate packet buffer
	printf("Sent test message to port 1234, result is %d Waiting...\r\n", res);
}


BaseType_t xNetworkInterfaceInitialise() {
/*
	if( ethernetif_init_port() == ENET_OK) {
		return pdPASS;
	} else {
		return pdFAIL;
	}
*/
	return pdPASS;
}


BaseType_t xApplicationDNSQueryHook( const char *pcName ) {
	BaseType_t xReturn = pdFALSE;
	return xReturn;
}

BaseType_t xNetworkInterfaceOutput(
		NetworkBufferDescriptor_t * const pxNetworkBuffer,
		BaseType_t xReleaseAfterSend )
{
	BaseType_t xReturn = pdFALSE;

	transmitToTx();

	return xReturn;
}


// not needed cause not using buffer 1 probably need to change the constant in the IP file
void vNetworkInterfaceAllocateRAMToBuffers( NetworkBufferDescriptor_t pxNetworkBuffers[ ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS ] )
{
	BaseType_t xReturn = pdFALSE;
	return;
}

BaseType_t xGetPhyLinkStatus( void )
{
	BaseType_t xReturn = pdFALSE;
	return pdTRUE;
}


UBaseType_t uxRand()
{

	return 0;
}

const char *pcApplicationHostnameHook( void )
{
	return NULL;
}


void vApplicationPingReplyHook(ePingReplyStatus_t eStatus, uint16_t usIdentifier)
{
}



void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent )
{

}
