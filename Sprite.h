#pragma once

#include "Point.h"
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
    int transparent;
    int ticks;
    float health;
}
Sprite;

int xsissprite(const int ascii);

int xsiscosmetic(const Sprite* sprite);

Sprite xsregistrar(const int ascii, const Point where);

void xsplace(Sprite* const sprite, const Point to);
