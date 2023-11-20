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
    iterator begin;
    iterator end;

    constexpr static size_type min_capacity = 16;

    constexpr static size_type value_size = sizeof (T);

    void grow() {
        auto old_begin = begin;
        auto old_size = size();
        capacity <<= 1;
        begin = reinterpret_cast<iterator>(::new byte[value_size * capacity]);
        end = begin + old_size;
        for (int i = 0; i < old_size; i ++ ) {
            begin[i] = std::move(old_begin[i]);
        }
        ::delete[] old_begin;
    }

public:
    vector() : vector(0) {}

    vector(size_type n) :
        capacity(std::max(min_capacity, n)),
        begin(reinterpret_cast<iterator>(::new byte[value_size * capacity])),
        end(begin + n) {}

    vector(size_type n, const value_type& init_val) : vector(n) {
        for (int i = 0; i < n; i ++ ) begin[i] = init_val;
    }

    template<typename Iter>
    vector(const Iter& first, const Iter& second) : 
        vector(second > first ? (second - first) : 0) {
        static_assert(std::is_same_v<Iter, iterator>);
    }

    void push_back(const value_type& rhs);

    void push_back(value_type&& rhs);

    template<typename... Args>
    void emplace_back(Args&&... args);

    size_type size() noexcept {
        return (end - begin);
    }

    bool empty() noexcept {
        return begin == end;
    }

    value_type& operator[](int idx) {
        return begin[idx];
    }

    value_type& at(int idx) {
        if (idx < 0 || idx >= size())
            throw std::runtime_error("index outof bound");
        return (*this)[idx];
    }
};

template<typename T>
void vector<T>::push_back(const value_type& rhs) {
    if (size() >= capacity) [[unlikely]] { 
        grow();
    }
    *end = rhs;
    end ++ ;
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
    ::new (end) value_type(std::forward<Args>(args)...);
    end ++ ;
}

};
