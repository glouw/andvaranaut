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
    g.max = 600;
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

Gauge xgwind(Gauge g, const Input input)
{
    if(input.l)
    {
        if(g.count == g.max)
            g = reset(g);

        g.points[g.count].x = (g.mx += input.dx);
        g.points[g.count].y = (g.my += input.dy);
        g.count++;
    }
    else g = reset(g);

    return g;
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
