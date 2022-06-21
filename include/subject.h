#ifndef SUBJECT_H
#define SUBJECT_H

#include "observer.h"
#include "vec.h"
#include <vector>

struct MovementData {
    vec2d speed;
    vec2d accel;
};

class Ball;

class Subject {
    std::vector<Observer*> observers_;
public:
    Subject();
    virtual ~Subject() noexcept;
    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    void notify(Event event);

    virtual void render() = 0;
    virtual void move() = 0;

    virtual double next_collision(Ball* b, double start) = 0;
};

#endif // SUBJECT_H
