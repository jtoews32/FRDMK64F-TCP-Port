#include "fsl_enet_driver.h"
#include "fsl_clock_manager.h"


#define ENET_INSTANCE      0U

#if (FSL_RTOS_MQX)
/* MQX RTOS require instalation of interrupt handlers. */
void MQX_ENET_Transmit_IRQHandler(void)
{
     ENET_DRV_TxIRQHandler(ENET_INSTANCE);
}

void MQX_ENET_Receive_IRQHandler(void)
{
     ENET_DRV_RxIRQHandler(ENET_INSTANCE);
}

#if FSL_FEATURE_ENET_SUPPORT_PTP
void MQX_ENET_1588_Timer_IRQHandler(void)
{
     ENET_DRV_TsIRQHandler(ENET_INSTANCE);
}
#endif

#else /* FSL_RTOS_MQX */

void ENET_Transmit_IRQHandler(void)
{
     ENET_DRV_TxIRQHandler(ENET_INSTANCE);
}

void ENET_Receive_IRQHandler(void)
{
     ENET_DRV_RxIRQHandler(ENET_INSTANCE);
}


#if FSL_FEATURE_ENET_SUPPORT_PTP
void ENET_1588_Timer_IRQHandler(void)
{
     ENET_DRV_TsIRQHandler(ENET_INSTANCE);
}
#endif
#endif /* FSL_RTOS_MQX */

