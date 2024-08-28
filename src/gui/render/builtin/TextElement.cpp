#include "gui/render/builtin/TextElement.h"

TextElement_::TextElement_(Font font)
{
    _p_font = font;
}

DEFAULT_PROPERTY_SETTER_IMPLEMENTATION_P(TextElement_, Font, font, Composition);

void TextElement_::i_render(const GFX& gfx)
{
    gfx.fill_screen(background_color());
    gfx.print_text(Vector(), get_text(), foreground_color(), -1, font());
}

Size TextElement_::i_min_size()
{
    cord_t width = font().get_string_width(get_text());
    cord_t height = font().get_height();

    return Size(width, height);
}

Size TextElement_::i_max_size()
{
    return i_min_size();
}
