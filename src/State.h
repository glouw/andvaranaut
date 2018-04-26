#pragma once

// Sprites have state. These states are laid out in order
// by row a sprite's sprite sheet.

typedef enum
{
    // In hot pursuit of the player.
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
    // Number of states.
    STATES
}
State;
