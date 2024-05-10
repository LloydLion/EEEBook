#ifndef GUI_UI_ELEMENT_H
#define GUI_UI_ELEMENT_H

#include "cordinates.h"
#include "GFX.h"
#include "std/property.h"
#include <Arduino.h>


#define _UI_CACHE_SLOT_STATUS_FIELD(SLOT) __cache_status_##SLOT
#define _UI_CACHE_SLOT_VALUE_FIELD(SLOT) __cache_value_##SLOT
#define _UI_CACHE_SLOT_ACCESSOR_FUNCTION(SLOT) SLOT
#define _UI_CACHE_SLOT_RESOLVER(SLOT) __cache_resolve_##SLOT

#define DEFINE_CACHE_SLOT(TYPE, NAME, ...) bool _UI_CACHE_SLOT_STATUS_FIELD(NAME); TYPE _UI_CACHE_SLOT_VALUE_FIELD(NAME); TYPE _UI_CACHE_SLOT_RESOLVER(NAME)(__VA_ARGS__)
#define CONNECT_CACHE_CHANNEL(NAME, CHANNEL) if (channel & CHANNEL) _UI_CACHE_SLOT_STATUS_FIELD(NAME) = false
#define IMPLEMENT_CACHE_SLOT(TYPE, CLASS, NAME, ARGS, ARGS_) \
    TYPE CLASS::_UI_CACHE_SLOT_ACCESSOR_FUNCTION(NAME)ARGS \
    { \
        if (_UI_CACHE_SLOT_STATUS_FIELD(NAME)) return _UI_CACHE_SLOT_VALUE_FIELD(NAME); \
        else \
        { \
            _UI_CACHE_SLOT_STATUS_FIELD(NAME) = true; \
            return _UI_CACHE_SLOT_VALUE_FIELD(NAME) = _UI_CACHE_SLOT_RESOLVER(NAME)ARGS_; \
        } \
    } \
    TYPE CLASS::_UI_CACHE_SLOT_RESOLVER(NAME)ARGS


class UIElement_;
typedef UIElement_ *UIElement;

class UIContainer_;
typedef UIContainer_ *UIContainer;

class UIElement_
{
private:
    transparent_color_t _p_foreground_color = color_t::Black;
    transparent_color_t _p_background_color = color_t::White;
    MarginSize _p_margin = MarginSize(0);
    UIContainer _p_parent;

    DEFINE_CACHE_SLOT(Size, min_size);
    DEFINE_CACHE_SLOT(Size, max_size);

    Size _previous_size = Size();

protected:
    enum CacheChannel : uint8_t
    {
        All = 0xFF,
        None = 0x00,

        Draw = 0b10000000,
        Composition = 0b01000000,
        Render = 0b00100000,
    };

    enum MutationType : uint8_t
    {
        DrawState,
        CompositionState,
    };

    virtual void i_render(const GFX& gfx) = 0;
    virtual Size i_min_size() = 0;
    virtual Size i_max_size() = 0;

    virtual void reset_cache(CacheChannel channel);
    void trigger_mutation(MutationType type);
    
public:
    PROPERTY(transparent_color_t, foreground_color) AUTO_GET(_p_foreground_color);
    PROPERTY(transparent_color_t, background_color) AUTO_GET(_p_background_color);
    PROPERTY(MarginSize, margin) AUTO_GET(_p_margin);
    PROPERTY(UIContainer, parent) AUTO_GET(_p_parent);

    void bind_parent(UIContainer parent);
    void unbind_parent(UIContainer parent);

    void render(const GFX& gfx);
    Size min_size();
    Size max_size();
};

#endif
