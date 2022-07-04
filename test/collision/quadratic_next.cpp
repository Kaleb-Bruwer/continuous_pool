#include <gtest/gtest.h>
#include <tuple>

#include "collision.h"

class T_quadratic_next : public ::testing::TestWithParam<
        std::tuple<double, double, double, double>
    >{
public:
    double a, b, c, expect;

    virtual void SetUp(){
        auto t = GetParam();
        a = std::get<0>(t);
        b = std::get<1>(t);
        c = std::get<2>(t);
        expect = std::get<3>(t);
    };
};

TEST_P(T_quadratic_next, test_result){
    double result = quadratic_next(a,b,c);

    // Can't make precise checks due to floating point errors
    double diff = fabs(result - expect);

    ASSERT_TRUE(diff < 0.00001) << "quadratic_next invalid return: " << result << " instead of " << expect;
}

INSTANTIATE_TEST_CASE_P(TC_quadratic_next, T_quadratic_next,
    ::testing::Values(
        std::make_tuple(0,0,0, -1), //flat line

        std::make_tuple(1,0,0, 0), //root is 0

        std::make_tuple(0.5,-1,0.5, 1), //single positive root
        std::make_tuple(0.75,-1.5,0.75, 1),
        std::make_tuple(1,-1.5,0.5625, 0.75),


        std::make_tuple(1,-1.5,0.5635, -1), //no roots
        std::make_tuple(1,0.5,0.5635, -1),

        std::make_tuple(1,1.5,0.5625, -1), //single negative root

        std::make_tuple(1,-1.5,-1, -1), //multiple roots
        std::make_tuple(1,-2.3,0.6, 0.3)


));
