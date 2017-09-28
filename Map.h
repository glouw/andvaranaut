#pragma once

#include "Point.h"

#include <stdbool.h>

typedef struct
{
    char** ceiling;
    char** walling;
    char** floring;
    int rows;
}
Map;

struct mapper
{
    Map (*open)(const int level);

    void (*close)(const Map map);

    Map (*reopen)(const Map map, const int level);

    bool (*isportal)(const Map map, const Point where);
};

extern const struct mapper mapper;
