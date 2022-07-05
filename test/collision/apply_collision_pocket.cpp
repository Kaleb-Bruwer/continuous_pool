#include <gtest/gtest.h>
#include <tuple>

#include "collision.h"
#include "collision_test_helpers.h"

class T_apply_collision_pocket : public ::testing::Test{
protected:

};

// Parameterized test impractical due to complex initialization of ball objects

TEST_F(T_apply_collision_pocket, pocket_collide){
    Pocket p;
    p.pos = vec2d{0,0}

    Ball ball_in;

    ball_in.posData.radius = 1;
    ball_in.path[0] = Path{vec2d{-2,0}, vec2d{1,0}, vec2d{0}, 0, -1};

    Ball ball_out;

    ball_out.posData.radius = 1;
    ball_out.path[0] = Path{vec2d{-2,1}, vec2d{1,0}, vec2d{0}, 0, 1};
    ball_out.path.push_back(Path{vec2d{-1,1}, vec2d{-1,0}, vec2d{0.4,0}, 1, -1, 0, true});


    apply_collision(ball_in, p, 1);

    assert_ball_path(ball_in, ball_out);
}
