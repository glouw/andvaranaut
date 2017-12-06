#pragma once

#include "Hero.h"
#include "State.h"

typedef struct
{
    Point where;
    int ascii;
    State state;
    float width;
    Point last;
    Point velocity;
    float speed;
    float acceleration;
    float scent;
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

Sprites xwake(const int floor);

void xkill(const Sprites);

Sprites xrewake(const Sprites, const int floor);

Sprites xorient(const Sprites, const Hero);

int xissprite(const int ascii);

Sprites xlay(Sprites, const Map, const Overview);

void xcaretake(Sprites, const Hero, const Input, const Map);

void xssave(const Sprites sprites, const int floor, const int ticks);
