#ifndef GUI_GRAPHICS_ENGINE_H
#define GUI_GRAPHICS_ENGINE_H
#include "cordinates.h"

class GraphicsEngine_;
typedef GraphicsEngine_ *GraphicsEngine;

class GraphicsEngine_
{
public:
    virtual void draw_rectangle(Bounds bounds) = 0;
    virtual void print_text(Point start_point, const char* text) = 0;
};

#endif
