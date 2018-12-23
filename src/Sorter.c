#include "Sorter.h"

#include "Tris.h"
#include "Sprite.h"

int s_nearest_sprite_first(const void* const a, const void* const b)
{
    const Sprite* const sa = (const Sprite*) a;
    const Sprite* const sb = (const Sprite*) b;
    return
        p_mag(sa->where) < p_mag(sb->where) ? -1 :
        p_mag(sa->where) > p_mag(sb->where) ? +1 : 0;
}

int s_furthest_sprite_first(const void* const a, const void* const b)
{
    return -1 * s_nearest_sprite_first(a, b);
}

int s_largest_int_first(const void* const a, const void* const b)
{
    const int ia = *((const int*) a);
    const int ib = *((const int*) b);
    return ia < ib;
}

static float len(const Tri edge)
{
    return p_mag(p_sub(edge.b, edge.a));
}

int s_descend_tris(const void* const a, const void* const b)
{
    const Tri ea = *(const Tri*) a;
    const Tri eb = *(const Tri*) b;
    return len(ea) < len(eb) ? 1 : len(ea) > len(eb) ? -1 : 0;
}
