#ifndef COLLISIONOBSERVER_H
#define COLLISIONOBSERVER_H

#include "observer.h"
#include "searchTable.h"

#include <utility>
#include <SDL.h>

class Ball;
class Rect;

class CollisionObserver : public Observer {
public:
    CollisionObserver();

    virtual void onNotify(Subject& entity, Event event) override;

    int    get_first_hit() const noexcept {return first_hit;}
    void reset_first_hit()       noexcept {first_hit = -1;}

    double global_time_safe(){return searchTable.global_time_safe();};
    void check_all_collisions(double until){
        searchTable.check_until(until);
    };
    void populateSearchTable(Ball* cueball);
private:
    SearchTable searchTable;

    int first_hit = -1;

    double distanceSquared(double x1, double y1, double x2, double y2);
    bool collided(Ball* b, Ball* b2);
    bool collided(Ball* b, Rect* r);

    std::pair<int,int> find_closer(Ball* b, Rect* rec);

    void fix(Ball* b, Rect* rec);
    void fix(Ball* b, Ball* b2);
};

#endif // COLLISIONOBSERVER_H
