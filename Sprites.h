#pragma once

#include "Hero.h"
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
    // Sprites with stronger scents will be avoided better by other 
    // sprites with smaller scents. Likewise, sprites with smaller
    // scents will be attracted to by sprites with larger scents.
    float scent;
    // If true, the sprite is rendered see through like a ghost.
    int transparent;
}
Sprite;

typedef struct
{
    Sprite* sprite;
    int count;
    int max;
}
Sprites;

// Loads all the sprites on a floor.
Sprites xwake(const int floor);

// Frees all loaded sprites from memory.
void xkill(const Sprites);

// Frees then loads a floor's sprites.
Sprites xrewake(const Sprites, const int floor);

// Arranges sprites furthest to nearest relative to
// the hero's orientation (angle and position). Useful for the sprite renderer.
Sprites xorient(const Sprites, const Hero);

// Returns true is the ascii value in question is a sprite.
int xissprite(const int ascii);

// Edits in new sprites with the sprite editor.
Sprites xlay(Sprites, const Map, const Overview);

// Self sustain for all sprites.
void xcaretake(Sprites, const Hero, const Input, const Map);

// Saves sprites to sprite data file.
void xssave(const Sprites sprites, const int floor, const int ticks);
