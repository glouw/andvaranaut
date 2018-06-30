#pragma once

typedef struct
{
    float x;
    float y;
}
Point;

Point xzpoint();

Point xtrn(const Point, const float t);

// Returns a point rotated about the origin by right angle degrees.
Point xrag(const Point);

Point xsub(const Point a, const Point b);

Point xadd(const Point a, const Point b);

Point xmul(const Point a, const float n);

float xmag(const Point);

Point xdiv(const Point a, const float n);

// Returns the unit vector of a point relative to (0, 0).
Point xunt(const Point);

float xslp(const Point);

// Shifts a point horizontally on a grid along a vector of point b and (0, 0).
Point xshr(const Point a, const Point b);

// Shifts a point vertically on a grid along a vector of point b and (0, 0).
Point xsvr(const Point a, const Point b);

Point xmid(const Point);

int xinf(const Point);

// Returns b if b is closer to a, else c.
Point xcmp(const Point a, const Point b, const Point c);

// Returns true if a is within b with some epsilon width.
int xeql(const Point a, const Point b, const float e);

// Returns true if a is equal to b without epsilon width.
int xpsame(const Point a, const Point b);

// Returns the block at point a.
int xblok(const Point, char** const blocks);

// Returns the tile number at point a.
int xtile(const Point, char** const blocks);

Point xabs(const Point);

Point xsnap(const Point, const int grid);

Point xrand(Point, const int grid);
