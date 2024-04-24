#include "common.h"
#include "Point.h"
#include "Size.h"

class GFX_;
typedef GFX_ *GFX;

class GFX_
{
protected:
    Point _absolute_point;
    Size _max_size;
public:
    GFX_(Point absolute_point, Size max_size);
    ~GFX_();

    virtual void print_text(Point start_point, wchar_t* text) = 0;

    virtual GFX slice(Point start_point, Size size) = 0;
};
