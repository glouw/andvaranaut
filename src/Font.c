#include "Font.h"

#include "util.h"

Font xfzero(void)
{
    static Font font;
    return font;
}

Font xfbuild(const char* const path, const int size, const uint32_t color, const int outline)
{
    if(!TTF_WasInit())
        TTF_Init();

    Font f = xfzero();
    f.type = TTF_OpenFont(path, size);
    if(f.type == NULL)
        xbomb("Could not open %s\n", path);

    f.color.r = (color >> 0x10) & 0xFF;
    f.color.g = (color >> 0x08) & 0xFF;
    f.color.b = (color >> 0x00) & 0xFF;
    TTF_SetFontOutline(f.type, outline);

    return f;
}

SDL_Texture* xtget(const Font f, SDL_Renderer* const rend, const int alpha, const char* text)
{
    SDL_Surface* const surface = TTF_RenderText_Solid(f.type, text, f.color);
    SDL_Texture* const texture = SDL_CreateTextureFromSurface(rend, surface);
    SDL_SetTextureAlphaMod(texture, alpha < 0 ? 0 : alpha);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Rect xfsize(const Font font, const char* const string)
{
    int w = 0;
    int h = 0;
    TTF_SizeText(font.type, string, &w, &h);
    const SDL_Rect size = { 0, 0, w, h };
    return size;
}

void xfrender(const Font fill, const Font line, SDL_Renderer* const rend, const SDL_Rect target, const char* const text, const int alpha)
{
    SDL_Texture* tfill = xtget(fill, rend, alpha, text);
    SDL_Texture* tline = xtget(line, rend, alpha, text);
    SDL_RenderCopy(rend, tfill, NULL, &target);
    SDL_RenderCopy(rend, tline, NULL, &target);
    SDL_DestroyTexture(tfill);
    SDL_DestroyTexture(tline);
}

void xfputmid(const Font fill, const Font line, SDL_Renderer* const rend, const int x, const int y, const char* const text, const int alpha)
{
    char* const copy = dups(text);
    const char* const delim = "\n";
    int newline = 0;
    for(char* tok = strtok(copy, delim); tok; tok = strtok(NULL, delim))
    {
        const SDL_Rect size = xfsize(fill, tok);
        const SDL_Rect target = {
            x - size.w / 2,
            y - size.h / 2 + size.h * newline,
            size.w, size.h
        };
        xfrender(fill, line, rend, target, tok, alpha);
        newline++;
    }
    free(copy);
}

void xfputbr(const Font fill, const Font line, SDL_Renderer* const rend, const int x, const int y, const char* const text, const int alpha)
{
    const SDL_Rect size = xfsize(fill, text);
    const SDL_Rect target = {
        x - size.w,
        y - size.h,
        size.w, size.h
    };
    xfrender(fill, line, rend, target, text, alpha);
}
