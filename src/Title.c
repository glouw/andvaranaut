#include "Title.h"

#include "util.h"

#include <stdarg.h>

Title xttzero()
{
    static Title title;
    return title;
}

Title xttnow(Title tt, const int now)
{
    tt.now = now;
    return tt;
}

Title xttset(Title tt, const int start, const int end)
{
    tt.start = start;
    tt.end = end;
    return tt;
}

Title xttnew(const int start, const int end)
{
    return xttset(xttzero(), start, end);
}

void xttshow(const Title tt, const Font fill, const Font line, const Sdl sdl, const char* const text, ...)
{
    if(tt.now > tt.end)
        return;
    va_list args;
    va_start(args, text);
    // Spoof snprintf to get varagrs length.
    const int len = vsnprintf(NULL, 0, text, args);
    // Varags were used. Must rewind.
    va_start(args, text);
    // This length builds a char buffer which vsprintf populates. Null byte too.
    char* const buffer = (char*) malloc(sizeof(char) * len + 1);
    vsprintf(buffer, text, args);
    // Alpha is calculated using a half sine wave from the start and end percentage.
    const float percent = (tt.now - tt.start) / (float) (tt.end - tt.start);
    const float alpha = 0xFF * sinf(percent * FPI);
    // Write to screen.
    xfwrt(fill, line, sdl, sdl.xres / 2, sdl.yres / 2, buffer, alpha);
    // Cleanup
    va_end(args);
    free(buffer);
}
