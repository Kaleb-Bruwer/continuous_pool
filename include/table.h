#ifndef TABLE_H
#define TABLE_H

#include "texture.h"
#include "rect.h"
#include "ball.h"
#include "observer.h"

class CollisionObserver; // Forward declare

class Table {
    int posX, posY;
    double width, height;
    Texture background;
    Rect r[6] {};  // Rect colliders for walls
    Ball b[12] {}; // Ball colliders to simulate pocket sides
    SDL_Point pocket_sensors[6] {}; // Check if ball in the pocket

public:
    Table(CollisionObserver* c = nullptr);
    Table(const Table&, CollisionObserver* c);

    void create(const Table&, CollisionObserver* c);

    void render();
    bool is_pocketed(const Ball& b);

    double  getWidth() const noexcept {return width; }
    double getHeight() const noexcept {return height;}
    int         getX() const noexcept {return posX;}
    int         getY() const noexcept {return posY;}
};

#endif // TABLE_H
