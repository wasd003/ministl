#pragma once
#include <iterator>
#include <ministl/type_traits.h>

namespace ministl
{

/**
 * 5 iterator tag
 */
struct input_iterator_tag {};
struct ouput_iterator_tag {};
struct forward_iterator_tag {};
struct bidirectional_iterator_tag {};
struct random_access_iterator_tag {};

/**
 * iterator traits for non-pointer
 */
template<typename Iter, typename = ministl::__void_t<>>
struct iterator_traits {};

template<typename Iter>
struct iterator_traits<Iter, __void_t<typename Iter::iterator_category,
                                typename Iter::value_type,
                                typename Iter::reference,
                                typename Iter::pointer,
                                typename Iter::difference_type>> {
    using iterator_category = typename Iter::iterator_category;
    using value_type = typename Iter::value_type;
    using reference = typename Iter::reference;
    using pointer = typename Iter::pointer;
    using difference_type = typename Iter::difference_type;
};

/**
 * iterator traits for pointer
 */
template<typename T>
struct iterator_traits<T*> {
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using difference_type = ptrdiff_t;
};

template<typename T>
struct iterator_traits<const T*> {
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using difference_type = ptrdiff_t;
};

}
