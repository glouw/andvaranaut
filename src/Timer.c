#include "Timer.h"

#include "Frame.h"

static Timer ztimer(void)
{
    static Timer timer;
    return timer;
}

static Timer rise(Timer tm)
{
    static int last;
    const int now = xtmhi(tm);
    tm.rise = last && now;
    last = xtmlo(tm);
    return tm;
}

Timer xtmnew(const int renders, const int ticks)
{
    Timer tm = ztimer();
    tm.renders = renders;
    tm.ticks = ticks;
    return rise(tm);
}

int xtmhi(const Timer tm)
{
    return tm.ticks % FRAMES == 0;
}

int xtmlo(const Timer tm)
{
    return tm.ticks % FRAMES == 1;
}
