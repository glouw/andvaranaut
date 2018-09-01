#include "Timer.h"

Timer xztimer(void)
{
    static Timer timer;
    return timer;
}

Timer xtmnew(const int renders, const int ticks)
{
    Timer tm = xztimer();
    tm.renders = renders;
    tm.ticks = ticks;
    return tm;
}
