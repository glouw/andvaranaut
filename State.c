#include "State.h"

int xishurt(const State state)
{
    return state == HURTN
        || state == HURTE
        || state == HURTS
        || state == HURTW;
}
