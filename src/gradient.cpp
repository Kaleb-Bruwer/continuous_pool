#include "gradient.h"

#include <cmath>

double gradient_descent(descendable func, double val, double min, double max){
    double gradient = 1;
    double rate = 0.05;

    double passes = 0;

    while(gradient > 0.001 && passes < 100){
        double gradient = func(val);
        val -= gradient * rate;

        rate *= 0.98;
        passes++;
    }
    return val;
}
