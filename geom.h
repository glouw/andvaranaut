#pragma once

#include <stdbool.h>

struct point
{
    double x, y;
};

struct point sub(const struct point i, const struct point j);
struct point add(const struct point i, const struct point j);
struct point mul(const struct point i, const double n);
double mag(const struct point point);

double wall_percentage(const struct point point);
bool out(const struct point point);
struct point cast(const struct point hero, const double radians);
int get_walling(const struct point point);
bool collision(const struct point point);
int get_flooring(const struct point point);
int get_ceiling(const struct point point);
