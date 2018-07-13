#include "Inventory.h"

#include "Title.h"
#include "util.h"

#include <SDL2/SDL.h>

Inventory xzinv()
{
    static Inventory inv;
    return inv;
}

Inventory xinvnew()
{
    const Inventory inv = { xitsnew(12), 0, 32, -1 };

    const Item noob[] = {
        xitnew(SHORTWEP, 0),
        xitnew(WAND, 0),
        xitnew(AMMO, 8),
        xitnew(AMMO, 14),
        xitnew(SCROLL, 3),
        xitnew(SCROLL, 1),
        xitnew(SCROLL, 9),
        xitnew(SCROLL, 12),
    };

    for(int i = 0; i < xlen(noob); i++)
        xitsadd(inv.items, noob[i]);

    return inv;
}

static int inside(const Inventory inv, const Input in, const int xres)
{
    return in.x > xres - inv.width;
}

Inventory xinvselect(Inventory inv, const Input in)
{
    // There are only four fingers (minus the thumb) on your left hand,
    // which makes inventory selection at best the first four items.
    if(in.key[SDL_SCANCODE_1]) inv.selected = 0;
    if(in.key[SDL_SCANCODE_2]) inv.selected = 1;
    if(in.key[SDL_SCANCODE_3]) inv.selected = 2;
    if(in.key[SDL_SCANCODE_4]) inv.selected = 3;
    return inv;
}

Inventory xinvhilite(Inventory inv, const Input in, const int xres)
{
    // The -1 will signal outside of inventory.
    inv.hilited = !xinvuse(in) ? -1 : inside(inv, in, xres) ? in.y / inv.width : -1;
    return inv;
}

static int tilechange(const Inventory inv)
{
    // Last hilited value.
    static int last;

    // A tile change occurs when the last hilited value changed.
    int change = false;
    if(inv.hilited != -1 && inv.hilited != last)
        change = true;

    // Update last hilite value.
    last = inv.hilited;

    return change;
}

void xwhatis(const Inventory inv, const Scroll sc, const Timer tm)
{
    if(tilechange(inv) && inv.hilited < inv.items.max)
    {
        const int a = tm.renders;
        const int b = tm.renders + 90;
        const Item it = inv.items.item[inv.hilited];

        // Scroll info (including sign).
        if(it.c == SCROLL)
        {
            char* const squares = xsstr(sc, it.index);
            xttset(a, b, true,
                "%s\n" // C-string
                "%s\n" // Description.
                "%s\n" // Name.
                "%s\n" // Squares
                ,
                it.cstr,
                it.desc,
                it.name,
                squares);
            free(squares);
        }
        // General item info.
        else
            xttset(a, b, true,
                "%s\n" // C-string
                "%s\n" // Description
                "%s\n" // Name.
                "Damage: %0.1f\n"
                ,
                it.cstr,
                it.desc,
                it.name,
                (double) it.damage);
    }
}
