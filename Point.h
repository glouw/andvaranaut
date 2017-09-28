#pragma once

#include <stdbool.h>

typedef struct
{
    float x;
    float y;
}
Point;

// Returns a point rotated about the origin by t radians
Point trn(const Point a, const float t);

// Returns a point rotated about the origin by right angle degrees
Point rag(const Point a);

// Returns the subtraction of two points (a - b)
Point sub(const Point a, const Point b);

// Returns the addition of two points (a + b)
Point add(const Point a, const Point b);

// Returns the multiplication of a point by a scalar (a * n)
Point mul(const Point a, const float n);

// Returns the magnitude of a point
float mag(const Point a);

// Returns the division of a point by a scalar (a / n)
Point dvd(const Point a, const float n);

// Returns the unit vector of a point relative to (0, 0)
Point unt(const Point a);

// Returns the slope of a point relative to (0, 0)
float slp(const Point a);

// Shifts a point horizontally on a grid along a vector of point b and (0, 0)
Point shr(const Point a, const Point b);

// Shifts a point vertically on a grid along a vector of point b and (0, 0)
Point svr(const Point a, const Point b);

// Returns a point snapped to the middle of a grid
Point mid(const Point a);

// Returns true if a point is infinitly far away from the (0, 0)
bool inf(const Point a);

// Returns b if b is closer to a, else c
Point cmp(const Point a, const Point b, const Point c);

// Returns true if a is within b with some epsilon width
bool eql(const Point a, const Point b, const float e);

// Returns the block at point a
int block(const Point a, char** const blocks);

// Returns the tile number at point a
int tile(const Point a, char** const blocks);
