#include "gui/UIElement.h"
#include "gui/UIContainer.h"

void UIElement_::render(const GFX& gfx)
{
    GFX ngfx = gfx;

    Size max = max_size();
    if (!max.is_null() and ngfx.size() > max)
    {
        //TODO: alignment logic
        ngfx = ngfx.slice(LocalBounds(Vector(), Size::intersect(max, ngfx.size())));
    }

    Size min = min_size();
    if (ngfx.size() < min)  
    {
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

void UIElement_::reset_cache(CacheChannel channel)
{
    CONNECT_CACHE_CHANNEL(min_size, CacheChannel::Composition);
    CONNECT_CACHE_CHANNEL(max_size, CacheChannel::Composition);
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
    switch (type)
    {
    case MutationType::CompositionState:
        if (_p_parent != nullptr)
            _p_parent->c_notify_composition_mutation(this);
        reset_cache(Composition);
        break;

    case MutationType::DrawState:
        reset_cache(Draw);
        break;
    
    default:
        break;
    }
}
