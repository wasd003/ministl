#pragma once

#include <ministl/iterator.h>

namespace ministl
{

template<typename Iter>
class reverse_iterator {
public:
    // any used defined iterator must implement 5 associated types
    using iterator_category = typename ministl::iterator_traits<Iter>::iterator_category;
    using value_type = typename ministl::iterator_traits<Iter>::value_type;
    using pointer = typename ministl::iterator_traits<Iter>::pointer;
    using reference = typename ministl::iterator_traits<Iter>::reference;
    using difference_type = typename ministl::iterator_traits<Iter>::difference_type;

private:
    Iter current;

public: // constructor
    reverse_iterator(Iter forward_iter) : current(forward_iter) {}


public: // basic operation
    Iter base() const {
        return current;
    }

    reverse_iterator& operator++() {
         -- current;
         return *this;
    }

    reverse_iterator operator++(int) {
        auto res = *this;
        -- current;
        return res;
    }

    reverse_iterator& operator--() {
         ++ current;
         return *this;
    }

    reverse_iterator operator--(int) {
        auto res = *this;
        ++ current;
        return res;
    }

    // pay attention to the semantic of reverse iterator!
    reference operator*() const {
        auto tmp = current;
        return *( -- tmp );
    }

    pointer operator->() const {
        return &(operator*());
    }

    reverse_iterator& operator+=(difference_type dist) {
        current -= dist;
        return *this;
    }

    reverse_iterator operator+(difference_type dist) const {
        auto res = *this;
        res.current -= dist;
        return *this;
    }

    reverse_iterator& operator-=(difference_type dist) {
        current += dist;
        return *this;
    }

    reverse_iterator operator-(difference_type dist) const {
        auto res = *this;
        res.current += dist;
        return *this;
    }

    bool operator=(const reverse_iterator& rhs) const {
        return current == rhs.current;
    }

    bool operator!=(const reverse_iterator& rhs) const {
        return current != rhs.current;
    }

};

}
