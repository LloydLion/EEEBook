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

template<class Tin, class Tout>
class SelectIterator : public Iterator<Tout>
{
public:
    typedef Tout &(*Selector)(Tin&);

    SelectIterator(Iterator<Tin> *inner_iterator, Selector selector);

    bool next() override;
    Tout &current() override;
    void reset() override;

private:
    Iterator<Tin> *_inner_iterator;
    Selector _selector;
};

template<class T>
class VectorIterator : public Iterator<T>
{
private:
    std::vector<T> _vector;
    size_t _current_index;
public:
    VectorIterator(std::vector<T> vector);

    bool next() override;
    T &current() override;
    void reset() override;
};

#endif
