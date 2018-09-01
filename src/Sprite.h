#pragma once

#include "Point.h"
#include "State.h"
#include "Speech.h"

#include <SDL2/SDL.h>

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

    int transparent;

    int ticks;

    float health;

    SDL_Rect seen;

    Speech speech;
}
Sprite;

Sprite xzsprite(void);

int xsissprite(const int ascii);

int xiscosmetic(const int ascii);

int xisinanimate(const int ascii);

int xisuseless(const Sprite* const);

int xisstuck(const Sprite* const);

Sprite xsregistrar(const int ascii, const Point where);

void xsplace(Sprite* const, const Point);

int xismute(Sprite* const sprite);
