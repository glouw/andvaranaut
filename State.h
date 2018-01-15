#pragma once

// Sprites have state. These states are laid out in order
// by row a sprite's sprite sheet.

typedef enum
{
    // In hot pursuit of the player.
    CHASING,
    // Grabbed by the player.
    GRABBED,
    // Sprite is doing nothing.
    IDLE,
    // Sprite is hurt!
    HURTN, // North
    HURTE, // East
    HURTW, // West // Number of states present.
    STATES
}
State;

int xishurt(const State state);
