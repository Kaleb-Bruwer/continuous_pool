#include "forwardlevel.h"
#include <cstdio>
#include <cmath>
#include <float.h>

#include "helpers.h"

ForwardLevel::ForwardLevel(const LevelNoGraphics& original){
    LevelNoGraphics* thisL = (LevelNoGraphics*) this;
    *thisL = original;
}


void ForwardLevel::run_to_stop(){
    do{
        logic();
    }while(moving_state);
}

double ForwardLevel::stop_dist(double angleR, double speed){
    double target_x = 400;
    double target_y = 200;

    double angleD = (angleR/PI) * 180.0;

    cue.setDeg(angleD);
    shoot(speed);
    run_to_stop();

    double diff_x = abs(cueball.posData.pos_x - target_x);
    double diff_y = abs(cueball.posData.pos_y - target_y);

    return sqrt(diff_x*diff_x + diff_y*diff_y);
}

double ForwardLevel::stop_dist_card(double x, double y){
    std::pair<double, double> rad = card_to_rad({x,y});
    return stop_dist(rad.first, rad.second);
}


double stop_dist_wrap(ForwardLevel* l, double angleR, double speed){
    return l->stop_dist(angleR, speed);
}

double ForwardLevel::min_dist_card(double x, double y, double x_t, double y_t){
    std::pair<double, double> rad = card_to_rad({x,y});

    cue.setDeg(rad.first);
    shoot(rad.second);

    double closest = DBL_MAX;

    auto dist_func = [](double a, double b, double a_t, double b_t) -> double{
        double a_diff = a - a_t;
        double b_diff = b - b_t;
        return sqrt(a_diff * a_diff + b_diff * b_diff);
    };

    do{
        double d = dist_func(cueball.posData.pos_x, cueball.posData.pos_y, x_t, y_t);
        closest = fmin(d, closest);
        logic();
    }while(moving_state);

    double d = dist_func(cueball.posData.pos_x, cueball.posData.pos_y, x_t, y_t);
    return fmin(d, closest);
}

double ForwardLevel::min_dist_pocket(bool player){
    for(int b : pockets){
        if(Ball::is_stripes(b) == player);
            return 0;
    }

    double closest = DBL_MAX;

    for(int i = 0; i < 15; i++){
        if(Ball::is_stripes(balls[i].id) == player){
            closest = fmin(tab.dist_to_pocket(balls[i].posData.pos_x, balls[i].posData.pos_y), closest);
        }
    }

    return closest;
}

double ForwardLevel::sink_balls(double x, double y, bool player){
    std::pair<double, double> rad = card_to_rad({x,y});

    cue.setDeg(rad.first);
    shoot(rad.second);

    double closest = DBL_MAX;

    do{
        double d = min_dist_pocket(player);
        closest = fmin(d, closest);
        if(closest == 0)
            return closest;
        logic();
    }while(moving_state);

    double d = min_dist_pocket(player);
    return fmin(d, closest);   
}
