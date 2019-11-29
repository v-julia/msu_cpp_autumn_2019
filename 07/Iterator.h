#pragma once

#include <iterator>


template <class T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T>
{
public:
    using iterator_category = typename std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = typename std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

private:
    T* ptr_;

public:

    Iterator() : ptr_(nullptr) {}
    explicit Iterator(pointer ptr) : ptr_(ptr) {}
    Iterator(const Iterator& other) : ptr_(other.ptr_) {}


    // i[n]
    reference operator[](difference_type n) const { return ptr_[n]; }
    reference operator*() const { return *ptr_; }
    const pointer operator->() const { return ptr_; }

    Iterator& operator++() { ++ptr_; return *this; }
    Iterator operator++() const { Iterator tmp(*this); ++ptr_; return tmp; }

    Iterator& operator--() { --ptr_; return *this; }
    Iterator operator--() const { Iterator tmp(*this); --ptr_; return tmp; }

    //r += n, a + n, n + a
    Iterator& operator+=(difference_type n) { ptr_ += n; return *this; }
    Iterator operator+(difference_type n) const { return Iterator(ptr_ + n); }
    friend Iterator operator+(difference_type n, const Iterator& other) { return Iterator(n + other.ptr_); }

    //r -= n, i - n, b - a
    Iterator& operator-=(difference_type n) { ptr_ -= n; return *this; }
    difference_type operator-(const Iterator& other) const { return ( static_cast<difference_type>(ptr_ - other.ptr_) ); }
    Iterator operator-(difference_type n) const { return Iterator(ptr_ - n); }

    //a < b a > b a >= b a <= b
    bool operator==(const Iterator& other) const { return ptr_ == other.ptr_; }
    bool operator!=(const Iterator& other) const { return !( *this == other ); }
    bool operator>(const Iterator& other) const { return ptr_ > other.ptr_; }
    bool operator<(const Iterator& other) const { return ptr_ < other.ptr_; }
    bool operator>=(const Iterator& other) const { return ptr_ >= other.ptr_; }
    bool operator<=(const Iterator& other) const { return ptr_ <= other.ptr_; }

};

