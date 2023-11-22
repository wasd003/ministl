#pragma once

namespace ministl
{

template<typename...>
using __void_t = void;

template<typename T, typename U>
struct is_same {
    constexpr static bool value = false;
};

template<typename T>
struct is_same<T, T> {
    constexpr static bool value = true;
};

}
