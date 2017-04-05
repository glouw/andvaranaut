#pragma once

#include "Point.h"

typedef struct
{
    Point where;
    char* name;
}
Portal;

typedef struct
{
    int count;
    Portal* portal;
}
Portals;

Portals populate(const char* const name);
void destroy(const Portals portals);
