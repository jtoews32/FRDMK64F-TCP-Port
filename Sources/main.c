/* ###################################################################
**     Filename    : main.c
**     Project     : FRDMK64F-TCP-Port
**     Processor   : MK64FN1M0VLL12
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-10-18, 16:12, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief`
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "clockMan1.h"
#include "pin_init.h"
#include "osa1.h"
#include "free_rtos.h"
#include "MainTask.h"
#include "eNet1.h"

#include "UTIL1.h"
#include "KSDK1.h"
#include "CS1.h"
#if CPU_INIT_CONFIG
  	  #include "Init_Config.h"
#endif
/* User includes (#include below this line is not maintained by Processor Expert) */
#define DHCP_TIMEOUT  10

#include <string.h>
#include <stdio.h>

#include "NetworkInterface.h"
#include "NetworkBufferManagement.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_UDP_IP.h"
#include "FreeRTOS_ARP.h"


#define ipconfigENABLE_BACKWARD_COMPATIBILITY 1

static const uint8_t ucIPAddress[4] = { 192, 168, 1, 200 };
static const uint8_t ucNetMask[4] = { 255, 255, 255, 255 };
static const uint8_t ucGatewayAddress[4] = { 192, 168, 1, 1 };
static const uint8_t ucDNSServerAddress[4] = { 208, 67, 222, 222 };
static uint8_t ucMACAddress[6] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };

static void vUDPSendTask( void *pvParameters )
{
	  Socket_t xSocket;
	  struct freertos_sockaddr xDestinationAddress;
	  uint8_t cString[ 50 ];
	  uint32_t ulCount = 0UL;
	  const TickType_t x1000ms = 1000UL / portTICK_PERIOD_MS;

	  xDestinationAddress.sin_addr = FreeRTOS_inet_addr( "192.168.1.116" );
	  xDestinationAddress.sin_port = FreeRTOS_htons( 9876 );

	  long int pxMACAddress = FreeRTOS_htons(0xd8cb8a7e073d);
	  uint32_t ulIPAddress = FreeRTOS_inet_addr( "192.168.1.116" );

	  vARPRefreshCacheEntry(&pxMACAddress, ulIPAddress);

	  xSocket = FreeRTOS_socket( FREERTOS_AF_INET, FREERTOS_SOCK_DGRAM, FREERTOS_IPPROTO_UDP );

	  configASSERT( xSocket != FREERTOS_INVALID_SOCKET );

	  for( ;; )
	  {
		  /* Create the string that is sent. */
		  sprintf( cString, " This is some test payload. ", ulCount );

		  FreeRTOS_sendto(xSocket,cString,strlen( cString ), 0, &xDestinationAddress, sizeof( xDestinationAddress ) );

		  ulCount++;

		  vTaskDelay( x1000ms );
	  }
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  MPU_BWR_CESR_VLD(MPU, 0);

  FreeRTOS_IPInit(ucIPAddress, ucNetMask,
                   ucGatewayAddress, ucDNSServerAddress,ucMACAddress );

  TaskHandle_t xHandle = NULL;

  xTaskCreate( vUDPSendTask, "NAME",  200 , NULL, 3, &xHandle );
 //  xTaskCreate( vSendDHCPDiscoverTask, "NAME",  200 , NULL, 3, &xDiscoverHandle );
  vTaskStartScheduler();

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
