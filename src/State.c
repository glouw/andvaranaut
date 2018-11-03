#include "State.h"

int s_hurting(const State state)
{
    return state == HURT_N
        || state == HURT_E
        || state == HURT_S
        || state == HURT_W;
}

int s_dead(const State state)
{
    return state == DEAD_N
        || state == DEAD_E
        || state == DEAD_S
        || state == DEAD_W;
}

int s_alive(const State state)
{
    return !s_dead(state);
}
