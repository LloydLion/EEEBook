#ifndef STD_UNIVERSAL_PARAMETERS_H
#define STD_UNIVERSAL_PARAMETERS_H

#include "std/byte.h"
#include <cstddef>
#include <stdexcept>

template<size_t size>
struct UniversalParameters
{
    byte data[size];

    template<class T>
    T &read(size_t &position)
    {
        if (position + sizeof(T) >= size)
            throw std::runtime_error("UniversalParameters instance too small");
        void *pointer = data + position;
        T *data = (T*)pointer;
        position += sizeof(T);
        return *data;
    }
    
    template<class T>
    T &read(size_t position)
    {
        if (position + sizeof(T) >= size)
            throw std::runtime_error("UniversalParameters instance too small");
        return *(T*)(data + position);
    }

    template<class T>
    T &as()
    {
        if (sizeof(T) >= size)
            throw std::runtime_error("UniversalParameters instance too small");
        return *(T*)(data);
    }

    template<class T>
    void write(size_t &position, const T &value)
    {
        if (position + sizeof(T) >= size)
            throw std::runtime_error("UniversalParameters instance too small");
        void *pointer = data + position;
        T *data = (T*)pointer;
        position += sizeof(T);

        *data = T(value);
    }

    template<class T>
    void write(size_t &position, T &&value)
    {
        if (position + sizeof(T) >= size)
            throw std::runtime_error("UniversalParameters instance too small");
        void *pointer = data + position;
        T *data = (T*)pointer;
        position += sizeof(T);

        *data = T(value);
    }

    template<class T>
    void write(size_t position, const T &value)
    {
        if (position + sizeof(T) >= size)
            throw std::runtime_error("UniversalParameters instance too small");
        *(T*)(data + position) = T(value);
    }
    
    template<class T>
    void write(size_t position, T &&value)
    {
        if (position + sizeof(T) >= size)
            throw std::runtime_error("UniversalParameters instance too small");
        *(T*)(data + position) = T(value);
    }

    template<class T>
    void set(const T &value)
    {
        if (sizeof(T) >= size)
            throw std::runtime_error("UniversalParameters instance too small");
        memset(data, 0, size);
        *(T*)(data) = T(value);
    }

    template<class T>
    void set(T &&value)
    {
        if (sizeof(T) >= size)
            throw std::runtime_error("UniversalParameters instance too small");
        memset(data, 0, size);
        *(T*)(data) = T(value);
    }
};


#endif
