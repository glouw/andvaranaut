#include "Ttf.h"

Ttf xttfnew(const char* const path, const int size, const uint32_t inner, const uint32_t outer)
{
    const Font fill = xfbuild(path, size, inner, 0);
    const Font line = xfbuild(path, size, outer, 1);
    const Ttf ttf = { fill, line };
    return ttf;
}
