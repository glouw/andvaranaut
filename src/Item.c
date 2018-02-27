#include "Item.h"

#include <stdlib.h>

void xiprint(const Surfaces ss)
{
    for(Classification cls = 0; cls < CLASSIFICATIONS; cls++)
        printf("%d\n", xcindices(cls, ss));
}

Item xirand(const Surfaces ss)
{
    const Classification cls = rand() % CLASSIFICATIONS;
    const int indices = rand() % xcindices(cls, ss);
    const Item item = { cls, indices };
    return item;
}
