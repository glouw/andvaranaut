#include "Gauge.h"

#include "util.h"

Gauge xgnew(const float sfactor)
{
    Gauge g;
    xzero(g);
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
    g.mx = g.my = g.top = 0;
    return g;
}

Gauge xgwind(Gauge g, const Weapon weapon, const Input input)
{
    if(weapon == HANDS)
        return g;
    if(input.l)
    {
        // Fizzle out.
        if(g.top == g.max) g = reset(g);
        // Trace.
        g.points[g.top].x = (g.mx += input.dx);
        g.points[g.top].y = (g.my += input.dy);
        g.top++;
    }
    // Done winding.
    else g = reset(g);
    return g;
}
