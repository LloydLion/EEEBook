#ifndef GUI_ELEMENTS_RECTANGLE_H
#define GUI_ELEMENTS_RECTANGLE_H 

#include "../coordinates.h"
#include "../UIElement.h"


class Rectangle_;
typedef Rectangle_ *Rectangle;

class Rectangle_ : public UIElement_
{
private:
    cord_t _p_thickness;

public:
    Rectangle_(cord_t thickness);
    Rectangle_();

    PROPERTY(cord_t, thickness) AUTO_GET(_p_thickness);

protected:
    void i_render(const GFX& gfx) override;
    Size i_min_size() override;
    Size i_max_size() override;
};

#endif
