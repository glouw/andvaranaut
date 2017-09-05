#pragma once

typedef enum
{
    // Hurt Eastwards
    H_E,
    // Hurt Southeastwards
    H_SE,
    // Hurt Southwards
    H_S,
    // Hurt Southwestwards
    H_SW,
    // Hurt Westwards
    H_W,
    // Hurt Northwestwards
    H_NW,
    // Hurt Northwards
    H_N,
    // Hurt Northeastwards
    H_NE,
    // Standing around doing nothing
    IDLE,
    // Grabbed by the player - moves while grabbed
    GRABBED,
    // Took such a hard blow - near death but not dead
    MERCY,
    // In hot pursuit of the player
    CHASING,
    // Number of states present
    STATES
}
State;
