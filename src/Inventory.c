#include "Inventory.h"

Inventory xinvnew()
{
    const Inventory inv = {
        xitsnew(10), // Equipped.
        xitsnew(30), // Backpack.
    };
    return inv;
}

int xinvuse(const Input in)
{
    return in.key[SDL_SCANCODE_LSHIFT];
}
