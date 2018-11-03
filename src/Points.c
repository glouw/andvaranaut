#include "Points.h"

#include "util.h"

Points p_new(const int max)
{
    const Points ps = { xtoss(Point, max), 0, max };
    return ps;
}

Points p_append(Points ps, const Point p)
{
    if(ps.count == ps.max)
        xbomb("points size limitation reached\n");
    ps.point[ps.count++] = p;
    return ps;
}

Points p_addunique(Points a, const Points b)
{
    const Point point = b.point[rand() % b.count];
    return p_find(a, point) ? p_addunique(a, b) : p_append(a, point);
}

Points p_cat(Points ps, const Points other)
{
    for(int i = 0; i < other.count; i++)
        ps = p_append(ps, other.point[i]);
    return ps;
}

int p_find(const Points ps, const Point p)
{
    for(int i = 0; i < ps.count; i++)
        if(p_eql(ps.point[i], p, 1.0))
            return true;
    return false;
}
