#pragma once

typedef struct
{
    int renders;
    int ticks;
    int rise;
    int fall;
    int last;
}
Timer;

Timer t_new(void);

Timer t_tick(Timer, const int renders);

int t_lo(const Timer);
