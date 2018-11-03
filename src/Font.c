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
        xbomb("Could not open %s\n", path);
    f.color.r = (color >> 0x10) & 0xFF;
    f.color.g = (color >> 0x08) & 0xFF;
    f.color.b = (color >> 0x00) & 0xFF;
    TTF_SetFontOutline(f.type, outline);
    return f;
}

static SDL_Texture* texture(const Font f, SDL_Renderer* const rend, const int alpha, const char* text)
{
    SDL_Surface* const surface = TTF_RenderText_Solid(f.type, text, f.color);
    SDL_Texture* const texture = SDL_CreateTextureFromSurface(rend, surface);
    SDL_SetTextureAlphaMod(texture, alpha < 0 ? 0 : alpha);
    SDL_FreeSurface(surface);
    return texture;
}

static SDL_Rect size(const Font font, const char* const string)
{
    int w = 0;
    int h = 0;
    TTF_SizeText(font.type, string, &w, &h);
    const SDL_Rect sz = { 0, 0, w, h };
    return sz;
}

static void render(const Font fill, const Font line, SDL_Renderer* const rend, const SDL_Rect target, const char* const text, const int alpha)
{
    SDL_Texture* tfill = texture(fill, rend, alpha, text);
    SDL_Texture* tline = texture(line, rend, alpha, text);
    SDL_RenderCopy(rend, tfill, NULL, &target);
    SDL_RenderCopy(rend, tline, NULL, &target);
    SDL_DestroyTexture(tfill);
    SDL_DestroyTexture(tline);
}

void f_put(const Font fill, const Font line, const char* const text, const int alpha, SDL_Renderer* const rend, const int x, const int y)
{
    const SDL_Rect sz = size(fill, text);
    const SDL_Rect to = {
        x - sz.w / 2,
        y - sz.h / 2,
        sz.w,
        sz.h,
    };
    render(fill, line, rend, to, text, alpha);
}

void f_putbr(const Font fill, const Font line, const char* const text, const int alpha, SDL_Renderer* const rend, const int x, const int y)
{
    const SDL_Rect sz = size(fill, text);
    const SDL_Rect target = {
        x - sz.w,
        y - sz.h,
        sz.w,
        sz.h,
    };
    render(fill, line, rend, target, text, alpha);
}

void f_print(const Font fill, const Font line, const char* const text, const int alpha, SDL_Renderer* const rend, const int x, const int y)
{
    char* const copy = dups(text);
    const char* const delim = "\n";
    int newline = 0;
    for(char* tok = strtok(copy, delim); tok; tok = strtok(NULL, delim))
    {
        const SDL_Rect sz = size(fill, tok);
        const SDL_Rect target = {
            x - sz.w / 2,
            y - sz.h / 2 + sz.h * newline,
            sz.w,
            sz.h,
        };
        render(fill, line, rend, target, tok, alpha);
        newline++;
    }
    free(copy);
}
