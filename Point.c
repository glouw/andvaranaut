#include "Point.h"

#include "Util.h"

Point zro()
{
    const Point out = { 0.0, 0.0 };
    return out;
}

Point trn(const Point a, const float t)
{
    const Point out = { a.x * cosf(t) - a.y * sinf(t), a.x * sinf(t) + a.y * cosf(t) };
    return out;
}

Point rag(const Point a)
{
    const Point out = { -a.y, a.x };
    return out;
}

Point sub(const Point a, const Point b)
{
    const Point out = { a.x - b.x, a.y - b.y };
    return out;
}

Point add(const Point a, const Point b)
{
    const Point out = { a.x + b.x, a.y + b.y };
    return out;
}

Point mul(const Point a, const float n)
{
    const Point out = { a.x * n, a.y * n };
    return out;
}

float mag(const Point a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}

Point dvd(const Point a, const float n)
{
    const Point out = { a.x / n, a.y / n };
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
    const float x = b.x > 0.0 ? fl(a.x + 1.0) : cl(a.x - 1.0);
    const float y = slp(b) * (x - a.x) + a.y;
    const Point out = { x, y };
    return out;
}

Point svr(const Point a, const Point b)
{
    const float y = b.y > 0.0 ? fl(a.y + 1.0) : cl(a.y - 1.0);
    const float x = (y - a.y) / slp(b) + a.x;
    const Point out = { x, y };
    return out;
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
