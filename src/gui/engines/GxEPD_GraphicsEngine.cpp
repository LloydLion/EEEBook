#include "gui/engines/GxEPD_GraphicsEngine.h"
#include <Fonts/FreeMonoBold12pt7b.h>

#if !IS_VIRTUAL_DISPLAY_USED

GxEPD_GraphicsEngine::GxEPD_GraphicsEngine(DISPLAY_TYPE *display): _display(display)
{

}

void GxEPD_GraphicsEngine::draw_rectangle(Bounds bounds)
{
    Point start = bounds.start_point;
    Size size = bounds.size;
    _display->setPartialWindow(start.x, start.y, size.width, size.height);
    _display->firstPage();
    do
    {
        _display->fillScreen(GxEPD_BLACK);
        //_display->fillRect(start.x, start.y, size.width, size.height, GxEPD_BLACK);
    }
    while (_display->nextPage());
}

void GxEPD_GraphicsEngine::print_text(Point start_point, const char* text)
{
    _display->setFont(&FreeMonoBold12pt7b);
    _display->setTextColor(GxEPD_BLACK);
    int16_t text_dx, text_dy;
    uint16_t text_width, text_height;
    _display->getTextBounds(text, 0, 0, &text_dx, &text_dy, &text_width, &text_height);

    Serial.print(text_dx);
    Serial.print(", ");
    Serial.print(text_dy);
    Serial.print(", ");
    Serial.print(text_width);
    Serial.print(", ");
    Serial.print(text_height);
    Serial.println();
    //_display->setPartialWindow(start_point.x, start_point.y, text_width, text_height);
    _display->firstPage();
    do
    {
        //_display->fillRect(start_point.x, start_point.y, text_width, text_height, GxEPD_WHITE);
        _display->fillRect(start_point.x, start_point.y + 1, text_width, 1, GxEPD_BLACK);
        _display->fillRect(start_point.x + 1, start_point.y, 1, text_height, GxEPD_BLACK);
        _display->setCursor(start_point.x - text_dx, start_point.y - text_dy);
        _display->print(text);
    }
    while (_display->nextPage());
}

#endif
