#include "Hits.h"

#include "Compass.h"
#include "Line.h"
#include "util.h"

static Hit* push(Hit* ceiling, const Hit hit)
{
    Hit* temp = u_toss(Hit, 1);
    *temp = hit;
    temp->next = ceiling;
    return temp;
}

static Hit collision(const Point ray, const Line test, char** const block)
{
    const float offset = u_dec(ray.x + ray.y);
    const int inverted = c_is_inverted(c_needle(test.a, test.b));
    const Hit hit = { p_tile(test.a, block), inverted ? 1.0f - offset : offset, ray, NULL };
    return hit;
}

static Hits step(Hits hits, const Point where, const Point direction, const Map map)
{
    const Point hor = p_jump_hor(where, direction);
    const Point ver = p_jump_ver(where, direction);
    const Point ray = p_cmp(where, hor, ver);

    const Point delta = p_mul(direction, 0.01f);

    const Point dx = { delta.x, 0.0f };
    const Point dy = { 0.0f, delta.y };

    const Point fixed = p_mag(p_sub(hor, ver)) < 0.001f ? delta : u_dec(ray.x) == 0.0f ? dx : dy;

    const Line test = { p_add(ray, fixed), p_sub(ray, fixed) };

    //
    // Floor wall.
    //

    if(p_tile(test.a, map.floring) && !p_tile(test.b, map.floring))
        hits.floring = push(hits.floring, collision(ray, test, map.floring));

    //
    // Ceiling wall.
    //

    if(p_tile(test.a, map.ceiling) && !p_tile(test.b, map.ceiling))
        hits.ceiling = push(hits.ceiling, collision(ray, test, map.ceiling));

    //
    // Eye level Wall.
    //

    if(p_tile(test.a, map.walling) && !hits.walling.surface)
        hits.walling = collision(ray, test, map.walling);

    //
    // Done casting?
    //

    if(hits.walling.surface && p_tile(test.a, map.ceiling) && p_tile(test.a, map.floring))
        return hits;

    //
    // Otherwise keep on stepping the ray.
    //

    return step(hits, ray, direction, map);
}

Hits h_march(const Point where, const Point direction, const Map map)
{
    static Hits zero;
    return step(zero, where, direction, map);
}
