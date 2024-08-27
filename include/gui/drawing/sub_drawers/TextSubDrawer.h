#ifndef GUI_DRAWING_SUB_DRAWERS_TEXT_SUB_DRAWER_H
#define GUI_DRAWING_SUB_DRAWERS_TEXT_SUB_DRAWER_H


#include "gui/coordinates.h"
#include "gui/drawing/Pattern.h"
#include "gui/drawing/Screen.h"
#include "gui/drawing/fonts/FontEngine.h"
#include "gui/drawing/fonts/Font.h"
#include "gui/drawing/PatternBag.h"

class TextSubDrawer
{
private:
    const char* _text;
    size_t _text_limit;
    FontId _font_id;
    const DrawFont *_font;
    Size _text_size;
    bool draw_char(Vector cursor, const DrawFont::Glyph *glyph);
public:
    Bounds bounds;
    PatternBag glyph_pattern, background_pattern;
    Screen output;
    void draw();
    TextSubDrawer(const char* text, size_t text_limit, FontId font_id);
};

#endif