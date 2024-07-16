#ifndef STD_UNIVERSAL_PARAMETERS_H
#define STD_UNIVERSAL_PARAMETERS_H

#include "std/byte.h"

template<size_t size>
struct UniversalParameters
{
    byte data[size];

    template<class T>
    T read(size_t &position)
    {
        void *pointer = data[position];
        T *data = (T*)pointer;
        position += sizeof(T);
        return *data;
    }

    template<class T>
    void write(size_t &position, const T &value)
    {
        void *pointer = data[position];
        T *data = (T*)pointer;
        position += sizeof(T);

        *data = T(value);
    }

    template<class T>
    void write(size_t &position, T &&value)
    {
        void *pointer = data[position];
        T *data = (T*)pointer;
        position += sizeof(T);

        *data = T(value);
    }
};


#endif
