#include "Inventory.h"

Inventory xinvnew()
{
    const Inventory inv = {
        xitsnew(20)
    };
    return inv;
}

int xinvuse(const Input in)
{
    return in.key[SDL_SCANCODE_LSHIFT];
}
