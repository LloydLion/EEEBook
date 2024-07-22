#include "gui/UIElement.h"
#include "gui/UIContainer.h"
#include <stdexcept>

UIElement_::UIElement_()
{
    _o_max_size = Size::max_size();
    _o_min_size = Size();
}

GFX apply_alignment(Axis axis, const GFX &original, Size max_size, int alignment_code)
{
    if(Size::relate(original.size(), max_size, axis) & Coordinates::Relationship::PBigger)
    {
        cord_t start_position;
        switch (alignment_code)
        {
        case 0: // Axis start
            start_position = 0;
            break;

        case 2: // Axis end
            start_position = original.size()[axis] - max_size[axis];
            break;

        case 1: //Center
            start_position = (original.size()[axis] - max_size[axis]) / 2;
            break;
        }
        return original.slice(LocalBounds(Vector(start_position, 0, axis), original.size().with(max_size[axis], axis)));
    }
    else return original;
}

void UIElement_::render(const GFX& gfx)
{
    GFX new_gfx = gfx;

    Size max = max_size();
    new_gfx = apply_alignment(AxisX, new_gfx, max, (int)_p_alignment.horizontal);
    new_gfx = apply_alignment(AxisY, new_gfx, max, (int)_p_alignment.vertical);

    Size min = min_size();
    if (Size::relate(new_gfx.size(), min) & Coordinates::Relationship::SmallerAnyDimension)
    {
        UI_PRINT_SELF;
        std_printf("Element min size restrict! required min Size(%d, %d), reaming Size(%d, %d), provided Size(%d, %d)\n",
            min.width(), min.height(), new_gfx.size().width(), new_gfx.size().height(), gfx.size().width(), gfx.size().height());
        new_gfx.fill_screen(ColorMap::Black);
        return;
    }

    new_gfx = new_gfx.slice(margin());
    
    if (new_gfx.size() != _previous_viewport)
    {
        _previous_viewport = new_gfx.size();
        reset_cache(Render);
    }

    i_render(new_gfx);
}

Size UIElement_::resolve_min_size()
{   
    Size normal_min_size = margin().expand(i_min_size());
    return clamp_size(normal_min_size);
}

Size UIElement_::resolve_max_size()
{
    Size normal_max_size = margin().expand(i_max_size());
    return clamp_size(normal_max_size);
}

DEFAULT_PROPERTY_SETTER_IMPLEMENTATION_P(UIElement_, transparent_color_t, foreground_color, Render);
DEFAULT_PROPERTY_SETTER_IMPLEMENTATION_P(UIElement_, transparent_color_t, background_color, Render);

DEFAULT_PROPERTY_SETTER_IMPLEMENTATION_P(UIElement_, MarginSize, margin, Composition);
DEFAULT_PROPERTY_SETTER_IMPLEMENTATION_P(UIElement_, Alignment, alignment, Render);
DEFAULT_PROPERTY_SETTER_IMPLEMENTATION(UIElement_, HorizontalAlignment, horizontal_alignment, _p_alignment.horizontal, Render);
DEFAULT_PROPERTY_SETTER_IMPLEMENTATION(UIElement_, VerticalAlignment, vertical_alignment, _p_alignment.vertical, Render);

void UIElement_::visibility(UIVisibility value)
{
    if (value == _p_visibility) return;
    if (value == UIVisibility::Collapsed or _p_visibility == UIVisibility::Collapsed)
        trigger_mutation(Composition);
    else trigger_mutation(Render);

    _p_visibility = value;
}

void UIElement_::reset_cache(CacheChannel channel)
{
#if GUI_DEBUG_OPTIONS & GUI_STATE_DEBUG
    UI_PRINT_SELF;
    std_print("Cache has been reset, channel mask: ");
    for (size_t bit = 0; bit < 8; bit++)
        { std_print((bool)(channel & 0b10000000)); channel = (CacheChannel)(channel << 1); }
    std_println();
#endif
}

void UIElement_::finish_initialization()
{
    _cached_min_size = resolve_min_size();
    _cached_max_size = resolve_max_size();
}

void UIElement_::override_min_size(Size o_min_size)
{
    if (Size::relate(o_min_size, _o_max_size) & Coordinates::Relationship::SmallerAnyDimensionOrEqual == false)
        throw std::runtime_error("Min size override value cannot be bigger then max size override value");

    _o_min_size = o_min_size;
}

void UIElement_::override_max_size(Size o_max_size)
{
    if (Size::relate(o_max_size, _o_min_size) & Coordinates::Relationship::BiggerAnyDimensionOrEqual == false)
        throw std::runtime_error("Max size override value cannot be smaller then min size override value");

    _o_max_size = o_max_size;
}

void UIElement_::bind_parent(UIContainer parent)
{
    if (_p_parent)
        throw std::runtime_error("Tried to bind parent to element that already has parent");

    _p_parent = parent;
}

void UIElement_::unbind_parent(UIContainer parent)
{
    if (_p_parent != parent)
        throw std::runtime_error("Tried to unbind invalid parent");

    _p_parent = nullptr;
    reset_cache(CacheChannel::All);
}

void UIElement_::trigger_mutation(CacheChannel channel)
{
#if GUI_DEBUG_OPTIONS & GUI_STATE_DEBUG
    UI_PRINT_SELF;
    std_println("Element mutation triggered");
#endif
    reset_cache(channel);
    recalculate_composition();
}

void UIElement_::recalculate_composition()
{
    Size old_cached_max_size = _cached_max_size;
    Size old_cached_min_size = _cached_min_size;

    _cached_max_size = resolve_max_size();
    _cached_min_size = resolve_min_size();

    bool notify_required = false;
    notify_required |= old_cached_max_size != _cached_max_size;
    notify_required |= old_cached_min_size != _cached_min_size;

    if (notify_required)
    {
        if (_p_parent != nullptr)
        {
            _p_parent->c_notify_composition_changed(this);

#if GUI_DEBUG_OPTIONS & GUI_STATE_DEBUG
            UI_PRINT_SELF; std_println("Element composition changed, parent notified");
        }
        else
        {
            UI_PRINT_SELF; std_println("Element composition changed, no parent to be notified");
#endif
        }
    }
    else
    {
        UI_PRINT_SELF; std_println("Element composition recalculated, no changes");
    }
}

Size UIElement_::clamp_size(Size size)
{
    return Size::intersect(Size::combine(size, _o_min_size), _o_max_size);
}
