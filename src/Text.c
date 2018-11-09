#include "Text.h"

#include "util.h"

Text t_build(const char* const path, const int size, const uint32_t inner, const uint32_t outer)
{
    const Font fill = f_build(path, size, inner, 0);
    const Font line = f_build(path, size, outer, 1);
    const Text text = { fill, line };
    return text;
}

static void render_text(const Text text, SDL_Renderer* const rend, const SDL_Rect target, const char* const str, const int alpha)
{
    SDL_Texture* tfill = f_get_texture(text.fill, rend, alpha, str);
    SDL_Texture* tline = f_get_texture(text.line, rend, alpha, str);
    SDL_RenderCopy(rend, tfill, NULL, &target);
    SDL_RenderCopy(rend, tline, NULL, &target);
    SDL_DestroyTexture(tfill);
    SDL_DestroyTexture(tline);
}

void t_put(const Text text, const char* const str, const int alpha, SDL_Renderer* const rend, const int x, const int y)
{
    const SDL_Rect sz = f_calc_size(text.fill, str);
    const SDL_Rect to = {
        x - sz.w / 2,
        y - sz.h / 2,
        sz.w,
        sz.h,
    };
    render_text(text, rend, to, str, alpha);
}

void t_put_bottom_right(const Text text, const char* const str, const int alpha, SDL_Renderer* const rend, const int x, const int y)
{
    const SDL_Rect sz = f_calc_size(text.fill, str);
    const SDL_Rect target = {
        x - sz.w,
        y - sz.h,
        sz.w,
        sz.h,
    };
    render_text(text, rend, target, str, alpha);
}

void t_print(const Text text, const char* const str, const int alpha, SDL_Renderer* const rend, const int x, const int y)
{
    char* const copy = u_dups(str);
    const char* const delim = "\n";
    int newline = 0;
    for(char* tok = strtok(copy, delim); tok; tok = strtok(NULL, delim))
    {
        const SDL_Rect sz = f_calc_size(text.fill, tok);
        const SDL_Rect target = {
            x - sz.w / 2,
            y - sz.h / 2 + sz.h * newline,
            sz.w,
            sz.h,
        };
        render_text(text, rend, target, tok, alpha);
        newline++;
    }
    free(copy);
}
