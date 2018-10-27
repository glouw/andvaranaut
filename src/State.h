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

    STATES
}
State;

int xishurt(const State);

int xisdead(const State);

int xisalive(const State);
