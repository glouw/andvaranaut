#include "Sorter.h"

#include "Sprite.h"

int s_nearest_first(const void* const a, const void* const b)
{
    const Sprite* const sa = (const Sprite*) a;
    const Sprite* const sb = (const Sprite*) b;
    return
        p_mag(sa->where) < p_mag(sb->where) ? -1 :
        p_mag(sa->where) > p_mag(sb->where) ? +1 : 0;
}

int s_furthest_first(const void* const a, const void* const b)
{
    return -s_nearest_first(a, b);
}
