#ifndef GUI_RENDER_BUILTIN_FRAME_H
#define GUI_RENDER_BUILTIN_FRAME_H

#include "gui/coordinates.h"
#include "gui/render/UIView.h"


class Frame_;
typedef Frame_ *Frame;

class Frame_ : public UIView_
{
public:
    Frame_(cord_t thickness, UIElement element);

    PROPERTY(cord_t, thickness) AUTO_GET(_p_thickness);

protected:
    void i_render(const GFX& gfx) override;
    Size i_min_size() override;
    Size i_max_size() override;

private:
    cord_t _p_thickness;
};

#endif
