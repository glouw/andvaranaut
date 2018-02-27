#include "Classification.h"

int xcavail(const Classification c)
{
    switch(c)
    {
    case AMMO     : return 1;
    case AMULET   : return 1;
    case ARMOR    : return 1;
    case BOOK     : return 1;
    case BOOT     : return 1;
    case CHEST    : return 1;
    case FLESH    : return 1;
    case FOOD     : return 1;
    case GLOVE    : return 1;
    case HAT      : return 1;
    case KEY      : return 1;
    case LIGHT    : return 1;
    case LONGWEP  : return 1;
    case MEDWEP   : return 1;
    case MONEY    : return 1;
    case MUSIC    : return 1;
    case POTION   : return 1;
    case RING     : return 1;
    case ROCK     : return 1;
    case SCROLL   : return 1;
    case SHIELD   : return 1;
    case SHORTWEP : return 1;
    case TOOL     : return 1;
    case WAND     : return 1;
    default       : return 1;
    }
}
