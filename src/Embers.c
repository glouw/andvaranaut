#include "Embers.h"

#include "util.h"

Embers e_append(Embers e, Sprite* const ember)
{
    if(e.max == 0)
        u_retoss(e.ember, Sprite*, e.max = 1);
    if(e.count >= e.max)
        u_retoss(e.ember, Sprite*, e.max *= 2);
    e.ember[e.count++] = ember;
    return e;
}

Embers e_reset(const Embers e)
{
    free(e.ember);
    static Embers zero;
    return zero;
}
