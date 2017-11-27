#pragma once

#include "Point.h"
#include "Overview.h"

typedef struct
{
    char** ceiling;
    char** walling;
    char** floring;
    int rows;
    int cols;
}
Map;

Map xopen(const int level);

void xclose(const Map);

Map xreopen(const Map, const int level);

int xisportal(const Map, const Point where);

void xedit(const Map, const Overview);

int xout(const Map, const Point where);
