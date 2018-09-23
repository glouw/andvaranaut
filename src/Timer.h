#pragma once

typedef struct
{
    int renders;
    int ticks;
    int rise;
    int last;
}
Timer;

Timer xtmnew(void);

Timer xtmtick(Timer, const int renders, const int ticks);

int xtmhi(const Timer);

int xtmlo(const Timer);
