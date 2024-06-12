#include "platform/time.h"
#include "platform/platform.h"

#if PLATFORM & PLATFORM_PC

#include <ctime>

unsigned long current_time()
{
    return (unsigned long)time(0);
}

#endif
