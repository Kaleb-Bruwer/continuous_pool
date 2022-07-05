#ifndef SEARCHTABLE_H
#define SEARCHTABLE_H

#include <vector>

#include "vec.h"

class Subject;
class Ball;

// Balls are the only moving objects, hence the distinction

class SearchTable{
public:
    static const int NUM_BALLS = 16;
    static const int NUM_NON_BALLS = 24;
private:
    int cueball_index = -1;

    Ball* balls[NUM_BALLS] = {0};
    Subject* non_balls[NUM_NON_BALLS] = {0};

    // Balls * (Balls, non_balls)
    // time_safe is the time up to which all collisions between these 2 objects
    // have been checked
    double time_safe[NUM_BALLS][NUM_BALLS + NUM_NON_BALLS];

    void set_index_max_time(int index, double t);

    int get_index(Subject* s);

public:
    void populate(const std::vector<Subject*>& subjects, Ball* cueball);

    void check_until(double until);
    double global_time_safe() const;

    void applyCue(vec2d vel, double time = -1);

    void set_subject_time(Subject* s, double t);
};

#endif
