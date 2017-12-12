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
    const int inverted = xinverted(xneedle(final, other));
    const Hit hit = { xtile(final, block), inverted ? 1.0 - offset : offset, ray, NULL };
    return hit;
}

static Hits step(Hits hits, const Point where, const Point direction, const Map map)
{
    // Advance the ray.
    const Point ray = xcmp(where, xshr(where, direction), xsvr(where, direction));
    const Point final = xadd(ray, xmul(direction, 1e-4));
    const Point other = xsub(ray, xmul(direction, 1e-3));
    // Ray out of bounds check.
    if(xout(map, final))
    {
        // If any eye level walls have not yet been hit, fill in the hit
        // with the last tile of the map.
        if(!hits.walling.surface)
        {
            const Hit hit = collision(ray, final, other, map.walling);
            hits.walling = hit;
        }
        return hits;
    }
    /* Flooring wall was hit. Push the flooring hit linked list. */
    if(xtile(final, map.floring) && !xtile(other, map.floring))
    {
        const Hit hit = collision(ray, final, other, map.floring);
        hits.floring = push(hits.floring, hit);
    }
    /* Ceiling wall was hit. Push the ceiling hit linked list. */
    if(xtile(final, map.ceiling) && !xtile(other, map.ceiling))
    {
        const Hit hit = collision(ray, final, other, map.ceiling);
        hits.ceiling = push(hits.ceiling, hit);
        // As an added optimization, if one eye level wall was already hit,
        // the upper ceilings can progress one hit further and stop.
        // Chances of artifacts are about 1%, but performance improves
        // vastly on large maps that were not so well designed.
        // Unfortunately, this will cause water artifacts when blocks are placed on
        // water floor tiles in certain ways, but thats okay,
        // because who puts floating floating blocks on water?
        if(hits.walling.surface) return hits;
    }
    /* Eye walling hit.
     * A linked list is not needed: Thanks to eye level projections,
     * only one projection is needed as everything else behind will
     * be overlapped by the first. */
    if(xtile(final, map.walling) && !hits.walling.surface)
    {
        const Hit hit = collision(ray, final, other, map.walling);
        hits.walling = hit;
    }
    return step(hits, ray, direction, map);
}

Hits xmarch(const Point where, const Point direction, const Map map)
{
    Hits hits;
    xzero(hits);
    return step(hits, where, direction, map);
}
