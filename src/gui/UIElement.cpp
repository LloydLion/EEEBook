#include "gui/UIElement.h"
#include "gui/UIContainer.h"

UIElement_::UIElement_()
{
    _o_max_size = Size::max_size();
    _o_min_size = Size();
}

void UIElement_::render(const GFX& gfx)
{
    GFX new_gfx = gfx;

    Size max = max_size();
    
    if(new_gfx.size() % max & Size::Relationship::Wider)
    {
        cord_t start_pos_x;
        switch (_p_alignment.horizontal)
        {
        case HorizontalAlignment::Left:
            start_pos_x = 0;
            break;

        case HorizontalAlignment::Right:
            start_pos_x = new_gfx.size().width - max.width;
            break;

        case HorizontalAlignment::Center:
            start_pos_x = (new_gfx.size().width - max.width) / 2;
            break;
        }
        new_gfx = new_gfx.slice(LocalBounds(Vector(start_pos_x, 0), Size(max.width, new_gfx.size().height)));

    }

    if(new_gfx.size() % max & Size::Relationship::Higher)
    {
        cord_t start_pos_y;
        switch (_p_alignment.vertical)
        {
        case VerticalAlignment::Top:
            start_pos_y = 0;
            break;

        case VerticalAlignment::Bottom:
            start_pos_y = new_gfx.size().height - max.height;
            break;

        case VerticalAlignment::Center:
            start_pos_y = (new_gfx.size().height - max.height) / 2;
            break;
        }
        new_gfx = new_gfx.slice(LocalBounds(Vector(0, start_pos_y), Size(new_gfx.size().width, max.height)));

    }


    Size min = min_size();
    if (new_gfx.size() % min & Size::Relationship::SmallerAnyDimension)
    {
        UI_PRINT_SELF;
        Serial.printf("min Size(%d, %d), realm Size(%d, %d), prov Size(%d, %d)",
            min.width, min.height, new_gfx.size().width, new_gfx.size().height, gfx.size().width, gfx.size().height);
        Serial.println(" Min size restrict!");
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
    Size normal_min_size = margin().expand(i_min_size());
    return clamp_size(normal_min_size);
}

IMPLEMENT_CACHE_SLOT(Size, UIElement_, max_size, (), ())
{
    Size normal_max_size = margin().safe_expand(i_max_size());
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
    if (o_min_size % _o_max_size & Size::Relationship::EqualOrSmaller == false)
        throw std::runtime_error("Min size override value cannot be bigger then max size override value");

    _o_min_size = o_min_size;
}

void UIElement_::override_max_size(Size o_max_size)
{
    if (o_max_size % _o_min_size & Size::Relationship::BiggerOrEqual == false)
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
