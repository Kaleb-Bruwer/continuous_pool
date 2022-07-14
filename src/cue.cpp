#include "cue.h"

Cue::Cue()
    : t{}, posX{0.0}, posY{0.0}, degrees{0.0}, center{}
{
    t.loadFromFile("./pool_assets/cue.png");
    center = {-10, static_cast<int>(t.getHeight()/2)};
}

const Cue& Cue::operator=(const Cue& rhs){
    // t = rhs.t;
    posX = rhs.posX;
    posY = rhs.posY;
    degrees = rhs.degrees;
    center = rhs.center;
}

void Cue::render()
{
    t.render(static_cast<int>(posX), static_cast<int>(posY), nullptr, degrees, &center, SDL_FLIP_HORIZONTAL);
}

void Cue::setPos(double x, double y)
{
    posX = x;
    posY = y;
}

void Cue::setDeg(double angle)
{
    degrees = angle;
}

void Cue::setX(double x)
{
    posX = x;
}
