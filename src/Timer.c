#include "Timer.h"

#include "Frame.h"

static int lo(const int ticks)
{
    return ticks % FRAMES == 0;
}

static int hi(const int ticks)
{
    return ticks % FRAMES == 1;
}

static int fall(const Timer tm)
{
    return hi(tm.last) && lo(tm.ticks);
}

static int rise(const Timer tm)
{
    return lo(tm.last) && hi(tm.ticks);
}

Timer t_new(void)
{
    static Timer timer;
    return timer;
}

Timer t_tick(Timer tm, const int slowmo, const int renders)
{
    tm.slowdown = 2.0f;
    tm.slowmo = slowmo;
    tm.last = tm.ticks;

    const int norm = 10;
    const int slow = norm * tm.slowdown;
    tm.ticks += (tm.renders % (slowmo ? slow : norm)) == 0;

    tm.renders = renders;
    tm.rise = rise(tm);
    tm.fall = fall(tm);
    return tm;
}

int t_lo(const Timer tm)
{
    return lo(tm.ticks);
}

int t_hi(const Timer tm)
{
    return hi(tm.ticks);
}
