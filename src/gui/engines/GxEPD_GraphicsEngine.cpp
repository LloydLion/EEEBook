#include "gui/engines/GxEPD_GraphicsEngine.h"
#include <Fonts/FreeMonoBold12pt7b.h>
#include <exception>

#if !IS_VIRTUAL_DISPLAY_USED

GxEPD_GraphicsEngine::GxEPD_GraphicsEngine(DISPLAY_TYPE *display): _display(display)
{
    _display->setFont(&FreeMonoBold12pt7b);
}

void GxEPD_GraphicsEngine::draw_rectangle(Bounds bounds)
{
    DrawOperation operation;
    operation.type = DrawOperationType::Text;
    operation.bounds = bounds;
    operation.color = color_t::Black; //TODO: Add color as parameter

    _operation_queue.push_back(operation);
}

void GxEPD_GraphicsEngine::print_text(Point start_point, cord_t width_limit, const char *text)
{
    Bounds bounds;
    Size size = get_text_size(text);

    if (width_limit < size.width)
    {
        throw std::runtime_error("Text is too long");
    }

    bounds.size.height = size.height;
    bounds.size.width = width_limit;

    DrawOperation operation;
    operation.type = DrawOperationType::Text;
    operation.bounds.start_point = start_point;
    operation.color = color_t::Black; //TODO: Add color as parameter

    *(const char**)&operation = text;

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

Size GxEPD_GraphicsEngine::get_text_size(const char *text)
{
    char *new_line = strchr(text, '\n');
    if (new_line != nullptr)
    {
        throw std::runtime_error("Multiple lines are disallowed");
    }

    int16_t text_dx, text_dy;
    uint16_t text_width, text_height;

    _display->getTextBounds(text, 0, 0, &text_dx, &text_dy, &text_width, &text_height);

    return create_size(text_width, text_height);
}

void GxEPD_GraphicsEngine::do_operation(const DrawOperation &operation)
{
    Bounds bounds = operation.bounds;
    color_t color = operation.color;

    switch (operation.type)
    {
    case DrawOperationType::Rectangle:
        _display->fillRect(
            bounds.start_point.x,
            bounds.start_point.y,
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

        _display->setCursor(bounds.start_point.x - text_dx, bounds.start_point.y - text_dy);
        _display->print(text);
        break;
    }
}

#endif
