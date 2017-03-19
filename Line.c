#include "Line.h"

Line rotate(const Line l, const double t)
{
    return (Line) { turn(l.a, t), turn(l.b, t) };
}

Point lerp(const Line l, const double n)
{
    return add(l.a, mul(sub(l.b, l.a), n));
}

double focal(const Line l)
{
    return l.a.x / (l.b.y - l.a.y);
}

double ccast(const Line fov, const int res, const int xx)
{
    return focal(fov) * res / (2 * xx - (res - 1));
}

double fcast(const Line fov, const int res, const int xx)
{
    return -ccast(fov, res, xx);
}
