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
    SDL_Texture* const tfill = f_get_texture(text.fill, rend, alpha, str);
    SDL_Texture* const tline = f_get_texture(text.line, rend, alpha, str);
    SDL_RenderCopy(rend, tfill, NULL, &target);
    SDL_RenderCopy(rend, tline, NULL, &target);
    SDL_DestroyTexture(tfill);
    SDL_DestroyTexture(tline);
}

void t_write(const Text text, SDL_Renderer* const rend, const int x, const int y, const Position position, const int alpha, const int line, const char* const str)
{
    const SDL_Rect sz = f_calc_size(text.fill, str);
    SDL_Rect to = { 0, 0, sz.w, sz.h };
    switch(position)
    {
        case TOP_LEFT:
            to.x = x;
            to.y = y + line * sz.h;
            to.w = sz.w;
            to.h = sz.h;
            break;

        case TOP_RITE:
            to.x = x - sz.w;
            to.y = y + line * sz.h;
            to.w = sz.w;
            to.h = sz.h;
            break;

        case BOT_LEFT:
            break;

        case BOT_RITE:
            to.x = x - sz.w;
            to.y = y - sz.h + line * sz.h;
            to.w = sz.w;
            to.h = sz.h;
            break;

        case MIDDLE:
            to.x = x - sz.w / 2;
            to.y = y - sz.h / 2 + line * sz.h;
            to.w = sz.w;
            to.h = sz.h;
            break;
    }
    render_text(text, rend, to, str, alpha);
}

// Handles '\n' but not extended formatting.
void t_print(const Text text, SDL_Renderer* const rend, const int x, const int y, const Position position, const int alpha, const char* const str)
{
    char* const copy = u_str_dup(str);
    const char* const delim = "\n";
    int line = 0;
    for(char* tok = strtok(copy, delim); tok; tok = strtok(NULL, delim))
        t_write(text, rend, x, y, position, alpha, line++, tok);
    free(copy);
}

// Handles extended formatting but not '\n'.
void t_scrib(const Text text, SDL_Renderer* const rend, const int x, const int y, const Position position, const int alpha, const int line, const char* const fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    const int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    va_start(args, fmt);
    char* const str = u_toss(char, len + 1);
    vsprintf(str, fmt, args);
    va_end(args);

    t_write(text, rend, x, y, position, alpha, line, str);

    free(str);
}
