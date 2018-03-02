#pragma once

#include "Items.h"
#include "Input.h"

typedef struct
{
    Items items;
    int selected;
}
Inventory;

Inventory xinvnew();

Inventory xinvselect(Inventory inv, const Input);

int xinvuse(const Input);
