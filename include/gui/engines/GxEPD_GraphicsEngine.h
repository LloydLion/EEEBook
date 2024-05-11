#ifndef GUI_ENGINES_GXEPD_GRAPHICSENGINE_H
#define GUI_ENGINES_GXEPD_GRAPHICSENGINE_H
#include "config.h"
#include "gui/color.h"
#include "gui/GraphicsEngine.h"
#include <vector>
#include "std/iterator.h"

#if !IS_VIRTUAL_DISPLAY_USED

class GxEPD_GraphicsEngine : public GraphicsEngine_
{
private:
    enum DrawOperationType : uint8_t
    {
        Rectangle,
        Text,
        Ellipse,
        Line,
        Picture
    };

    struct LineDrawOperationArgs
    {
        bool is_main_diagonal;
        cord_t thickness;
        Pattern pattern;
    };

    struct TextDrawOperationArgs
    {
        const char *text;
        Font font;
        size_t len_limit;
    };

    struct RectDrawOperationArgs
    {
        cord_t thickness;
        Pattern pattern;
    };

    struct EllipseDrawOperationArgs
    {
        cord_t thickness;
        Pattern pattern;
    };

    struct PictureDrawOperationArgs
    {
        Bitmap bitmap;
        PictureDrawOperationArgs(): bitmap(nullptr, 0, 0) {};
    };

    union DrawOperationArgs
    {
        TextDrawOperationArgs text;
        RectDrawOperationArgs rect;
        EllipseDrawOperationArgs ellipse; 
        PictureDrawOperationArgs picture;
        LineDrawOperationArgs line;
        DrawOperationArgs(){};
    };

    struct DrawOperation
    {
        DrawOperationType type;
        Bounds bounds;
        color_t color;
        DrawOperationArgs args;
    };
    

    class Fonts : public FontEngine_
    {
    private:
        struct InternalFont
        {
            const GFXfont* font;
            cord_t height;
            cord_t y_offset;
        };

        std::vector<InternalFont> _fonts;

    public:
        Fonts();

        cord_t get_char_width(font_id_t font, char c) override;
        cord_t get_height(font_id_t font) override;

        bool is_legit_char(font_id_t font, char c) override;
        size_t first_non_legit_char(font_id_t font, const char* str) override;

        font_id_t register_font(const GFXfont *font);
        const GFXfont *get_font(font_id_t id);
        cord_t get_yoffset(font_id_t id);
    };

    std::vector<DrawOperation> _operation_queue;
    DISPLAY_TYPE *_display;
    Fonts _fonts;
    Font_ _default_font;

    void do_operation(const DrawOperation &operation);

public:
    GxEPD_GraphicsEngine(DISPLAY_TYPE *display);

    void draw_rectangle(Bounds bounds, color_t color, cord_t thickness, Pattern pattern) override;
    void draw_line(Vector start_point, Vector end_point, color_t color, cord_t thickness, cord_t bias, Pattern pattern) override;
    void draw_ellipse(Bounds bounds, color_t color, cord_t thickness, Pattern pattern) override;
    void draw_bitmap(Vector star_point, Bitmap bitmap, color_t color) override;
    void print_text(Vector start_point, cord_t width_limit, const char *text, size_t len_limit, color_t color, Font font) override;

    Font get_default_font() override;
    FontEngine get_font_engine() override;

    void push(DrawSettings settings) override;

    font_id_t register_font(const GFXfont *font);
};


#endif
#endif
