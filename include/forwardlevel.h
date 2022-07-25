#pragma once

#include "levelNoGraphics.h"

class ForwardLevel : public LevelNoGraphics{
public:
    ForwardLevel(const LevelNoGraphics& original);

    void run_to_stop();

    double stop_dist(double angleR, double speed);

};

double stop_dist_wrap(ForwardLevel* l, double angleR, double speed);