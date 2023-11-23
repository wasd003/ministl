#include <cassert>
#include <ministl/log.h>
#include <ministl/vector.h>
#include <ministl/test.h>
#include <stdexcept>

struct test_struct {
    int field_a, field_b;
    test_struct() = delete;
    test_struct(int field_a, int field_b) : field_a(field_a), field_b(field_b) {}
};

static test_result basic_test() {
    int n = 100, score = 0, full_score = 0;
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

    return {score, full_score};
}

static test_result test_n_ctor() {
    int n = 100, score = 0, full_score = 0;
    ministl::vector<test_struct> vec(n);
    for (int i = 0; i < n; i ++ ) {
        vec[i] = {i, i};
        auto [a, b] = vec[i];
        assert(a == i && b == i);
        score ++ ;
        full_score ++ ;
    }
    return {score, full_score};
}

static test_result test_dtor() {
    int n = 100, score = 0, full_score = 0;
    {
        ministl::vector<test_struct> vec;
        vec.emplace_back(1, 2);
        full_score ++ ;
    }
    score ++ ;
    return {score, full_score};
}

static test_result test_n_ele_ctor() {
    int n = 100, score = 0, full_score = 0;
    ministl::vector<test_struct> vec(n, {42, 24});
    for (int i = 0; i < n; i ++ ) {
        auto [a, b] = vec[i];
        assert(a == 42 && b == 24);
        score ++ ;
        full_score ++ ;
    }
    return {score, full_score};
}

static test_result test_copy_move_ctor() {
    int n = 100, score = 0, full_score = 0;
    ministl::vector<test_struct> vec(n, {42, 24});
    auto copy_vec = vec;
    auto move_vec = std::move(vec);
    assert(vec.empty());
    for (int i = 0; i < n; i ++ ) {
        auto [a, b] = copy_vec[i];
        assert(a == 42 && b == 24);
        auto [a2, b2] = move_vec[i];
        assert(a2 == 42 && b2 == 24);
        score ++ ;
        full_score ++ ;
    }
    return {score, full_score};
}

static test_result test_assignment() {
    int n = 100, score = 0, full_score = 0;
    ministl::vector<test_struct> vec(n, {42, 24});
    ministl::vector<test_struct> copy_vec, move_vec;
    copy_vec = vec;
    move_vec = std::move(vec);
    assert(vec.empty());
    for (int i = 0; i < n; i ++ ) {
        auto [a, b] = copy_vec[i];
        assert(a == 42 && b == 24);
        auto [a2, b2] = move_vec[i];
        assert(a2 == 42 && b2 == 24);
        score ++ ;
        full_score ++ ;
    }
    return {score, full_score};
}

static test_result test_init_list_ctor() {
    int score = 0, full_score = 0;
    ministl::vector<int> vec = {1, 2, 3};
    for (auto i = 0; i < vec.size(); i ++ ) {
        assert(vec[i] == i + 1);
        score ++ ;
        full_score ++ ;
    }
    return {score, full_score};
}

static test_result test_init_list_assignment() {
    int score = 0, full_score = 0;
    ministl::vector<int> vec = {1, 2, 3};
    vec = {0, 1, 2};
    for (auto i = 0; i < vec.size(); i ++ ) {
        assert(vec[i] == i);
        score ++ ;
        full_score ++ ;
    }
    return {score, full_score};
}

static test_result test_reverse_iterator() {
    int score = 0, full_score = 0;
    ministl::vector<int> vec = {1, 2, 3};
    int val = 3;
    for (auto iter = vec.rbegin(); iter != vec.rend(); iter ++ ) {
        full_score ++ ;
        assert(*iter == val -- );
        score ++ ;
    }
    return {score, full_score};
}

static test_result test_assign() {
    int score = 0, full_score = 0;
    { // test assign larger than capacity
        ministl::vector<int> v;
        v.assign(32, 42);
        assert(v.size() == 32);
        for (int i = 0; i < v.size(); i ++ ) {
            assert(v[i] == 42);
        }
        score ++ , full_score ++ ;
    }

    { // test assign smaller than capacity
        ministl::vector<int> v;
        v.assign(8, 42);
        assert(v.size() == 8);
        for (int i = 0; i < v.size(); i ++ ) {
            assert(v[i] == 42);
        }
        score ++ , full_score ++ ;
    }

    { // test assign larger then size()
        ministl::vector<int> v(8, 12);
        v.assign(12, 42);
        assert(v.size() == 12);
        for (int i = 0; i < v.size(); i ++ ) {
            assert(v[i] == 42);
        }
        score ++ , full_score ++ ;
    }

    return {score, full_score};
}

static test_result test_sort() {
    int score = 0, full_score = 0;
    { // test normal sort
        ministl::vector<int> vec = {3, 5, 1};
        ministl::sort(vec.begin(), vec.end());
        for (int i = 1; i < vec.size(); i ++ ) {
            assert(vec[i] >= vec[i - 1]);
        }
        score ++ , full_score ++ ;
    }

    { // test custom sort
        ministl::vector<int> vec = {5, 1, 3, 1};
        ministl::sort(vec.begin(), vec.end(), [](auto a, auto b) {
                return a >= b;
        });
        for (int i = 1; i < vec.size(); i ++ ) {
            assert(vec[i] <= vec[i - 1]);
        }
        score ++ , full_score ++ ;
    }

    return {score, full_score};
}

int dtor_cnt = 0;
static test_result test_pop_back() {
    int score = 0, full_score = 0;
    struct tmp_struct {
        ~tmp_struct() {
            dtor_cnt ++ ;
        }
    };
    int n = 3;
    ministl::vector<tmp_struct> vec(n);
    while (vec.size()) vec.pop_back();
    assert(dtor_cnt == n);
    score ++ , full_score ++ ;
    return {score, full_score};
}

static test_result test_reverse() {
    int score = 0, full_score = 0;
    ministl::vector<int> vec = {1, 2, 3, 4, 5};
    ministl::reverse(vec.begin(), vec.end());
    for (int i = 0, j = 5; i < vec.size(); i ++ , j -- ) {
        assert(vec[i] == j);
        score ++ ;
        full_score ++ ;
    }
    return {score, full_score};
}

test_result vector_test() {
    int score = 0, full_score = 0;

    auto tmp = test_dtor();
    score += tmp.first, full_score += tmp.second;

    tmp = basic_test();
    score += tmp.first, full_score += tmp.second;

    tmp = test_n_ctor();
    score += tmp.first, full_score += tmp.second;

    tmp = test_n_ele_ctor();
    score += tmp.first, full_score += tmp.second;

    tmp = test_copy_move_ctor();
    score += tmp.first, full_score += tmp.second;

    tmp = test_assignment();
    score += tmp.first, full_score += tmp.second;

    tmp = test_init_list_ctor();
    score += tmp.first, full_score += tmp.second;

    tmp = test_init_list_assignment();
    score += tmp.first, full_score += tmp.second;

    tmp = test_reverse_iterator();
    score += tmp.first, full_score += tmp.second;

    tmp = test_assign();
    score += tmp.first, full_score += tmp.second;

    tmp = test_sort();
    score += tmp.first, full_score += tmp.second;

    tmp = test_pop_back();
    score += tmp.first, full_score += tmp.second;

    tmp = test_reverse();
    score += tmp.first, full_score += tmp.second;

    return {score, full_score};
}
