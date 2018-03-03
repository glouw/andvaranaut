#include "Item.h"

#include "util.h"

#include <stdlib.h>

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

Item xitnew(const Classification c, const int index)
{
    Item it;
    xzero(it);
    it.index = index;
    it.c = c;
    switch((int) c)
    {
    case AMMO:
        it = ammo(it);
        break;
    case AMULET:
        it = amulet(it);
        break;
    case ARMOR:
        it = armor(it);
        break;
    case BOOK:
        it = book(it);
        break;
    case BOOT:
        it = boot(it);
        break;
    // DO the rest when you're not so tired.
    case CHEST:
        it.cstr = "Chest";
        switch(index)
        {
        }
        break;
    case FLESH:
        it.cstr = "Flesh";
        switch(index)
        {
        }
        break;
    case FOOD:
        it.cstr = "Food";
        switch(index)
        {
        }
        break;
    case GLOVE:
        it.cstr = "Glove";
        switch(index)
        {
        }
        break;
    case HAT:
        it.cstr = "Hat";
        switch(index)
        {
        }
        break;
    case KEY:
        it.cstr = "Key";
        switch(index)
        {
        }
        break;
    case LIGHT:
        it.cstr = "Light";
        switch(index)
        {
        }
        break;
    case LONGWEP:
        it.cstr = "Long Weapon";
        switch(index)
        {
        }
        break;
    case MEDWEP:
        it.cstr = "Medium Weapon";
        switch(index)
        {
        }
        break;
    case MONEY:
        it.cstr = "Money";
        switch(index)
        {
        }
        break;
    case MUSIC:
        it.cstr = "Music";
        switch(index)
        {
        }
        break;
    case POTION:
        it.cstr = "Potion";
        switch(index)
        {
        }
        break;
    case RING:
        it.cstr = "Ring";
        switch(index)
        {
        }
        break;
    case ROCK:
        it.cstr = "Rock";
        switch(index)
        {
        }
        break;
    case SCROLL:
        it.cstr = "Scroll";
        switch(index)
        {
        }
        break;
    case SHIELD:
        it.cstr = "Shield";
        switch(index)
        {
        }
        break;
    case SHORTWEP:
        it.cstr = "Short Weapon";
        switch(index)
        {
        }
        break;
    case TOOL:
        it.cstr = "Tool";
        switch(index)
        {
        }
        break;
    case WAND:
        it.cstr = "Wand";
        switch(index)
        {
        }
        break;
    case NONE:
        it.cstr = "None";
        switch(index)
        {
        }
        break;
    case CLASSIFICATIONS:
        break;
    }
    return it;
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
