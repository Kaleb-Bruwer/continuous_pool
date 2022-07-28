#include "level.h"
#include "mainwindow.h"
#include "enzyme.h"
#include "forwardlevel.h"
#include "helpers.h"

#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>
#include <cstdio>

#include <SDL.h>

Level::Level()
    : background{}, p1{}, p2{}, f1{}, b8{}, b1{}, b9{},
      audio{}, LevelNoGraphics()
{
    background.loadFromFile("./pool_assets/background.png");
    f1.loadFromFile("./pool_assets/Purisa-BoldOblique.ttf", 20);
    p1.loadFromRenderedText("Player 1", f1, SDL_Color{0xFF,0xFF,0xFF,0xFF});
    p2.loadFromRenderedText("Player 2", f1, SDL_Color{0xFF,0xFF,0xFF,0xFF});
    b8.loadFromFile("./pool_assets/ball8.png");
    b1.loadFromFile("./pool_assets/ball1.png");
    b9.loadFromFile("./pool_assets/ball9.png");

    create_balls();
    create_cue_ball();
}

void Level::handle_events()
{
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
            GameState::next_state = GameStates::Quit;
        else if (!moving_state)
            handle_when_still(e);
    }
}

void Level::render()
{
    mainwindow->clear();

    background.render(0, 0);
    tab.render();

    for (auto& b: balls)
        if (b.is_visible)
            b.render();

    if (cueball.is_visible)
        cueball.render();

    if (!moving_state && cueball.is_visible)
        cue.render();

    render_head();

    mainwindow->update();
}

void Level::handle_when_still(SDL_Event& e)
{
    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_e){
        auto_shoot();
    }
    else if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_RETURN))
    {
        shoot(11.0);
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        mouse_pressed = true;
    }
    else if (e.type == SDL_MOUSEBUTTONUP)
    {
        double force = std::hypot(cueball.posData.pos_x - cue.getX(),
                                  cueball.posData.pos_y - cue.getY());

        force /= 8.0;

        if (force > 12.0)
            force = 12.0;
        else if (force < 1.0)
            force = 1.0;

        shoot(force);
    }
    else if (e.type == SDL_MOUSEMOTION)
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );

        auto oposed = cueball.posData.pos_y - y;
        auto hyp = std::hypot(cueball.posData.pos_x - x, cueball.posData.pos_y - y);
        double degrees = (std::asin(oposed / hyp) * 180.0) / PI + 180.0;

        if (x > cueball.posData.pos_x)
            degrees = 180.0 - degrees;

        if (degrees < 0.0)
            degrees += 360.0;

        if (!mouse_pressed)
            cue.setDeg(degrees);
        else
        {
            double angle = (cue.getAngle() * PI) / 180.0;

            double px = cueball.posData.pos_x + (std::cos(angle) * hyp) + cueball.posData.radius;
            double py = cueball.posData.pos_y + (std::sin(angle) * hyp) - cueball.posData.radius + (cue.getHeight()/2.0);

            cue.setPos(px, py);
        }
    }
}

