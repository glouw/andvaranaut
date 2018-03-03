#pragma once

#include "Items.h"
#include "Input.h"

typedef struct
{
    Items items;
    int selected;
    int w;
}
Inventory;

Inventory xinvnew();

Inventory xinvselect(Inventory inv, const Input);

int xinvuse(const Input);

void xwhatis(const Inventory, const Input, const int xres);
