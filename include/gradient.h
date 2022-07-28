#pragma once

#include <float.h>
#include "helpers.h"

class Level;

typedef double (*descendable)(double);
using level_func = void (Level::*)(double, double&, double, double&);

double gradient_descent(descendable func, double start = 0, double min = -DBL_MAX, double max = DBL_MAX);

std::pair<double, double> stop_dist_multistart(level_func, Level*);
double descent_stop_dist(level_func, Level*, double&, double&);

void descent_stop_dist_card(level_func func, Level* l, std::pair<double, double>* start, double* result);