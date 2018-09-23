#include "Timer.h"

#include "Frame.h"

static Timer ztimer(void)
{
    static Timer timer;
    return timer;
}

static Timer rise(Timer tm)
{
    tm.rise = tm.last && xtmhi(tm);
    tm.last = xtmlo(tm);
    return tm;
}

Timer xtmnew(void)
{
    return ztimer();
}

Timer xtmtick(Timer tm, const int renders, const int ticks)
{
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
