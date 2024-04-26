#include "gui/engines/Serial_GraphicsEngine.h"
#include <Arduino.h>

#if IS_VIRTUAL_DISPLAY_USED

void Serial_GraphicsEngine::draw_rectangle(Bounds bounds)
{
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
    Serial.print("text: ");
    Serial.print(start_point.x);
    Serial.print(",");
    Serial.print(start_point.y);
    Serial.print('\n');
    Serial.print(text);
    Serial.print('\0');
    Serial.print('\n');
}

#endif
