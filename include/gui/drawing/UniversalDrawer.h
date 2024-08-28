#ifndef GUI_DRAWING_UNIVERSAL_DRAWER_H
#define GUI_DRAWING_UNIVERSAL_DRAWER_H

#include "gui/drawing/Screen.h"
#include "gui/drawing/DrawOperationQueue.h"

class UniversalDrawer_;
typedef UniversalDrawer_ *UniversalDrawer;

class UniversalDrawer_
{
private:
    Screen _screen;

public:
    UniversalDrawer_(Screen screen);

    void draw(DrawOperationQueue queue);
    void draw(const DrawOperation &operation);
};

#endif
