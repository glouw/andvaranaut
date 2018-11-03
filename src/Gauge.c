#include "Gauge.h"

#include "Title.h"

#include "util.h"

Gauge g_new(void)
{
    static Gauge zero;
    Gauge g = zero;
    g.max = 100;
    g.points = xtoss(Point, g.max);
    g.divisor = 2;
    return g;
}

void g_free(const Gauge g)
{
    free(g.points);
}

static Gauge reset(Gauge g)
{
    g.mx = 0;
    g.my = 0;
    g.count = 0;
    return g;
}

static Gauge fizzle(Gauge g, const Timer tm)
{
    g = reset(g);

    const int timeout = 30;
    g.ticks = tm.ticks + timeout;

    const char* const tireds[] = {
        "Exausted...",
        "So tired...",
        "Muscles aching...",
    };
    const int which = rand() % xlen(tireds);
    const char* const tired = tireds[which];
    xttset(tm.renders, tm.renders + 120, false, tired);

    return g;
}

int g_fizzled(const Gauge g, const Timer tm)
{
    return tm.ticks < g.ticks;
}

Gauge g_wind(Gauge g, const Input input, const Timer tm)
{
    if(g_fizzled(g, tm))
        return g;
    else
    {
        if(input.r)
            return reset(g);
        else
        if(input.l)
        {
            g.points[g.count].x = (g.mx += input.dx);
            g.points[g.count].y = (g.my += input.dy);
            g.count++;
            return g.count == g.max ? fizzle(g, tm) : g;
        }
        else return reset(g);
    }
}

Point g_sum(const Gauge g, const int count)
{
    Point sum = p_zero();
    for(int i = 0; i < count; i++)
        sum = p_add(sum, g.points[i]);
    return sum;
}
