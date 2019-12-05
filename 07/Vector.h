#pragma once


#include "Iterator.h"
#include "Allocator.h"


template <class T, class Alloc = Allocator<T> >
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

    using iterator = Iterator<T>;
    using const_iterator = const iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    pointer data_;
    size_type size_;
    size_type capacity_;

public:

    // конструкторы 

    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    explicit Vector(size_type count)
    {
        size_ = count;
        capacity_ = 2 * count;
        data_ = alloc_.allocate(capacity_);
        pointer current = data_;
        for ( size_t i = 0; i < size_; ++i ) {
            alloc_.construct(current, T());
            ++current;
        }
    }

    Vector(size_type count, const value_type& value)
    {
        size_ = count;
        capacity_ = 2 * count;
        data_ = alloc_.allocate(capacity_);
        pointer current = data_;
        for ( size_t i = 0; i < size_; ++i ) {
            alloc_.construct(current, value);
            ++current;
        }
    }

    Vector(std::initializer_list<T> init)
    {
        size_type i = 0;
        auto current = init.begin();
        const auto end = init.end();
        size_ = init.size();
        capacity_ = 2 * size_;
        data_ = alloc_.allocate(capacity_);
        pointer dc = data_;
        while ( current != end ) {
            alloc_.construct(dc, static_cast<T>(*current) );
            ++current;
            ++dc;
        }
    }

    Vector(const Vector<T>& other)
    {
        size_type i = 0;
        auto current = other.cbegin();
        auto end = other.cend();
        capacity_ = other.capacity_;
        size_ = other.size_;
        resize(capacity_);
        while ( current != end ) {
            data_[i++] = *current++;
        }
    }

    Vector(Vector<T>&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
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
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
        }
        return *this;
    }

    ~Vector() { deallocate(); }




    // методы: begin, end, rbegin, rend

    iterator begin() noexcept { return iterator(data_); }

    const_iterator сbegin() const noexcept { return iterator(data_); };

    iterator end() noexcept { return iterator(data_ + size_); }

    const_iterator сend() const noexcept { return iterator(data_ + size_); }

    reverse_iterator rbegin()  noexcept { return reverse_iterator(end()); }

    reverse_iterator rend()  noexcept { return reverse_iterator(begin()); }

    const_reverse_iterator сrbegin() const noexcept { return reverse_iterator(сend()); }

    const_reverse_iterator сrend() const noexcept { return reverse_iterator(сbegin()); }



    // методы: [], at

    reference operator[](size_type idx) { return data_[idx]; }

    const_reference operator[](size_type idx) const { return data_[idx]; }

    reference at(size_type idx)
    {
        if ( idx < 0 || idx >= size() ) {
            throw std::out_of_range("Invalid index");
        }
        return data_[idx];
    }

    const_reference at(size_type idx) const
    {
        if ( idx < 0 || idx >= size() ) {
            throw std::out_of_range("Invalid index");
        }
        return data_[idx];
    }


    // методы: push_back, pop_back

    void emplace_back(T&& val)
    {
        if ( size_ == capacity_ ) {
            size_type new_capacity = ( size_ != 0 ) ? 2 * size_ : 1;
            reallocate(new_capacity);
        }
        data_[size_++] = std::move(val);
    }

    template<typename... Args>
    void emplace_back(T&& val, Args&&... args)
    {
        emplace_back(std::forward<T>(val));
        emplace_back(std::forward<Args>(args)...);
    }

    void push_back(const value_type& value) { emplace_back(value); }

    void push_back(value_type&& value) { emplace_back(std::move(value)); }

    void pop_back()
    {
        if ( size_ > 0 ) {
            data_[size_ - 1].~T();
            size_--;
        }
    }

    // методы: size, resize, reserve, clear

    size_type size() const noexcept { return size_; }

    bool empty() const noexcept { return size_ == 0; }

    size_type capacity() const noexcept { return capacity_; }

    void reserve(size_type new_capacity)
    {
        if ( new_capacity <= capacity() ) {
            return;
        }
        reallocate(new_capacity);
    }

    void resize(size_type new_size, const_reference value)
    {
        if ( new_size > size() ) {
            if ( new_size > capacity() ) {
                reallocate(2 * new_size);
            }
            pointer ptr = data_+size_;
            pointer ptr_end = data_ + new_size;
            while ( ptr < ptr_end ) {
                alloc_.construct(ptr, static_cast<T>(value));
                ++ptr;
            }
            size_ = new_size;
        }
        else if ( new_size < size() ) {
            pointer ptr = data_ + new_size;
            pointer ptr_end = data_ + size_;
            while ( ptr < ptr_end ) {
                alloc_.destroy(ptr);
                ++ptr;
            }
            reallocate(new_size);
            size_ = new_size;
        }
    }

    void resize(size_type new_size)
    {
        if ( new_size > size() ) {
            if ( new_size > capacity() ) {
                reallocate(2 * new_size);
            }
            size_type i = size_;
            while ( i < new_size ) {
                data_[i++] = T();
            }
            size_ = new_size;
        }
        else if ( new_size < size() ) {
            for ( size_type i = new_size; i < size(); ++i ) {
                data_[i].~T();
            }
            size_ = new_size;
        }
    }

    void shrink_to_fit()
    {
        reallocate(size());
    }


    void clear() noexcept
    {
        resize(0);
    }

    void swap(Vector<T>& vector) noexcept
    {
        std::swap(this->data_, vector.data_);
        std::swap(this->size_, vector.size_);
        std::swap(this->capacity_, vector.capacity_);
    }


private:

    void reallocate(size_type new_capacity)
    {
        pointer newData = alloc_.allocate(new_capacity);
        size_type new_size = ( size_ <= new_capacity ) ? size_ : new_capacity;
        pointer new_end = data_ + new_size;
        std::copy(data_, new_end, newData);
        deallocate();
        data_ = newData;
        size_ = new_size;
        capacity_ = new_capacity;
    }

    void deallocate()
    {
        if ( data_ ) {
            pointer current = data_;
            size_type i = 0;
            for ( size_t i = 0; i < size_; ++i ) {
                alloc_.destroy(current);
                ++current;
            }
        }
        alloc_.deallocate(data_, capacity_);
    }

};

template<typename T>
void swap(Vector<T>& left, Vector<T>& right) { left.swap(right); }



