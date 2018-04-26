#pragma once

#include "Point.h"
#include "State.h"

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
}
Sprite;

Sprite xzsprite();

int xishurt(const State);

int xisdead(const State);

int xsissprite(const int ascii);

int xsiscosmetic(const int ascii);

int xisuseless(const Sprite* const);

int xisstuck(const Sprite* const);

Sprite xsregistrar(const int ascii, const Point where);

void xsplace(Sprite* const, const Point);
