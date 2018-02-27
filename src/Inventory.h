#pragma once

#include "Items.h"
#include "Input.h"

typedef struct
{
    Items equipped;
    Items backpack;
}
Inventory;

Inventory xinvnew();

int xinvuse(const Input);
