#pragma once


#include <memory>

// требования к аллокатору:
// должно быть size_type, rebind, allocate, deallocate, ==, != (минимум)

template <class T>
class Allocator
{
public:
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;


public:
    Allocator() = default;
    template <class U> constexpr Allocator(const Allocator<U>&) noexcept {}

    T* allocate(std::size_t n)
    {
        if ( n > (std::size_t(-1) / sizeof(T) ) ) throw std::bad_alloc();
        return new T[n];
    }


    void deallocate(pointer ptr, std::size_t count)
    {
        delete[] ptr;
    }



};
template <typename T, typename U>
bool operator == (const Allocator<T>&, const Allocator<U>&)
{
    return true;
}

template <typename T, typename U>
bool operator != (const Allocator<T>& a, const Allocator<U>& b)
{
    return !( a == b );
}

