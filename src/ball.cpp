#include "ball.h"

#include <cmath>

#include "level.h"

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

// TODO: update to get speed from path instead
bool Ball::is_moving() const noexcept
{
    return is_movable && !movData.speed.is_zero();
}
