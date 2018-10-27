#include "State.h"

int xishurt(const State state)
{
    return state == HURT_N
        || state == HURT_E
        || state == HURT_S
        || state == HURT_W;
}

int xisdead(const State state)
{
    return state == DEAD_N
        || state == DEAD_E
        || state == DEAD_S
        || state == DEAD_W;
}

int xisalive(const State state)
{
    return !xisdead(state);
}
