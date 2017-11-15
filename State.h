#pragma once

typedef enum
{
    // In hot pursuit of the player.
    CHASING,
    // Grabbed by the player.
    GRABBED,
    // Sprite is doing nothing.
    IDLE,
    // Number of states present.
    STATES
}
State;
