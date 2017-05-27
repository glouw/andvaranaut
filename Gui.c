#include "Gui.h"

#include "Sdl.h"
#include "Util.h"

typedef struct
{
    int margin;
    int height;
}
Ruler;

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

static char interpret(const Party party)
{
    return party == CEILING ? 'C' : party == WALLING ? 'W' : 'F';
}

static void print(const Sdl sdl, const int x, const int y, char* const text)
{
    const Ttf inner = { text, { 0xFF, 0xFF, 0x00, 0x00 }, 0 };
    const Ttf outer = { text, { 0x00, 0x00, 0x00, 0x00 }, 1 };
    scribble(outer, x, y, sdl);
    scribble(inner, x, y, sdl);
}

static Ruler select(const Ruler ruler, const Sdl sdl, const Hero hero)
{
    Ruler temp = ruler;
    char string[] = { interpret(hero.party), (char) hero.surface, '\0' };
    const SDL_Rect select = size(sdl.font, string);
    print(sdl, temp.margin, 0, string);
    temp.height += select.h;
    return temp;
}

static Ruler count(const Ruler ruler, const Sdl sdl, const Sprites sprites)
{
    Ruler temp = ruler;
    char string[MINTS];
    const SDL_Rect count = size(sdl.font, string);
    sprintf(string, "%d", sprites.count);
    print(sdl, temp.margin, temp.height, string);
    temp.height += count.h;
    return temp;
}

static void saved(const Ruler ruler, const Sdl sdl, const Hero hero)
{
    if(!hero.saved)
        return;
    const char* const string = "Saved!";
    const SDL_Rect saved = size(sdl.font, (char*) string);
    print(sdl, ruler.margin, sdl.res - saved.h, (char*) string);
}

static void insert(const Ruler ruler, const Sdl sdl, const Hero hero)
{
    if(!hero.inserting)
        return;
    const char* const string = "-- INSERT --";
    const SDL_Rect insert = size(sdl.font, (char*) string);
    print(sdl, ruler.margin, sdl.res - insert.h, (char*) string);
}

extern void gui(const Sdl sdl, const Hero hero, const Sprites sprites)
{
    // Gui only active when consoling
    if(!hero.consoling)
        return;
    Ruler ruler;
    ruler.height = 0;
    ruler.margin = sdl.res / 32;
    ruler = select(ruler, sdl, hero);
    ruler = count(ruler, sdl, sprites);
    saved(ruler, sdl, hero);
    insert(ruler, sdl, hero);
}
