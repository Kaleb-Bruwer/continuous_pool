#include "gradient.h"

#include "level.h"
#include "helpers.h"

#include <cmath>
#include <cstdio>


double gradient_descent(descendable func, double val, double min, double max){
    double gradient = 1;
    double rate = 0.05;

    int passes = 0;

    while(gradient > 0.001 && passes < 100){
        double gradient = func(val);
        val -= gradient * rate;

        rate *= 0.98;
        passes++;
    }
    return val;
}

void descent_stop_dist(level_func func, Level* l, double& x, double& y){
    double d_x = 0, d_y = 0;

    double rate = 0.01;
    double momentum_factor = 0.7;

    double change_x = 0, change_y = 0;
    double old_change_x = 0, old_change_y = 0;

    int passes = 0;
    do{
        printf("Pass: %d, xy: (%f, %f), d_xy: (%f, %f)\n", passes, x, y, d_x, d_y);
        (l->*func)(x, d_x, y, d_y);

        d_x *= d_x;
        d_y *= d_y;

        change_x = d_x * rate + old_change_x;
        change_y = d_y * rate + old_change_y;

        x -= change_x;
        y -= change_y;

        double mag = sqrt(x*x + y*y);
        if(mag > 11){
            x *= 11/mag;
            y *= 11/mag;
        }

        old_change_x = change_x * momentum_factor;
        old_change_y = change_y * momentum_factor;

        passes++;
        rate *= 0.95;
    }while((abs(d_x) > 0.01 || abs(d_y) > 0.01) && passes < 100);
    printf("Pass: %d, xy: (%f, %f), d_xy: (%f, %f)\n", passes, x, y, d_x, d_y);
}
