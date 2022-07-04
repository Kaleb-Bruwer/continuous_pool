#include <gtest/gtest.h>
#include <tuple>

#include "collision.h"

class T_next_collision_rect : public ::testing::TestWithParam<
        std::tuple<Path, Line, double, double>
    >{
public:
    Path p;
    Line l;
    double radius, expect;

    virtual void SetUp(){
        auto t = GetParam();
        p = std::get<0>(t);
        l = std::get<1>(t);
        radius = std::get<2>(t);
        expect = std::get<3>(t);
    };
};

TEST_P(T_next_collision_rect, test_result){
    double result = _next_collision(p, radius, l);

    // Can't make precise checks due to floating point errors
    double diff = fabs(result - expect);

    ASSERT_TRUE(diff < 0.00001) << "next_collision(balls) invalid return: " << result << " instead of " << expect;
}

INSTANTIATE_TEST_CASE_P(TC_quadratic_next_r, T_next_collision_rect,
    ::testing::Values(
        //no collision
        std::make_tuple(
            Path{vec2d{}, vec2d{}, vec2d{}, 0, -1},
            Line{vec2d{2,0}, vec2d{2, 1}},
            1, -1),

        // collision
        std::make_tuple(
            Path{vec2d{0,0}, vec2d{1,0}, vec2d{}, 0, -1},
            Line{vec2d{2,-1}, vec2d{2, 1}},
            1, 1),

        std::make_tuple(
            Path{vec2d{0,0}, vec2d{-1,0}, vec2d{}, 0, -1},
            Line{vec2d{-2,-1}, vec2d{-2, 1}},
            1, 1),

        std::make_tuple(
            Path{vec2d{0,0}, vec2d{-1,0}, vec2d{}, 0, -1},
            Line{vec2d{-2,1}, vec2d{-2,-1}},
            1, 1),

        std::make_tuple(
            Path{vec2d{0,0}, vec2d{0,1}, vec2d{}, 0, -1},
            Line{vec2d{-1, 2}, vec2d{1, 2}},
            1, 1),

        std::make_tuple(
            Path{vec2d{0,1}, vec2d{1,0}, vec2d{}, 0, -1},
            Line{vec2d{1, 0}, vec2d{2, 2}},
            1, 0.381966),

        // collision outside time window
        std::make_tuple(
            Path{vec2d{0,0}, vec2d{1,0}, vec2d{}, 0, 2},
            Line{vec2d{5,-1}, vec2d{5, 1}},
            1, -1),

        std::make_tuple(
            Path{vec2d{3,0}, vec2d{3,0}, vec2d{}, 0, -1},
            Line{vec2d{1,-1}, vec2d{1, 1}},
            1, -1),

        // pass the line, but miss
        std::make_tuple(
            Path{vec2d{0,0}, vec2d{1,0}, vec2d{}, 0, -1},
            Line{vec2d{2,-1}, vec2d{2, -3}},
            1, -1)
));
