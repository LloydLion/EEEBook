#include "gui/engines/Serial_GraphicsEngine.h"
#include <Arduino.h>
#define CLEAR_SCREEN_IF_REQUIRED if (_clearing_required) { Serial.println("clear"); _clearing_required = false; }

#if IS_VIRTUAL_DISPLAY_USED

void Serial_GraphicsEngine::draw_rectangle(Bounds bounds)
{
    CLEAR_SCREEN_IF_REQUIRED

    Serial.print("rect: ");
    Serial.print(bounds.start_point.x);
    Serial.print(",");
    Serial.print(bounds.start_point.y);
    Serial.print(",");
    Serial.print(bounds.size.width);
    Serial.print(",");
    Serial.print(bounds.size.height);
    Serial.print('\n');
}

void Serial_GraphicsEngine::print_text(Point start_point, const char* text)
{
    CLEAR_SCREEN_IF_REQUIRED

    Serial.print("text: ");
    Serial.print(start_point.x);
    Serial.print(",");
    Serial.print(start_point.y);
    Serial.print('\n');
    Serial.print(text);
    Serial.print('\0');
    Serial.print('\n');
}

void Serial_GraphicsEngine::push()
{
    _clearing_required = true;
}

#endif
