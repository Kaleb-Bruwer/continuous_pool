#include "gradient.h"

#include "level.h"

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

void descent_stop_dist(level_func func, Level* l, double& angleR, double& speed){
    double d_angle = 0, d_speed = 0;

    double rate_sp = 0.005;
    double rate_ang = 0.0005;

    int passes = 0;
    do{
        printf("Pass: %d, Angle: %f, d_angle: %f, Speed: %f, d_speed: %f\n", passes, angleR, d_angle, speed, d_speed);
        (l->*func)(angleR, d_angle, speed, d_speed);

        angleR -= d_angle * rate_ang;

        speed -= d_speed * rate_sp;
        speed = fmax(speed, 0.01);
        speed = fmin(speed, 11);

        passes++;
        rate_sp *= 0.98;
        rate_ang *= 0.98;
    }while((abs(d_angle) > 0.001 || abs(d_speed) > 0.001) && passes < 100);
    printf("Pass: %d, Angle: %f, d_angle: %f, Speed: %f, d_speed: %f\n", passes, angleR, d_angle, speed, d_speed);
}
