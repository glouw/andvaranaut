#include "print.h"
#include "Sdl.h"
#include "util.h"

typedef struct
{
    char* text;
    SDL_Color color;
    int outline;
}
Ttf;

static SDL_Rect size(TTF_Font* const font, char* const text)
{
    int w = 0;
    int h = 0;
    TTF_SizeText(font, text, &w, &h);
    const SDL_Rect rect = { 0, 0, w, h };
    return rect;
}

static SDL_Rect placing(TTF_Font* const font, char* const text, const int x, const int y)
{
    SDL_Rect target = size(font, text);
    target.x = x;
    target.y = y;
    return target;
}

static void scribble(const Ttf ttf, const int x, const int y, const Sdl sdl)
{
    TTF_SetFontOutline(sdl.font, ttf.outline);
    SDL_Surface* const surface = TTF_RenderText_Solid(sdl.font, ttf.text, ttf.color);
    SDL_Texture* const texture = SDL_CreateTextureFromSurface(sdl.renderer, surface);
    SDL_FreeSurface(surface);
    const SDL_Rect place = placing(sdl.font, ttf.text, x, y);
    SDL_RenderCopy(sdl.renderer, texture, NULL, &place);
    SDL_DestroyTexture(texture);
}

void print(const Sdl sdl, const int x, const int y, char* const text)
{
    const Ttf inner = { text, { 0xFF, 0xFF, 0x00, 0x00 }, 0 };
    const Ttf outer = { text, { 0x00, 0x00, 0x00, 0x00 }, 1 };
    scribble(outer, x, y, sdl);
    scribble(inner, x, y, sdl);
}
