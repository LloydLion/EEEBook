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
        _display->fillScreen(GxEPD_WHITE);
        _display->fillRect(start.x, start.y, size.width, size.height, GxEPD_BLACK);
    }
    while (_display->nextPage());
}

void GxEPD_GraphicsEngine::print_text(Point start_point, char* text)
{
    _display->setFont(&FreeMonoBold12pt7b);
    _display->setTextColor(GxEPD_BLACK);
    _display->firstPage();
    do
    {
        _display->fillScreen(GxEPD_WHITE);
        _display->setCursor(start_point.x, start_point.y);
        _display->print(text);
    }
    while (_display->nextPage());
}

#endif
