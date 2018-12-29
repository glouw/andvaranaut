#pragma once

#include "Classification.h"
#include "Gauge.h"
#include "Input.h"
#include "Timer.h"

typedef struct
{
    int index;
    Classification clas;
    const char* cstr;
    const char* name;
    const char* desc;
    float damage;
    int hurts;
    float amplitude;
    float period;
}
Item;

Item i_new(const Classification, const int index);

Item i_rand(void);

void i_test(void);

int i_can_block(const Item);

int i_successful_block(const Item, const Gauge);

Item i_none(void);
