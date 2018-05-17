#pragma once

typedef struct
{
    int renders;
    int ticks;
}
Timer;

Timer xztimer();

Timer xtmnew(const int renders, const int ticks);
