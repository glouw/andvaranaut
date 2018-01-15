#include "State.h"

int xishurt(const State state)
{
    return state == HURTN
        || state == HURTE
        || state == HURTS
        || state == HURTW;
}

int xisdead(const State state)
{
    return state == DEADN
        || state == DEADE
        || state == DEADS
        || state == DEADW;
}
