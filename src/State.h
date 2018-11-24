#pragma once

// Always append states as art work relies on offsets.
typedef enum
{
    CHASING,
    LIFTED,
    IDLE,

    HURT_N,
    HURT_E,
    HURT_S,
    HURT_W,

    DEAD_N,
    DEAD_E,
    DEAD_S,
    DEAD_W,

    ATTACK_N,
    ATTACK_E,
    ATTACK_S,
    ATTACK_W,

    BLOCK_N,
    BLOCK_E,
    BLOCK_S,
    BLOCK_W,

    SPEAKING,
    STUNNED,

    STATES
}
State;

int s_hurting(const State);

int s_dead(const State);

int s_alive(const State);

int s_stunned(const State);

int s_attacking(const State);

int s_blocking(const State);
