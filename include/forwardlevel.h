#pragma once

#include "levelNoGraphics.h"

class ForwardLevel : public LevelNoGraphics{
public:
    ForwardLevel(const LevelNoGraphics& original);

    void run_to_stop();

    double stop_dist(double angleR, double speed);
    double stop_dist_card(double x, double y);

    double min_dist_card(double x, double y, double x_t, double y_t);

    double sink_balls(double x, double y, bool player);

    double ball_to_pocket(int i);
    double min_dist_pocket(bool player); //0:stripes, 1:solids

};

double stop_dist_wrap(ForwardLevel* l, double angleR, double speed);