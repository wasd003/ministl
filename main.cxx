#include <ministl/log.h>
#include <ministl/test.h>
#include <cassert>

int main() {
    /**
     * vector test
     */
    auto [vec_score, vec_full_score] = vector_test();
    assert(vec_score == vec_full_score);

    debug("Pass All Test :)");
    return 0;
}
