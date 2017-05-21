#include "Ttf.h"

static SDL_Rect placing(TTF_Font* const font, char* const text, const int x, const int y)
{
    SDL_Rect target = size(font, text);
    target.x = x;
    target.y = y;
    return target;
}

extern SDL_Rect size(TTF_Font* const font, char* const text)
{
    int w = 0;
    int h = 0;
    TTF_SizeText(font, text, &w, &h);
    const SDL_Rect rect = { 0, 0, w, h };
    return rect;
}

extern void scribble(const Ttf ttf, const int x, const int y, const Sdl sdl)
{
    TTF_SetFontOutline(sdl.font, ttf.outline);
    SDL_Surface* const surface = TTF_RenderText_Solid(sdl.font, ttf.text, ttf.color);
    SDL_Texture* const texture = SDL_CreateTextureFromSurface(sdl.renderer, surface);
    SDL_FreeSurface(surface);
    const SDL_Rect place = placing(sdl.font, ttf.text, x, y);
    SDL_RenderCopy(sdl.renderer, texture, NULL, &place);
    SDL_DestroyTexture(texture);
}
