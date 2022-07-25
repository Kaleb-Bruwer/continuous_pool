#pragma once

#include <float.h>

class Level;

typedef double (*descendable)(double);
using level_func = void (Level::*)(double, double&, double, double&);

double gradient_descent(descendable func, double start = 0, double min = -DBL_MAX, double max = DBL_MAX);

void descent_stop_dist(level_func, Level*, double&, double&);