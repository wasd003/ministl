#include <cstdlib>
#include <ministl/test.h>
#include <ministl/iterator.h>
#include <ministl/vector.h>
#include <ministl/type_traits.h>

test_result vector_iterator_test() {
    int score = 0, full_score = 0;

    static_assert(ministl::is_same<ministl::iterator_traits<
                  ministl::vector<int>::iterator>::iterator_category,
                  ministl::random_access_iterator_tag>::value);
    score ++ , full_score ++ ;

    static_assert(!ministl::has_value_type<ministl::vector<int>::iterator>::value);
    score ++ , full_score ++ ;

    return {score, full_score};
}

test_result iterator_category_checker_test() {
    struct input_iter {
        using iterator_category = ministl::input_iterator_tag;
        using value_type = int;
        using pointer = value_type*;
        using reference = value_type&;
        using difference_type = ptrdiff_t;
    };
    struct output_iter {
        using iterator_category = ministl::output_iterator_tag;
        using value_type = int;
        using pointer = value_type*;
        using reference = value_type&;
        using difference_type = ptrdiff_t;
    };
    struct forward_iter {
        using iterator_category = ministl::forward_iterator_tag;
        using value_type = int;
        using pointer = value_type*;
        using reference = value_type&;
        using difference_type = ptrdiff_t;
    };
    struct bidirectional_iter {
        using iterator_category = ministl::bidirectional_iterator_tag;
        using value_type = int;
        using pointer = value_type*;
        using reference = value_type&;
        using difference_type = ptrdiff_t;
    };
    struct random_access_iter {
        using iterator_category = ministl::random_access_iterator_tag;
        using value_type = int;
        using pointer = value_type*;
        using reference = value_type&;
        using difference_type = ptrdiff_t;
    };
    static_assert(ministl::is_input_iterator<input_iter>::value);
    static_assert(!ministl::is_input_iterator<output_iter>::value);
    static_assert(ministl::is_input_iterator<forward_iter>::value);
    static_assert(ministl::is_forward_iterator<forward_iter>::value);
    static_assert(ministl::is_input_iterator<bidirectional_iter>::value);
    static_assert(ministl::is_forward_iterator<bidirectional_iter>::value);
    static_assert(ministl::is_bidirectional_iterator<bidirectional_iter>::value);
    static_assert(ministl::is_input_iterator<random_access_iter>::value);
    static_assert(ministl::is_forward_iterator<random_access_iter>::value);
    static_assert(ministl::is_bidirectional_iterator<random_access_iter>::value);
    static_assert(ministl::is_random_access_iterator<random_access_iter>::value);
    static_assert(ministl::is_random_access_iterator<int*>::value);
    static_assert(ministl::is_random_access_iterator<const char*>::value);

    return {0, 0};
};

test_result iterator_traits_test() {
    int score = 0, full_score = 0;
    test_result tmp;

    tmp = vector_iterator_test();
    score += tmp.first, full_score += tmp.second;

    tmp = iterator_category_checker_test();
    score += tmp.first, full_score += tmp.second;

    return {score, full_score};
}
