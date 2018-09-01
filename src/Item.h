#pragma once

#include "Classification.h"

typedef struct
{
    int index;

    Classification c;

    const char* name;
    const char* desc;

    // Classification string.
    const char* cstr;

    // Stats,
    float damage;
    int hurts;

    // Additional stats for bows.
    float amplitude;
    float period;
}
Item;

Item xzitem(void);

Item xitnew(const Classification, const int index);

Item xitrand(void);
