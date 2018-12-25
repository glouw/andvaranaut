#include "Sorter.h"

#include "Tris.h"
#include "Sprite.h"

int s_nearest_sprite_first(const void* const a, const void* const b)
{
    const Sprite* const sa = (Sprite*) a;
    const Sprite* const sb = (Sprite*) b;
    return
        p_mag(sa->where) < p_mag(sb->where) ? -1 :
        p_mag(sa->where) > p_mag(sb->where) ? +1 : 0;
}

int s_furthest_sprite_first(const void* const a, const void* const b)
{
    return -1 * s_nearest_sprite_first(a, b);
}

int s_largest_theme_first(const void* const a, const void* const b)
{
    const Theme ta = *((Theme*) a);
    const Theme tb = *((Theme*) b);
    return (int) ta < (int) tb;
}

static float len(const Tri edge)
{
    return p_mag(p_sub(edge.b, edge.a));
}

int s_descend_tris(const void* const a, const void* const b)
{
    const Tri ea = *((Tri*) a);
    const Tri eb = *((Tri*) b);
    return len(ea) < len(eb) ? 1 : len(ea) > len(eb) ? -1 : 0;
}
