#pragma once

#include "Classification.h"

typedef struct
{
    int slowdown;
    int slowmo;
    int renders;
    int ticks;
    int rise;
    int fall;
    int last;
}
Timer;

Timer t_new(void);

Timer t_tick(Timer, const int slowmo, const int renders);

int t_lo(const Timer);

int t_hi(const Timer);
