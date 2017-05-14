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
    char string[] = { interpret(hero.party), hero.block, '\0' };
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
    temp.height = count.h;
    return temp;
}

Ruler insertion(const Ruler ruler)
{
    Ruler temp = ruler;
    char* const string = "-- INSERT --";
    const SDL_Rect insert = size(temp.sdl.font, string);
    print(temp.sdl, temp.margin, temp.sdl.res - insert.h, string);
    temp.height = temp.sdl.res;
    return temp;
}
