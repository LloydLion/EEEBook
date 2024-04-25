#include "gui/GFX.h"

GFX::GFX(Bounds bounds): _bounds(bounds)
{

}

GFX::~GFX()
{
    
}

Point GFX::local_to_absolute(Point local) const
{
    Point result;
    result.x = local.x + _bounds.start_point.x;
    result.y = local.y + _bounds.start_point.y;
    return result;
}

Size GFX::size() const
{
    return _bounds.size;
}
