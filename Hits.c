#include "Hits.h"

#include "Compass.h"
#include "util.h"

static Hit* push(Hit* ceiling, const Hit hit)
{
    Hit* temp = xtoss(Hit, 1);
    *temp = hit;
    temp->next = ceiling;
    return temp;
}

static Hit collision(const Point ray, const Point final, const Point other, char** const block)
{
    const float offset = xdec(ray.x + ray.y);
    const bool inverted = xinverted(xneedle(final, other));
    const Hit hit = { xtile(final, block), inverted ? 1.0 - offset : offset, ray, NULL };
    return hit;
}

static Hits step(Hits hits, const Point where, const Point direction, const Map map)
{
    const Point ray = xcmp(where, xshr(where, direction), xsvr(where, direction));
    const Point delta = xmul(direction, 1e-3);
    const Point final = xadd(ray, delta);
    const Point other = xsub(ray, delta);
    // Ceiling hit linked list.
    if(xtile(final, map.ceiling) && !xtile(other, map.ceiling))
    {
        const Hit hit = collision(ray, final, other, map.ceiling);
        hits.ceiling = push(hits.ceiling, hit);
    }
    // Walling hit.
    if(xtile(final, map.walling))
    {
        const Hit hit = collision(ray, final, other, map.walling);
        hits.walling = hit;
        return hits;
    }
    return step(hits, ray, direction, map);
}

Hits xmarch(const Point where, const Point direction, const Map map)
{
    Hits hits;
    xzero(hits);
    return step(hits, where, direction, map);
}
