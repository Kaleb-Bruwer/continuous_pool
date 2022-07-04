#ifndef COLLISION_H
#define COLLISION_H

#include "rect.h"
#include "ball.h"
#include "pocket.h"

// single contact (sqrt term is 0) is ignored
// all solutions <= 0 are ignored
// if no valid solutions, return -1
double quadratic_next(double a, double b, double c);

// checks if derivative of distance is negative
// a to d are terms of a polymorphic equation of t
bool moving_towards(double a, double b, double c, double d, double t);

void collision2Ds(double m1, double m2, double R,
                  double x1, double y1, double x2, double y2,
                  double& vx1, double& vy1, double& vx2, double& vy2);

// returns time until contact between given balls, -1 if they won't touch
double _next_collision(const Path p1, double r1, const Path p2, double r2);
double _next_collision(const Path p, double r, Line l);

invalids apply_collision(Ball& b1, Subject* s, double time);

invalids apply_collision(Ball& b1, Ball& b2, double time);
invalids apply_collision(Ball& b, const Line l, double time);
invalids apply_collision(Ball& b, const Pocket &p, double time);

#endif
