#include "gui/coordinates.h"

template<>
Axis Axis::create<0>()
{
    Axis result = Axis();
    result._index = 0;
    return result;
}

template<>
Axis Axis::create<1>()
{
    Axis result = Axis();
    result._index = 1;
    return result;
}

Axis Axis::operator~() const
{
    return _index == 0 ? create<1>() : create<0>();
}

uint8_t Axis::index() const
{
    return _index;
}


Axis AxisX = Axis::create<0>();
Axis AxisY = Axis::create<1>();
