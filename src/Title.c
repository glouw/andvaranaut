#include "Title.h"

#include "util.h"

#include <stdarg.h>

typedef struct
{
    int start;
    int now;
    int end;
    char* str;
}
Title;

// Andvaranaut does not use global variables, but it makes no sense
// to pass a title pointer to every function that must present a title.

static Title* tt = NULL;

void xttadvance(const int now)
{
    tt->now = now;
}

void xttset(const int start, const int end, const char* const text, ...)
{
    va_list args;
    va_start(args, text);

    // Set the start and end time.
    tt->start = start;
    tt->end = end;

    // Spoof snprintf to get varargs length.
    const int len = vsnprintf(NULL, 0, text, args);

    // Varags were fully used. Must rewind.
    va_start(args, text);

    // This length builds a char buffer which vsprintf populates. Null byte too.
    // First free whatever came before, even if it is NULL.
    free(tt->str);
    tt->str = xtoss(char, len + 1);
    vsprintf(tt->str, text, args);

    // Cleanup
    va_end(args);
}

void xttinit(const int start, const int end, const char* const text)
{
    tt = xwipe(Title, 1);

    // Must use xttset to set text as text is malloc'd through xttset.
    xttset(start, end, text);
}

void xttshow(const Font fill, const Font line, const Sdl sdl)
{
    if(tt->now > tt->end)
        return;

    // Alpha is calculated using a half sine wave from the start and end percentage.
    const float percent = (tt->now - tt->start) / (float) (tt->end - tt->start);
    const float alpha = 0xFF * sinf(percent * FPI);

    // Write to screen.
    xfwrt(fill, line, sdl.renderer, sdl.xres / 2, sdl.yres / 2, tt->str, alpha);
}
