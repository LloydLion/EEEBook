#ifndef GUI_RENDER_BUILTIN_IMAGE_H
#define GUI_RENDER_BUILTIN_IMAGE_H

#include "gui/coordinates.h"
#include "gui/UIElement.h"
#include "gui/Bitmap.h"

class Image_;
typedef Image_ *Image;

class Image_ : public UIElement_
{
private:
    Bitmap bitmap;
public:
    Image_(Bitmap bitmap);

    void render(const GFX& gfx) override;
    Size min_size() override;
};

#endif