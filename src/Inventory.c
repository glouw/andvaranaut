#include "Inventory.h"

#include "util.h"

Inventory xinvnew()
{
    const Inventory inv = { xitsnew(10), 0, 32 };
    const Item noob[] = {
        xitnew(SHORTWEP, 0),
        xitnew(WAND, 0),
        xitnew(AMMO, 8),
        xitnew(AMMO, 14),
    };
    for(int i = 0; i < xlen(noob); i++)
        xitsadd(inv.items, noob[i]);
    return inv;
}

Inventory xinvselect(Inventory inv, const Input in)
{
    if(in.key[SDL_SCANCODE_1]) inv.selected = 0;
    if(in.key[SDL_SCANCODE_2]) inv.selected = 1;
    if(in.key[SDL_SCANCODE_3]) inv.selected = 2;
    return inv;
}

void xwhatis(const Inventory inv, const Input in, const int xres)
{
    const int y = in.y / inv.w;
    if(in.x > xres - inv.w)
        xitprint(inv.items.item[y]);
}

int xinvuse(const Input in)
{
    return in.key[SDL_SCANCODE_LSHIFT];
}
