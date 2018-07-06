#include "Hits.h"

#include "Compass.h"
#include "Line.h"
#include "util.h"

Hits xzhits()
{
    static Hits hits;
    return hits;
}

static Hit* push(Hit* ceiling, const Hit hit)
{
    Hit* temp = xtoss(Hit, 1);
    *temp = hit;
    temp->next = ceiling;
    return temp;
}

static Hit collision(const Point ray, const Line test, char** const block)
{
    const float offset = xdec(ray.x + ray.y);
    const int inverted = xinverted(xneedle(test.a, test.b));
    const Hit hit = { xtile(test.a, block), inverted ? 1.0f - offset : offset, ray, NULL };
    return hit;
}

static Hits step(Hits hits, const Point where, const Point direction, const Map map)
{
    const Point hor = xshr(where, direction);
    const Point ver = xsvr(where, direction);
    const Point ray = xcmp(where, hor, ver);
    const Point delta = xmul(direction, 0.01f);
    const Point dx = { delta.x, 0.0f };
    const Point dy = { 0.0f, delta.y };

    const Line test = {
        xadd(ray, xmag(xsub(hor, ver)) < 0.001f ? delta : xdec(ray.x) == 0.0f ? dx : dy),
        xsub(ray, xmag(xsub(hor, ver)) < 0.001f ? delta : xdec(ray.x) == 0.0f ? dx : dy),
    };

    // Flooring wall was hit. Push the flooring hit linked list.
    if(xtile(test.a, map.floring) && !xtile(test.b, map.floring))
        hits.floring = push(hits.floring, collision(ray, test, map.floring));

    // Ceiling wall was hit. Push the ceiling hit linked list.
    if(xtile(test.a, map.ceiling) && !xtile(test.b, map.ceiling))
        hits.ceiling = push(hits.ceiling, collision(ray, test, map.ceiling));

    // Eye walling hit.
    // A linked list is not needed: Thanks to eye level projections,
    // only one projection is needed as every projection behind will
    // be overlapped by the first projection.
    if(xtile(test.a, map.walling) && !hits.walling.surface)
        hits.walling = collision(ray, test, map.walling);

    // Done when a wall was hit and a ceiling wall exists above the wall and
    // a floor wall exist below the wall.
    if(hits.walling.surface && xtile(test.a, map.ceiling) && xtile(test.a, map.floring))
        return hits;

    // Otherwise, keep on stepping.
    return step(hits, ray, direction, map);
}

Hits xmarch(const Point where, const Point direction, const Map map)
{
    return step(xzhits(), where, direction, map);
}
