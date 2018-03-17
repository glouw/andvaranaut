#pragma once

#include "Classification.h"

typedef struct
{
    int index;
    Classification c;
    const char* desc;
    // Classification string.
    const char* cstr;
}
Item;

Item xzitem();

Item xitnew(const Classification, const int index);

Item xitrand(const Surfaces);

void xitprint(const Item);
