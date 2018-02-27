#include "Items.h"

#include "util.h"

Items xisnew(const int max)
{
    const Items is = { xtoss(Item, max), 0, max };
    return is;
}

Items xisadd(Items is, const Item item)
{
    if(is.count == is.max)
        xbomb("items size limitation reached\n");
    is.item[is.count++] = item;
    return is;
}
