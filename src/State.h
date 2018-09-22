#pragma once

typedef enum
{
    // Sprite is persuing hero.
    CHASING,

    // Sprite is lifted into the air
    LIFTED,

    // Sprite is doing nothing.
    IDLE,

    // Sprite is hurt with direction.
    HURTN, // North.
    HURTE, // East.
    HURTS, // South.
    HURTW, // West.

    // Sprite is dead with direction.
    DEADN, // North.
    DEADE, // East.
    DEADS, // South.
    DEADW, // West.

    // Sprite is attacking hero.
    ATTACK,

    // Number of sprite states.
    STATES
}
State;

int xishurt(const State);

int xisdead(const State);

int xisalive(const State);
