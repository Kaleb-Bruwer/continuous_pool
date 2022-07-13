#ifndef LEVEL_H
#define LEVEL_H

#include "levelNoGraphics.h"
#include "texture.h"
#include "font.h"
#include "table.h"
#include "collisionobserver.h"
#include "audioobserver.h"
#include "ball.h"
#include "cue.h"

#include "gradient.h"

#include <vector>
#include <utility>

class Level : public LevelNoGraphics {
    Texture background;
    Texture p1, p2;
    Font f1;
    Texture b8, b1, b9;

    AudioObserver audio;
public:
    Level();

    virtual void handle_events() override;
    virtual void render() override;

private:
    void handle_when_still(SDL_Event& e);
    virtual void create_balls();
    virtual void create_cue_ball();
    void render_head();
    virtual void won(bool cur_turn);
    virtual void lost(bool cur_turn);
    virtual void message(const std::string& msg, unsigned delay);
    double d_shoot(double speed, double angle);
    void shoot(double speed);

    friend double shoot_wrap(Level *l, double speed, double angle);
};

double angle_descend(double angle);
double shoot_wrap(Level *l, double speed, double angle);

#endif // LEVEL_H
