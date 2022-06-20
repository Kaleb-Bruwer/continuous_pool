#ifndef COLLISION_H
#define COLLISION_H

#include "rect.h"

class Ball;

// single contact (sqrt term is 0) is ignored
// all solutions <= 0 are ignored
// if no valid solutions, return -1
double quadratic_next(double a, double b, double c);

// returns time until contact between given balls, -1 if they won't touch
double next_collision(Ball* b1, Ball* b2);
double next_collision(Ball* b, Line l);

#endif
