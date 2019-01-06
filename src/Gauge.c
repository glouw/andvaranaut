#include "Gauge.h"

#include "util.h"

Gauge g_new(const int max)
{
    static Gauge zero;
    Gauge g = zero;
    g.max = max;
    g.points = u_toss(Point, g.max);
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
    g.ready = false;
    g.warning = false;
    g.alternate = false;

    return g;
}

static Gauge fizzle(Gauge g, const Timer tm)
{
    g = reset(g);
    g.ticks = tm.ticks + 30;
    g.warning = true;
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
        if(input.l
        || input.r)
        {
            g.alternate = input.r;

            g.points[g.count].x = (g.mx += input.dx);
            g.points[g.count].y = (g.my += input.dy);
            g.count++;

            return g.count == g.max ? fizzle(g, tm) : g;
        }
        else
        if(input.lu
        || input.ru)
        {
            g.ready = true;
            return g;
        }
        else
            return reset(g);
    }
}

Point g_sum(const Gauge g, const int count)
{
    static Point zero;
    Point sum = zero;
    for(int i = 0; i < count; i++)
        sum = p_add(sum, g.points[i]);
    return sum;
}

Point g_velocity(const Gauge g)
{
    if(g.count > 4)
    {
        const Point a = p_sub(g.points[g.count - 1], g.points[g.count - 2]);
        const Point b = p_sub(g.points[g.count - 3], g.points[g.count - 4]);
        return p_add(a, b);
    }
    static Point zero;
    return zero;
}

int g_blocking(const Gauge g)
{
    return g.alternate;
}

int g_slowmo(const Gauge g)
{
    return g.count > 0
        && g_blocking(g);
}

int g_successful_block(const Gauge g)
{
    return g.ready
        && g_blocking(g);
}
