#include "Text.h"

Text t_build(const char* const path, const int size, const uint32_t inner, const uint32_t outer)
{
    const Font fill = f_build(path, size, inner, 0);
    const Font line = f_build(path, size, outer, 1);
    const Text text = { fill, line };
    return text;
}
