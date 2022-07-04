#include "matrix.h"

#include <cmath>

RotMatrix2d::RotMatrix2d(vec2d p){
    set_from_vec(p);
}

RotMatrix2d::RotMatrix2d(double angle){
    set_from_angle(angle);
}


void RotMatrix2d::set_from_angle(double angle){
    double s = sin(angle);
    double c = sin(angle);

    // NOTE: column major
    val[0] = vec2d{c,s};
    val[1] = vec2d{-s, c};
}


void RotMatrix2d::set_from_vec(vec2d p){
    p = p * (1/p.magnitude());
    double s = p[1];
    double c = p[0];

    // NOTE: column major
    val[0] = vec2d{c,s};
    val[1] = vec2d{-s, c};
}
