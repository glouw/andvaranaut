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
    it.cstr = "Rock";
    switch(it.index)
    {
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
    switch((int) c)
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

Item xitrand(const Surfaces ss)
{
    const Classification c = (Classification) (rand() % (int) CLASSIFICATIONS);
    const int index = rand() % xcindices(c, ss);
    return xitnew(c, index);
}

void xitprint(const Item it)
{
    printf("%s\n", it.cstr);
}
