#ifndef PLATFORM_STDOUT_H
#define PLATFORM_STDOUT_H

void std_print(const char* text);
void std_print(long long number, unsigned short base = 10);

void std_println();
void std_println(const char* text);
void std_println(long long number, unsigned short base = 10);

void std_printf(const char* format, ...);

#endif
