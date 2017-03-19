#include "Point.h"

#include "misc.h"

#include <math.h>

Point turn(const Point a, const double t)
{
    return (Point) { a.x * cos(t) - a.y * sin(t), a.x * sin(t) + a.y * cos(t) };
}

Point rag(const Point a)
{
    return (Point) { -a.y, a.x };
}

Point sub(const Point a, const Point b)
{
    return (Point) { a.x - b.x, a.y - b.y };
}

Point add(const Point a, const Point b)
{
    return (Point) { a.x + b.x, a.y + b.y };
}

Point mul(const Point a, const double n)
{
    return (Point) { a.x * n, a.y * n };
}

double mag(const Point a)
{
    return sqrt(a.x * a.x + a.y * a.y);
}

Point dvd(const Point a, const double n)
{
    return (Point) { a.x / n, a.y / n };
}

Point unt(const Point a)
{
    return dvd(a, mag(a));
}

double slope(const Point a)
{
    return a.y / a.x;
}

Point sh(const Point a, const Point b)
{
    const double x = b.x > 0.0 ? fl(a.x + 1.0) : cl(a.x - 1.0);
    const double y = slope(b) * (x - a.x) + a.y;
    return (Point) { x, y };
}

Point sv(const Point a, const Point b)
{
    const double y = b.y > 0.0 ? fl(a.y + 1.0) : cl(a.y - 1.0);
    const double x = (y - a.y) / slope(b) + a.x;
    return (Point) { x, y };
}

Point cmp(const Point a, const Point b, const Point c)
{
    return mag(sub(b, a)) < mag(sub(c, a)) ? b : c;
}

int tile(const Point a, char** const tiles)
{
    const int x = a.x;
    const int y = a.y;
    return tiles[y][x] - ' ';
}
