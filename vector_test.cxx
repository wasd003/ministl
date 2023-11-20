#include <ministl/log.h>
#include <ministl/vector.h>
#include <ministl/test.h>
#include <stdexcept>

struct test_struct {
    int field_a, field_b;
    test_struct() = delete;
    test_struct(int field_a, int field_b) : field_a(field_a), field_b(field_b) {}
};

test_result vector_test() {
    int score = 0, full_score = 0;
    {
        int n = 100;
        ministl::vector<test_struct> vec;
        test_struct obj = {0, 0};

        /**
         * test push_back && emplace_back
         */
        for (int i = 0; i < n; i ++ ) {
            switch (i % 3) {
                case 0:
                    vec.push_back({i, i});
                    break;
                case 1:
                    obj = {i, i};
                    vec.push_back(obj);
                    break;
                case 2:
                    vec.emplace_back(i, i);
                    break;
            }
        }


        /**
         * test iterator constructor
         */
        ministl::vector<test_struct> vec2(vec.begin(), vec.end());

        /**
         * test operator[]
         */
        for (int i = 0; i < n; i ++ ) {
            auto [a, b] = vec[i];
            assert(a == i && b == i);
            auto [a2, b2] = vec2[i];
            assert(a2 == i && b2 == i);
            score ++ ;
            full_score ++ ;
        }

        /**
         * test at()
         */
        try {
            vec.at(n);
        } catch (const std::runtime_error& err) {
            score ++ ;
        }
        full_score ++ ;
    }

    {
        int n = 100;
        ministl::vector<test_struct> vec(n);
        for (int i = 0; i < n; i ++ ) {
            vec[i] = {i, i};
            auto [a, b] = vec[i];
            assert(a == i && b == i);
            score ++ ;
            full_score ++ ;
        }
    }

    {
        int n = 100;
        ministl::vector<test_struct> vec(n, {42, 24});
        for (int i = 0; i < n; i ++ ) {
            auto [a, b] = vec[i];
            assert(a == 42 && b == 24);
            score ++ ;
            full_score ++ ;
        }
    }

    return {score, full_score};
}
