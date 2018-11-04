#include "Embers.h"

#include "util.h"

Embers e_append(Embers embers, Sprite* const ember)
{
    if(embers.max == 0)
        u_retoss(embers.ember, Sprite*, embers.max = 1);
    if(embers.count >= embers.max)
        u_retoss(embers.ember, Sprite*, embers.max *= 2);
    embers.ember[embers.count++] = ember;
    return embers;
}

Embers e_reset(const Embers embers)
{
    free(embers.ember);
    static Embers zero;
    return zero;
}
