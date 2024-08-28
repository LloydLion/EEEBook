#include "platform/stdout.h"
#include "platform/platform.h"

#if PLATFORM & PLATFORM_MCU

#include <Arduino.h>

void std_print(const char* text)
{
    Serial.print(text);
}

void std_print(long long number, unsigned short base)
{
    Serial.print(number, base);
}

void std_println()
{
    Serial.println();
}

void std_println(const char* text)
{
    Serial.println(text);
}

void std_println(long long number, unsigned short base)
{
    Serial.println(number, base);
}

void std_printf(const char* format, ...)
{
    va_list args;
    char buf[1000];
    va_start(args, format);

    vsnprintf(buf, sizeof(buf), format, args);
    std_print(buf);

    va_end(args);
}

#endif
