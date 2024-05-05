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

    void render(const GFX& gfx) override;

    Size min_size() override;

private:
    cord_t _thickness;
};

#endif
