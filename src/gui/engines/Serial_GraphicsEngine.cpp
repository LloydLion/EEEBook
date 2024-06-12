#include "gui/engines/Serial_GraphicsEngine.h"
#include "platform/stdout.h"
#include "platform/platform.h"
#define CLEAR_SCREEN_IF_REQUIRED if (_clearing_required) { std_print("clear"); _clearing_required = false; }

#if IS_VIRTUAL_DISPLAY_USED && (PLATFORM & PLATFORM_MCU_ESP32)

void Serial_GraphicsEngine::draw_rectangle(Bounds bounds)
{
    CLEAR_SCREEN_IF_REQUIRED

    std_print("rect: ");
    std_print(bounds.start_point.x);
    std_print(",");
    std_print(bounds.start_point.y);
    std_print(",");
    std_print(bounds.size.width);
    std_print(",");
    std_print(bounds.size.height);
    std_print('\n');
}

void Serial_GraphicsEngine::print_text(Point start_point, const char* text)
{
    CLEAR_SCREEN_IF_REQUIRED

    std_print("text: ");
    std_print(start_point.x);
    std_print(",");
    std_print(start_point.y);
    std_print('\n');
    std_print(text);
    std_print('\0');
    std_print('\n');
}

void Serial_GraphicsEngine::push(UpdateRule rule) // Ignoring UpdateRule b.s It doesn't matter for serial display
{
    _clearing_required = true;
}

#endif
