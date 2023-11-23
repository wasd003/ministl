#pragma once
#include <exception>
#include <ministl/log.h>
#include <ministl/reverse_iterator.h>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <cassert>

namespace ministl
{

template <typename T>
class vector {
public:
    using value_type = T;
    using size_type = size_t;
    using iterator = T*;
    using byte = uint8_t;
    using pointer = value_type*;

private:
    size_type capacity;
    iterator begin_iter;
    iterator end_iter;

    constexpr static size_type min_capacity = 16;

    constexpr static size_type value_size = sizeof (T);

    void release_vector(pointer& begin_pointer, pointer& end_pointer) {
        // destruct object
        for (pointer cur = begin_pointer; cur != end_pointer; cur ++ )
            cur->~T();
        // release memory
        byte *raw = reinterpret_cast<byte *>(begin_pointer);
        ::delete[] raw;
        begin_pointer = end_pointer = nullptr;
    }

    void grow() {
        auto old_begin = begin_iter, old_end = end_iter;
        auto old_size = size();
        capacity <<= 1;
        begin_iter = reinterpret_cast<iterator>(::new byte[value_size * capacity]);
        end_iter = begin_iter + old_size;
        for (int i = 0; i < old_size; i ++ ) {
            begin_iter[i] = std::move(old_begin[i]);
        }
        release_vector(old_begin, old_end);
    }

public:
    /**
     * Constructor 
     */
    vector() : vector(0) {}

    vector(size_type n) :
        capacity(std::max(min_capacity, n)),
        begin_iter(reinterpret_cast<iterator>(::new byte[value_size * capacity])),
        end_iter(begin_iter + n) {}

    vector(size_type n, const value_type& init_val) : vector(n) {
        for (int i = 0; i < n; i ++ ) begin_iter[i] = init_val;
    }

    template<typename Iter>
    vector(Iter first, Iter second) : 
        vector(second > first ? (second - first) : 0) {
        static_assert(std::is_same_v<Iter, iterator>);
        size_type n = second - first;
        for (int i = 0; i < n; i ++, first ++ )
            begin_iter[i] = *first;
        assert(first == second);
    }

    vector(const std::initializer_list<value_type>& list) : vector(list.size()) {
        size_type i = 0;
        for (auto val : list) {
            begin_iter[i ++ ] = val;
        }
    }

    vector(const vector& rhs) : vector(rhs.capacity) {
        assert(size() == rhs.size());
        auto n = size();
        for (int i = 0; i < n; i ++ ) begin_iter[i] = rhs.begin_iter[i];
    }

    /**
     * TODO: reuse current memory, instead of just deleting them
     */
    vector& operator=(const vector& rhs) {
        if (this == &rhs) return *this;
        release_vector(begin_iter, end_iter);
        capacity = rhs.capacity;
        begin_iter = reinterpret_cast<iterator>(::new byte[rhs.capacity * value_size]);
        end_iter = begin_iter + rhs.size();
        auto n = size();
        for (int i = 0; i < n; i ++ ) begin_iter[i] = rhs.begin_iter[i];
        return *this;
    }

    vector& operator=(const std::initializer_list<value_type>& list) {
        release_vector(begin_iter, end_iter);
        capacity = std::max(min_capacity, list.size());
        begin_iter = reinterpret_cast<iterator>(::new byte[capacity * value_size]);
        end_iter = begin_iter + list.size();
        auto n = size(), i = 0;
        for (auto val : list) begin_iter[i ++ ] = val;
        return *this;
    }

    vector(vector&& rhs) : capacity(rhs.capacity), begin_iter(rhs.begin_iter), end_iter(rhs.end_iter) {
        rhs.begin_iter = rhs.end_iter = nullptr;
        rhs.capacity = 0;
    }

    vector& operator=(vector&& rhs) {
        assert(this != &rhs);
        release_vector(begin_iter, end_iter);
        capacity = rhs.capacity;
        begin_iter = rhs.begin_iter;
        end_iter = rhs.end_iter;
        rhs.begin_iter = rhs.end_iter = nullptr;
        rhs.capacity = 0;
        return *this;
    }

    ~vector() {
        try {
            release_vector(begin_iter, end_iter);
        } catch (const std::exception& err) {
            debug(err.what());
            std::abort();
        }
    }


    /**
     * Operation
     */
    void push_back(const value_type& rhs);

    void push_back(value_type&& rhs);

    template<typename... Args>
    void emplace_back(Args&&... args);

    size_type size() const noexcept {
        return (end_iter - begin_iter);
    }

    bool empty() noexcept {
        return begin_iter == end_iter;
    }

    value_type& operator[](int idx) {
        return begin_iter[idx];
    }

    const value_type& operator[](int idx) const {
        return begin_iter[idx];
    }

    value_type& at(int idx) {
        return const_cast<value_type&> (static_cast<const vector *>(this)->at(idx));
    }

    const value_type& at(int idx) const {
        if (idx < 0 || idx >= size())
            throw std::runtime_error("index outof bound");
        return (*this)[idx];
    }


    /**
     * Iterator
     */
    iterator begin() noexcept { return begin_iter; }

    iterator end() noexcept { return end_iter; }

    const iterator begin() const noexcept { return begin_iter; }

    const iterator end() const noexcept { return end_iter; }

    ministl::reverse_iterator<iterator> rbegin() {
        return reverse_iterator<iterator> (end());
    }

    ministl::reverse_iterator<iterator> rend() {
        return reverse_iterator<iterator> (begin());
    }

    const ministl::reverse_iterator<iterator> rbegin() const {
        return reverse_iterator<iterator> (end());
    }

    const ministl::reverse_iterator<iterator> rend() const {
        return reverse_iterator<iterator> (begin());
    }
};

template<typename T>
void vector<T>::push_back(const value_type& rhs) {
    if (size() >= capacity) [[unlikely]] { 
        grow();
    }
    *end_iter = rhs;
    end_iter ++ ;
}

template<typename T>
void vector<T>::push_back(value_type&& rhs) {
    // TODO: use ministl:move
    emplace_back(std::move(rhs));
}

template<typename T>
template<typename... Args>
void vector<T>::emplace_back(Args&&... args) {
    if (size() >= capacity) [[unlikely]] {
        grow();
    }
    // TODO: use ministl:forward
    ::new (end_iter) value_type(std::forward<Args>(args)...);
    end_iter ++ ;
}

};
