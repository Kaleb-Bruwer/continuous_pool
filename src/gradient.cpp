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
    double d_x, d_y;
    double rate = 0.005;

    int passes = 0;
    do{
        printf("Pass: %d, xy: (%f, %f), d_xy: (%f, %f)\n", passes, x, y, d_x, d_y);
        (l->*func)(x, d_x, y, d_y);

        x -= d_x * rate;
        y -= d_y * rate;

        passes++;
        rate *= 0.95;
    }while((abs(d_x) > 0.001 || abs(d_y) > 0.001) && passes < 100);
    printf("Pass: %d, xy: (%f, %f), d_xy: (%f, %f)\n", passes, x, y, d_x, d_y);
}
