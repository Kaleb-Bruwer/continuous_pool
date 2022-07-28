#include "gradient.h"

#include "level.h"
#include "forwardlevel.h"

#include <cmath>
#include <cstdio>
#include <float.h>
#include <thread>
#include <functional>


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
        {-0.2, -0.2},
        {-0.2, 0.2},
        {0.2, -0.2},
        {0.2, 0.2},
        {1,0},
        {0,1},
        {-1,0},
        {0, -1},
        {-4,0},
        {-4,0.2},
        {-4,-0.2}
    };

    int index = -1;
    double best = DBL_MAX;

    std::vector<double> results(starts.size(), DBL_MAX);
    std::vector<std::thread*> threads(starts.size());

    // Each is checked independently
    for(int i=0; i<starts.size(); i++){
        // threads[i] = new std::thread(dummy_func);
        threads[i] = new std::thread(descent_stop_dist_card, func, l, &starts[i], &results[i]);
        threads[i]->join();
    }

    for(int i=0; i<starts.size(); i++){
        delete threads[i];

        if(results[i] < best){
            best = results[i];
            index = i;
        }
    }

    return starts[index];
}

void descent_stop_dist_card(level_func func, Level* l, std::pair<double, double>* start, double* result){
    double& x = start->first;
    double& y = start->second;

    double d_x = 0, d_y = 0;

    double rate = 0.01;
    double momentum_factor = 0.7;

    double change_x = 0, change_y = 0;
    double old_change_x = 0, old_change_y = 0;

    int passes = 0;
    const int max_passes = 150;
    do{
        printf("Pass: %d, xy: (%f, %f), d_xy: (%f, %f)\n", passes, x, y, d_x, d_y);
        (l->*func)(x, d_x, y, d_y);

        d_x = fmin(d_x, 100);
        d_y = fmin(d_y, 100);

        d_x = fmax(d_x, -100);
        d_y = fmax(d_y, -100);

        change_x = d_x * rate + old_change_x;
        change_y = d_y * rate + old_change_y;

        if(change_x == 0 && change_y == 0)
            return;

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
    *result = forward.stop_dist_card(x,y);
}
