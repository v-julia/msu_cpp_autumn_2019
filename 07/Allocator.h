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
    constexpr Allocator() noexcept {}
    constexpr Allocator(const Allocator&) noexcept = default;
    template <class U> constexpr Allocator(const Allocator<U>&) noexcept {}

    template <class U> struct rebind { using other = Allocator<U>; };

    T* address(T& value) const noexcept { return std::addressof(value); }
    const T* address(const T& value) const noexcept { return std::addressof(value); }


    T* allocate(size_t n)
    {
        if ( n > max_size() )
            throw std::bad_alloc();
        T* ptr = static_cast<T*>( ::operator new( sizeof(T) * n ) );
        if ( !ptr )
            throw std::bad_alloc();
        return ptr;
    }

    T* allocate(size_t n, const void*) { return allocate(n); }


    void deallocate(pointer ptr, size_t count)
    {
        ::operator delete( ptr, sizeof(T) * count );
    }


    void construct(T* const ptr, T&& value)
    {
        if ( ptr )
            ::new ( ptr ) T(std::forward<T>(value));
    }

    void destroy(T* const ptr) {
        if ( ptr ) 
            ptr->~T(); 
    };


    size_t max_size() const noexcept
    {
        return static_cast<size_t>( -1 ) / sizeof(T);
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