void Level::create_balls()
{
    // Create 15 balls
    for (int i = 1; i <= 15; ++i)
    {
        balls[i-1].setTex("./pool_assets/ball" + std::to_string(i) + ".png");
        balls[i-1].id = i;
        balls[i-1].addObserver(&collobserver);
        balls[i-1].addObserver(&audio);
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

void Level::create_cue_ball()
{
    // Create and place the cue ball
    cueball = Ball{};
    cueball.setTex("./pool_assets/ball0.png");
    cueball.id = 0;
    cueball.mass = 11.0;
    cueball.addObserver(&collobserver);
    cueball.addObserver(&audio);

    auto pos = get_safe_pos();
    cueball.setPos(pos.first, pos.second);
}


void Level::render_head()
{
    p1.render(5, 5);
    p2.render(static_cast<int>(background.getWidth() - p2.getWidth()) - 5, 5);

    if (player1turn)
    {
        switch(team_color)
        {
            case 0:
            default:
                b8.render(10 + static_cast<int>(p1.getWidth()), 10);
                break;
            case 1:
                b1.render(10 + static_cast<int>(p1.getWidth()), 10);
                break;
            case 2:
                b9.render(10 + static_cast<int>(p1.getWidth()), 10);
                break;
        }
    }
    else // player 2 turn
    {
        switch(team_color)
        {
            case 0:
            default:
                b8.render(static_cast<int>(background.getWidth() - p2.getWidth()) - 30, 10);
                break;
            case 1:
                b9.render(static_cast<int>(background.getWidth() - p2.getWidth()) - 30, 10);
                break;
            case 2:
                b1.render(static_cast<int>(background.getWidth() - p2.getWidth()) - 30, 10);
                break;
        }
    }
}


void Level::won(bool cur_turn)
{
    LevelNoGraphics::won(cur_turn);
    if (cur_turn)
        message("Player 1 Wins!!", 4000);
    else
        message("Player 2 Wins!!", 4000);

    GameState::next_state = GameStates::Intro;
}

void Level::lost(bool cur_turn)
{
    LevelNoGraphics::lost(cur_turn);
    if (cur_turn)
        message("Player 1 Lost!!", 4000);
    else
        message("Player 2 Lost!!", 4000);

    GameState::next_state = GameStates::Intro;
}

void Level::message(const std::string& msg, unsigned delay)
{
    Font f {};
    f.loadFromFile("./pool_assets/Purisa-BoldOblique.ttf", 20);

    Texture t {};
    t.loadFromRenderedText(msg, f, SDL_Color{0xFF,0xFF,0xFF,0xFF});

    this->render();

    t.render(static_cast<int>((mainwindow->getWidth() - t.getWidth()) / 2),
             static_cast<int>((mainwindow->getHeight() - t.getHeight()) / 2));

    mainwindow->update();

    SDL_Delay(delay);
}

void Level::auto_shoot(){
    printf("!!!!!!!!!!START AUTOSHOOT !!!!!!!!!!\n");
    
    std::pair<double, double> velocity = stop_dist_multistart(&Level::d_pocket_dist_num, this);    
    std::pair<double, double> rad = card_to_rad(velocity);

    printf("Chosen parameters (angle,speed): (%f, %f)\n", rad.first, rad.second);

    cue.setDeg( (rad.first/PI) * 180.0);
    shoot(rad.second);

    ForwardLevel forward(*this);
    forward.cue.setDeg((rad.first/PI) * 180.0);
    forward.shoot(rad.second);
    forward.run_to_stop();
    printf("Pos after autoshoot: (%f, %f)\n", forward.cueball.posData.pos_x, forward.cueball.posData.pos_y);
    
    printf("!!!!!!!!!!END AUTOSHOOT !!!!!!!!!!\n\n");

}

double Level::d_shoot(double speed, double angle){
    return __enzyme_autodiff((void*)shoot_wrap,
            enzyme_const, this,
            enzyme_const, speed,
            enzyme_out, angle);
}

void Level::d_stop_dist(double angleR, double& d_angleR, double speed, double& d_speed){
    ForwardLevel forward(*this);

    __enzyme_autodiff((void*)stop_dist_wrap,
        enzyme_const, &forward,
        enzyme_dup, angleR, d_angleR,
        enzyme_dup, speed, d_speed
    );
}

void Level::d_stop_dist_num(double x, double& d_x, double y, double& d_y){
    double step_size = 0.00001;

    ForwardLevel forward(*this);
    double baseline = forward.stop_dist_card(x, y);

    forward = ForwardLevel(*this);
    double step_x = forward.stop_dist_card(x + step_size, y);

    forward = ForwardLevel(*this);
    double step_y = forward.stop_dist_card(x, y + step_size);

    d_x = (step_x - baseline)/step_size;
    d_y = (step_y - baseline)/step_size;
}

void Level::d_pocket_dist_num(double x, double& d_x, double y, double& d_y){
    double step_size = 0.00001;

    d_x = 0;
    d_y = 0;

    bool player = ((player1turn && team_color == 2) || (!player1turn && team_color == 1));

    ForwardLevel forward(*this);
    double baseline = forward.sink_balls(x, y, player);
    printf("Pocket dist %f\n", baseline);

    if(baseline == 0)
        return;

    forward = ForwardLevel(*this);
    double step_x = forward.sink_balls(x + step_size, y, player);

    forward = ForwardLevel(*this);
    double step_y = forward.sink_balls(x, y + step_size, player);

    d_x = -(step_x - baseline)/step_size;
    d_y = -(step_y - baseline)/step_size;  
}

double angle_descend(double angle){
    return __enzyme_autodiff((void*)shoot_wrap,
        enzyme_const, (Level*)0,
        enzyme_const, 1.0, //Speed has linear influence (for now)
        enzyme_out, angle);
}

double shoot_wrap(Level* t, double speed, double angle){
    // Basic proof-of-concept before I do some rewrites (to support reversibility)
    // double y = -1 * std::sin(angle) * speed;
    double x = -1 * std::cos(angle) * speed;    

    return x;
}

