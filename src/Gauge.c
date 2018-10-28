#include "Gauge.h"

#include "util.h"

Gauge xzgauge(void)
{
    static Gauge gauge;
    return gauge;
}

Gauge xgnew(void)
{
    Gauge g = xzgauge();
    g.max = 100;
    g.points = xtoss(Point, g.max);

    return g;
}

void xgfree(const Gauge g)
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
    const int timeout = 30;
    g = reset(g);
    g.ticks = tm.ticks + timeout;
    return g;
}

Gauge xgwind(Gauge g, const Input input, const Timer tm)
{
    if(tm.ticks > g.ticks)
    {
        if(input.l)
        {
            g.points[g.count].x = (g.mx += input.dx);
            g.points[g.count].y = (g.my += input.dy);
            g.count++;

            return g.count == g.max ? fizzle(g, tm) : g;
        }
        else return reset(g);
    }
    else return g;
}

float xgmag(const Gauge g, const float damage)
{
    float mag = 0.0f;
    for(int i = 0; i < g.count - 1; i++)
        mag += xmag(xsub(g.points[i + 1], g.points[i + 0]));
    mag += damage;

    return mag;
}

Point xgsum(const Gauge g, const int count)
{
    Point sum = xzpoint();
    for(int i = 0; i < count; i++)
        sum = xadd(sum, g.points[i]);

    return sum;
}
