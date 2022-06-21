#ifndef SEARCHTABLE_H
#define SEARCHTABLE_H

#include <vector>

class Subject;
class Ball;

// Balls are the only moving objects, hence the distinction

class SearchTable{
private:
    Ball* balls[11] = {0};
    Subject* non_balls[24] = {0};

    // Balls * (Balls, non_balls)
    // time_safe is the time up to which all collisions between these 2 objects
    // have been checked
    double time_safe[11][35];

    void set_index_time(int index, double t);
    void set_index_max_time(int index, double t);

    int get_index(Subject* s);

public:
    void populate(const std::vector<Subject*>& subjects);

    void check_until(double until);
    double global_time_safe() const;

};

#endif
