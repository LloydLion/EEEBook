#ifndef GUI_ENGINES_STD_GRAPHICSENGINE_H
#define GUI_ENGINES_STD_GRAPHICSENGINE_H

#include "gui/GraphicsEngine.h"

class StdOut_GraphicsEngine : public GraphicsEngine_
{
private:
    class Fonts : public FontEngine_
    {
    public:
        cord_t get_char_width(FontId font, char c) override;
        cord_t get_height(FontId font) override;

        bool is_legit_char(FontId font, char c) override;
        size_t first_non_legit_char(FontId font, const char* str) override;
    };

    Fonts _fonts;
    Font_ _default_font;

public:
    StdOut_GraphicsEngine();

    void draw_rectangle(Bounds bounds, color_t color, cord_t thickness);
    void print_text(Vector start_point, cord_t width_limit, const char *text, size_t len_limit, color_t color, Font font) override;

    Font get_default_font() override;
    FontEngine get_font_engine() override;

    void push(DrawSettings settings) override;
};

#endif
