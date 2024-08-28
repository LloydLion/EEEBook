#include "gui/coordinates.h"

template<class TInheritor, class TCord>
CoordinateBase<TInheritor, TCord>::CoordinateBase(TCord p, TCord s, Axis p_axis)
{
    _elements[p_axis.index()] = p;
    _elements[(~p_axis).index()] = s;
}

template<class TInheritor, class TCord>
TCord CoordinateBase<TInheritor, TCord>::operator[](const Axis &axis) const
{
    return _elements[axis.index()];
}

template<class TInheritor, class TCord>
bool CoordinateBase<TInheritor, TCord>::operator==(const TInheritor &other) const
{
    return other._elements[0] == _elements[0] and other._elements[1] == _elements[1];
}

template<class TInheritor, class TCord>
bool CoordinateBase<TInheritor, TCord>::operator!=(const TInheritor &other) const
{
    return other._elements[0] != _elements[0] or other._elements[1] != _elements[1];
}

template<class TInheritor, class TCord>
Coordinates::Relationship CoordinateBase<TInheritor, TCord>::relate(TInheritor a, TInheritor b, Axis p_axis)
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

template<class TInheritor, class TCord>
TInheritor CoordinateBase<TInheritor, TCord>::with(TCord value, Axis axis)
{
    return TInheritor(value, this->operator[](~axis), axis);
}

template class CoordinateBase<Vector>;
template class CoordinateBase<SignedVector, s_cord_t>;
template class CoordinateBase<Size>;
