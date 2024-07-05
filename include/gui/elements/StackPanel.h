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
    cord_t _p_spacing = 0;

public:
    StackPanel_(std::vector<UIElement> elements);

    PROPERTY(Axis, orientation) AUTO_GET(_p_orientation);
    PROPERTY(cord_t, spacing) AUTO_GET(_p_spacing);

    Iterator<UIElement> *list_children() override;
    size_t count_children() override;

    void i_render(const GFX& gfx) override;
    Size i_min_size() override;
    Size i_max_size() override;

    void add_child(UIElement child, size_t index);
    inline void add_child(UIElement child) { return add_child(child, count_children()); }
    void modify_child(UIElement child, size_t index);
    void swap_children(UIElement child_a, UIElement child_b);
    void remove_child(UIElement child) override;
};

#endif
