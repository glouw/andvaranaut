#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct point
{
    double x, y;
}
Point;

Point
Point_Cast(const Point where, const double radians, uint8_t** const party);

Point
Point_Sub(const Point i, const Point j);

Point
Point_Add(const Point i, const Point j);

Point
Point_Mul(const Point i, const double n);

uint8_t
Point_TileEnclosure(const Point point, uint8_t** party);

uint8_t
Point_TileParty(const Point point, uint8_t** party);

bool
Point_Collision(const Point point, uint8_t** party);

double
Point_Magnitude(const Point point);

double
Point_Percent(const Point point, uint8_t** party);

double
Point_Decimal(const double d);
