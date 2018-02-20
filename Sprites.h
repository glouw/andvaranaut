#pragma once

#include "Hero.h"
#include "State.h"
#include "Attack.h"
#include "Sprite.h"
#include "Field.h"

typedef struct
{
    Sprite* sprite;
    int count;
    int max;
}
Sprites;

// Loads all the sprites on a floor.
Sprites xsnew(const int max);

// Frees all loaded sprites from memory.
void xkill(const Sprites);

// Arranges sprites furthest to nearest relative to
// the hero's orientation (angle and position). Useful for the sprite renderer.
Sprites xorient(const Sprites, const Hero);

// Edits in new sprites with the sprite editor.
Sprites xlay(Sprites, const Map, const Overview);

// Self sustain for all sprites.
Sprites xcaretake(Sprites, const Hero, const Input, const Map, const Attack, const Field, const int ticks);
