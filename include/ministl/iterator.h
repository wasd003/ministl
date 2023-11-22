#pragma once
#include <iterator>
#include <ministl/type_traits.h>
#include <type_traits>

namespace ministl
{

/**
 * 5 iterator tag
 */
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

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
    using reference = const T&;
    using pointer = const T*;
    using difference_type = ptrdiff_t;
};

template<typename Iter, typename TargetCategory, typename = void>
struct is_iterator_helper
{};

template<typename Iter, typename TargetCategory>
struct is_iterator_helper<Iter, TargetCategory,
    typename std::enable_if_t<std::is_convertible<
                                typename ministl::iterator_traits<Iter>::iterator_category,
                                TargetCategory>::value>>
    :ministl::true_type
{};

template<typename Iter, typename TargetCategory>
struct is_iterator_helper<Iter, TargetCategory,
    typename std::enable_if_t<!std::is_convertible<
                                typename ministl::iterator_traits<Iter>::iterator_category,
                                TargetCategory>::value>>
    :ministl::false_type
{};

/**
 * iterator category checker
 */
template<typename Iter>
struct is_input_iterator : is_iterator_helper<Iter, input_iterator_tag> {};

template<typename Iter>
struct is_output_iterator : is_iterator_helper<Iter, output_iterator_tag> {};

template<typename Iter>
struct is_forward_iterator : is_iterator_helper<Iter, forward_iterator_tag> {};

template<typename Iter>
struct is_bidirectional_iterator : is_iterator_helper<Iter, bidirectional_iterator_tag> {};

template<typename Iter>
struct is_random_access_iterator : is_iterator_helper<Iter, random_access_iterator_tag> {};

/**
 * some other ierator traits.
 * just for practice
 */

template<typename Iter, typename = ministl::__void_t<>>
struct has_value_type : ministl::false_type
{};

template<typename Iter>
struct has_value_type<Iter, typename Iter::value_type> : ministl::true_type
{};

/**
 * generic operation on iterator
 */

/**
 * 1. advance operation
 */

// advance for InputIterator && Forward Iterator
template<typename Iter, typename DistanceType>
void advance_dispatch(Iter& iter, DistanceType n, input_iterator_tag) {
    assert(n >= 0);
    while (n -- ) ++ iter;
}

// advance for BidirectionalIterator
template<typename Iter, typename DistanceType>
void advance_dispatch(Iter& iter, DistanceType n, bidirectional_iterator_tag) {
    if (n > 0) {
        for (auto i = 0; i < n; i ++ ) ++ iter;
    } else {
        for (auto i = 0; i > -n; i -- ) -- iter;
    }
}

// advance for RandomAccessIterator
template<typename Iter, typename DistanceType>
void advance_dispatch(Iter& iter, DistanceType n, random_access_iterator_tag) {
    iter += n;
}

template<typename Iter, typename DistanceType>
void advance(Iter& iter, DistanceType n) {
    advance_dispatch(iter, n, typename ministl::iterator_traits<Iter>::iterator_category {});
}

/**
 * distance operation
 */

}
