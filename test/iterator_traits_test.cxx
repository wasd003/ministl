#include <ministl/test.h>
#include <ministl/iterator.h>
#include <ministl/vector.h>
#include <ministl/type_traits.h>

test_result vector_iterator_test() {
    int score = 0, full_score = 0;

    // TODO: replace std::is_same_v with ministl ver.
    static_assert(ministl::is_same<ministl::iterator_traits<
                  ministl::vector<int>::iterator>::iterator_category,
                  ministl::random_access_iterator_tag>::value);
    score ++ , full_score ++ ;

    static_assert(!ministl::has_value_type<ministl::vector<int>::iterator>::value);
    score ++ , full_score ++ ;

    return {score, full_score};
}

test_result iterator_traits_test() {
    int score = 0, full_score = 0;
    test_result tmp;

    tmp = vector_iterator_test();
    score += tmp.first, full_score += tmp.second;

    return {score, full_score};
}
