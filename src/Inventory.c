#include "Inventory.h"

Inventory xinvnew()
{
    const Inventory inv = { xitsnew(8), 0 };
    return inv;
}

Inventory xinvselect(Inventory inv, const Input in)
{
    if(in.key[SDL_SCANCODE_1]) inv.selected = 0;
    if(in.key[SDL_SCANCODE_2]) inv.selected = 1;
    if(in.key[SDL_SCANCODE_3]) inv.selected = 2;
    return inv;
}

int xinvuse(const Input in)
{
    return in.key[SDL_SCANCODE_LSHIFT];
}
