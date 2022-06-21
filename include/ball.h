#ifndef BALL_H
#define BALL_H

#include <vector>
#include <tuple>

#include "subject.h"
#include "texture.h"
#include "vec.h"

typedef std::vector<std::tuple<double, Subject*>> invalids;
const double friction = 0.4;

struct Circle {
    vec2d pos;
    double radius;
};

// Path gives the movement equation for each segment of the ball's future path,
// with a new struct after every collision/stop

// time_start is taken as time 0 within pos and vel, accel is constant

struct Path{
    vec2d pos_0;
    vec2d vel_0;
    vec2d accel;
    double time_start = 0;
    double time_end = -1;
    Subject* collider = 0; // for collision at start of Path, not end

    bool time_overlap(const Path rhs);

    // Sets accel to properly apply friction and time_end to when motion will stop
    void apply_friction();

    vec2d pos(double t) const;
    vec2d vel(double t) const;
};

class Ball : public Subject  {
    Texture texture;
public:
    bool is_movable = true;
    bool is_visible = true;
    Circle posData;
    MovementData movData;
    double mass = 10.0;
    int id = 0; // the number of the ball

    std::vector<Path> path;

    Ball();
    Ball(const std::string& path);

    void setTex(const std::string& path);
    void setPos(double px, double py);
    void setVel(double sx, double sy);

    Path& get_curr_path(double t);

    vec2d pos_from_path();
    vec2d vel_from_path();
    vec2d accel_from_path();

    vec2d pos_from_path(double t);
    vec2d vel_from_path(double t);
    vec2d accel_from_path(double t);

    // Also applies friction logic to path
    // return: balls that need to be re-checked from specified times
    invalids append_path(double time, vec2d pos, vec2d vel, Subject* collider);
    invalids prune_path(double time);

    virtual void   move() override;
    virtual void render() override;

    virtual double next_collision(Ball* b, double start);


    bool is_moving() const noexcept;

    static bool   is_solid(int num) {return num > 0 && num < 8;}
    static bool is_stripes(int num) {return num > 8 && num < 16;}
};

#endif // BALL_H
