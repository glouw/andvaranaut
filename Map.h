#pragma once

#include "Point.h"

typedef struct
{
    // Clang like to break these pointers
    char** volatile ceiling;
    char** volatile walling;
    char** volatile floring;
    int rows;
}
Map;

Map open(const int level);

void dump(const Map map, const int level);

void close(const Map map);

Map reopen(const Map map, const int level);

int isportal(const Map map, const Point where);
