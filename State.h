#pragma once

/*
 * The hurt directions are placed first
 * to allow for easy typecasting from Compass to State
 */

typedef enum
{
    // Hurt Eastwards
    HE,
    // Hurt Southwards
    HS,
    // Hurt Westwards
    HW,
    // Hurt Northwards
    HN,
    // Standing around doing nothing
    IDLE,
    // Grabbed by the player
    GRABBED,
    // Near death but not dead
    MERCY,
    // Number of states present
    STATES
}
State;
