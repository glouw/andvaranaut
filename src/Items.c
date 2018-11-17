#include "Items.h"

#include "util.h"

Items i_build(const int max)
{
    const Items its = { u_toss(Item, max), max };
    for(int i = 0; i < max; i++)
        its.item[i] = i_new(NONE, 0);
    return its;
}

static int avail(const Items its)
{
    for(int i = 0; i < its.max; i++)
        if(its.item[i].clas == NONE)
            return i;
    return -1;
}

int i_add(Items its, const Item it)
{
    const int index = avail(its);
    if(index == -1)
        return false;
    its.item[index] = it;
    return true;
}
