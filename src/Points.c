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

Points xpsrandpop(const Points ps, const int max)
{
    Points out = xpsnew(max);
    for(int i = 0; i < max; i++)
    {
        const int index = rand() % ps.count;
        const Point point = ps.point[index];
        const Point mid = xmid(point);
        out = xpsadd(out, mid);
    }
    return out;
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
        if(xpsame(ps.point[i], p))
            return true;
    return false;
}
