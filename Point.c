#include "Point.h"

#include "Util.h"

Point zro()
{
    Point out;
    out.x = 0.0;
    out.y = 0.0;
    return out;
}

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

Point cmp(const Point a, const Point b, const Point c)
{
    return mag(sub(b, a)) < mag(sub(c, a)) ? b : c;
}

bool eql(const Point a, const Point b, const float epsilon)
{
    return a.x < b.x + (epsilon / 2.0)
        && a.x > b.x - (epsilon / 2.0)
        && a.y < b.y + (epsilon / 2.0)
        && a.y > b.y - (epsilon / 2.0);
}

int block(const Point a, char** const blocks)
{
    return blocks[(int) a.y][(int) a.x];
}

int tile(const Point a, char** const blocks)
{
    return block(a, blocks) - ' ';
}
