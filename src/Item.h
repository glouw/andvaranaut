#pragma once

#include "Classification.h"

typedef struct
{
    int index;
    Classification c;
    const char* name;
    const char* desc;
    const char* cstr;
    float damage;
    int hurts;
    float amplitude;
    float period;
}
Item;

Item xzitem(void);

Item xitnew(const Classification, const int index);

Item xitrand(void);
