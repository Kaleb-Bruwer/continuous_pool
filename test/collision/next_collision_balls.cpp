#include <gtest/gtest.h>
#include <tuple>

#include "collision.h"

class T_next_collision_balls : public ::testing::TestWithParam<
        std::tuple<Path, Path, double, double>
    >{
public:
    Path p1, p2;
    double radius, expect;

    virtual void SetUp(){
        auto t = GetParam();
        p1 = std::get<0>(t);
        p2 = std::get<1>(t);
        radius = std::get<2>(t);
        expect = std::get<3>(t);
    };
};

TEST_P(T_next_collision_balls, test_result){
    double result = _next_collision(p1, radius, p2, 0);

    // Can't make precise checks due to floating point errors
    double diff = fabs(result - expect);

    ASSERT_TRUE(diff < 0.00001) << "next_collision(balls) invalid return: " << result << " instead of " << expect;
}

INSTANTIATE_TEST_CASE_P(TC_quadratic_next, T_next_collision_balls,
    ::testing::Values(
        //no time overlap
        std::make_tuple(
            Path{vec2d{}, vec2d{}, vec2d{}, 0, 1},
            Path{vec2d{}, vec2d{}, vec2d{}, 2, 5},
            1, -1),

        std::make_tuple(
            Path{vec2d{}, vec2d{}, vec2d{}, 0, 1},
            Path{vec2d{}, vec2d{}, vec2d{}, 2, -1},
            1, -1),

        // single moment overlap, with contact
        std::make_tuple(
            Path{vec2d{0,0}, vec2d{0,0}, vec2d{}, 0, 1},
            Path{vec2d{1,0}, vec2d{-1,0}, vec2d{}, 1, -1},
            1, 1),

        // will make contact, no accel
        std::make_tuple(
            Path{vec2d{0,0}, vec2d{1,1}, vec2d{}, 0, -1},
            Path{vec2d{3,3}, vec2d{}, vec2d{}, 0, -1},
            1, 2.292893),

        std::make_tuple(
            Path{vec2d{0,0}, vec2d{1,1}, vec2d{}, 0, -1},
            Path{vec2d{3,3}, vec2d{}, vec2d{}, 1, -1},
            1, 2.292893),

        std::make_tuple(
            Path{vec2d{0,0}, vec2d{1,1}, vec2d{}, 2, -1},
            Path{vec2d{3,3}, vec2d{}, vec2d{}, 0, -1},
            1, 4.292893),

        // will make contact, with accel
        std::make_tuple(
            Path{vec2d{0,0}, vec2d{1,1}, vec2d{-0.1, -0.1}, 0, -1},
            Path{vec2d{3,3}, vec2d{}, vec2d{}, 0, -1},
            1, 2.641866),

        // won't make contact, no accel
        std::make_tuple(
            Path{vec2d{0,0}, vec2d{1,-1}, vec2d{}, 0, -1},
            Path{vec2d{3,3}, vec2d{}, vec2d{}, 0, -1},
            1, -1),

        // will make contact, with accel
        std::make_tuple(
            Path{vec2d{0,0}, vec2d{-1,1}, vec2d{-0.1, -0.1}, 0, -1},
            Path{vec2d{3,3}, vec2d{}, vec2d{}, 0, -1},
            1, -1),

        // start with contact, no movement
        std::make_tuple(
            Path{vec2d{0,0}, vec2d{}, vec2d{}, 0, -1},
            Path{vec2d{0,1}, vec2d{}, vec2d{}, 0, -1},
            1, -1),

        // start with contact, move away
        std::make_tuple(
            Path{vec2d{0,0}, vec2d{-1,-1}, vec2d{}, 0, -1},
            Path{vec2d{0,1}, vec2d{}, vec2d{}, 0, -1},
            1, -1),

        // start with contact, move away
        std::make_tuple(
            Path{vec2d{0,0}, vec2d{0,1}, vec2d{}, 0, -1},
            Path{vec2d{0,1}, vec2d{}, vec2d{}, 0, -1},
            1, 0)

));
