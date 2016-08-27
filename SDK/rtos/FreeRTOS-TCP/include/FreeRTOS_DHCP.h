#ifndef FREERTOS_DHCP_H
#define FREERTOS_DHCP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Application level configuration options. */
#include "FreeRTOSIPConfig.h"
#include "IPTraceMacroDefaults.h"

#if( ipconfigDHCP_USES_USER_HOOK != 0 )
	typedef enum eDHCP_QUESTIONS
	{
		eDHCPOffer,			/* Driver is about to ask for a DHCP offer. */
		eDHCPRequest,		/* Driver is about to request DHCP an IP address. */
	} eDHCPCallbackQuestion_t;

	typedef enum eDHCP_ANSWERS
	{
		eDHCPContinue,			/* Continue the DHCP process */
		eDHCPUseDefaults,		/* Stop DHCP and use the static defaults. */
		eDHCPStopNoChanges,		/* Stop DHCP and continue with current settings. */
	} eDHCPCallbackAnswer_t;
#endif	/* ipconfigDHCP_USES_USER_HOOK */


/*
 * NOT A PUBLIC API FUNCTION.
 */
void vDHCPProcess( BaseType_t xReset );

/* Internal call: returns true if socket is the current DHCP socket */
BaseType_t xIsDHCPSocket( Socket_t xSocket );



#if( ipconfigDHCP_USES_USER_HOOK != 0 )
	/* The following function must be provided by the user.
	The user may decide to continue or stop, after receiving a DHCP offer. */
	BaseType_t xApplicationDHCPUserHook(
			eDHCPCallbackQuestion_t eQuestion,
			uint32_t ulIPAddress,
			uint32_t ulNetMask );

#endif	/* ipconfigDHCP_USES_USER_HOOK */

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif /* FREERTOS_DHCP_H */













