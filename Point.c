#include "Point.h"

#include "util.h"

#include <math.h>

Point trn(const Point a, const float t)
{
    return (Point) { a.x * cosf(t) - a.y * sinf(t), a.x * sinf(t) + a.y * cosf(t) };
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

Point mul(const Point a, const float n)
{
    return (Point) { a.x * n, a.y * n };
}

float mag(const Point a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}

Point dvd(const Point a, const float n)
{
    return (Point) { a.x / n, a.y / n };
}

Point unt(const Point a)
{
    return dvd(a, mag(a));
}

float slp(const Point a)
{
    return a.y / a.x;
}

Point shr(const Point a, const Point b)
{
    const float x = b.x > 0.0 ? fl(a.x + 1.0) : cl(a.x - 1.0);
    const float y = slp(b) * (x - a.x) + a.y;
    return (Point) { x, y };
}

Point svr(const Point a, const Point b)
{
    const float y = b.y > 0.0 ? fl(a.y + 1.0) : cl(a.y - 1.0);
    const float x = (y - a.y) / slp(b) + a.x;
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
