#include "gui/UIElement.h"
#include "gui/UIContainer.h"

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
    if (_p_visibility != UIVisibility::Visible)
        return;

    GFX new_gfx = gfx;

    Size max = max_size();
    new_gfx = apply_alignment(AxisX, new_gfx, max, (int)_p_alignment.horizontal);
    new_gfx = apply_alignment(AxisY, new_gfx, max, (int)_p_alignment.vertical);

    Size min = min_size();
    if (Size::relate(new_gfx.size(), min) & Coordinates::Relationship::SmallerAnyDimension)
    {
        UI_PRINT_SELF;
        Serial.printf("Element min size restrict! required min Size(%d, %d), reaming Size(%d, %d), provided Size(%d, %d)\n",
            min.width(), min.height(), new_gfx.size().width(), new_gfx.size().height(), gfx.size().width(), gfx.size().height());
        new_gfx.fill_screen(color_t::Black);
        return;
    }

    new_gfx = new_gfx.slice(margin());
    
    if (new_gfx.size() != _previous_size)
    {
        _previous_size = new_gfx.size();
        reset_cache(Render);
    }

    i_render(new_gfx);
}

IMPLEMENT_CACHE_SLOT(Size, UIElement_, min_size, (), ())
{
    if (_p_visibility == UIVisibility::Collapsed) return Size();
    
    Size normal_min_size = margin().expand(i_min_size());
    return clamp_size(normal_min_size);
}

IMPLEMENT_CACHE_SLOT(Size, UIElement_, max_size, (), ())
{
    if (_p_visibility == UIVisibility::Collapsed) return Size();

    Size normal_max_size = margin().expand(i_max_size());
    return clamp_size(normal_max_size);
}

void UIElement_::foreground_color(transparent_color_t value)
{
    trigger_mutation(DrawState);
    _p_foreground_color = value;
}

void UIElement_::background_color(transparent_color_t value)
{
    trigger_mutation(DrawState);
    _p_background_color = value;
}

void UIElement_::margin(MarginSize value)
{
    trigger_mutation(CompositionState);
    _p_margin = value;
}

void UIElement_::alignment(Alignment value)
{
    trigger_mutation(DrawState);
    _p_alignment = value;
}

void UIElement_::horizontal_alignment(HorizontalAlignment value)
{
    trigger_mutation(DrawState);
    _p_alignment.horizontal = value;
}

void UIElement_::vertical_alignment(VerticalAlignment value)
{
    trigger_mutation(DrawState);
    _p_alignment.vertical = value;
}

void UIElement_::visibility(UIVisibility value)
{
    if (value == _p_visibility) return;
    if (value == UIVisibility::Collapsed or _p_visibility == UIVisibility::Collapsed)
        trigger_mutation(CompositionState);
    else trigger_mutation(DrawState);

    _p_visibility = value;
}

void UIElement_::reset_cache(CacheChannel channel)
{
    CONNECT_CACHE_CHANNEL(min_size, CacheChannel::Composition);
    CONNECT_CACHE_CHANNEL(max_size, CacheChannel::Composition);
#if GUI_DEBUG_OPTIONS & GUI_STATE_DEBUG
    UI_PRINT_SELF;
    Serial.print("Cache has been reset, channel mask: ");
    for (size_t bit = 0; bit < 8; bit++)
        { Serial.print((bool)(channel & 0b10000000)); channel = (CacheChannel)(channel << 1); }
    Serial.println();
#endif
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

void UIElement_::trigger_mutation(MutationType type)
{
#if GUI_DEBUG_OPTIONS & GUI_STATE_DEBUG
    UI_PRINT_SELF;
    Serial.print("Element mutation triggered. Type: ");
#endif
    switch (type)
    {
    case MutationType::CompositionState:
    
#if GUI_DEBUG_OPTIONS & GUI_STATE_DEBUG
        if (_p_parent != nullptr)
            Serial.println("CompositionState, parent will be notified");
        else Serial.println("CompositionState, no parent to be notified");
#endif
        if (_p_parent != nullptr)
            _p_parent->c_notify_composition_mutation(this);
        reset_cache(Composition);
        break;

    case MutationType::DrawState:
#if GUI_DEBUG_OPTIONS & GUI_STATE_DEBUG
        Serial.println("DrawState");
#endif
        reset_cache(Draw);
        break;
    
    default:
        break;
    }
}

Size UIElement_::clamp_size(Size size)
{
    return Size::intersect(Size::combine(size, _o_min_size), _o_max_size);
}
