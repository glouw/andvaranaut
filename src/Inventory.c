#include "Inventory.h"

#include "Title.h"
#include "util.h"

#include <SDL2/SDL.h>

Inventory xinvnew(void)
{
    const Inventory inv = { xitsnew(18), 0, 32, -1, false, false };
    const Item noobits[] = {
        xitnew(SHORTWEP, 0),
        xitnew(WAND, 0),
        xitnew(AMMO, 8),
        xitnew(AMMO, 14),
        xitnew(SCROLL, 3),
        xitnew(SCROLL, 1),
        xitnew(SCROLL, 9),
        xitnew(SCROLL, 12),
    };
    for(int i = 0; i < xlen(noobits); i++)
        xitsadd(inv.items, noobits[i]);
    return inv;
}

static int inside(const Inventory inv, const Input in, const int xres)
{
    return in.x > xres - inv.width;
}

Inventory xinvselect(Inventory inv, const Input in)
{
    if(in.key[SDL_SCANCODE_1]) inv.selected = 0;
    if(in.key[SDL_SCANCODE_2]) inv.selected = 1;
    if(in.key[SDL_SCANCODE_3]) inv.selected = 2;
    if(in.key[SDL_SCANCODE_4]) inv.selected = 3;
    return inv;
}

Inventory xinvhilite(Inventory inv, const Input in, const int xres)
{
    inv.hilited = !xinvuse(in) ? -1 : inside(inv, in, xres) ? in.y / inv.width : -1;
    return inv;
}

static Inventory tilechange(Inventory inv)
{
    inv.change = false;
    if(inv.hilited != -1 && inv.hilited != inv.last)
        inv.change = true;
    inv.last = inv.hilited;
    return inv;
}

Inventory xwhatis(Inventory inv, const Scroll sc, const Timer tm)
{
    inv = tilechange(inv);
    if(inv.change && inv.hilited < inv.items.max)
    {
        const int a = tm.renders;
        const int b = tm.renders + 90;
        const Item it = inv.items.item[inv.hilited];

        if(it.c == SCROLL)
        {
            char* const squares = xsstr(sc, it.index);
            xttset(a, b, true,
                "%s\n"
                "%s\n"
                "%s\n"
                "%s\n",
                it.cstr, it.desc, it.name, squares);
            free(squares);
        }
        else
            xttset(a, b, true,
                "%s\n"
                "%s\n"
                "%s\n"
                "Damage: %0.1f\n",
                it.cstr, it.desc, it.name, (double) it.damage);
    }
    return inv;
}
