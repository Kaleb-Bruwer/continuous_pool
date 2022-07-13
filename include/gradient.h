#pragma once

#include <float.h>

typedef double (*descendable)(double);

double gradient_descent(descendable func, double start = 0, double min = -DBL_MAX, double max = DBL_MAX);