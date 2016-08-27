/* ###################################################################
**     Filename    : Events.h
**     Project     : FRDMK64F-TCP-Port
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-10-18, 16:12, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "fsl_device_registers.h"
#include "clockMan1.h"
#include "pin_init.h"
#include "osa1.h"
#include "free_rtos.h"
#include "MainTask.h"
#include "eNet1.h"
// #include "FRTOS1.h"
#include "UTIL1.h"
#include "KSDK1.h"
#include "CS1.h"

#ifdef __cplusplus
extern "C" {
#endif 


void FRTOS1_vApplicationStackOverflowHook(xTaskHandle pxTask, char *pcTaskName);
/*
** ===================================================================
**     Event       :  FRTOS1_vApplicationStackOverflowHook (module Events)
**
**     Component   :  FRTOS1 [FreeRTOS]
**     Description :
**         if enabled, this hook will be called in case of a stack
**         overflow.
**     Parameters  :
**         NAME            - DESCRIPTION
**         pxTask          - Task handle
**       * pcTaskName      - Pointer to task name
**     Returns     : Nothing
** ===================================================================
*/

void FRTOS1_vApplicationTickHook(void);
/*
** ===================================================================
**     Event       :  FRTOS1_vApplicationTickHook (module Events)
**
**     Component   :  FRTOS1 [FreeRTOS]
**     Description :
**         If enabled, this hook will be called by the RTOS for every
**         tick increment.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void FRTOS1_vApplicationIdleHook(void);
/*
** ===================================================================
**     Event       :  FRTOS1_vApplicationIdleHook (module Events)
**
**     Component   :  FRTOS1 [FreeRTOS]
**     Description :
**         If enabled, this hook will be called when the RTOS is idle.
**         This might be a good place to go into low power mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void FRTOS1_vApplicationMallocFailedHook(void);
/*
** ===================================================================
**     Event       :  FRTOS1_vApplicationMallocFailedHook (module Events)
**
**     Component   :  FRTOS1 [FreeRTOS]
**     Description :
**         If enabled, the RTOS will call this hook in case memory
**         allocation failed.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void RTOSTRC1_OnTraceWrap(void);
/*
** ===================================================================
**     Event       :  RTOSTRC1_OnTraceWrap (module Events)
**
**     Component   :  RTOSTRC1 [PercepioTrace]
**     Description :
**         Called for trace ring buffer wrap around. This gives the
**         application a chance to dump the trace buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Callback    : free_rtos_vApplicationIdleHook
**     Description : This callback occurs if the RTOS is idle. This
**     might be a good place to go into low power mode.
**     Parameters  : None
**     Returns : Nothing
** ===================================================================
*/
void free_rtos_vApplicationIdleHook(void);

/*
** ===================================================================
**     Callback    : free_rtos_vApplicationTickHook
**     Description : This callback will be called by the RTOS for every
**     tick increment.
**     Parameters  : None
**     Returns : Nothing
** ===================================================================
*/
void free_rtos_vApplicationTickHook(void);

/*
** ===================================================================
**     Callback    : free_rtos_vApplicationStackOverflowHook
**     Description : This callback occurs if a stack overflow is
**     detected during the context switch.
**     Parameters  :
**       pxTask - Task handle.
**       pcTaskName - A pointer to task name.
**     Returns : Nothing
** ===================================================================
*/
void free_rtos_vApplicationStackOverflowHook(xTaskHandle pxTask, char *pcTaskName);

/*
** ===================================================================
**     Callback    : free_rtos_vApplicationMallocFailedHook
**     Description : This callback will be call this hook in case
**     memory allocation failed.
**     Parameters  : None
**     Returns : Nothing
** ===================================================================
*/
void free_rtos_vApplicationMallocFailedHook(void);

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
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
