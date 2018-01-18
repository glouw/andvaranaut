#include "Gauge.h"

#include "util.h"

Gauge xgnew()
{
    Gauge g;
    xzero(g);
    // Maybe have max change with weapon? Stronger weapons have longer windup.
    g.max = 120; /* About two seconds max. */
    g.points = xtoss(Point, g.max);
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
    // Cannot wind with hands.
    if(weapon == HANDS) return g;
    // Winds with left mouse button down.
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
