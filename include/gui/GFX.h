#ifndef GUI_GFX_H
#define GUI_GFX_H 
#include "cordinates.h"


class GFX
{
protected:
    Bounds _bounds;

    Point local_to_absolute(Point local) const;
public:
    GFX(Bounds bounds);
    ~GFX();

    virtual void draw_rectangle(Bounds bounds) const = 0;
    virtual void print_text(Point start_point, char* text) const = 0;

    virtual GFX* slice(Bounds bounds) const = 0;

    Size size() const;
};

#endif
