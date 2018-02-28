#pragma once

#include "Items.h"
#include "Input.h"

typedef struct
{
    Items items;
}
Inventory;

Inventory xinvnew();

int xinvuse(const Input);
