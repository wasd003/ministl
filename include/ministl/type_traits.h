#pragma once

namespace ministl
{

template<typename...>
using __void_t = void;

struct true_type {
    constexpr static bool value = true;
};

struct false_type {
    constexpr static bool value = false;
};

template<typename T, typename U>
struct is_same : false_type {};

template<typename T>
struct is_same<T, T> : true_type {};

}
