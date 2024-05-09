#include "gui/engines/GxEPD_GraphicsEngine.h"
#include <exception>

#if !IS_VIRTUAL_DISPLAY_USED

bool drawChar(GFXglyph *glyph, const GFXfont *font, uint16_t color, uint16_t pos_y, uint16_t cursor_x, cord_t width_limit, DISPLAY_TYPE *display);

GxEPD_GraphicsEngine::GxEPD_GraphicsEngine(DISPLAY_TYPE *display):
    _display(display), _default_font(0, &_fonts)
{

}

void GxEPD_GraphicsEngine::draw_rectangle(Bounds bounds, color_t color, cord_t thickness)
{
    DrawOperation operation;
    operation.type = DrawOperationType::Rectangle;
    operation.bounds = bounds;
    operation.color = color;

    operation.args.rect.thickness = thickness;

    _operation_queue.push_back(operation);
}

void GxEPD_GraphicsEngine::print_text(Vector start, cord_t width_limit, const char *text, size_t len_limit, color_t color, Font font)
{
    DrawOperation operation;
    operation.type = DrawOperationType::Text;
    operation.bounds = Bounds(start, Size(width_limit, font->get_height()));;
    operation.color = color;

    operation.args.text.text = text;
    operation.args.text.font = font;
    operation.args.text.len_limit = len_limit;

    _operation_queue.push_back(operation);
}

void GxEPD_GraphicsEngine::push(DrawSettings draw_settings)
{   
    if(draw_settings.update_rule->is_partial_update())
        _display->setPartialWindow(0, 0, _display->width(), _display->height());

    _display->firstPage();

#if GxEPD_GE_DEBUG_OPTIONS & GxEPD_GE_RENDER_DEBUG
    Serial.println("GxEPD_GraphicsEngine: pushing operations to display");

    Serial.printf("DrawSettings: background=%d", draw_settings.background_color);
    Serial.println();

    for (size_t i = 0; i < _operation_queue.size(); i++)        
    {
        auto operation = _operation_queue[i];

        Serial.printf("[%d]: Bounds(%d, %d, %d, %d), color=%d | ",
            i,
            operation.bounds.start.x,
            operation.bounds.start.y,
            operation.bounds.size.width,
            operation.bounds.size.height,
            operation.color
        );

        switch (operation.type)
        {
        case DrawOperationType::Rectangle:
            Serial.printf("RECT thickness=%d", operation.args.rect.thickness);
            break;

            
        case DrawOperationType::Text:
            Serial.printf("TEXT len_limit=%d, font=%d, text=", operation.args.text.len_limit, (int)operation.args.text.font);
            Serial.print(operation.args.text.text);
            break;
        
        default:
            Serial.print("UNKNOWN fix it");
            break;
        }
        Serial.println();
    }
#endif

    do
    {
        _display->fillScreen(color_to_rgb565(draw_settings.background_color));

        for (auto operation : _operation_queue)
        {
            do_operation(operation);
        }
    }
    while (_display->nextPage());

    _operation_queue.clear();
}

void GxEPD_GraphicsEngine::do_operation(const DrawOperation &operation)
{
    Bounds bounds = operation.bounds;
    uint16_t color = color_to_rgb565(operation.color);

    switch (operation.type)
    {
    case DrawOperationType::Rectangle:
        {
            cord_t thickness = operation.args.rect.thickness;
            if (thickness == 0)
            {
                _display->fillRect(
                    bounds.start.x,
                    bounds.start.y,
                    bounds.size.width,
                    bounds.size.height,
                    color
                );
            }
            else
            {
                _display->startWrite();
                Vector end = bounds.end();
                for (cord_t delta = 0; delta < thickness; delta++)
                {
                    _display->drawFastVLine(bounds.start.x + delta, bounds.start.y, bounds.size.height, color);   
                    _display->drawFastVLine(end.x - delta, bounds.start.y, bounds.size.height, color);

                    _display->drawFastHLine(bounds.start.x, bounds.start.y + delta, bounds.size.width, color);
                    _display->drawFastHLine(bounds.start.x, end.y - delta, bounds.size.width, color);
                }
                _display->endWrite();
            }
        }
        break;
        
    case DrawOperationType::Text:
        {
            const char *text = operation.args.text.text;
            font_id_t font_id = operation.args.text.font->id();
            const GFXfont *font = _fonts.get_font(font_id);

            uint16_t cursor_x = bounds.start.x;
            uint16_t pos_y = bounds.start.y + _fonts.get_yoffset(font_id);

            size_t i = 0;
            while (char c = *text++)
            {
                if (i++ >= operation.args.text.len_limit) break;

                uint8_t first = font->first;
                if ((c >= first) && (c <= font->last))
                {
                    GFXglyph *glyph = font->glyph + (c - first);

#if GxEPD_GE_DEBUG_OPTIONS & GxEPD_GE_TEXT_DEBUG
                    _display->drawFastHLine(cursor_x, bounds.start.y, glyph->xAdvance, color);
                    _display->drawFastVLine(cursor_x, bounds.start.y - 1, 5, color);
#endif

                    bool was_clipped = drawChar(glyph, font, color, pos_y, cursor_x, bounds.end().x, _display);

                    if (was_clipped)
                        break;

                    cursor_x += glyph->xAdvance;
                }
            }
        }
        break;
    }
}

