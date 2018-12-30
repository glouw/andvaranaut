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
    int drag;
    Item trade;
}
Inventory;

Inventory i_create(void);

Inventory i_select(Inventory, const Input);

Inventory i_hilite(Inventory, const Input, const int xres);

Inventory i_what_is(Inventory, const Scroll, const Timer);

Item i_get_equipped(const Inventory);

Inventory i_manage(Inventory, const Input, const int xres);

Inventory i_unhilite(Inventory);
