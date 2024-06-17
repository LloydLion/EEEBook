#ifndef GUI_ELEMENTS_H
#define GUI_ELEMENTS_H

#include "gui/UIComposer.h"

class StackPanel_;
typedef StackPanel_ *StackPanel;

class StackPanel_ : public UIComposer_
{
private:
    std::vector<UIElement> _elements;
    VectorIterator<UIElement> _elements_iterator;
    Axis _p_orientation = AxisY;

public:
    StackPanel_(std::vector<UIElement> elements);

    PROPERTY(Axis, orientation) AUTO_GET(_p_orientation);

    Iterator<UIElement> *list_children() override;
    size_t count_children() override;

    void i_render(const GFX& gfx) override;    
    Size i_min_size() override;    
    Size i_max_size() override;
};

#endif