bool drawChar(GFXglyph *glyph, const GFXfont *font, uint16_t color, uint16_t pos_y, uint16_t cursor_x, cord_t end_x, DISPLAY_TYPE *display)
{
    bool was_clipped = false;

    uint8_t w = glyph->width;
    uint8_t h = glyph->height;

    if ((w > 0) && (h > 0)) // Is there an associated bitmap?
    {
        int8_t xo = glyph->xOffset;
        int8_t yo = glyph->yOffset;

#if GxEPD_GE_DEBUG_OPTIONS & GxEPD_GE_TEXT_DEBUG
        display->drawRect(cursor_x + xo,
            pos_y + yo,
            w, h, color);
#endif
        
        uint8_t *bitmap = font->bitmap;
        uint16_t bo = glyph->bitmapOffset;


        //Draw bitmap
        uint8_t bits = 0, bit = 0;

        display->startWrite();
        for (uint8_t yy = 0; yy < h; yy++)
        {
            for (uint8_t xx = 0; xx < w; xx++)
            {
                if (!(bit++ & 7))
                    bits = bitmap[bo++];

                if (bits & 0x80)
                    if (cursor_x + xo + xx <= end_x)
                        display->writePixel(cursor_x + xo + xx, pos_y + yo + yy, color);
                    else
                        was_clipped = true;
                
#if GxEPD_GE_DEBUG_OPTIONS & GxEPD_GE_TEXT_DEBUG
                if (cursor_x + xo + xx > width_limit)
                    display->writePixel(cursor_x + xo + xx, pos_y + yo + yy, color);
#endif

                bits <<= 1;
            }
        }
        display->endWrite();

    }

    return was_clipped;
}


font_id_t GxEPD_GraphicsEngine::register_font(const GFXfont *font)
{
    return _fonts.register_font(font);
}

FontEngine GxEPD_GraphicsEngine::get_font_engine()
{
    return &_fonts;
}

Font GxEPD_GraphicsEngine::get_default_font()
{
    return &_default_font;
}

GxEPD_GraphicsEngine::Fonts::Fonts()
{

}

cord_t GxEPD_GraphicsEngine::Fonts::get_char_width(font_id_t font, char c)
{
    const GFXfont *adafruit_font = _fonts[font].font;
    if (is_legit_char(font, c) == false)
    {
        throw std::runtime_error("Enable to get width of illegal char");
    }

    size_t index = c - adafruit_font->first;
    const GFXglyph &glyph = adafruit_font->glyph[index];
    return glyph.xAdvance;
}

bool GxEPD_GraphicsEngine::Fonts::is_legit_char(font_id_t font, char c)
{
    const GFXfont *adafruit_font =  _fonts[font].font;
    return adafruit_font->first < c and c < adafruit_font->last;
}

size_t GxEPD_GraphicsEngine::Fonts::first_non_legit_char(font_id_t font, const char* str)
{
    const GFXfont *adafruit_font =  _fonts[font].font;
    size_t index = 0;
    while (char c = *str++)
    {
        if (c < adafruit_font->first or c > adafruit_font->last)
            return index;
        index++;
    }

    return -1;
}

font_id_t GxEPD_GraphicsEngine::Fonts::register_font(const GFXfont *font)
{
    InternalFont ifont;
    ifont.font = font;

    int8_t min_y = 127, max_y = -128;
    for (uint16_t c = 0; c <= font->last - font->first; c++)
    {
        GFXglyph *glyph = font->glyph + c;

        min_y = min(min_y, glyph->yOffset);
        max_y = max(max_y, (int8_t)(glyph->yOffset + glyph->height));
    }

    ifont.height = max_y - min_y;
    ifont.y_offset = -min_y;

    _fonts.push_back(ifont);
    return _fonts.size() - 1;
}

const GFXfont *GxEPD_GraphicsEngine::Fonts::get_font(font_id_t id) { return _fonts[id].font; }

cord_t GxEPD_GraphicsEngine::Fonts::get_yoffset(font_id_t id) { return _fonts[id].y_offset; }

cord_t GxEPD_GraphicsEngine::Fonts::get_height(font_id_t font) { return _fonts[font].height; }

#endif
