#include "pocket.h"

#include "ball.h"
#include "collision.h"

double Pocket::next_collision(Ball* b, double start){
    double result = -1;

    for(Path &p : b->path){
        if(p.time_end <= start && p.time_end != -1)
            continue;

        if(p.pocketed)
            return -1;

        Path p2 = p;
        p2.pos_0 = pos;
        p2.vel_0 = vec2d{0,0};
        p2.accel = vec2d{0,0};

        result = _next_collision(p, b->posData.radius, p2, 0);
        if(result != -1)
            return result;
    }
    return result;
}
