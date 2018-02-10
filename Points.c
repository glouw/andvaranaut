#include "Points.h"

#include "util.h"

Points xpsnew(const int max)
{
    const Points ps = { xtoss(Point, max), 0, max };
    return ps;
}

Points xpsadd(Points ps, const Point p, const char* why)
{
    if(ps.count == ps.max)
    {
        fprintf(stderr, "points size limitation reached: %s\n", why);
        exit(1);
    }
    ps.point[ps.count++] = p;
    return ps;
}

Points xpspop(const Points ps, const int max)
{
    Points out = xpsnew(max);
    for(int i = 0; i < max; i++)
    {
        const Point point = ps.point[rand() % ps.count];
        const Point mid = xmid(point);
        out = xpsadd(out, mid, "adding mid in xpspop");
    }
    return out;
}

Point xpsrand(const Points ps)
{
    return ps.point[rand() % ps.count];
}
