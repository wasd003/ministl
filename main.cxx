#include <ministl/log.h>
#include <ministl/test.h>
#include <cassert>

int main() {
    auto [vec_score, vec_full_score] = vector_test();
    assert(vec_score == vec_full_score);

    auto [it_traits_score, it_trais_full_score] = iterator_traits_test();
    assert(it_traits_score == it_trais_full_score);

    debug("Pass All Test :)");
    return 0;
}
