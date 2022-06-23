#ifndef POCKET_H
#define POCKET_H

#include "subject.h"
#include "vec.h"

class Pocket : public Subject{
public:
    vec2d pos;
    virtual double next_collision(Ball* b, double start);

    virtual void render(){};
    virtual void move(){};

};

#endif
