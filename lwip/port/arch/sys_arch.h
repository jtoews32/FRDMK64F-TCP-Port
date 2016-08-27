#ifndef __SYS_C64_H__
#define __SYS_C64_H__

#include "fsl_os_abstraction.h"
#include "fsl_debug_console.h"
#include <stdio.h>

#if !NO_SYS
typedef struct mboxHandler{
   msg_queue_handler_t queueHandler;
   semaphore_t _semSync ;
}sys_mbox_t ;

typedef semaphore_t sys_sem_t;
typedef task_handler_t sys_thread_t;
typedef mutex_t sys_mutex_t ;
/*FSL:workaround from lwIP port (1.1.1 to 1.3.1)*/
#define sys_arch_mbox_tryfetch(mbox,msg) \
    sys_arch_mbox_fetch(mbox,msg,1)
#endif


void sys_assert( const char *const msg );
void time_init(void);

#endif /* __SYS_C64_H__ */
