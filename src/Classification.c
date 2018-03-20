#include "Classification.h"

#include "util.h"

int xcindex(const Classification c)
{
    return '~' + 1 - ' ' + c;
}

int xcindices(const Classification c, const Surfaces ss)
{
    const int index = xcindex(c);
    return c == NONE ? 1 : ss.surface[index]->h / ss.surface[index]->w;
}

int xismelee(const Classification c)
{
    return c == ARMOR
        || c == AMULET
        || c == BOOK
        || c == CHEST
        || c == BOOT
        || c == GLOVE
        || c == HAT
        || c == KEY
        || c == LIGHT
        || c == LONGWEP
        || c == MEDWEP
        || c == MUSIC
        || c == FLESH
        || c == FOOD
        || c == RING
        || c == ROCK
        || c == SHIELD
        || c == SHORTWEP
        || c == TOOL;
}

int xisrange(const Classification c)
{
    return c == AMMO
        || c == MONEY
        || c == POTION;
}

int xismagic(const Classification c)
{
    return c == SCROLL
        || c == WAND;
}

int xisconsumable(const Classification c)
{
    return c == BOOK
        || c == FLESH
        || c == FOOD
        || c == TOOL;
}
