#include "collisionobserver.h"

#include "rect.h"
#include "ball.h"
#include "subject.h"
#include "level.h"
#include "collision.h"

#include <cmath>

CollisionObserver::CollisionObserver() {}

void CollisionObserver::populateSearchTable(){
    searchTable.populate(observed_);
}

void CollisionObserver::onNotify(Subject& entity, Event event)
{
    // if (event == Event::SUBJECT_MOVED)
    //     checkCollisions(dynamic_cast<Ball*>(&entity));
}

double CollisionObserver::distanceSquared(double x1, double y1, double x2, double y2)
{
    double deltaX = x2 - x1;
    double deltaY = y2 - y1;

    return deltaX*deltaX + deltaY*deltaY;
}

// Check if 2 balls collided
bool CollisionObserver::collided(Ball* b, Ball* b2)
{
    vec2d dist = b->posData.pos - b2->posData.pos;
    double radius = b->posData.radius + b2->posData.radius;
    return (dist.magnitude() <= radius);
}

// Check is Ball* b collided with Rect* r
bool CollisionObserver::collided(Ball* b, Rect* r)
{
    //Closest point on collision box
    auto closer = find_closer(b, r);

    return distanceSquared(closer.first, closer.second, b->posData.pos[0], b->posData.pos[1]) < b->posData.radius * b->posData.radius;
}

// Find closest point on collision box
std::pair<int, int> CollisionObserver::find_closer(Ball* b, Rect* rec)
{
    int cX, cY;

    //Find closest x offset
    if(b->posData.pos[0] < rec->rect.x)
        cX = rec->rect.x;
    else if(b->posData.pos[0] > rec->rect.x + rec->rect.w )
        cX = rec->rect.x + rec->rect.w;
    else
        cX = static_cast<int>(b->posData.pos[0]);

    //Find closest y offset
    if(b->posData.pos[1] < rec->rect.y)
        cY = rec->rect.y;
    else if( b->posData.pos[1] > rec->rect.y + rec->rect.h )
        cY = rec->rect.y + rec->rect.h;
    else
        cY = static_cast<int>(b->posData.pos[1]);

    return {cX, cY};
}

// Fix movement of the ball when collided with wall
void CollisionObserver::fix(Ball* b, Rect* rec)
{
    auto closer = find_closer(b, rec);
    int cX = closer.first, cY = closer.second;

    if (b->movData.speed[1] > 0)
    {
        if (cY == rec->rect.y) // collided from above
        {
            b->movData.speed[1] *= -1;
            b->movData.speed[1] *= 0.8;
            b->posData.pos[1] = cY - b->posData.radius;
        }
        else // collided from side
        {
            b->movData.speed[0] *= -1;
            b->movData.speed[0] *= 0.8;
            if (b->movData.speed[0] > 0) // from left side
                b->posData.pos[0] = cX + b->posData.radius;
            else // from right side
                b->posData.pos[0] = cX - b->posData.radius;
        }
    }
    else
    {
        if (cY == rec->rect.y + rec->rect.h) //collided from bellow
        {
            b->movData.speed[1] *= -1;
            b->movData.speed[1] *= 0.8;
            b->posData.pos[1] = cY + b->posData.radius;
        }
        else // collided from side
        {
            b->movData.speed[0] *= -1;
            b->movData.speed[0] *= 0.8;

            if (b->movData.speed[0] > 0) // from left side
                b->posData.pos[0] = cX + b->posData.radius;
            else // from right side
                b->posData.pos[0] = cX - b->posData.radius;
        }
    }
}

void CollisionObserver::fix(Ball* b, Ball* b2)
{
    // collision2Ds(b->mass, b2->mass, 0.92, b->posData.pos[0], b->posData.pos[1],
    //              b2->posData.pos[0], b2->posData.pos[1], b->movData.speed[0], b->movData.speed[1],
    //              b2->movData.speed[0], b2->movData.speed[1]);
}
