#include "gui/engines/GxEPD_GraphicsEngine.h"
#include <Fonts/FreeMonoBold12pt7b.h>
#include <exception>

#if !IS_VIRTUAL_DISPLAY_USED

GxEPD_GraphicsEngine::GxEPD_GraphicsEngine(DISPLAY_TYPE *display): _display(display)
{
    
}

void GxEPD_GraphicsEngine::draw_rectangle(Bounds bounds)
{
    DrawOperation operation;
    operation.type = DrawOperationType::Text;
    operation.bounds = bounds;
    operation.color = color_t::Black; //TODO: Add color as parameter

    _operation_queue.push_back(operation);
}

void GxEPD_GraphicsEngine::print_text(Vector start, cord_t width_limit, const char *text, Font font)
{
    Bounds bounds;
    cord_t width = font->get_string_width(text);

    if (width_limit < width)
    {
        throw std::runtime_error("Text is too long");
    }

    bounds.size.height = font->get_height();
    bounds.size.width = width_limit;

    DrawOperation operation;
    operation.type = DrawOperationType::Text;
    operation.bounds.start = start;
    operation.color = color_t::Black; //TODO: Add color as parameter

    operation.args.text.text = text;
    operation.args.text.font = font;

    _operation_queue.push_back(operation);
}

void GxEPD_GraphicsEngine::push()
{
    _display->setPartialWindow(0, 0, _display->width(), _display->height());

    _display->firstPage();

    do
    {
        for (auto operation : _operation_queue)
            do_operation(operation);
    }
    while (_display->nextPage());

    _operation_queue.clear();
}

void GxEPD_GraphicsEngine::do_operation(const DrawOperation &operation)
{
    Bounds bounds = operation.bounds;
    color_t color = operation.color;

    switch (operation.type)
    {
    case DrawOperationType::Rectangle:
        _display->fillRect(
            bounds.start.x,
            bounds.start.y,
            bounds.size.width,
            bounds.size.height,
            color
        );
        break;
        
    case DrawOperationType::Text:
        const char *text = *(const char**)&operation;

        _display->setTextColor(color);

        int16_t text_dx, text_dy;
        uint16_t text_width, text_height;

        _display->getTextBounds(text, 0, 0, &text_dx, &text_dy, &text_width, &text_height);

        _display->setCursor(bounds.start.x - text_dx, bounds.start.y - text_dy);
        _display->print(text);
        break;
    }
}

#endif
