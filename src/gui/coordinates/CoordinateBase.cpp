#include "gui/coordinates.h"

template<class TInheritor>
CoordinateBase<TInheritor>::CoordinateBase(cord_t p, cord_t s, Axis p_axis)
{
    _elements[p_axis.index()] = p;
    _elements[(~p_axis).index()] = s;
}

template<class TInheritor>
cord_t CoordinateBase<TInheritor>::operator[](const Axis &axis) const
{
    return _elements[axis.index()];
}

template<class TInheritor>
bool CoordinateBase<TInheritor>::operator==(const TInheritor &other) const
{
    return other._elements[0] == _elements[0] and other._elements[1] == _elements[1];
}

template<class TInheritor>
bool CoordinateBase<TInheritor>::operator!=(const TInheritor &other) const
{
    return other._elements[0] != _elements[0] or other._elements[1] != _elements[1];
}

template<class TInheritor>
Coordinates::Relationship CoordinateBase<TInheritor>::relate(TInheritor a, TInheritor b, Axis p_axis)
{
    if (a[p_axis] > b[p_axis] && a[~p_axis] > b[~p_axis])
        return Coordinates::Relationship::Bigger;
    else if (a[p_axis] > b[p_axis] && a[~p_axis] == b[~p_axis])
        return Coordinates::Relationship::PBiggerSEqual;
    else if (a[p_axis] > b[p_axis] && a[~p_axis] < b[~p_axis])
        return Coordinates::Relationship::PBiggerSSmaller;

    else if (a[p_axis] == b[p_axis] && a[~p_axis] > b[~p_axis])
        return Coordinates::Relationship::PEqualSBigger;
    else if (a[p_axis] == b[p_axis] && a[~p_axis] == b[~p_axis])
        return Coordinates::Relationship::Equal;
    else if (a[p_axis] == b[p_axis] && a[~p_axis] < b[~p_axis])
        return Coordinates::Relationship::PEqualSSmaller;

    else if (a[p_axis] < b[p_axis] && a[~p_axis] > b[~p_axis])
        return Coordinates::Relationship::PSmallerSBigger;
    else if (a[p_axis] < b[p_axis] && a[~p_axis] == b[~p_axis])
        return Coordinates::Relationship::PSmallerSEqual;
    else //if (a[p_axis] < b[p_axis] && a[~p_axis] < b[~p_axis])
        return Coordinates::Relationship::Smaller;
}

template<class TInheritor>
TInheritor CoordinateBase<TInheritor>::with(cord_t value, Axis axis)
{
    return TInheritor(value, this->operator[](~axis), axis);
}

template class CoordinateBase<Vector>;
template class CoordinateBase<Size>;
