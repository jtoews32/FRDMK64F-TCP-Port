#include "fsl_phy_driver.h"
#include "fsl_enet_hal.h"

uint16_t oldPhyStatus, newPhyStatus;
extern ENET_Type * const g_enetBase[];





 /*FUNCTION****************************************************************
 * This interface writes data over the SMI to the specified PHY register.
 * This function is called by all PHY interfaces.
 *END*********************************************************************/

enet_status_t PHY_DRV_Write(
		uint32_t instance,
		uint32_t phyAddr,
		uint32_t phyReg,
		uint32_t data)
{
    uint32_t counter = kEnetMaxTimeout;
    ENET_Type * base = g_enetBase[instance];
    uint64_t mask = 0;
    enet_cur_status_t curStatus;
    mask |= ENET_GET_SMI_CONFIG_MASK;
    ENET_HAL_GetStatus(base, mask, &curStatus);

    /* Check if the mii is enabled*/
    if (!(curStatus.statusFlags & ENET_SMI_CONFIG_FLAG))
    {
        return kStatus_ENET_SMIUninitialized;
    }

    /* Clear the SMI interrupt event*/
    ENET_HAL_ClearIntStatusFlag(base, kEnetMiiInterrupt);

    /* Set write command*/
    ENET_HAL_SetSMIWrite(base, phyAddr, phyReg, kEnetWriteValidFrame, data);

    /* Wait for MII complete*/
    for(counter = kEnetMaxTimeout; counter > 0; counter --)
    {
        if(!ENET_HAL_GetIntStatusFlag(base, kEnetMiiInterrupt))
        {
            break;
        }
    }

    /* Check for timeout*/
    if (!counter)
    {
        return kStatus_ENET_SMIVisitTimeout;
    }

    /* Clear MII intrrupt event*/
    ENET_HAL_ClearIntStatusFlag(base, kEnetMiiInterrupt);
	
    return kStatus_ENET_Success;
}





/*FUNCTION****************************************************************
 * This interface read data over the SMI from the specified PHY register,
 * This function is called by all PHY interfaces.
 *END*********************************************************************/

enet_status_t PHY_DRV_Read(
		uint32_t instance,
		uint32_t phyAddr,
		uint32_t phyReg,
		uint32_t *dataPtr)
{
    ENET_Type * base;
    uint32_t  counter = kEnetMaxTimeout;
    uint64_t mask = 0;
    enet_cur_status_t curStatus;
	
    /* Check the input parameters*/
    if (!dataPtr)
    {
        return kStatus_ENET_InvalidInput;
    }

    base = g_enetBase[instance];
    mask |= ENET_GET_SMI_CONFIG_MASK;
    ENET_HAL_GetStatus(base, mask, &curStatus);


    /* Check if the mii is enabled*/
    if (!(curStatus.statusFlags & ENET_SMI_CONFIG_FLAG))
    {
        return kStatus_ENET_SMIUninitialized;
    }

    /* Clear the MII interrupt event*/
    ENET_HAL_ClearIntStatusFlag(base, kEnetMiiInterrupt);


    /* Read command operation*/
    ENET_HAL_SetSMIRead(base, phyAddr, phyReg, kEnetReadValidFrame);


    /* Wait for MII complete*/
    for(counter = kEnetMaxTimeout; counter > 0; counter --)
    {
        if(ENET_HAL_GetIntStatusFlag(base, kEnetMiiInterrupt))
        {
            break;
        }
    }
    
    /* Check for timeout*/
    if (!counter)
    {
        return kStatus_ENET_SMIVisitTimeout;
    }

    /* Get data from mii register*/
    *dataPtr = ENET_HAL_GetSMIData(base);


    /* Clear MII interrupt event*/
    ENET_HAL_ClearIntStatusFlag(base, kEnetMiiInterrupt);
	

    return kStatus_ENET_Success;
}




/*FUNCTION****************************************************************
 * This interface provides initialize functions for PHY, This is called by enet  
 * initialize function. PHY is usually default auto-negotiation. so there is no
 * need to do the intialize about this. we just need to check the loop mode.
 *END*********************************************************************/

enet_status_t PHY_DRV_Init(
		uint32_t instance,
		uint32_t phyAddr,
		bool isLoopEnabled)
{

    uint32_t data, dataStatus;
    uint32_t counter = kEnetMaxTimeout;
    enet_status_t result;
   

    /* Reset Phy*/
    result = PHY_DRV_Write(instance, phyAddr, kEnetPhyCR, kEnetPhyReset);

    if(result != kStatus_ENET_Success)
    {
        return result;
    }
    else
    {
        do {
            counter --;
    		/* Wait for complete*/
            result = PHY_DRV_Read(instance, phyAddr, kEnetPhyCR, &data);
            if(result != kStatus_ENET_Success)
            {
                return result;
            }
        } while((data & kEnetPhyReset) && (counter > 0));

        /* Check for timeout */
        if (!counter)
        {
            return kStatus_ENET_SMIVisitTimeout;
        }
    }
	
    result = PHY_DRV_Read(instance, phyAddr, kEnetPhySR, &dataStatus);

    if(result != kStatus_ENET_Success)
    {
        return result;
    }


    if (((dataStatus & kEnetPhyAutoNegAble) != 0) && ((dataStatus & kEnetPhyAutoNegComplete) == 0))		
    {
        /* Set Autonegotiation*/
       if(PHY_DRV_Write(instance, phyAddr, kEnetPhyCR, data | kEnetPhyAutoNeg)== kStatus_ENET_Success)
       {
    	   for (counter = 0; counter < kEnetMaxTimeout; counter++)
    	   {
    		   if (PHY_DRV_Read(instance, phyAddr, kEnetPhySR, &dataStatus)== kStatus_ENET_Success)
    		   {
    			   if ((dataStatus & kEnetPhyAutoNegComplete) != 0)
    			   {
    				   break;
    			   }
    		   }
    	   }
       }
    }

    if (isLoopEnabled)
    {
        /* First read the current status in control register*/ 
        if (PHY_DRV_Read(instance, phyAddr, kEnetPhyCR, &data) == kStatus_ENET_Success)
        {
            result = PHY_DRV_Write(instance, phyAddr,kEnetPhyCR,(data|kEnetPhyLoop));
            return result;
        }		
    }

    return result;
}



