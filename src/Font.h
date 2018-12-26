#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct
{
    TTF_Font* type;
    SDL_Color color;
    SDL_Rect size;
}
Font;

Font f_build(const char* const path, const int size, const uint32_t color, const int outline);

SDL_Texture* f_get_texture(const Font, SDL_Renderer* const rend, const int alpha, const char* str);

SDL_Rect f_calc_size(const Font, const char* const str);
