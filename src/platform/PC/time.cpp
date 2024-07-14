#include "platform/time.h"
#include "platform/platform.h"

#if PLATFORM & PLATFORM_PC

#include <ctime>
#include <thread>
#include <chrono>

unsigned long current_time()
{
    return (unsigned long)time(nullptr);
}

void delay_ms(uint32_t timeout)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
}

#endif
