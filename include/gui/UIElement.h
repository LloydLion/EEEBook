#include "common.h"
#include "GFX.h"

class UIElement_;
typedef UIElement_ *UIElement;

class UIElement_
{
public:
    UIElement_();
    ~UIElement_();

    virtual void render(GFX gfx) = 0;

    virtual boolean is_updated() = 0;
    virtual boolean is_visible() = 0;
    virtual Size min_size() = 0;
};
