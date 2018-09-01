#include "Points.h"

#include "util.h"

Points xzpoints()
{
    static Points points;
    return points;
}

Points xpsnew(const int max)
{
    const Points ps = { xtoss(Point, max), 0, max };
    return ps;
}

Points xpsadd(Points ps, const Point p)
{
    if(ps.count == ps.max)
        xbomb("points size limitation reached\n");
    ps.point[ps.count++] = p;
    return ps;
}

Points xpsadduq(Points a, const Points b)
{
    const Point point = b.point[rand() % b.count];
    return xpsfind(a, point) ? xpsadduq(a, b) : xpsadd(a, point);
}

Points xpscat(Points ps, const Points other)
{
    for(int i = 0; i < other.count; i++)
        ps = xpsadd(ps, other.point[i]);
    return ps;
}

int xpsfind(const Points ps, const Point p)
{
    for(int i = 0; i < ps.count; i++)
        if(xeql(ps.point[i], p, 1.0))
            return true;
    return false;
}
