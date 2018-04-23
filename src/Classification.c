#include "Classification.h"

#include "util.h"

int xcindex(const Classification c)
{
    return '~' + 1 - ' ' + (int) c;
}

int xcindices(const Classification c)
{
    switch(c)
    {
        // TODO: Fill all these in by hand.
        case     AMMO: return 4;
        case   AMULET: return 4;
        case    ARMOR: return 4;
        case     BOOK: return 4;
        case     BOOT: return 4;
        case    CHEST: return 4;
        case    FLESH: return 4;
        case     FOOD: return 4;
        case    GLOVE: return 4;
        case      HAT: return 4;
        case      KEY: return 4;
        case    LIGHT: return 4;
        case  LONGWEP: return 4;
        case   MEDWEP: return 4;
        case    MONEY: return 4;
        case    MUSIC: return 4;
        case   POTION: return 4;
        case     RING: return 4;
        case     ROCK: return 4;
        case   SCROLL: return 4;
        case   SHIELD: return 4;
        case SHORTWEP: return 4;
        case     TOOL: return 4;
        case     WAND: return 4;
        default:
            return 1;
    }
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
