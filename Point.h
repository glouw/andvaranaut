#pragma once

typedef struct
{
    float x;
    float y;
}
Point;

// Returns a point rotated about the origin by t radians.
Point xtrn(const Point, const float t);

// Returns a point rotated about the origin by right angle degrees.
Point xrag(const Point);

// Returns the subtraction of two points (a - b).
Point xsub(const Point a, const Point b);

// Returns the addition of two points (a + b).
Point xadd(const Point a, const Point b);

// Returns the multiplication of a point by a scalar (a * n).
Point xmul(const Point a, const float n);

// Returns the magnitude of a point.
float xmag(const Point);

// Returns division by scalar.
Point xdiv(const Point a, const float n);

// Returns the unit vector of a point relative to (0, 0).
Point xunt(const Point);

// Returns the slope of a point relative to (0, 0).
float xslp(const Point);

// Shifts a point horizontally on a grid along a vector of point b and (0, 0).
Point xshr(const Point a, const Point b);

// Shifts a point vertically on a grid along a vector of point b and (0, 0).
Point xsvr(const Point a, const Point b);

// Returns a point snapped to the middle of a grid.
Point xmid(const Point);

// Returns true if a point is infinitly far away from the (0, 0).
int xinf(const Point);

// Returns b if b is closer to a, else c.
Point xcmp(const Point a, const Point b, const Point c);

// Returns true if a is within b with some epsilon width.
int xeql(const Point a, const Point b, const float e);

// Returns the block at point a.
int xblok(const Point, char** const blocks);

// Returns the tile number at point a.
int xtile(const Point, char** const blocks);

// Returns an absolute value point of a point.
Point xabs(const Point);

Point xsnap(const Point, const int grid);

// Verbose pixel equality without epsilon.
int xpsame(const Point a, const Point b);
