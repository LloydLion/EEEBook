#include "platform/stdout.h"
#include "platform/platform.h"

#if PLATFORM & PLATFORM_PC

#include <cstdio>
#include <iostream>
#include <cstdarg>

void std_print(const char* text)
{
    std::cout << text;
}

void std_print(long long number)
{
    std::cout << number;
}

void std_println(const char* text)
{
    std::cout << text << std::endl;
}

void std_println(long long number)
{
    std::cout << number << std::endl;
}

void std_printf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    printf(format, args);
    va_end(args);
}

#endif
