#include "Ruler.h"

#include "Ttf.h"
#include "Sdl.h"

#include <limits.h>

static char interpret(const Party party)
{
    return party == CEILING ? 'C' : party == WALLING ? 'W' : 'F';
}

Ruler selection(const Ruler ruler, const Hero hero)
{
    Ruler temp = ruler;
    char string[] = { interpret(hero.party), (char) hero.surface, '\0' };
    const SDL_Rect select = size(temp.sdl.font, string);
    print(temp.sdl, temp.margin, 0, string);
    temp.height = select.h;
    return temp;
}

Ruler countings(const Ruler ruler, const Sprites sprites)
{
    Ruler temp = ruler;
    const int max = (CHAR_BIT * sizeof(int) - 1) / 3 + 2;
    char string[max];
    const SDL_Rect count = size(temp.sdl.font, string);
    sprintf(string, "%d", sprites.count);
    print(temp.sdl, temp.margin, temp.height, string);
    temp.height += count.h;
    return temp;
}

void saved(const Ruler ruler, const Hero hero)
{
    if(!hero.saved)
        return;
    const char* const string = "Saved!";
    const SDL_Rect saved = size(ruler.sdl.font, (char*) string);
    print(ruler.sdl, ruler.margin, ruler.sdl.res - saved.h, (char*) string);
}

void insertion(const Ruler ruler, const Hero hero)
{
    if(!hero.consoling)
        return;
    const char* const string = "-- INSERT --";
    const SDL_Rect insert = size(ruler.sdl.font, (char*) string);
    print(ruler.sdl, ruler.margin, ruler.sdl.res - insert.h, (char*) string);
}
