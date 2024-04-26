#include "gui/engines/GxEPD_GraphicsEngine.h"
#include <Fonts/FreeMonoBold9pt7b.h>

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

void GxEPD_GraphicsEngine::print_text(Point start_point, const char* text)
{
    _display->setFont(&FreeMonoBold9pt7b);
    _display->setTextColor(GxEPD_BLACK);
    int16_t tbx, tby; uint16_t tbw, tbh;
    _display->getTextBounds(text, start_point.x, start_point.y, &tbx, &tby, &tbw, &tbh);
    _display->setPartialWindow(start_point.x, start_point.y, tbw, tbh);
    _display->firstPage();
    do
    {
        _display->fillScreen(GxEPD_WHITE);
        _display->setCursor(2 * start_point.x - tbx, 2 * start_point.y - tby);
        _display->print(text);
    }
    while (_display->nextPage());
}

#endif
