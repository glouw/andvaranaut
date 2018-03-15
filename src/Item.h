#pragma once

#include "Classification.h"

typedef struct
{
    int index;
    Classification c;
    char* desc;
    // Classification string.
    char* cstr;
}
Item;

Item xitnew(const Classification, const int index);

Item xitrand(const Surfaces);

void xitprint(const Item);
