#include "collision.h"

#include <float.h>
#include <complex>

#include "quartic.h"
#include "vec.h"

using namespace std;

#define sq(a) pow(a,2)

double next_collision(Ball* b1, Ball* b2){
    vec2d rel_pos = b1->posData.pos - b2->posData.pos;
    vec2d rel_speed = b1->movData.speed - b2->movData.speed;
    vec2d rel_accel = b1->movData.accel - b2->movData.accel;

    double min_dist = b1->posData.radius + b2->posData.radius;

    // Keep in mind that this doesn't have to be repeated every tick

    double a = 0.5 * (sq(rel_accel[0]) + sq(rel_accel[1]));
    double b = rel_accel[0] * rel_speed[0] + rel_accel[1] * rel_speed[1];
    double c = rel_accel[0] * rel_pos[0] + rel_accel[1] * rel_pos[1]
            + sq(rel_speed[0]) + sq(rel_speed[1]);
    double d = 2 * rel_pos[0] * rel_speed[0] + 2 * rel_pos[1] * rel_speed[1];
    double e = sq(rel_pos[0]) + sq(rel_pos[1]) - min_dist*min_dist;

    // solve_quartic solves quartic equations of the form x^4 + a*x^3 + b*x^2 + c*x + d
    // so every parameter must be divided by a to scale 1st term to 1
    DComplex* solutions = solve_quartic(b/a, c/a, d/a, e/a);

    double next_coll = DBL_MAX;

    for(int i=0; i<4; i++){
        if(solutions[i].imag() != 0)
            continue;
        if(solutions[i].real() > 0 && solutions[i].real() < next_coll)
            next_coll = solutions[i].real();
    }
    delete [] solutions;

    return next_coll;
}
