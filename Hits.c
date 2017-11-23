#include "Hits.h"

#include "Compass.h"
#include "util.h"

static Hit collision(const Point ray, const Point direction, char** const block)
{
    const float epsilon = 1e-3;
    const Point where = xadd(ray, xmul(direction, epsilon));
    const Point other = xsub(ray, xmul(direction, epsilon));
    const float offset = xdec(ray.x + ray.y);
    const Compass face = xneedle(where, other);
    Hit hit;
    hit.surface = xtile(where, block);
    hit.changed = xtile(where, block) && xtile(other, block) == 0;
    hit.offset = xinverted(face) ? 1.0 - offset : offset;
    hit.where = ray;
    hit.facing = face;
    return hit;
}

static Hit* push(Hit* ceiling, const Hit hit)
{
    Hit* temp = xtoss(Hit, 1);
    *temp = hit;
    temp->next = ceiling;
    return temp;
}

static Hits step(Hits hits, const Point where, const Point direction, const Map map)
{
    const Point ray = xcmp(where, xshr(where, direction), xsvr(where, direction));
    // Walling.
    hits.walling = collision(ray, direction, map.walling);
    // Ceiling trail appending.
    const Hit ceiling = collision(ray, direction, map.ceiling);
    // Linked list.
    if(ceiling.changed) hits.ceiling = push(hits.ceiling, ceiling);
    return hits.walling.surface ? hits : step(hits, ray, direction, map);
}

Hits xmarch(const Point where, const Point direction, const Map map)
{
    Hits hits;
    xzero(hits);
    return step(hits, where, direction, map);
}
