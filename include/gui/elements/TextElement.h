#ifndef GUI_TEXT_ELEMENT_H
#define GUI_TEXT_ELEMENT_H

#include "gui/coordinates.h"
#include "gui/UIElement.h"
#include "gui/drawing/fonts/Font.h"
#include "std/property.h"

class TextElement_;
typedef TextElement_ *UITextElement;

class TextElement_ : public UIElement_
{
private:
    Font _p_font;

protected:
    TextElement_(Font font);

    void i_render(const GFX& gfx) override;
    Size i_min_size() override;
    Size i_max_size() override;

    virtual const char *get_text() = 0;

public:
    PROPERTY(Font, font) AUTO_GET(_p_font);
};

#endif