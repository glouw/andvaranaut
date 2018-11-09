#include "Title.h"

#include "util.h"

#include <stdarg.h>

// Does need to be passed around as only one title will be used.
static Title tt;

void t_advance(const int now)
{
    tt.now = now;
}

void t_set(const int start, const int end, const int linger, const char* const fmt, ...)
{
    va_list args;

    // Get string length for formatter.
    va_start(args, fmt);
    const int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    // Rewind and format.
    va_start(args, fmt);
    free(tt.str);
    tt.str = u_toss(char, len + 1);
    vsprintf(tt.str, fmt, args);
    va_end(args);

    // If lingering the max alpha for the alpha will be used after 50% sine in/out fade.
    tt.linger = linger;
    tt.start = start;
    tt.end = end;
}

void t_clear(void)
{
    t_set(0, 0, false, "");
}

void t_clear_when_linger(void)
{
    if(tt.linger)
        t_clear();
}

void t_init(void)
{
    t_clear();
}

static int done(void)
{
    return !tt.linger && tt.now > tt.end;
}

void t_show(const Text text, const Sdl sdl)
{
    if(done())
        return;
    const float percent = (tt.now - tt.start) / (float) (tt.end - tt.start);
    const float max = 0xFF;
    const float alpha = max * sinf(percent * U_PI);
    t_print(
        text,
        tt.str,
        (percent > 0.5f && tt.linger) ? max : alpha, // If lingering, do not fade out.
        sdl.renderer,
        sdl.xres / 2,
        sdl.yres / 2);
}
