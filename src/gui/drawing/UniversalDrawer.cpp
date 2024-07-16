#include "gui/drawing/UniversalDrawer.h"
#include <stdexcept>

UniversalDrawer_::UniversalDrawer_(Screen screen): _screen(screen) { }

void UniversalDrawer_::draw(DrawOperationQueue queue)
{
    for (size_t i = 0; i < queue->size(); i++)
    {
        auto operation = queue->get(i);

        draw(operation);
    }
}

void UniversalDrawer_::draw(const DrawOperation &operation)
{
    //TODO
}
