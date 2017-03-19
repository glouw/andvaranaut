#pragma once

#include "Point.h"

typedef struct
{
    Point where;
    char* blocks;
}
Portal;

typedef struct
{
    Portal* portal;
    int count;
}
Portals;

Portals populate(const char* const path);
void destroy(const Portals portals);
