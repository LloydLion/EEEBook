#ifndef GUI_UI_ELEMENT_H
#define GUI_UI_ELEMENT_H

#include "coordinates.h"
#include "GFX.h"
#include "std/property.h"
#include "config.h"
#include "Alignment.h"
#include <Arduino.h>


#define UI_PRINT_SELF Serial.print("[UI="); Serial.print((uint32_t)this, 16); Serial.print("|"); if (name) Serial.print(name); Serial.print("]: ");

#define _UI_CACHE_SLOT_STATUS_FIELD(SLOT) __cache_status_##SLOT
#define _UI_CACHE_SLOT_VALUE_FIELD(SLOT) __cache_value_##SLOT
#define _UI_CACHE_SLOT_ACCESSOR_FUNCTION(SLOT) SLOT
#define _UI_CACHE_SLOT_RESOLVER(SLOT) __cache_resolve_##SLOT


#define DEFINE_CACHE_SLOT(TYPE, NAME, ...) bool _UI_CACHE_SLOT_STATUS_FIELD(NAME) = false; TYPE _UI_CACHE_SLOT_VALUE_FIELD(NAME); TYPE _UI_CACHE_SLOT_RESOLVER(NAME)(__VA_ARGS__)
#define DEFINE_CACHE_SLOT_ACCESSOR(TYPE, NAME, ...) TYPE _UI_CACHE_SLOT_ACCESSOR_FUNCTION(NAME)(__VA_ARGS__)

#if GUI_DEBUG_OPTIONS & GUI_STATE_DEBUG
    #define CONNECT_CACHE_CHANNEL(NAME, CHANNEL) UI_PRINT_SELF; if (channel & CHANNEL) { _UI_CACHE_SLOT_STATUS_FIELD(NAME) = false; Serial.print("\tReseting cache slot "); Serial.println(#NAME); } \
        else { Serial.print("\tCache slot reseting skipped "); Serial.println(#NAME); }

    #define IMPLEMENT_CACHE_SLOT(TYPE, CLASS, NAME, ARGS, ARGS_) \
        TYPE CLASS::_UI_CACHE_SLOT_ACCESSOR_FUNCTION(NAME)ARGS \
        { \
            UI_PRINT_SELF; \
            if (_UI_CACHE_SLOT_STATUS_FIELD(NAME)) \
            { \
                Serial.print("# Value restored from cache, slot: "); Serial.println(#NAME); \
                return _UI_CACHE_SLOT_VALUE_FIELD(NAME); \
            } \
            else \
            { \
                Serial.print("# Cache miss, resolving value using resolver, slot: "); Serial.println(#NAME); \
                _UI_CACHE_SLOT_STATUS_FIELD(NAME) = true; \
                return _UI_CACHE_SLOT_VALUE_FIELD(NAME) = _UI_CACHE_SLOT_RESOLVER(NAME)ARGS_; \
            } \
        } \
        TYPE CLASS::_UI_CACHE_SLOT_RESOLVER(NAME)ARGS
#else
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

#endif

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
    UIContainer _p_parent = nullptr;
    Alignment _p_alignment = Alignment();
    Size _o_max_size;
    Size _o_min_size;

    DEFINE_CACHE_SLOT(Size, min_size);
    DEFINE_CACHE_SLOT(Size, max_size);

    Size _previous_size = Size();

    Size clamp_size(Size size);

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

    UIElement_();

    const char *name = nullptr;

    PROPERTY(transparent_color_t, foreground_color) AUTO_GET(_p_foreground_color);
    PROPERTY(transparent_color_t, background_color) AUTO_GET(_p_background_color);
    PROPERTY(MarginSize, margin) AUTO_GET(_p_margin);
    PROPERTY(UIContainer, parent) AUTO_GET(_p_parent);
    PROPERTY(Alignment, alignment) AUTO_GET(_p_alignment);
    PROPERTY(VerticalAlignment, vertical_alignment) AUTO_GET(_p_alignment.vertical);
    PROPERTY(HorizontalAlignment, horizontal_alignment) AUTO_GET(_p_alignment.horizontal);
    
    void override_min_size(Size o_min_size);
    void override_max_size(Size o_max_size);

    void bind_parent(UIContainer parent);
    void unbind_parent(UIContainer parent);

    void render(const GFX& gfx);
    Size min_size();
    Size max_size();
};

#endif
