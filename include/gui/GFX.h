#ifndef GUI_GFX_H
#define GUI_GFX_H 
#include "cordinates.h"
#include "GraphicsEngine.h"
#include "Font.h"

class GFX
{
private:
    Bounds _bounds;
    GraphicsEngine _engine;

    GFX(GraphicsEngine engine, Bounds bounds);
public:
    GFX(GraphicsEngine engine, Size display_size);

    void draw_rectangle(LocalBounds bounds) const;

    void print_text(LocalVector start, cord_t width_limit, const char *text, Font font = nullptr) const;
    void print_text(LocalVector start, const char *text, Font font = nullptr) const;
    void cut_and_print_text(LocalVector start, char *text_buffer, Font font = nullptr) const;

    Font get_default_font() const;

    GFX slice(LocalBounds local_bounds) const;

    Size size() const;
};

#endif
