#include "Item.h"

#include <stdlib.h>

void xitprint(const Surfaces ss)
{
    for(Classification cls = 0; cls < CLASSIFICATIONS; cls++)
        printf("%d\n", xcindices(cls, ss));
}

Item xitrand(const Surfaces ss)
{
    const Classification cls = rand() % CLASSIFICATIONS;
    const int index = rand() % xcindices(cls, ss);
    const Item item = { cls, index };
    return item;
}
