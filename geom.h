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

double percentage(const struct point point);
bool out(const struct point point);
struct point cast(const struct point hero, const double radians);
int getwalling(const struct point point);
bool collision(const struct point point);
int getflooring(const struct point point);
int getceiling(const struct point point);
