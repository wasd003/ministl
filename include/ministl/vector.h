#pragma once
#include <cstdlib>
#include <cstdint>
#include <algorithm>
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

    void push_back(const value_type& rhs);

    void push_back(value_type&& rhs);

    template<typename... Args>
    void emplace_back(Args&&... args);

    size_type size() noexcept {
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

    iterator begin() { return begin_iter; }

    iterator end() { return end_iter; }
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
