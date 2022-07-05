#include <gtest/gtest.h>
#include <tuple>

#include "collision.h"
#include "collision_test_helpers.h"

class T_apply_collision_rect : public ::testing::Test{
protected:

};

// Parameterized test impractical due to complex initialization of ball objects

TEST_F(T_apply_collision_rect, simple_case1){
    Line l{vec2d{0,0}, vec2d{0,2}};
    double time{1};

    Ball b_in;

    Ball b_out;
    b_in.path[0] = Path{vec2d{-2,1}, vec2d{1,0}, vec2d{0}, 0, -1};
    b_out.path[0] = Path{vec2d{-2,1}, vec2d{1,0}, vec2d{0}, 0, 1};
    b_out.path.push_back(Path{vec2d{-1,1}, vec2d{-1,0}, vec2d{0.4,0}, 1, 3.5}); //friction
    b_out.path.push_back(Path{vec2d{-2.25,1}, vec2d{0,0}, vec2d{0}, 3.5, -1});


    apply_collision(b_in, l, time);

    assert_ball_path(b_in, b_out);
}

TEST_F(T_apply_collision_rect, simple_case2){
    Line l{vec2d{0,2}, vec2d{0,0}};
    double time{1};

    Ball b_in;

    Ball b_out;
    b_in.path[0] = Path{vec2d{-2,1}, vec2d{1,0}, vec2d{0}, 0, -1};
    b_out.path[0] = Path{vec2d{-2,1}, vec2d{1,0}, vec2d{0}, 0, 1};
    b_out.path.push_back(Path{vec2d{-1,1}, vec2d{-1,0}, vec2d{0.4,0}, 1, 3.5}); //friction
    b_out.path.push_back(Path{vec2d{-2.25,1}, vec2d{0,0}, vec2d{0}, 3.5, -1});


    apply_collision(b_in, l, time);

    assert_ball_path(b_in, b_out);
}

TEST_F(T_apply_collision_rect, simple_case3){
    Line l{vec2d{0,0}, vec2d{2,0}};
    double time{1};

    Ball b_in;

    Ball b_out;
    b_in.path[0] = Path{vec2d{1,-2}, vec2d{0,1}, vec2d{0}, 0, -1};
    b_out.path[0] = Path{vec2d{1,-2}, vec2d{0,1}, vec2d{0}, 0, 1};
    b_out.path.push_back(Path{vec2d{1,-1}, vec2d{0,-1}, vec2d{0,0.4}, 1, 3.5}); //friction
    b_out.path.push_back(Path{vec2d{1,-2.25}, vec2d{0,0}, vec2d{0}, 3.5, -1});


    apply_collision(b_in, l, time);

    assert_ball_path(b_in, b_out);
}

TEST_F(T_apply_collision_rect, angle_case){
    Line l{vec2d{0,0}, vec2d{2,0}};
    double time{1};

    Ball b_in;

    Ball b_out;
    b_in.path[0] = Path{vec2d{0,-2}, vec2d{1,1}, vec2d{0}, 0, -1};
    b_out.path[0] = Path{vec2d{0,-2}, vec2d{1,1}, vec2d{0}, 0, 1};
    b_out.path.push_back(Path{vec2d{1,-1}, vec2d{1,-1}, vec2d{-0.2828427,0.2828427}, 1, 4.535533}); //friction
    b_out.path.push_back(Path{vec2d{2.767767,-2.767767}, vec2d{0,0}, vec2d{0}, 4.535533, -1});


    apply_collision(b_in, l, time);

    assert_ball_path(b_in, b_out);
}
