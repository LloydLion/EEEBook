#ifndef GUI_DRAWING_SUB_DRAWERS_LINE_SUB_DRAWER_H
#define GUI_DRAWING_SUB_DRAWERS_LINE_SUB_DRAWER_H

#include "gui/coordinates.h"
#include "gui/drawing/Pattern.h"
#include "gui/drawing/Screen.h"

class LineSubDrawer
{
private:
    Vector _start;
    SignedVector _pre_end;
    cord_t _length;
    cord_t _thickness;

    Vector _end;
    Bounds _outline_box;

    enum class SubLineCornerMode { None, Post, Pre };

    void draw_line_pixel_using_pattern(cord_t length_position, s_cord_t thickness_position, SignedVector position);
    void draw_sub_line(SignedVector global_cursor, SignedVector direction, SubLineCornerMode corner_mode, cord_t length_position);
    void draw_thick_line();

public:
    Bounds bounds;
    Screen output;
    Pattern pattern;
    
    LineSubDrawer(Vector start, SignedVector pre_end, cord_t length, cord_t thickness);

    void draw();
};

#endif
