#ifndef GUI_GFX_H
#define GUI_GFX_H

#include "coordinates.h"
#include "gui/drawing/Pattern.h"
#include "gui/drawing/DrawOperationQueue.h"
#include "gui/drawing/fonts/Font.h"

class GFX
{
private:
    Bounds _bounds;
    DrawOperationQueue _queue;

    GFX(DrawOperationQueue queue, Bounds bounds);

public:
    GFX(DrawOperationQueue queue, Size display_size);

    void draw_rectangle(LocalBounds bounds, transparent_color_t color, cord_t thickness = 0) const;
    void draw_rectangle(LocalBounds bounds, Pattern fill_pattern) const;
    void draw_rectangle(LocalBounds bounds, cord_t thickness, Pattern interior, Pattern boundary) const;
    void draw_rectangle(LocalBounds bounds, cord_t thickness,
        Pattern interior, Pattern corners, Pattern vertical, Pattern horizontal) const;
    void fill_screen(transparent_color_t color) const;
    void fill_screen(Pattern pattern) const;

    void print_text(LocalVector start, cord_t width_limit, const char *text, transparent_color_t color, size_t len_limit = -1, Font font = Font()) const;
    void print_text(LocalVector start, const char *text, transparent_color_t color, size_t len_limit = -1, Font font = Font()) const;
    void print_text(LocalVector start, cord_t width_limit, const char *text, Pattern pattern, size_t len_limit, Font font) const;

    void draw_line(LocalVector start, LocalVector end, transparent_color_t color, cord_t thickness = 0, cord_t length = 0);
    void draw_line(LocalVector start, LocalVector end, Pattern pattern, cord_t thickness = 0, cord_t length = 0);
    void draw_line(LocalVector start, SignedVector direction, cord_t length, Pattern pattern, cord_t thickness = 0);

    GFX slice(LocalBounds local_bounds) const;
    GFX slice(Distance4Sides distances) const;

    Size size() const;
};

#endif
