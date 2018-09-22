#include "Timer.h"

#include "Frame.h"

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

int xtmhi(const Timer tm)
{
    return tm.ticks % FRAMES == 0;
}

int xtmlo(const Timer tm)
{
    return tm.ticks % FRAMES == 1;
}
