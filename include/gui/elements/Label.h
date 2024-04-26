#ifndef GUI_ELEMENTS_LABEL_H
#define GUI_ELEMENTS_LABEL_H

#include "../cordinates.h"
#include "../UIElement.h"

class Label_;
typedef Label_ *Label;

class Label_ : public UIElement_
{
public:
    Label_(const char* text);

    void render(const GFX& gfx) override;

    bool is_updated() override;

    Size min_size() override;
private:
    const char* _text;
    bool _is_drawn = false;
};

#endif