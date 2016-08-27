#ifndef __CC_H__
#define __CC_H__


//#define LWIP_PROVIDE_ERRNO 1

#include "sys_arch.h"//FSL

#undef LWIP_NOASSERT//FSL

#define BYTE_ORDER LITTLE_ENDIAN

// Typedefs for the types used by lwip

typedef unsigned   char    u8_t;
typedef signed     char    s8_t;
typedef unsigned   short   u16_t;
typedef signed     short   s16_t;
typedef unsigned   int    u32_t;
typedef signed     int    s32_t;
typedef u32_t mem_ptr_t;
typedef int sys_prot_t;

// Compiler hints for packing lwip's structures
//FSL: very important at high optimization level
/* */


#if __GNUC__
#define PACK_STRUCT_BEGIN
#elif defined(__IAR_SYSTEMS_ICC__)
#define PACK_STRUCT_BEGIN _Pragma("pack(2)")
#elif defined(__arm__) && defined(__ARMCC_VERSION)
#define PACK_STRUCT_BEGIN __packed
#else
#define PACK_STRUCT_BEGIN
#endif

#if __GNUC__
#define PACK_STRUCT_STRUCT __attribute__ ((__packed__))
#elif defined(__IAR_SYSTEMS_ICC__)
#define PACK_STRUCT_STRUCT
#elif defined(__arm__) && defined(__ARMCC_VERSION)
#define PACK_STRUCT_STRUCT
#else
#define PACK_STRUCT_STRUCT
#endif

#if __GNUC__
#define PACK_STRUCT_END
#elif defined(__IAR_SYSTEMS_ICC__)
#define PACK_STRUCT_END _Pragma("pack()")
#elif defined(__arm__) && defined(__ARMCC_VERSION)
#define PACK_STRUCT_END
#else
#define PACK_STRUCT_END
#endif






#define PACK_STRUCT_FIELD(x) x

// Platform specific diagnostic output

// non-fatal, print a message.
#define LWIP_PLATFORM_DIAG(x)                     do {PRINTF x;PRINTF("\r\n");} while(0)
// fatal, print message and abandon execution.
#define LWIP_PLATFORM_ASSERT(x)                   sys_assert( x )

#endif /* __CC_H__ */
