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

Inventory i_create(void);

Inventory i_select(Inventory, const Input);

Inventory i_highlite(Inventory, const Input, const int xres);

Inventory i_whatis(Inventory, const Scroll, const Timer);
