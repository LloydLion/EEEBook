#ifndef GUI_ENGINES_GXEPD_GRAPHICSENGINE_H
#define GUI_ENGINES_GXEPD_GRAPHICSENGINE_H
#include "config.h"
#include "gui/color.h"
#include "gui/GraphicsEngine.h"
#include <vector>

#if !IS_VIRTUAL_DISPLAY_USED

class GxEPD_GraphicsEngine : public GraphicsEngine_
{
private:
    enum DrawOperationType : uint8_t
    {
        Rectangle,
        Text
    };

    struct DrawOperation
    {
        //Order is important!
        byte arguments[6];
        DrawOperationType type;
        Bounds bounds;
        color_t color;
    };

    std::vector<DrawOperation> _operation_queue;
    DISPLAY_TYPE *_display;

    void do_operation(const DrawOperation &operation);

public:
    GxEPD_GraphicsEngine(DISPLAY_TYPE *display);

    void draw_rectangle(Bounds bounds) override;
    void print_text(Point start_point, cord_t width_limit, const char *text) override;
    void push() override;
    Size get_text_size(const char *text) override;
};

#endif
#endif
