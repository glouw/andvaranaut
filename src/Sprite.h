#pragma once

#include "Point.h"
#include "State.h"
#include "Timer.h"
#include "Speech.h"

#include <SDL2/SDL.h>

typedef struct
{
    int ascii;
    int evil;
    int transparent;
    int cooltick;
    int cooldown;
    int durticks;
    int hold;
    float size;
    float speed;
    float acceleration;
    float health;
    float damage;
    float defense;
    Point where;
    Point last;
    Point velocity;
    State state;
    SDL_Rect seen;
    Speech speech;
    Speech angers;
    int ticks;
}
Sprite;

Sprite xzsprite(void);

int xsissprite(const int ascii);

int xiscosmetic(const int ascii);

int xisinanimate(const int ascii);

int xisuseless(const Sprite* const);

int xnocount(const Sprite* const);

int xisstuck(const Sprite* const);

void xstest(void);

Sprite xsregistrar(const int ascii, const Point where);

void xsplace(Sprite* const, const Point);

int xismute(const Sprite* const);

void xstick(Sprite* const, const Timer);

int xisattack(const Sprite* const);

int xiscontact(const Sprite* const, const Timer);
