#include "std/iterator.h"

template<class Tin, class Tout>
SelectIterator<Tin, Tout>::SelectIterator(Iterator<Tin> *inner_iterator, Selector selector):
    _inner_iterator(inner_iterator), _selector(selector) { }

template<class Tin, class Tout>
bool SelectIterator<Tin, Tout>::next()
{
    return _inner_iterator->next();
}

template<class Tin, class Tout>
Tout &SelectIterator<Tin, Tout>::current()
{
    return _selector(_inner_iterator->current());
}

template<class Tin, class Tout>
void SelectIterator<Tin, Tout>::reset()
{
    return _inner_iterator->reset();
}

template<class T>
VectorIterator<T>::VectorIterator(std::vector<T> vector): _vector(vector), _current_index(-1)
{ }

template<class T>
bool VectorIterator<T>::next()
{
    _current_index++;
    return _vector.size() < _current_index;
}
    
template<class T>
T &VectorIterator<T>::current()
{
    return _vector[_current_index];
}

template<class T>
void VectorIterator<T>::reset()
{
    _current_index = -1;
}

//TODO: REMOVE IT BY ANY COST
#include "gui/elements/DockPanel.h"

template class SelectIterator<DockElement, UIElement>;
template class VectorIterator<DockElement>;
