#ifndef GUI_ENGINES_GXEPD_GRAPHICSENGINE_H
#define GUI_ENGINES_GXEPD_GRAPHICSENGINE_H
#include "config.h"
#include "../GraphicsEngine.h"

#if !IS_VIRTUAL_DISPLAY_USED

class GxEPD_GraphicsEngine : public GraphicsEngine_
{
private:
    DISPLAY_TYPE *_display;

public:
    GxEPD_GraphicsEngine(DISPLAY_TYPE *display);

    void draw_rectangle(Bounds bounds) override;
    void print_text(Point start_point, const char* text) override;
};

#endif
#endif
