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

int s_stunned(const State state)
{
    return state == STUNNED;
}

int s_attacking(const State state)
{
    return state == ATTACK_N
        || state == ATTACK_E
        || state == ATTACK_S
        || state == ATTACK_W;
}

int s_blocking(const State state)
{
    return state == BLOCK_N
        || state == BLOCK_E
        || state == BLOCK_S
        || state == BLOCK_W;
}
