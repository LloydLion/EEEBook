#include "gui/UIContainer.h"


GFX UIContainer_::assume_padding(const GFX &gfx)
{
    return gfx.slice(padding);
}
