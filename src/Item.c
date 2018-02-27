#include "Item.h"

#include <stdlib.h>

void xitprint(const Surfaces ss)
{
    for(int i = 0; i < (int) CLASSIFICATIONS; i++)
    {
        const Classification cls = (Classification) i;
        printf("%d\n", xcindices(cls , ss));
    }
}

Item xitrand(const Surfaces ss)
{
    const Classification cls = (Classification) (rand() % (int) CLASSIFICATIONS);
    const int index = rand() % xcindices(cls, ss);
    const Item item = { cls, index };
    return item;
}
