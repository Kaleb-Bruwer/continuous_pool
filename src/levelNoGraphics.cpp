#include "levelNoGraphics.h"

#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>
#include <cstdio>

LevelNoGraphics::LevelNoGraphics()
    : collobserver{}, tab{&collobserver}
{
    create_balls();
    create_cue_ball();
}

LevelNoGraphics::LevelNoGraphics(const LevelNoGraphics& rhs){
    *this = rhs;
}

const LevelNoGraphics& LevelNoGraphics::operator=(const LevelNoGraphics& rhs){
    collobserver = CollisionObserver();

    tab.create(rhs.tab, &collobserver);

    for(int i=0; i<15; i++){
        balls[i] = rhs.balls[i].clone();
        balls[i].addObserver(&collobserver);
    }

    cueball = rhs.cueball.clone();
    cueball.addObserver(&collobserver);

    cue = rhs.cue;
    pockets = rhs.pockets;

    moving_state = rhs.moving_state;
    player1turn = rhs.player1turn;
    move_was_made = rhs.move_was_made;
    team_color = rhs.team_color;
    winner = rhs.winner;
    mouse_pressed = rhs.mouse_pressed;

    return *this;
}


void LevelNoGraphics::logic()
{
    moving_state = false;

    for (auto& b: balls)
        if (b.is_moving())
        {
            moving_state = true;
            b.move();
            check_pocket(b);
        }

    if (cueball.is_moving())
    {
        moving_state = true;
        cueball.move();
        check_pocket(cueball);
    }


    if (!moving_state) // Check player moves and state changes
    {
        if (!mouse_pressed)
            recenter_cue();

        if (move_was_made)
            change_state();
    }
}

void LevelNoGraphics::create_balls()
{
    // Create 15 balls
    for (int i = 1; i <= 15; ++i)
    {
        balls[i-1].id = i;
        balls[i-1].addObserver(&collobserver);
    }

    // Place the balls
    int cur = 0;
    double radius = balls[0].posData.radius + 0.5;
    double px = 200.0;
    double py = (tab.getY() + tab.getHeight()/2) - 5*radius;
    for (int i = 5; i > 0; --i)
    {
        for (int j = 0; j < i; ++j)
            balls[cur++].setPos(px, py + radius*j*2);

        px += radius * std::sqrt(3);
        py += radius;
    }

    // 8-ball must be at the center of the triangle
    std::swap(balls[7].posData, balls[10].posData);

    // Oposite signs at the corners
    std::swap(balls[4].posData, balls[8].posData);
}

void LevelNoGraphics::create_cue_ball()
{
    // Create and place the cue ball
    cueball = Ball{};
    cueball.id = 0;
    cueball.mass = 11.0;
    cueball.addObserver(&collobserver);

    auto pos = get_safe_pos();
    cueball.setPos(pos.first, pos.second);
}

void LevelNoGraphics::check_pocket(Ball& b)
{
    if (tab.is_pocketed(b)) // hit the pocket
    {
        b.notify(Event::SUBJECT_POCKET_COLLIDED);
        b.removeObserver(&collobserver);
        b.is_movable = false;
        b.is_visible = false;

        pockets.push_back(b.id);
    }
}

// Check if a ball has fallen off the table
bool LevelNoGraphics::ball_off_table(Ball& b)
{
    if (b.posData.pos_x < tab.getX())
        return true;
    else if (b.posData.pos_x > tab.getX() + tab.getWidth())
        return true;
    else if (b.posData.pos_y < tab.getY())
        return true;
    else if (b.posData.pos_y > tab.getY() + tab.getHeight())
        return true;
    else
        return false;
}

// Get a safe position to place a ball that returns to the table
std::pair<double, double> LevelNoGraphics::get_safe_pos()
{
    double posX = 600.0;
    double posY = tab.getY() + tab.getHeight()/2 - 2.5;

    bool shifted = true;
    while (shifted)
    {
        shifted = false;

        for (auto& b: balls)
            if (b.is_visible)
                if (std::hypot(b.posData.pos_x - posX, b.posData.pos_y - posY) < b.posData.radius*2)
                {
                    posX -= b.posData.radius * 2.0;
                    shifted = true;
                    break;
                }

        if (std::hypot(cueball.posData.pos_x - posX, cueball.posData.pos_y - posY) < cueball.posData.radius*2)
        {
            posX -= cueball.posData.radius;
            shifted = true;
        }
    }

    return {posX, posY};
}

void LevelNoGraphics::recenter_cue()
{
    cue.setPos(cueball.posData.pos_x + cueball.posData.radius, cueball.posData.pos_y - cue.getHeight()/2);
}

void LevelNoGraphics::change_state()
{
    bool cur_turn = player1turn;

    // Check own ball not hit first
    check_first_hit(cur_turn);

    // Balls pocketed
    check_balls_in_pockets(cur_turn);

    // Check balls off the table
    check_balls_off_table(cur_turn);

    move_was_made = false;
}

