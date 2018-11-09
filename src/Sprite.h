#pragma once

#include "Point.h"
#include "State.h"
#include "Timer.h"
#include "Speech.h"
#include "Compass.h"

#include <SDL2/SDL.h>

typedef struct
{
    int ascii;
    int evil;
    int transparent;

    // Stats.
    float size;
    float speed;
    float acceleration;
    float health;
    float damage;
    float defense;

    // Physics.
    Point where;
    Point last;
    Point velocity;

    State state;

    SDL_Rect seen;
    Speech speech;

    int stunticks;
}
Sprite;

int s_sprite(const int ascii);

int s_cosmetic(const int ascii);

int s_firey(const int ascii);

int s_inanimate(const int ascii);

int s_useless(const Sprite* const);

int s_no_count(const Sprite* const);

int s_stuck(const Sprite* const);

void s_test(void);

Sprite s_register(const int ascii, const Point where);

void s_place(Sprite* const, const Point);

int s_muted(const Sprite* const);

int s_attacking(const Sprite* const);

int s_stunned(const Sprite* const, const Timer);

void s_stun(Sprite* const, const Timer);

int s_impulse(Sprite* const, const Timer);
