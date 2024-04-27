#ifndef GUI_ENGINES_SERIAL_GRAPHICSENGINE_H
#define GUI_ENGINES_SERIAL_GRAPHICSENGINE_H
#include "config.h"
#include "../GraphicsEngine.h"

#if IS_VIRTUAL_DISPLAY_USED

class Serial_GraphicsEngine : public GraphicsEngine_
{
public:
    void draw_rectangle(Bounds bounds) override;
    void print_text(Point start_point, const char* text) override;
};

#endif
#endif