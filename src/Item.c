#include "Item.h"

#include <stdlib.h>

Item irand()
{
    const Classification c = rand() % CLASSIFICATIONS;
    const int which = rand() % xcavail(c);
    const Item item = { c, which };
    return item;
}
