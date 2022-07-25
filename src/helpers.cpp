#include "helpers.h"

#include <cmath>

const double PI = std::atan(1) * 4;

std::pair<double, double> rad_to_card(std::pair<double, double> rad){
    double x, y;

    double angle = rad.first;
    double mag = rad.second;

    x = std::cos(angle) * mag;
    y = std::sin(angle) * mag;
    return {x,y};
}

std::pair<double, double> card_to_rad(std::pair<double, double> card){
    double angle, mag;

    double x = card.first;
    double y = card.second;

    if(x==0){
        angle = (y>0) ? PI/2 : 3*PI/2;

    }
    else angle = std::atan(y/x);

    mag = sqrt(x*x + y*y);

    return {angle, mag};
}