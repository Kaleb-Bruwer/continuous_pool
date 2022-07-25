#pragma once

#include <utility>

extern const double PI;

//pair layouts: {x,y}, {angle (radians), magnitude}
std::pair<double, double> rad_to_card(std::pair<double, double>);
std::pair<double, double> card_to_rad(std::pair<double, double>);
