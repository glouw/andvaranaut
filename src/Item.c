#include "Item.h"

#include "util.h"

#include <stdlib.h>

Item xzitem()
{
    static Item item;
    return item;
}

static Item ammo(Item it)
{
    it.cstr = "Ammo";
    switch(it.index)
    {
    }
    return it;
}

static Item amulet(Item it)
{
    it.cstr = "Amulet";
    switch(it.index)
    {
    }
    return it;
}

static Item armor(Item it)
{
    it.cstr = "Armor";
    switch(it.index)
    {
    }
    return it;
}

static Item book(Item it)
{
    it.cstr = "Book";
    switch(it.index)
    {
    }
    return it;
}

static Item boot(Item it)
{
    it.cstr = "Boot";
    switch(it.index)
    {
    }
    return it;
}

static Item chest(Item it)
{
    it.cstr = "Chest";
    switch(it.index)
    {
    }
    return it;
}

static Item flesh(Item it)
{
    it.cstr = "Flesh";
    switch(it.index)
    {
    }
    return it;
}

static Item food(Item it)
{
    it.cstr = "Food";
    switch(it.index)
    {
    }
    return it;
}

static Item glove(Item it)
{
    it.cstr = "Glove";
    switch(it.index)
    {
    }
    return it;
}

static Item hat(Item it)
{
    it.cstr = "Hat";
    switch(it.index)
    {
    }
    return it;
}

static Item key(Item it)
{
    it.cstr = "Key";
    switch(it.index)
    {
    }
    return it;
}

static Item light(Item it)
{
    it.cstr = "Light";
    switch(it.index)
    {
    }
    return it;
}

static Item longwep(Item it)
{
    it.cstr = "Longwep";
    switch(it.index)
    {
    }
    return it;
}

static Item medwep(Item it)
{
    it.cstr = "Medwep";
    switch(it.index)
    {
    }
    return it;
}

static Item money(Item it)
{
    it.cstr = "Money";
    switch(it.index)
    {
    }
    return it;
}

static Item music(Item it)
{
    it.cstr = "Music";
    switch(it.index)
    {
    }
    return it;
}

static Item potion(Item it)
{
    it.cstr = "Potion";
    switch(it.index)
    {
    }
    return it;
}

static Item ring(Item it)
{
    it.cstr = "Ring";
    switch(it.index)
    {
    }
    return it;
}

static Item rock(Item it)
{
    it.cstr = "Rock";
    switch(it.index)
    {
    }
    return it;
}

static Item scroll(Item it)
{
    it.cstr = "Scroll";
    switch(it.index)
    {
        case  0: it.name = "Odal"  , it.desc = "Home";    break;
        case  1: it.name = "Daeg"  , it.desc = "Day";     break;
        case  2: it.name = "Jera"  , it.desc = "Harvest"; break;
        case  3: it.name = "Lagu"  , it.desc = "Water";   break;
        case  4: it.name = "Man"   , it.desc = "Human";   break;
        case  5: it.name = "Eh"    , it.desc = "Horse";   break;
        case  6: it.name = "Beorc" , it.desc = "Puriry";  break;
        case  7: it.name = "Tyr"   , it.desc = "Arrow";   break;
        case  8: it.name = "Sigel" , it.desc = "Sun";     break;
        case  9: it.name = "Elhaz" , it.desc = "Elk";     break;
        case 10: it.name = "Peorth", it.desc = "Dice";    break;
        case 11: it.name = "Eoh"   , it.desc = "Yew";     break;
        case 12: it.name = "Ng"    , it.desc = "Hero";    break;
        case 13: it.name = "Is"    , it.desc = "Ice";     break;
        case 14: it.name = "Nyd"   , it.desc = "Need";    break;
        case 15: it.name = "Hagal" , it.desc = "Hail";    break;
        case 16: it.name = "Wynn"  , it.desc = "Wind";    break;
        case 17: it.name = "Gifu"  , it.desc = "Gift";    break;
        case 18: it.name = "Ken"   , it.desc = "Wood";    break;
        case 19: it.name = "Rad"   , it.desc = "Wheel";   break;
        case 20: it.name = "As"    , it.desc = "God";     break;
        case 21: it.name = "Thorn" , it.desc = "Giant";   break;
        case 22: it.name = "Ur"    , it.desc = "Ox";      break;
        case 23: it.name = "Feoh"  , it.desc = "Cattle";  break;
    }
    return it;
}

static Item shield(Item it)
{
    it.cstr = "Shield";
    switch(it.index)
    {
    }
    return it;
}

static Item shortwep(Item it)
{
    it.cstr = "Shortwep";
    switch(it.index)
    {
    }
    return it;
}

static Item tool(Item it)
{
    it.cstr = "Tool";
    switch(it.index)
    {
    }
    return it;
}

static Item wand(Item it)
{
    it.cstr = "Wand";
    switch(it.index)
    {
    }
    return it;
}

static Item none(Item it)
{
    it.cstr = "None";
    switch(it.index)
    {
    }
    return it;
}

Item xitnew(const Classification c, const int index)
{
    Item it = xzitem();
    it.index = index;
    it.c = c;
    it.period = 0.05f;
    it.amplitude = 360.0f;
    it.damage = 25.0f;
    it.hurts = 3;
    switch(c)
    {
    case     AMMO: return     ammo(it);
    case   AMULET: return   amulet(it);
    case    ARMOR: return    armor(it);
    case     BOOK: return     book(it);
    case     BOOT: return     boot(it);
    case    CHEST: return    chest(it);
    case    FLESH: return    flesh(it);
    case     FOOD: return     food(it);
    case    GLOVE: return    glove(it);
    case      HAT: return      hat(it);
    case      KEY: return      key(it);
    case    LIGHT: return    light(it);
    case  LONGWEP: return  longwep(it);
    case   MEDWEP: return   medwep(it);
    case    MONEY: return    money(it);
    case    MUSIC: return    music(it);
    case   POTION: return   potion(it);
    case     RING: return     ring(it);
    case     ROCK: return     rock(it);
    case   SCROLL: return   scroll(it);
    case   SHIELD: return   shield(it);
    case SHORTWEP: return shortwep(it);
    case     TOOL: return     tool(it);
    case     WAND: return     wand(it);
    case     NONE: return     none(it);
    default:
        return it;
    }
}

Item xitrand()
{
    const Classification c = (Classification) (rand() % (int) CLASSIFICATIONS);
    const int index = rand() % xcindices(c);
    return xitnew(c, index);
}
