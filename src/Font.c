#include "Font.h"

#include "util.h"

Font xfzero()
{
    static Font font;
    return font;
}

Font xfbuild(const char* const path, const int size, const uint32_t color)
{
    if(!TTF_WasInit())
        TTF_Init();
    Font f = xfzero();
    f.type = TTF_OpenFont(path, size);
    if(f.type == NULL)
        xbomb("Could not open %s\n", path);
    // Inside color of font.
    f.in.r = (color >> 0x10) & 0xFF;
    f.in.g = (color >> 0x08) & 0xFF;
    f.in.b = (color >> 0x00) & 0xFF;
    // Outside color of font (A simple black outline).
    f.out.r = 0x00;
    f.out.g = 0x00;
    f.out.b = 0x00;
    return f;
}

// Alligned left.
static SDL_Rect aleft(TTF_Font* type, const Sdl sdl, const int x, const int y, const char* const text)
{
    (void) sdl;
    int w = 0;
    int h = 0;
    TTF_SizeText(type, text, &w, &h);
    const SDL_Rect target = { x, y, w, h };
    return target;
}

// Alligned right.
static SDL_Rect arght(TTF_Font* type, const Sdl sdl, const int x, const int y, const char* const text)
{
    int w = 0;
    int h = 0;
    TTF_SizeText(type, text, &w, &h);
    const SDL_Rect target = {
        x + sdl.xres - w,
        y + sdl.yres - h,
        w, h
    };
    return target;
}

static SDL_Texture* tget(TTF_Font* type, const Sdl sdl, const char* text, const SDL_Color color, const int outlined)
{
    TTF_SetFontOutline(type, outlined);
    SDL_Surface* const surface = TTF_RenderText_Solid(type, text, color);
    SDL_Texture* const texture = SDL_CreateTextureFromSurface(sdl.renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

static void xfput(TTF_Font* type, const Sdl sdl, const int x, const int y, const char* const text, const SDL_Color color, const int outlined)
{
    SDL_Texture* texture = tget(type, sdl, text, color, outlined);
    const SDL_Rect where = arght(type, sdl, x, y, text);
    SDL_RenderCopy(sdl.renderer, texture, NULL, &where);
    SDL_DestroyTexture(texture);
}

void xfwrt(const Font f, const Sdl sdl, const int x, const int y, const char* const text)
{
    xfput(f.type, sdl, x, y, text, f.in, false);
    xfput(f.type, sdl, x, y, text, f.out, true);
}
