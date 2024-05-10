#ifndef STD_ITERATOR_H
#define STD_ITERATOR_H
#include <vector>

template<class T>
class Iterator
{
public:
    virtual bool next() = 0;
    virtual T &current() = 0;
    virtual void reset() = 0;
};

template<class Tbase, class Tin, class Tout>
class SelectIterator : public Iterator<Tout>
{
public:
    static_assert(std::is_base_of<Iterator<Tin>, Tbase>::value, "Tbase must inherit Iterator<Tin>");

    typedef Tout &(*Selector)(Tin&);

private:
    Tbase _inner_iterator;
    Selector _selector;

public:
    SelectIterator(Tbase inner_iterator, Selector selector):
        _inner_iterator(inner_iterator), _selector(selector) { }

    bool next() override
    { return _inner_iterator.next(); }

    Tout &current() override
    { return _selector(_inner_iterator.current()); }

    void reset() override
    { return _inner_iterator.reset(); }
};

template<class T>
class VectorIterator : public Iterator<T>
{
private:
    std::vector<T> _vector;
    size_t _current_index;
public:
    VectorIterator(std::vector<T> vector): _vector(vector), _current_index(-1) { }

    bool next() override
    {
        _current_index++;
        return _vector.size() < _current_index;
    }

    T &current() override
    { return _vector[_current_index]; }

    void reset() override
    { _current_index = -1; }
};

template<class T>
class SingleElementIterator : public Iterator<T>
{
private:
    T *_element;
    bool _state;
public:
    SingleElementIterator(T *element): _element(element) { }

    bool next() override
    {   
        if (_state == false)
            return _state = true;
        else return false;
    }

    T &current() override
    { return *_element; }

    void reset() override
    { _state = false; }
};

#endif
