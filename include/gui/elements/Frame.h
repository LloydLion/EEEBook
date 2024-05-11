#ifndef GUI_ELEMENTS_FRAME_H
#define GUI_ELEMENTS_FRAME_H 

#include "../cordinates.h"
#include "gui/UIView.h"


class Frame_;
typedef Frame_ *Frame;

class Frame_ : public UIView_
{
public:
    Frame_(cord_t thickness, UIElement element);

    void render(const GFX& gfx) override;
    Size min_size() override;
    UIElement get_element() override;

private:
    cord_t _thickness;
    UIElement _element;
};

#endif
