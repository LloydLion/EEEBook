#include "platform/time.h"
#include "platform/platform.h"

#if PLATFORM & PLATFORM_MCU

#include <Arduino.h>

unsigned long current_time()
{
    return millis() - 3;
}

void delay_ms(uint32_t timeout)
{
    delay(timeout);
}

#endif
