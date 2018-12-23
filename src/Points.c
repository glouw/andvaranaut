#include "Points.h"

#include "util.h"

Points p_new(const int max)
{
    const Points ps = { u_toss(Point, max), 0, max };
    return ps;
}

Points p_append(Points ps, const Point p)
{
    if(ps.count == ps.max)
        u_bomb("points size limitation reached: count %d: max %d\n", ps.count, ps.max);
    ps.point[ps.count++] = p;
    return ps;
}

Points p_add_unique(Points a, const Points b)
{
    const int index = rand() % b.count;
    const Point point = b.point[index];
    return p_find(a, point) ? p_add_unique(a, b) : p_append(a, point);
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
