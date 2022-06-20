#include "ball.h"

#include <cmath>

#include "level.h"
#include "collision.h"

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

vec2d Ball::pos_from_path(){
    vec2d result;
    double t = Level::get_time();

    int i;
    for(i=0; i<path.size(); i++){
        double t_end = path[i].time_end;
        if(t_end < t && t_end != -1)
            continue;

        double t_delta = t - path[i].time_start;
        result = path[i].pos_0 + path[i].vel_0 * t_delta
            + 0.5 * pow(t_delta, 2) * path[i].accel;
        break;
    }

    if(i > 0){
        path.erase(path.begin(), path.begin() + i);
    }

    return result;
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
