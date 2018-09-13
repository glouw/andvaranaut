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
    case     AMMO: return 25;
    case   AMULET: return 17;
    case    ARMOR: return 59;
    case     BOOK: return 65;
    case     BOOT: return 10;
    case    CHEST: return 16;
    case    FLESH: return 62;
    case     FOOD: return 43;
    case    GLOVE: return  4;
    case      HAT: return 15;
    case      KEY: return  3;
    case    LIGHT: return  7;
    case  LONGWEP: return 33;
    case   MEDWEP: return  9;
    case    MONEY: return 60;
    case    MUSIC: return 15;
    case   POTION: return 40;
    case     RING: return 44;
    case     ROCK: return  8;
    case   SCROLL: return 24;
    case   SHIELD: return  7;
    case SHORTWEP: return 20;
    case     TOOL: return 20;
    case     WAND: return 49;

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
        || c == FOOD
        || c == FLESH
        || c == TOOL;
}
