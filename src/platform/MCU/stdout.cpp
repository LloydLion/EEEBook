#include "platform/stdout.h"
#include "platform/platform.h"

#if PLATFORM & PLATFORM_MCU

#include <Arduino.h>

void std_print(const char* text)
{
    Serial.print(text);
}

void std_print(long long number)
{
    Serial.print(number);
}

void std_println(const char* text)
{
    Serial.println(text);
}

void std_println(long long number)
{
    Serial.println(number);
}

void std_printf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    Serial.printf(format, args);
    va_end(args);
}

#endif
