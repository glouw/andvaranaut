#include "Title.h"

#include "util.h"

#include <stdarg.h>

// Does need to be passed around as only one title will be used.
static Title tt;

void xttadvance(const int now)
{
    tt.now = now;
}

void xttset(const int start, const int end, const int linger, const char* const fmt, ...)
{
    va_list args;

    // Get string length for formatter.
    va_start(args, fmt);
        const int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    // Rewind and format.
    va_start(args, fmt);
        free(tt.str);
        tt.str = xtoss(char, len + 1);
        vsprintf(tt.str, fmt, args);
    va_end(args);

    // If lingering the max alpha for the alpha will be used after 50% sine in/out fade.
    tt.linger = linger;
    tt.start = start;
    tt.end = end;
}

void xttclear(void)
{
    xttset(0, 0, false, "");
}

void xttstuckclear(void)
{
    if(tt.linger)
        xttclear();
}

void xttinit(void)
{
    xttclear();
}

static int done(void)
{
    return !tt.linger && tt.now > tt.end;
}

void xttshow(const Text text, const Sdl sdl)
{
    if(done())
        return;
    const float percent = (tt.now - tt.start) / (float) (tt.end - tt.start);
    const float max = 0xFF;
    const float alpha = max * sinf(percent * FPI);
    f_print(
        text.fill,
        text.line,
        tt.str,
        (percent > 0.5f && tt.linger) ? max : alpha, // If lingering, do not fade out.
        sdl.renderer,
        sdl.xres / 2,
        sdl.yres / 2);
}
