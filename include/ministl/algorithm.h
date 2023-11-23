#include <ministl/log.h>

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

}
