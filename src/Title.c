#include "Title.h"

#include "util.h"

#include <stdarg.h>

typedef struct
{
    int start;
    int now;
    int end;
    int linger;
    char* str;
}
Title;

static Title* tt = NULL;

void xttadvance(const int now)
{
    tt->now = now;
}

void xttset(const int start, const int end, const int linger, const char* const text, ...)
{
    va_list args;
    va_start(args, text);

    // If lingering the max alpha for the alpha will be used after 50% sine in/out fade.
    tt->linger = linger;

    tt->start = start;
    tt->end = end;

    const int len = vsnprintf(NULL, 0, text, args);

    va_start(args, text);

    free(tt->str);
    tt->str = xtoss(char, len + 1);
    vsprintf(tt->str, text, args);

    va_end(args);
}

void xttclear(void)
{
    xttset(0, 0, false, "");
}

void xttstuckclear(void)
{
    if(tt->linger)
        xttclear();
}

void xttinit(void)
{
    tt = xwipe(Title, 1);
    xttclear();
}

static int done(void)
{
    return !tt->linger && tt->now > tt->end;
}

void xttshow(const Text text, const Sdl sdl)
{
    if(done())
        return;

    const float percent = (tt->now - tt->start) / (float) (tt->end - tt->start);
    const float max = 0xFF;
    const float alpha = max * sinf(percent * FPI);

    xfwrt(text.fill, text.line, sdl.renderer,
        sdl.xres / 2,
        sdl.yres / 2,
        tt->str,
        // If lingering, do not fade out.
        (percent > 0.5f && tt->linger) ? max : alpha);
}
