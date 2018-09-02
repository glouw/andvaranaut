#include "Items.h"

#include "util.h"

Items xzitems(void)
{
    static Items items;
    return items;
}

Items xitsnew(const int max)
{
    const Items its = { xtoss(Item, max), max };
    for(int i = 0; i < max; i++)
        its.item[i] = xitnew(NONE, 0);

    return its;
}

static int avail(const Items its)
{
    for(int i = 0; i < its.max; i++)
        if(its.item[i].c == NONE)
            return i;

    return -1;
}

int xitsadd(Items its, const Item it)
{
    const int index = avail(its);
    if(index == -1)
        return false;

    its.item[index] = it;

    return true;
}
