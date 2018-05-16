#include "Title.h"

#include "util.h"

#include <stdarg.h>

Title xttzero()
{
    static Title title;
    return title;
}

void xttnow(Title* tt, const int now)
{
    tt->now = now;
}

void xttset(Title* tt, const int start, const int end, const char* const text, ...)
{
    va_list args;
    va_start(args, text);

    // Set the start and end time.
    tt->start = start;
    tt->end = end;

    // Spoof snprintf to get varagrs length.
    const int len = vsnprintf(NULL, 0, text, args);

    // Varags were used. Must rewind.
    va_start(args, text);

    // This length builds a char buffer which vsprintf populates. Null byte too.
    // First free whatever came before, even if it is NULL.
    free(tt->str);
    tt->str = (char*) malloc(sizeof(char) * len + 1);
    vsprintf(tt->str, text, args);

    // Cleanup
    va_end(args);
}

Title* xttnew(const int start, const int end, const char* const text)
{
    Title* tt = malloc(sizeof(*tt));
    *tt = xttzero();

    // Must use xttset to set text as text is malloc'd through xttset.
    xttset(tt, start, end, text);
    return tt;
}

void xttshow(Title* tt, const Font fill, const Font line, SDL_Renderer* const rend, const int xres, const int yres)
{
    if(tt->now > tt->end)
        return;

    // Alpha is calculated using a half sine wave from the start and end percentage.
    const float percent = (tt->now - tt->start) / (float) (tt->end - tt->start);
    const float alpha = 0xFF * sinf(percent * FPI);

    // Write to screen.
    xfwrt(fill, line, rend, xres / 2, yres / 2, tt->str, alpha);
}
