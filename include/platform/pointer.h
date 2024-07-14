#ifndef PLATFORM_POINTER_H
#define PLATFORM_POINTER_H

#include "platform/platform.h"
#include <inttypes.h>

#if PLATFORM & PLATFORM_PC
    typedef uint64_t ptr_int_t;
#elif PLATFORM & PLATFORM_MCU
    typedef uint32_t ptr_int_t;
#else
    #error "Not supported platform"
#endif

#endif
