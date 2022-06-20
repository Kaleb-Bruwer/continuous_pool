#ifndef COLLISION_H
#define COLLISION_H

#include "rect.h"
#include "ball.h"

// single contact (sqrt term is 0) is ignored
// all solutions <= 0 are ignored
// if no valid solutions, return -1
double quadratic_next(double a, double b, double c);

// returns time until contact between given balls, -1 if they won't touch
double _next_collision(const Path p1, double r1, const Path p2, double r2);
double _next_collision(const Path p, double r, Line l);

#endif
