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

Timer xtmnew(void);

Timer xtmtick(Timer, const int renders, const int ticks);

int xtmlo(const Timer);