/*FUNCTION****************************************************************
 * This function provides a interface to get phy address using phy address auto 
 * discovering, this interface is used when the phy address is unknown.
 *END*********************************************************************/

enet_status_t PHY_DRV_Autodiscover(
		uint32_t instance,
		uint32_t *phyAddr)
{
    uint32_t addrIdx,data;
    enet_status_t result = kStatus_ENET_PHYAutoDiscoverFail;

    /* Check input param*/
    if(!phyAddr)
    {
        return kStatus_ENET_InvalidInput;
    }
    
    for (addrIdx = 0; addrIdx < kEnetPhyRegAll; addrIdx++)
    {
        result = PHY_DRV_Read(instance, addrIdx, kEnetPhyId1, &data);
        if ((result == kStatus_ENET_Success) && (data != 0) && (data != 0xffff) )
        {
            *phyAddr = addrIdx;
            return kStatus_ENET_Success;
        }
    }
    return result;
}




/*FUNCTION****************************************************************
 * This function provides a interface to get link speed.
 *END*********************************************************************/

enet_status_t PHY_DRV_GetLinkSpeed(
		uint32_t instance,
		uint32_t phyAddr,
		enet_phy_speed_t *speed)
{
    enet_status_t result = kStatus_ENET_Success;
    uint32_t data;
	
    /* Check input parameters*/
    if (!speed)
    {
        return kStatus_ENET_InvalidInput;
    }

    result = PHY_DRV_Read(instance, phyAddr, kEnetPhyCt1,&data);
    if (result == kStatus_ENET_Success)
    {
        data &= kEnetPhySpeedDulpexMask; 
        if ((kEnetPhy100HalfDuplex == data) || (kEnetPhy100FullDuplex == data))
        {
            *speed = kEnetSpeed100M;
        }
        else
        {
            *speed = kEnetSpeed10M;
        }
    }

    return result;
}




/*FUNCTION****************************************************************
 * This function provides a interface to get link status to see if the link 
 * status is on or off.
 *END*********************************************************************/

enet_status_t PHY_DRV_GetLinkStatus(
		uint32_t instance,
		uint32_t phyAddr,
		bool *status)
{
    enet_status_t result = kStatus_ENET_Success;
    uint32_t data;
	
    /* Check input parameters*/
    if (!status)
    {
        return kStatus_ENET_InvalidInput;
    }

    result = PHY_DRV_Read(instance, phyAddr, kEnetPhyCR, &data);
    if ((result == kStatus_ENET_Success) && (!(data & kEnetPhyReset)))
    {
        data = 0;
        result = PHY_DRV_Read(instance, phyAddr, kEnetPhySR, &data);
        if (result == kStatus_ENET_Success)
        {
            if (!(kEnetPhyLinkStatus & data))
            {
                *status = false;
            }
            else
            {
                *status = true;
            }
        }
    }

    return result;     
}



/*FUNCTION****************************************************************
 * This function provides a interface to get link duplex to see if the link 
 * duplex is full or half.
 *END*********************************************************************/

enet_status_t PHY_DRV_GetLinkDuplex(
		uint32_t instance,
		uint32_t phyAddr,
		enet_phy_duplex_t *status)
{
    enet_status_t result = kStatus_ENET_Success;
    uint32_t data;
	
    /* Check input parameters*/
    if (!status)
    {
        return kStatus_ENET_InvalidInput;
    }

    result = PHY_DRV_Read(instance, phyAddr,kEnetPhyCt1,&data);
    if (result == kStatus_ENET_Success)
    {
        data &= kEnetPhySpeedDulpexMask; 
        if ((kEnetPhy10FullDuplex == data) || (kEnetPhy100FullDuplex == data))
        {
            *status = kEnetFullDuplex;
        }
        else
        {
            *status = kEnetHalfDuplex;
        }
    }

    return result;
}



/*FUNCTION****************************************************************
 * This function provides a interface to moniter the change of the link speed.
 *END*********************************************************************/

bool PHY_DRV_UpdateSpeed(uint32_t instance, uint32_t phyAddr, enet_phy_speed_t *status)
{
    bool link;
	
    if(PHY_DRV_GetLinkSpeed(instance, phyAddr, status)== kStatus_ENET_Success)
    {
        if(status)
        {
            newPhyStatus |= kPhyLinkSpeed;
        }
        else
        {
            newPhyStatus &= ~kPhyLinkSpeed;
        }	
    }
    if(PHY_DRV_GetLinkStatus(instance, phyAddr, &link)== kStatus_ENET_Success)
    {
        if(link)
        {
            newPhyStatus |= kPhyLinkon;
        }
        else
        {
            newPhyStatus &= ~kPhyLinkon;
        }
    }

    if(newPhyStatus != oldPhyStatus)
    {
        oldPhyStatus = newPhyStatus;
    
        if(newPhyStatus & kPhyLinkon)
        {
            return true;
        }
    	else 
        {
            return false;
        }
    }
    return false;
}

