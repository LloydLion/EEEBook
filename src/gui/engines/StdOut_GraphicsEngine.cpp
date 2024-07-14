#include "gui/engines/StdOut_GraphicsEngine.h"
#include "platform/stdout.h"
#include "cstring"

#define UNPACK_BOUNDS(BOUNDS) BOUNDS.start.x(), BOUNDS.start.y(), BOUNDS.size.width(), BOUNDS.size.height()
#define MESSAGE(OP_CODE, FORMAT, ...) std_print("### StdOut_GraphicsEngine>"); std_print(OP_CODE); std_print(">"); std_printf(FORMAT, __VA_ARGS__); std_println();


StdOut_GraphicsEngine::StdOut_GraphicsEngine() : _fonts(), _default_font(0, &_fonts)
{

}

void StdOut_GraphicsEngine::draw_rectangle(Bounds bounds, color_t color, cord_t thickness)
{
    MESSAGE("DRAW", "(%d,%d,%d,%d):%d:RECT(%d)", UNPACK_BOUNDS(bounds), color, thickness);
}

void StdOut_GraphicsEngine::print_text(Vector start_point, cord_t width_limit, const char *text, size_t len_limit, color_t color, Font font)
{
    MESSAGE("TEXT", "%d:%s", strlen(text), text);
    MESSAGE("DRAW", "(%d,%d,%d,%d):%d:TEXT(%d, %d)", start_point.x(), start_point.y(), width_limit, font->get_height(), color, font->id(), len_limit);
}

Font StdOut_GraphicsEngine::get_default_font()
{
    return &_default_font;
}

FontEngine StdOut_GraphicsEngine::get_font_engine()
{
    return &_fonts;
}

void StdOut_GraphicsEngine::push(DrawSettings settings)
{
    MESSAGE("PUSH", "%d,%d", settings.background_color, settings.update_rule->is_partial_update());
}

cord_t StdOut_GraphicsEngine::Fonts::get_char_width(font_id_t font, char c)
{
    return 6;
}

cord_t StdOut_GraphicsEngine::Fonts::get_height(font_id_t font)
{
    return 6;
}

bool StdOut_GraphicsEngine::Fonts::is_legit_char(font_id_t font, char c)
{
    return true;
}

size_t StdOut_GraphicsEngine::Fonts::first_non_legit_char(font_id_t font, const char* str)
{
    return -1;
}
