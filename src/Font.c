#include "Font.h"

#include "util.h"

Font f_build(const char* const path, const int size, const uint32_t color, const int outline)
{
    if(!TTF_WasInit())
        TTF_Init();
    static Font zero;
    Font f = zero;
    f.type = TTF_OpenFont(path, size);
    if(f.type == NULL)
        u_bomb("Could not open %s\n", path);
    f.color.r = (color >> 0x10) & 0xFF;
    f.color.g = (color >> 0x08) & 0xFF;
    f.color.b = (color >> 0x00) & 0xFF;
    TTF_SetFontOutline(f.type, outline);
    return f;
}

SDL_Texture* f_get_texture(const Font f, SDL_Renderer* const rend, const int alpha, const char* str)
{
    SDL_Surface* const surface = TTF_RenderText_Solid(f.type, str, f.color);
    SDL_Texture* const texture = SDL_CreateTextureFromSurface(rend, surface);
    SDL_SetTextureAlphaMod(texture, alpha < 0 ? 0 : alpha);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Rect f_calc_size(const Font font, const char* const str)
{
    int w = 0;
    int h = 0;
    TTF_SizeText(font.type, str, &w, &h);
    const SDL_Rect sz = { 0, 0, w, h };
    return sz;
}
