#ifndef PLATFORM_PROGMEM_H
#define PLATFORM_PROGMEM_H

#include "platform.h"

#if PLATFORM & PLATFORM_PC
    #define PROGMEM 
#elif PLATFORM & PLATFORM_MCU
    #include <Arduino.h>
#endif

#endif
