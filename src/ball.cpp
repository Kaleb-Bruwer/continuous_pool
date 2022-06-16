#include "ball.h"

#include <cmath>

Ball::Ball()
    : texture{},
      posData{0.0, 0.0, 0.0},
      movData{0.0, 0.0, 0.0, 0.0}
{

}

Ball::Ball(const std::string& path)
    : Ball()
{
    setTex(path);
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

void Ball::move()
{
    if (is_movable)
    {
        // Update position
        posData.pos[0] += movData.speed[0];
        posData.pos[1] += movData.speed[1];

        // Update Speed
        movData.speed[0] += movData.accel[0];
        movData.speed[1] += movData.accel[1];

        // Apply friction
        movData.speed[0] *= 0.995;
        movData.speed[1] *= 0.995;

        // Check for complete stop
        if (std::abs(movData.speed[0]) < 0.08 && std::abs(movData.speed[1]) < 0.08)
        {
            movData.speed[0] = 0.0;
            movData.speed[1] = 0.0;
        }

        // Notify event
        notify(Event::SUBJECT_MOVED);
    }
}

void Ball::render()
{
    texture.render(static_cast<int>(posData.pos[0] - posData.radius),
                   static_cast<int>(posData.pos[1] - posData.radius));
}

bool Ball::is_moving() const noexcept
{
    return is_movable && !movData.speed.is_zero();
}
