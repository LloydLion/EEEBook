#include "gui/UIElement.h"
#include "gui/UIContainer.h"

void UIElement_::render(const GFX& gfx)
{
    GFX ngfx = gfx;

    Size max = max_size();
        //TODO: alignment logic
        if(ngfx.size().width > max.width and max.width != 0)
        {
            cord_t start_pos_x;
            switch (_p_alignment.horizontal)
            {
            case HorizontalAlignment::Left:
                start_pos_x = 0;
                break;

            case HorizontalAlignment::Right:
                start_pos_x = ngfx.size().width - max.width;
                break;

            case HorizontalAlignment::Center:
                start_pos_x = (ngfx.size().width - max.width) / 2;
                break;
            }
            ngfx = ngfx.slice(LocalBounds(Vector(start_pos_x, 0), Size(max.width, ngfx.size().height)));

        }

        if(ngfx.size().height > max.height and max.height != 0)
        {
            cord_t start_pos_y;
            switch (_p_alignment.vertical)
            {
            case VerticalAlignment::Top:
                start_pos_y = 0;
                break;

            case VerticalAlignment::Bottom:
                start_pos_y = ngfx.size().height - max.height;
                break;

            case VerticalAlignment::Center:
                start_pos_y = (ngfx.size().height - max.height) / 2;
                break;
            }
            ngfx = ngfx.slice(LocalBounds(Vector(0, start_pos_y), Size(ngfx.size().height, max.height)));

        }


    Size min = min_size();
    if (ngfx.size() < min)
    {
        UI_PRINT_SELF;
        Serial.printf("min Size(%d, %d), realm Size(%d, %d), prov Size(%d, %d)",
            min.width, min.height, ngfx.size().width, ngfx.size().height, gfx.size().width, gfx.size().height);
        Serial.println(" Min size restrict!");
        ngfx.fill_screen(color_t::Black);
        return;
    }

    ngfx = ngfx.slice(margin());
    
    if (ngfx.size() != _previous_size)
    {
        _previous_size = ngfx.size();
        reset_cache(Render);
    }

    i_render(ngfx);
}

IMPLEMENT_CACHE_SLOT(Size, UIElement_, min_size, (), ())
{
    return margin().expand(i_min_size());
}

IMPLEMENT_CACHE_SLOT(Size, UIElement_, max_size, (), ())
{
    Size max_size = i_max_size();
    return max_size.is_null() ? Size() : margin().expand(max_size);
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
