#ifndef COLLISION_H
#define COLLISION_H

#include "ball.h"

// returns time until contact between given balls, -1 if they won't touch
double next_collision(Ball* b1, Ball* b2);

#endif
