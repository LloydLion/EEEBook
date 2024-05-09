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

    void i_render(const GFX& gfx) override;
    Size i_min_size() override;
    Size i_max_size() override;
    UIElement get_element() override;

    cord_t _thickness;
private:
    UIElement _element;
};

#endif
