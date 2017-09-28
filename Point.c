#include "Point.h"

#include "util.h"

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>

Point trn(const Point a, const float t)
{
    Point out;
    out.x = a.x * cosf(t) - a.y * sinf(t);
    out.y = a.x * sinf(t) + a.y * cosf(t);
    return out;
}

Point rag(const Point a)
{
    Point out;
    out.x = -a.y;
    out.y = +a.x;
    return out;
}

Point sub(const Point a, const Point b)
{
    Point out;
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    return out;
}

Point add(const Point a, const Point b)
{
    Point out;
    out.x = a.x + b.x;
    out.y = a.y + b.y;
    return out;
}

Point mul(const Point a, const float n)
{
    Point out;
    out.x = a.x * n;
    out.y = a.y * n;
    return out;
}

float mag(const Point a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}

Point dvd(const Point a, const float n)
{
    Point out;
    out.x = a.x / n;
    out.y = a.y / n;
    return out;
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
    Point out;
    out.x = b.x > 0.0 ? fl(a.x + 1.0) : cl(a.x - 1.0);
    out.y = slp(b) * (out.x - a.x) + a.y;
    return out;
}

Point svr(const Point a, const Point b)
{
    Point out;
    out.y = b.y > 0.0 ? fl(a.y + 1.0) : cl(a.y - 1.0);
    out.x = (out.y - a.y) / slp(b) + a.x;
    return out;
}

Point mid(const Point a)
{
    Point out;
    out.x = fl(a.x) + 0.5;
    out.y = fl(a.y) + 0.5;
    return out;
}

bool inf(const Point a)
{
    return isinf(a.x) || isinf(a.y);
}

Point cmp(const Point a, const Point b, const Point c)
{
    return inf(c) ? b : inf(b) ? c : mag(sub(b, a)) < mag(sub(c, a)) ? b : c;
}

bool eql(const Point a, const Point b, const float e)
{
    return a.x < b.x + (e / 2.0)
        && a.x > b.x - (e / 2.0)
        && a.y < b.y + (e / 2.0)
        && a.y > b.y - (e / 2.0);
}

int block(const Point a, char** const blocks)
{
    const int y = a.y;
    const int x = a.x;
    return blocks[y][x];
}

int tile(const Point a, char** const blocks)
{
    return block(a, blocks) - ' ';
}
