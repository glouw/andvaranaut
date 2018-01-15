#include "State.h"

int xishurt(const State state)
{
    return state == HURTN
        || state == HURTE
        || state == HURTW;
}
