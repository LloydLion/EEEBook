#include "gui/drawing/UniversalDrawer.h"
#include "gui/drawing/sub_drawers/RectSubDrawer.h"
#include "gui/drawing/sub_drawers/LineSubDrawer.h"
#include "gui/drawing/sub_drawers/BitmapSubDrawer.h"
#include "gui/drawing/sub_drawers/TextSubDrawer.h"
#include <stdexcept>

UniversalDrawer_::UniversalDrawer_(Screen screen) : _screen(screen) {}

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
    DrawAreaType type = operation.area_type;
    switch (type)
    {
    case DrawAreaType::Rect:
    {
        RectSubDrawer drawer(operation.arguments.rect.thickness);
        drawer.output = _screen;
        drawer.bounds = operation.bounds;

        drawer.patterns.interior = operation.patterns[0];
        drawer.patterns.horizontal_strips = operation.patterns[1];
        drawer.patterns.vertical_strips = operation.patterns[2];
        drawer.patterns.corners = operation.patterns[3];

        drawer.draw();
    }
    break;
    case DrawAreaType::Ellipse:

        break;
    case DrawAreaType::Line:
    {
        auto args = operation.arguments.line;
        LineSubDrawer drawer(
            Vector(args.start_x, args.start_y),
            SignedVector(args.end_x, args.end_y),
            args.length, args.thickness);

        drawer.output = _screen;
        drawer.bounds = operation.bounds;

        drawer.pattern = operation.patterns[0];

        drawer.draw();
    }
    break;
    case DrawAreaType::Text:
    {
        auto arguments = operation.arguments.text;
        TextSubDrawer drawer(arguments.text, arguments.limit, arguments.font);

        drawer.output = _screen;
        drawer.bounds = operation.bounds;
        drawer.glyph_pattern = operation.patterns[0];
        drawer.background_pattern = operation.patterns[1];

        drawer.draw();
    }
    break;
    case DrawAreaType::Bitmap:
    {
        auto args = operation.arguments.bitmap;
        BitmapSubDrawer drawer(args.map, args.flags);

        drawer.output = _screen;
        drawer.bounds = operation.bounds;

        drawer.patterns.for_enabled = operation.patterns[0];
        drawer.patterns.for_disabled = operation.patterns[1];

        drawer.draw();
    }
    break;
    }
}