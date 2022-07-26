#include "gradient.h"

#include "level.h"
#include "forwardlevel.h"

#include <cmath>
#include <cstdio>
#include <float.h>


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

std::pair<double, double> stop_dist_multistart(level_func func, Level* l){
    std::vector<std::pair<double, double>> starts = {
        {0,0}, 
        {-2, -2},
        {-2, 2},
        {2, -2},
        {2, 2},
        {1,0},
        {0,1},
        {-1,0},
        {0, -1}
    };

    int index = 0;
    double best = descent_stop_dist(func, l, starts[0].first, starts[0].second);
    printf("start (%f, %f) --> %f\n", starts[0].first, starts[0].second, best);

    for(int i=1; i<starts.size(); i++){
        double temp = descent_stop_dist(func, l, starts[i].first, starts[i].second);
        printf("start (%f, %f) --> %f\n", starts[i].first, starts[i].second, temp);
        if(temp < best){
            best = temp;
            index = i;
        }
    }

    return starts[index];
}


double descent_stop_dist(level_func func, Level* l, double& x, double& y){
    double d_x = 0, d_y = 0;

    double rate = 0.01;
    double momentum_factor = 0.7;

    double change_x = 0, change_y = 0;
    double old_change_x = 0, old_change_y = 0;

    int passes = 0;
    const int max_passes = 150;
    do{
        if(passes % 10 == 0)
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
    }while((abs(d_x) > 0.01 || abs(d_y) > 0.01) && passes < max_passes);
    printf("Final Pass (%d), xy: (%f, %f), d_xy: (%f, %f)\n", passes, x, y, d_x, d_y);

    ForwardLevel forward(*l);
    return forward.stop_dist_card(x,y);
}
