#include "ball.h"

#include <cmath>

#include "level.h"
#include "collision.h"

using namespace std;

Ball::Ball()
    : texture{},
      posData{0.0, 0.0, 0.0},
      movData{0.0, 0.0, 0.0, 0.0}
{
    path.push_back(Path());
}

Ball::Ball(const std::string& path_p)
    : Ball()
{
    setTex(path_p);
    path.push_back(Path());
}

void Ball::setTex(const std::string& path)
{
    texture.loadFromFile(path);
    posData.radius = texture.getWidth() / 2.0;
}

void Ball::setPos(double px, double py)
{
    posData.pos[0] = px;
    posData.pos[1] = py;
}

void Ball::setVel(double sx, double sy)
{
    movData.speed[0] = sx;
    movData.speed[1] = sy;
}

Path& Ball::get_curr_path(double t){
    for(int i=0; i<path.size(); i++){
        double t_end = path[i].time_end;
        if(t_end < t && t_end != -1)
            continue;
        return path[i];
    }
    return path[path.size()-1];
}


vec2d Ball::pos_from_path(){
        pos_from_path(Level::get_time());
}

vec2d Ball::pos_from_path(double t){
    Path curr_p = get_curr_path(t);

    double t_delta = t - curr_p.time_start;
    return curr_p.pos_0 + curr_p.vel_0 * t_delta
        + 0.5 * pow(t_delta, 2) * curr_p.accel;
}

vec2d Ball::vel_from_path(){
    return vel_from_path(Level::get_time());
}

vec2d Ball::vel_from_path(double t){
    Path curr_p = get_curr_path(t);

    double t_delta = t - curr_p.time_start;
    return curr_p.vel_0 + t_delta * curr_p.accel;
}

vec2d Ball::accel_from_path(){
    return accel_from_path(Level::get_time());
}
vec2d Ball::accel_from_path(double t){
    return get_curr_path(t).accel;
}

invalids Ball::append_path(double time, vec2d pos, vec2d vel, Subject* collider){
    // future collisions that have been destroyed by this one
    invalids cancelled;

    Path p;
    p.time_start = time;
    p.pos_0 = pos;
    p.vel_0 = vel;
    p.collider = collider;

    p.apply_friction();

    int i;
    for(i=0; i<path.size(); i++){
        double t_end = path[i].time_end;
        if(t_end < time && t_end != -1)
            continue;
    }

    if(i + 1 < path.size()){
        for(int j=i+1; j< path.size(); j++)
            if(path[j].collider)
                cancelled.push_back(make_tuple(path[j].time_start, path[j].collider));
        path.erase(path.begin() + i + 1, path.end());
    }

    path[i].time_end = time;
    path.push_back(p);

    Path cap;
    cap.time_start = p.time_end;
    cap.time_end = -1;
    cap.pos_0 = pos_from_path(p.time_end);
    path.push_back(cap);

    return cancelled;
}


void Ball::move()
{
    if (is_movable)
        posData.pos = pos_from_path();

}

void Ball::render()
{
    texture.render(static_cast<int>(posData.pos[0] - posData.radius),
                   static_cast<int>(posData.pos[1] - posData.radius));
}

bool Path::time_overlap(const Path rhs){
    if(time_end == -1)
        return rhs.time_end == -1 || rhs.time_end > time_start;
    if(rhs.time_end == -1)
        return time_end > rhs.time_start;

    return time_end > rhs.time_start && time_start < rhs.time_end;
}

void Path::apply_friction(){
    double v = vel_0.magnitude();

    accel[0] = (friction * vel_0[0]) / v;
    accel[1] = (friction * vel_0[1]) / v;

    double dt = v/friction;
    time_end = time_start + dt;
};

double Ball::next_collision(Ball* b){
    // Go through both balls' path & check (time) overlaps chronologically
    int step_l = 0;
    int step_r = 0;

    double result = -1;

    //  .........|XX....|.....
    //  ....|.....XX|.........
    // Check for temporal overlap
    while(true){
        if(step_l >= path.size()
        || step_r >= b->path.size())
            break;

        Path& lhs = path[step_l];
        Path& rhs = b->path[step_r];

        if(lhs.time_overlap(rhs)){
            result = _next_collision(lhs, posData.radius, rhs, b->posData.radius);
            if(result != -1) // 1st solution is the valid one
                return result;
        }
        else{
            if(lhs.time_end <= rhs.time_start)
                step_l++;
            else
                step_r++;
        }

    }
    return result;
}


// TODO: update to get speed from path instead
bool Ball::is_moving() const noexcept
{
    return is_movable && !movData.speed.is_zero();
}
