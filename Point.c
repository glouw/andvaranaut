#include "Point.h"

#include "util.h"

#include <stdio.h>
#include <math.h>
#include <float.h>

Point xtrn(const Point a, const float t)
{
    Point out;
    out.x = a.x * cosf(t) - a.y * sinf(t);
    out.y = a.x * sinf(t) + a.y * cosf(t);
    return out;
}

Point xrag(const Point a)
{
    Point out;
    out.x = -a.y;
    out.y = +a.x;
    return out;
}

Point xsub(const Point a, const Point b)
{
    Point out;
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    return out;
}

Point xadd(const Point a, const Point b)
{
    Point out;
    out.x = a.x + b.x;
    out.y = a.y + b.y;
    return out;
}

Point xmul(const Point a, const float n)
{
    Point out;
    out.x = a.x * n;
    out.y = a.y * n;
    return out;
}

float xmag(const Point a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}

Point xdiv(const Point a, const float n)
{
    Point out;
    out.x = a.x / n;
    out.y = a.y / n;
    return out;
}

Point xunt(const Point a)
{
    return xdiv(a, xmag(a));
}

float xslp(const Point a)
{
    return a.y / a.x;
}

Point xshr(const Point a, const Point b)
{
    Point out;
    out.x = b.x > 0.0 ? xfl(a.x + 1.0) : xcl(a.x - 1.0);
    out.y = xslp(b) * (out.x - a.x) + a.y;
    return out;
}

Point xsvr(const Point a, const Point b)
{
    Point out;
    out.y = b.y > 0.0 ? xfl(a.y + 1.0) : xcl(a.y - 1.0);
    out.x = (out.y - a.y) / xslp(b) + a.x;
    return out;
}

Point xmid(const Point a)
{
    Point out;
    out.x = xfl(a.x) + 0.5;
    out.y = xfl(a.y) + 0.5;
    return out;
}

int xinf(const Point a)
{
    return isinf(a.x) || isinf(a.y);
}

Point xcmp(const Point a, const Point b, const Point c)
{
    return xinf(c) ? b : xinf(b) ? c : xmag(xsub(b, a)) < xmag(xsub(c, a)) ? b : c;
}

int xeql(const Point a, const Point b, const float e)
{
    return a.x < b.x + (e / 2.0)
        && a.x > b.x - (e / 2.0)
        && a.y < b.y + (e / 2.0)
        && a.y > b.y - (e / 2.0);
}

int xblok(const Point a, char** const blocks)
{
    const int y = a.y;
    const int x = a.x;
    return blocks[y][x];
}

int xtile(const Point a, char** const blocks)
{
    return xblok(a, blocks) - ' ';
}

Point xabs(const Point a)
{
    Point b;
    b.x = fabs(a.x);
    b.y = fabs(a.y);
    return b;
}
