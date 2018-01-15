#include "Gauge.h"

#include "util.h"

Gauge xgnew(const float sfactor)
{
    Gauge g;
    xzero(g);
    // Maybe have max change with weapon? Stronger weapons have longer windup.
    g.max = 120; /* About two seconds max. */
    g.points = xtoss(Point, g.max);
    g.sfactor = sfactor;
    return g;
}

void xgfree(const Gauge g)
{
    free(g.points);
}

static Gauge reset(Gauge g)
{
    g.mx = g.my = g.count = 0;
    return g;
}

Gauge xgwind(Gauge g, const Weapon weapon, const Input input)
{
    if(weapon == HANDS)
        return g;
    if(input.l)
    {
        // Fizzle out.
        if(g.count == g.max)
            g = reset(g);
        // Trace.
        g.points[g.count].x = (g.mx += input.dx);
        g.points[g.count].y = (g.my += input.dy);
        g.count++;
    }
    // Done winding.
    else g = reset(g);
    return g;
}

Attack xgpower(const Gauge g, const Input input)
{
    Attack none;
    xzero(none);
    if(g.count < 2)
        return none;
    if(input.lu)
    {
        float mag = 0.0;
        for(int i = 0; i < g.count - 1; i++)
        {
            const Point diff = xsub(
                g.points[i + 1],
                g.points[i + 0]);
            mag += xmag(diff);
        }
        const Point dir = xsub(
            g.points[g.count - 1],
            g.points[g.count - 2]);
        const Attack attack =  { mag, xunt(dir) };
        return xmag(dir) > 0.0 ? attack : none;
    }
    return none;
}
