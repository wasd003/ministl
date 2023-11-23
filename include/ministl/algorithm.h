#include <ministl/iterator.h>
#include <ministl/log.h>
#include <functional>

namespace ministl
{

template<typename Iter, typename ValueType>
void fill(Iter begin, Iter end, const ValueType& val) {
    for (auto it = begin; it != end; it ++ ) {
        *it = val;
    }
}

template<typename ValueType>
void swap(ValueType& first, ValueType& second) {
    auto tmp = first;
    first = second;
    second = tmp;
}

// TODO: replace std::function with ministl ver.
template<typename Iter>
void sort(Iter begin, Iter end, std::function<bool(
            typename ministl::iterator_traits<Iter>::value_type,
            typename ministl::iterator_traits<Iter>::value_type)> cmp) {
    if (begin + 1 >= end) return;
    auto left = begin - 1, right = end;
    auto pivot = *begin; // TODO: random pivot
    while (left < right) {
        do left ++ ; while (cmp(*left, pivot) && *left != pivot);
        do right -- ; while (!cmp(*right, pivot) && *right != pivot);
        if (left < right) swap(*left, *right);
    }
    right ++ ;
    sort(begin, right, cmp);
    sort(right, end, cmp);
}

template<typename Iter>
void sort(Iter begin, Iter end) {
    sort(begin, end, [](auto first, auto second) { return first < second; } );
}

template<typename Iter>
void reverse(Iter begin, Iter end) {
    for (auto i = begin, j = end - 1; i < j; i ++ , j -- ) {
        swap(*i, *j);
    }
}

template<typename Iter, typename ValueType>
Iter find(Iter begin, Iter end, ValueType target) {
    for (auto i = begin; i != end; i ++ ) {
        if (*i == target) return i;
    }
    return end;
}

}
