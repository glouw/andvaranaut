#pragma once

#include "Classification.h"

typedef struct
{
    int index;
    Classification clas;
}
Identification;

int i_same_id(const Identification a, const Identification b);
