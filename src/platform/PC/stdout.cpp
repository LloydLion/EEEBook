#include "platform/stdout.h"
#include "platform/platform.h"

#if PLATFORM & PLATFORM_PC

#include <cstdio>
#include <iostream>
#include <cstdarg>
#include <bitset>

void std_print(const char* text)
{
    std::cout << text;
}

void std_print(long long number, unsigned short base)
{
    if (base == 2)
    {
        std::bitset<sizeof(long long)> bits;
        std::cout << bits;
    }

    switch (base)
    {
    case 10: break;

    case 16:
        std::cout << std::hex;
        break;

    case 8:
        std::cout << std::oct;
        break;
    
    default:
        throw std::runtime_error("Not supported number base");
    }

    std::cout << number;
}

void std_println()
{
    std::cout << std::endl;
}

void std_println(const char* text)
{
    std_print(text);
    std_println();
}

void std_println(long long number, unsigned short base)
{
    std_print(number, base);
    std_println();
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
