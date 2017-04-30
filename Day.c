#include "Types.h"

Day begin()
{
    const int ticks = 2048;
    const float time = 0.0, brightest = 64.0, light = 0.0;
    const Day day = { time, ticks, brightest, light };
    return day;
}

Day progress(const Day day, const int ticks)
{
    const float pi = acosf(-1.0);
    Day temp = day;
    temp.time = (-cosf(pi * ticks / day.ticks) + 1.0) / 2.0;
    temp.light = temp.time * day.brightest;
    return temp;
}
