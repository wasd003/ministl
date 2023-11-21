#pragma once
#include <ministl/log.h>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <cassert>
#include <utility>

namespace ministl
{

template <typename T>
class vector {
public:
    using value_type = T;
    using size_type = size_t;
    using iterator = T*;
    using byte = uint8_t;

private:
    size_type capacity;
    iterator begin_iter;
    iterator end_iter;

    constexpr static size_type min_capacity = 16;

    constexpr static size_type value_size = sizeof (T);

    void grow() {
        auto old_begin = begin_iter;
        auto old_size = size();
        capacity <<= 1;
        begin_iter = reinterpret_cast<iterator>(::new byte[value_size * capacity]);
        end_iter = begin_iter + old_size;
        for (int i = 0; i < old_size; i ++ ) {
            begin_iter[i] = std::move(old_begin[i]);
        }
        ::delete[] old_begin;
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
        ::delete[] begin_iter;
        capacity = rhs.capacity;
        begin_iter = reinterpret_cast<iterator>(::new byte[rhs.capacity * value_size]);
        end_iter = begin_iter + rhs.size();
        auto n = size();
        for (int i = 0; i < n; i ++ ) begin_iter[i] = rhs.begin_iter[i];
        return *this;
    }

    vector& operator=(const std::initializer_list<value_type>& list) {
        ::delete []begin_iter;
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
        ::delete[] begin_iter;
        capacity = rhs.capacity;
        begin_iter = rhs.begin_iter;
        end_iter = rhs.end_iter;
        rhs.begin_iter = rhs.end_iter = nullptr;
        rhs.capacity = 0;
        return *this;
    }

    ~vector() {
        ::delete[] begin_iter;
        begin_iter = end_iter = nullptr;
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

    value_type& at(int idx) {
        if (idx < 0 || idx >= size())
            throw std::runtime_error("index outof bound");
        return (*this)[idx];
    }

    iterator begin() noexcept { return begin_iter; }

    iterator end() noexcept { return end_iter; }
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
