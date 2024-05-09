#include "gui/UIElement.h"

void UIElement_::render(const GFX& gfx)
{
    int id = (int)this;
    GFX ngfx = gfx;
    Serial.printf("EL=%d: ", id);
    Serial.printf("Render to (%d, %d)", ngfx.size().width, ngfx.size().height);
    Serial.println();

    Serial.printf("EL=%d: ", id);
    Serial.println("Render - 1");
    Size max = max_size();
    if (ngfx.size() > max)
    {
        Serial.printf("EL=%d: ", id);
        Serial.println("Render - max size triggered");
        //TODO: alignment logic
        ngfx = ngfx.slice(LocalBounds(Vector(), Size::intersect(max, ngfx.size())));
    }

    Serial.printf("EL=%d: ", id);
    Serial.printf("Render - 2 (%d, %d)", ngfx.size().width, ngfx.size().height);
    Serial.println();
    Size min = min_size();
    if (ngfx.size() < min)  
    {
        Serial.printf("EL=%d: ", id);
        Serial.println("Render - min size triggered");
        ngfx.fill_screen(color_t::Black);
        return;
    }

    Serial.printf("EL=%d: ", id);
    Serial.printf("Render - 3 (%d, %d)", ngfx.size().width, ngfx.size().height);
    Serial.println();

    ngfx = ngfx.slice(margin);

    Serial.printf("EL=%d: ", id);   
    Serial.println("Preporations finished, Render initialized");
    
    i_render(ngfx);

    Serial.printf("EL=%d: ", id);
    Serial.println("Render finished");
}

Size UIElement_::min_size()
{
    return margin.expand(i_min_size());
}

Size UIElement_::max_size()
{
    Size max_size = i_max_size();
    if (max_size.is_null())
        return Size();
    return margin.expand(max_size);
}
