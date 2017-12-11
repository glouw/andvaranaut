#include "Gauge.h"

#include "util.h"

Gauge xgnew(const float sensitivity)
{
    Gauge g;
    xzero(g);
    g.max = 60;
    g.points = xtoss(Point, g.max); /* About one second. */
    g.sensitivity = sensitivity;
    return g;
}

void xgfree(const Gauge g)
{
    free(g.points);
}

Point xgadd(const Gauge g)
{
    Point sum = { 0.0, 0.0 };
    for(int i = 0; i < g.max; i++)
    {
        const Point p = g.points[i];
        sum = xadd(sum, p);
    }
    return sum;
}

static Gauge reset(Gauge g)
{
    g.mx = g.my = g.top = 0;
    return g;
}

Gauge xgwind(Gauge g, const Input input)
{
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
