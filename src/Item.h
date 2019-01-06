#pragma once

#include "Identification.h"
#include "Gauge.h"
#include "Input.h"
#include "Timer.h"

typedef struct
{
    Identification id;
    const char* cstr;
    const char* name;
    const char* desc;
    float damage;
    int hurts;
    float amplitude;
    float period;
}
Item;

Item i_new(const Identification);

Item i_rand(void);

void i_test(void);

Item i_none(void);
