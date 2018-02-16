#pragma once

#include "Point.h"
#include "State.h"

typedef struct
{
    Point where;
    // The ASCII value of the sprite will determine which
    // surface to look up for the renderer, as well as the
    // inherihent properties of hte sprite.
    int ascii;
    State state;
    float width;
    // The last position of the sprite is logged when the sprite
    // is moved. This is handy for collision detection.
    Point last;
    Point velocity;
    float speed;
    float acceleration;
    // If true, the sprite is rendered see through like a ghost.
    int transparent;
    //
    int ticks;
    float health;
}
Sprite;

int xsissprite(const int ascii);
