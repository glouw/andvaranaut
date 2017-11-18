#pragma once

#include "Hero.h"
#include "State.h"

typedef struct
{
    Point where;
    int ascii;
    State state;
    int transparent;
    float width;
    Point last;
    Point velocity;
    float speed;
    float acceleration;
}
Sprite;

typedef struct
{
    Sprite* sprite;
    int count;
    int max;
}
Sprites;

Sprites xwake(const int level);

void xkill(const Sprites sprites);

Sprites xrewake(const Sprites sprites, const int level);

Sprites xorient(const Sprites sprites, const Hero hero);

bool xissprite(const int ascii);

void xcaretake(const Sprites sprites, const Hero hero, const Input input, const Map map);
