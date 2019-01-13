#pragma once

#include "Point.h"
#include "State.h"
#include "Timer.h"
#include "Identification.h"
#include "Speech.h"

#include <SDL2/SDL.h>

typedef struct
{
    // Trait.
    int ascii;
    int evil;
    int transparent;
    int block_time;
    int stun_ticks;

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

    int busy_ticks;
    int block_start;

    Speech speech;

    Identification wants;
    Identification gives;

    char* quest_start;

    char* quest_completed;

    char* quest_failed;
}
Sprite;

int s_sprite(const int ascii);

int s_cosmetic(const int ascii);

int s_firey(const int ascii);

int s_inanimate(const int ascii);

int s_useless(const Sprite* const);

int s_not_agent(const Sprite* const);

int s_stuck(const Sprite* const);

void s_test(void);

Sprite s_register(const int ascii, const Point where, const Timer);

void s_place(Sprite* const, const Point);

int s_muted(const Sprite* const);

int s_busy(const Sprite* const, const Timer);

void s_go_busy(Sprite* const, const Timer, const int ticks, const State);

int s_impulse(const Sprite* const, const Timer);

int s_evil_act(const Sprite* const, const Timer);

int s_will_rage(const Sprite* const, const Timer);

int s_must_spread(const Sprite* const, char** const floring);

void s_parried(Sprite* const, const Point, const Timer);
