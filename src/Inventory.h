#pragma once

#include "Items.h"
#include "Input.h"
#include "Scroll.h"
#include "Timer.h"

typedef struct
{
    Items items;
    int selected;
    int width;
    int hilited;
    int change;
    int last;
}
Inventory;

Inventory xinvnew(void);

Inventory xinvselect(Inventory, const Input);

Inventory xinvhilite(Inventory, const Input, const int xres);

Inventory xwhatis(Inventory, const Scroll, const Timer);
