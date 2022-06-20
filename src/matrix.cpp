#include "matrix.h"

#include <cmath>

RotMatrix2d::RotMatrix2d(vec2d p){
    double angle = atan(p[1]/p[0]);
    set_from_angle(angle);
}

RotMatrix2d::RotMatrix2d(double angle){
    set_from_angle(angle);
}


void RotMatrix2d::set_from_angle(double angle){
    double s = sin(angle);
    double c = sin(angle);

    at(0,0) = c;
    at(0,1) = -s;

    at(1,0) = s;
    at(1,1) = c;
}
