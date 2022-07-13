#pragma once

#include "gamestate.h"
#include "table.h"
#include "collisionobserver.h"
#include "ball.h"
#include "cue.h"

#include <vector>
#include <utility>

extern const double PI;

class LevelNoGraphics : public GameState{
protected:
    CollisionObserver collobserver;
    Table tab;
    Ball balls[15] {};
    Ball cueball {};
    Cue cue {};

    std::vector<int> pockets {};
    bool moving_state = false;
    bool player1turn = true;
    bool move_was_made = false;
    int team_color = 0; // 0 = undefined, 1 = player1 kills solid, 2 = player1 kills stripes
    int winner = 0; // 0 = undefined, 1 = player1, 2 = player2
    bool mouse_pressed = false;

public:
    LevelNoGraphics();

    virtual void handle_events(){};
    virtual void logic() override;
    virtual void render(){};

protected:
    virtual void create_balls();
    virtual void create_cue_ball();
    void check_pocket(Ball& b);
    bool ball_off_table(Ball& b);
    std::pair<double,double> get_safe_pos();
    void recenter_cue();
    void change_state();
    void check_first_hit(bool cur_turn);
    void check_balls_off_table(bool cur_turn);
    void check_team_color(bool cur_turn);
    void check_balls_in_pockets(bool cur_turn);
    virtual void won(bool cur_turn);
    virtual void lost(bool cur_turn);
    virtual void message(const std::string& msg, unsigned delay){};
    void shoot(double speed);
};