void LevelNoGraphics::check_first_hit(bool cur_turn)
{
    // Check own ball not hit first
    int f = collobserver.get_first_hit();

    if (f == -1)
        message("Ball not hit", 2000);
    else if (cur_turn)
    {
        if (team_color == 1 && Ball::is_stripes(f))
        {
            player1turn = !cur_turn;
            message("Own Ball not hit first", 2000);
        }
        else if (team_color == 2 && Ball::is_solid(f))
        {
            player1turn = !cur_turn;
            message("Own Ball not hit first", 2000);
        }
    }
    else // Player 2 turn
    {
        if (team_color == 2 && Ball::is_stripes(f))
        {
            player1turn = !cur_turn;
            message("Own Ball not hit first", 2000);
        }
        else if (team_color == 1 && Ball::is_solid(f))
        {
            player1turn = !cur_turn;
            message("Own Ball not hit first", 2000);
        }
    }

}

void LevelNoGraphics::check_balls_off_table(bool cur_turn)
{
    // Check balls off the table
    for (auto& b : balls)
        if (ball_off_table(b))
        {
            auto pos = get_safe_pos();
            b.posData.pos_x = pos.first;
            b.posData.pos_y = pos.second;
            b.movData.speed_x = 0.0;
            b.movData.speed_y = 0.0;

            player1turn = !cur_turn;
            message("Ball fell off the table", 2000);
        }

    if (ball_off_table(cueball))
    {
        create_cue_ball();
        recenter_cue();
        player1turn = !cur_turn;
        message("Cue Ball fell off the table", 2000);
    }
}

void LevelNoGraphics::check_team_color(bool cur_turn)
{
    // Check team color
    if (team_color == 0)
    {
        if (std::all_of(pockets.begin(), pockets.end(), &Ball::is_solid))
            team_color = cur_turn ? 1 : 2;
        else if (std::all_of(pockets.begin(), pockets.end(), &Ball::is_stripes))
            team_color = cur_turn ? 2 : 1;
    }
}

void LevelNoGraphics::check_balls_in_pockets(bool cur_turn)
{
    // Balls pocketed
    if (pockets.size() == 0)
        player1turn = !cur_turn;
    else
    {
        // Cue ball in the pocket
        if (std::find(pockets.begin(), pockets.end(), 0) != pockets.end())
        {
            create_cue_ball();
            recenter_cue();
            player1turn = !cur_turn;
            message("Cue Ball pocketed", 2000);
        }

        if (cur_turn) // Player 1 turn
        {
            // Own ball not in the pocket
            if (team_color == 1 && std::none_of(pockets.begin(), pockets.end(), &Ball::is_solid))
                player1turn = !cur_turn;
            else if (team_color == 2 && std::none_of(pockets.begin(), pockets.end(), &Ball::is_stripes))
                player1turn = !cur_turn;

            // 8 ball in the pocket
            if (std::find(pockets.begin(), pockets.end(), 8) != pockets.end())
            {
                if (team_color == 0)
                    lost(cur_turn);
                else if (team_color == 1 && std::any_of(std::begin(balls),
                                                        std::end(balls),
                                                        [](Ball& b) {
                                                            return b.is_visible && Ball::is_solid(b.id);
                                                        }))
                {
                    lost(cur_turn);
                }
                else if (team_color == 2 && std::any_of(std::begin(balls),
                                                        std::end(balls),
                                                        [](Ball& b) {
                                                            return b.is_visible && Ball::is_stripes(b.id);
                                                        }))
                {
                    lost(cur_turn);
                }
                else
                {
                    won(cur_turn);
                }
            }
        }
        else // Player 2 turn
        {
            // Own ball not in the pocket
            if (team_color == 2 && std::none_of(pockets.begin(), pockets.end(), &Ball::is_solid))
                player1turn = !cur_turn;
            else if (team_color == 1 && std::none_of(pockets.begin(), pockets.end(), &Ball::is_stripes))
                player1turn = !cur_turn;

            // 8 ball in the pocket
            if (std::find(pockets.begin(), pockets.end(), 8) != pockets.end())
            {
                if (team_color == 0)
                    lost(cur_turn);
                else if (team_color == 2 && std::any_of(std::begin(balls),
                                                        std::end(balls),
                                                        [](Ball& b) {
                                                            return b.is_visible && Ball::is_solid(b.id);
                                                        }))
                {
                    lost(cur_turn);
                }
                else if (team_color == 1 && std::any_of(std::begin(balls),
                                                        std::end(balls),
                                                        [](Ball& b) {
                                                            return b.is_visible && Ball::is_stripes(b.id);
                                                        }))
                {
                    lost(cur_turn);
                }
                else
                {
                    won(cur_turn);
                }
            }
        }

        // Check team color
        check_team_color(cur_turn);

        pockets.clear();
    }
}

void LevelNoGraphics::won(bool cur_turn){
    winner = cur_turn ? 1 : 2;
}

void LevelNoGraphics::lost(bool cur_turn){
    winner = cur_turn ? 2 : 1;
}

void LevelNoGraphics::shoot(double speed){
    collobserver.reset_first_hit();

    double angle = (cue.getAngle() * PI) / 180.0;

    // std::cout << "Angle: " << angle << ", d_angle: " << d_shoot(speed, angle) << std::endl;

    cueball.movData.speed_y = -1 * std::sin(angle) * speed;
    cueball.movData.speed_x = -1 * std::cos(angle) * speed;

    cueball.notify(Event::SUBJECT_CUE_COLLIDED);
    move_was_made = true;
    mouse_pressed = false;
}