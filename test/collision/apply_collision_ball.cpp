#include <gtest/gtest.h>
#include <tuple>

#include "collision.h"
#include "collision_test_helpers.h"

class T_apply_collision_ball : public ::testing::Test{
protected:

};

// Parameterized test impractical due to complex initialization of ball objects

TEST_F(T_apply_collision_ball, simple_case){
    double time{1};

    Ball ball1_in;
    Ball ball2_in;

    ball1_in.posData.radius = 1;
    ball1_in.path[0] = Path{vec2d{-2,1}, vec2d{1,0}, vec2d{0}, 0, -1};

    ball2_in.posData.radius = 1;
    ball2_in.path[0] = Path{vec2d{2,1}, vec2d{-1,0}, vec2d{0}, 0, -1};

    Ball ball1_out;
    Ball ball2_out;

    ball1_out.posData.radius = 1;
    ball1_out.path[0] = Path{vec2d{-2,1}, vec2d{1,0}, vec2d{0}, 0, 1};
    ball1_out.path.push_back(Path{vec2d{-1,1}, vec2d{-1,0}, vec2d{0.4,0}, 1, 3.5});
    ball1_out.path.push_back(Path{vec2d{-2.25,1}, vec2d{0,0}, vec2d{0,0}, 3.5, -1});

    ball2_out.posData.radius = 1;
    ball2_out.path[0] = Path{vec2d{2,1}, vec2d{-1,0}, vec2d{0}, 0, 1};
    ball2_out.path.push_back(Path{vec2d{1,1}, vec2d{1,0}, vec2d{-0.4,0}, 1, 3.5});
    ball2_out.path.push_back(Path{vec2d{2.25,1}, vec2d{0,0}, vec2d{0,0}, 3.5, -1});


    apply_collision(ball1_in, ball2_in, time);

    assert_ball_path(ball1_in, ball1_out);
    assert_ball_path(ball2_in, ball2_out);
}
