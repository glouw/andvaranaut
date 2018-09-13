#include "Rect.h"

SDL_Rect xzrect(void)
{
    static SDL_Rect rect;
    return rect;
}
