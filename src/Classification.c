#include "Classification.h"

#include "util.h"

int c_get_surface_index(const Classification c)
{
    return '~' + 1 - ' ' + (int) c;
}

int c_max_indices(const Classification c)
{
    switch(c)
    {
    // Do not use default.
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
    case   LETTER: return  2;
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

    case NONE:
    case CLASSIFICATIONS:
        return 1;
    }
    return 1;
}

int c_is_melee(const Classification c)
{
    return c == LONGWEP
        || c == MEDWEP
        || c == ROCK
        || c == SHIELD
        || c == SHORTWEP;
}

int c_is_range(const Classification c)
{
    return c == AMMO;
}

int c_is_magic(const Classification c)
{
    return c == WAND;
}

int c_is_consumable(const Classification c)
{
    return c == BOOK
        || c == FOOD
        || c == FLESH
        || c == TOOL;
}

Classification c_rand(void)
{
    const Classification c = (Classification) (rand() % (int) CLASSIFICATIONS);
    return (c == NONE || c == CLASSIFICATIONS) ? c_rand() : c;
}
