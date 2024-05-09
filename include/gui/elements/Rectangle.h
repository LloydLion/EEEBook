#ifndef GUI_ELEMENTS_RECTANGLE_H
#define GUI_ELEMENTS_RECTANGLE_H 

#include "../cordinates.h"
#include "../UIElement.h"


class Rectangle_;
typedef Rectangle_ *Rectangle;

class Rectangle_ : public UIElement_
{
public:
    Rectangle_(cord_t thickness);
    Rectangle_();

    void i_render(const GFX& gfx) override;
    Size i_min_size() override;
    Size i_max_size() override;

private:
    cord_t _thickness;
};

#endif
