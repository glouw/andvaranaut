#pragma once

typedef struct
{
    int renders;
    int ticks;
    int rise;
}
Timer;

Timer xtmnew(const int renders, const int ticks);

int xtmhi(const Timer);

int xtmlo(const Timer);
