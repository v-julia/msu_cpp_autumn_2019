#pragma once


#include <iterator>



template <class T, class Alloc = std::allocator<T> >
class Vector
{
private:
    Alloc alloc_;

public:
    using size_type = size_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;
    using allocator_type = Alloc;

    using iterator = T*;
    using const_iterator = const iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    pointer begin_ptr;
    pointer end_ptr;
    pointer capacity_ptr;


public:

    // конструкторы 

    Vector() : begin_ptr(nullptr), end_ptr(nullptr), capacity_ptr(nullptr) {}

    explicit Vector(size_type count) : Vector<T>(count, T()) {}

    Vector(size_type count, const value_type& value)
    {
        begin_ptr = alloc_.allocate(count);
        end_ptr = std::uninitialized_fill_n(begin_ptr, count, value);
        capacity_ptr = begin_ptr + count;
    }

    Vector(std::initializer_list<T> initializer)
    {
        allocate_and_copy(initializer.begin(), initializer.end());
    }

    Vector(const Vector<T>& other)
    {
        allocate_and_copy(other.cbegin(), other.cend());
    }

    Vector(Vector<T>&& other) noexcept : begin_ptr(other.begin_ptr), end_ptr(other.end_ptr), capacity_ptr(other.capacity_ptr)
    {
        other.begin_ptr = nullptr;
        other.end_ptr = nullptr;
        other.capacity_ptr = nullptr;
    }

    Vector<T>& operator=(const Vector<T>& other)
    {
        Vector<T> temp(other);
        swap(temp);
        return *this;
    }

    Vector<T>& operator=(Vector<T>&& other) noexcept
    {
        if ( this != &other ) {
            deallocate();
            begin_ptr = other.begin_ptr;
            end_ptr = other.end_ptr;
            capacity_ptr = other.capacity_ptr;
            other.begin_ptr = nullptr;
            other.end_ptr = nullptr;
            other.capacity_ptr = nullptr;
        }
        return *this;
    }

    ~Vector() { deallocate(); }




    // методы: begin, end, rbegin, rend

    iterator begin() noexcept { return iterator(begin_ptr); }

    const_iterator сbegin() const noexcept { return iterator(begin_ptr); };

    iterator end() noexcept { return end_ptr; }

    const_iterator сend() const noexcept { return end_ptr; }

    reverse_iterator rbegin()  noexcept { return reverse_iterator(end()); }

    reverse_iterator rend()  noexcept { return reverse_iterator(begin()); }

    const_reverse_iterator сrbegin() const noexcept { return reverse_iterator(сend()); }

    const_reverse_iterator сrend() const noexcept { return reverse_iterator(сbegin()); }



    // методы: [], at

    reference operator[](size_type idx) { return begin_ptr[idx]; }

    const_reference operator[](size_type idx) const { return begin_ptr[idx]; }

    reference at(size_type idx)
    {
        if ( idx < 0 || idx >= size() ) {
            throw std::out_of_range("Invalid index");
        }
        return begin_ptr[idx];
    }

    const_reference at(size_type idx) const
    {
        if ( idx < 0 || idx >= size() ) {
            throw std::out_of_range("Invalid index");
        }
        return begin_ptr[idx];
    }


    // методы: push_back, pop_back

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        reallocate_if_full();

        std::allocator_traits<Alloc>::construct(alloc_, end_ptr, std::forward<Args>(args)...);
        ++end_ptr;
    }

    void push_back(const value_type& value) { emplace_back(value); }

    void push_back(value_type&& value) { emplace_back(std::move(value)); }

    void pop_back()
    {
        --end_ptr;
        std::allocator_traits<Alloc>::destroy(alloc_, end_ptr);
    }

    // методы: size, resize, reserve, clear

    size_type size() const noexcept { return static_cast<size_type>( end_ptr - begin_ptr ); }

    bool empty() const noexcept { return size() == 0; }

    size_type capacity() const noexcept { return static_cast<size_type>( capacity_ptr - begin_ptr ); }

    void reserve(size_type new_capacity)
    {
        if ( new_capacity <= capacity() ) {
            return;
        }
        reallocate(new_capacity);
    }

    void resize(size_type new_size, const_reference value)
    {
        if ( new_size > capacity() ) {
            reallocate(2 * new_size);
            end_ptr = std::uninitialized_fill_n(end_ptr, new_size - size(), value);
        }
        else if ( new_size > size() ) {
            end_ptr = std::uninitialized_fill_n(end_ptr, new_size - size(), value);
        }
        else if ( new_size < size() ) {
            for ( size_type i = 0; i < size() - new_size; ++i ) {
                --end_ptr;
                std::allocator_traits<Alloc>::destroy(alloc_, end_ptr);
            }
        }
    }

    void resize(size_type new_size) { resize(new_size, T()); }

    void shrink_to_fit() { reallocate(size()); }


    void clear() noexcept
    {
        deallocate();
        begin_ptr = nullptr;
        end_ptr = nullptr;
        capacity_ptr = nullptr;
    }

    void swap(Vector<T>& vector) noexcept
    {
        std::swap(this->begin_ptr, vector.begin_ptr);
        std::swap(this->end_ptr, vector.end_ptr);
        std::swap(this->capacity_ptr, vector.capacity_ptr);
    }


private:

    void reallocate(size_type new_capacity)
    {
        T* new_begin_ptr = alloc_.allocate(new_capacity);

        T* new_end_ptr = std::uninitialized_copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), new_begin_ptr);

        deallocate();

        begin_ptr = new_begin_ptr;
        end_ptr = new_end_ptr;
        capacity_ptr = begin_ptr + new_capacity;
    }

    void reallocate_if_full()
    {
        if ( size() == capacity() ) {
            size_type new_capacity = ( size() != 0 ) ? 2 * size() : 1;
            reallocate(new_capacity);
        }
    }

    void deallocate()
    {
        if ( begin_ptr ) {
            std::for_each(begin_ptr, end_ptr,
                          [&alloc_ = alloc_](T& value) { std::allocator_traits<Alloc>::destroy(alloc_, &value); });
            alloc_.deallocate(begin_ptr, capacity_ptr - begin_ptr);
        }
    }

    void allocate_and_copy(const_iterator begin, const_iterator end)
    {
        size_type new_capacity = end - begin;
        begin_ptr = alloc_.allocate(new_capacity);
        end_ptr = std::uninitialized_copy(begin, end, begin_ptr);
        capacity_ptr = end_ptr;
    }

};

template<typename T>
void swap(Vector<T>& left, Vector<T>& right) { left.swap(right); }



