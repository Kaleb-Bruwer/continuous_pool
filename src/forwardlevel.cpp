#include "forwardlevel.h"
#include <cstdio>

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
