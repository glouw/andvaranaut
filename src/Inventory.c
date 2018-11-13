#include "Inventory.h"

#include "Title.h"
#include "util.h"

#include <SDL2/SDL.h>

Inventory i_create(void)
{
    const Inventory inv = { i_build(16), 0, 32, -1, false, false };
    const Item noobits[] = {
        i_new(SHORTWEP, 0),
        i_new(WAND, 0),
        i_new(AMMO, 8),
        i_new(AMMO, 14),
        i_new(SCROLL, 3),
        i_new(SCROLL, 1),
        i_new(SCROLL, 9),
        i_new(SCROLL, 12),
        i_new(LETTER, 0),
    };
    for(int i = 0; i < u_len(noobits); i++)
        i_add(inv.items, noobits[i]);
    return inv;
}

static int inside(const Inventory inv, const Input in, const int xres)
{
    return in.x > xres - inv.width;
}

Inventory i_select(Inventory inv, const Input in)
{
    if(in.key[SDL_SCANCODE_1]) inv.selected = 0;
    if(in.key[SDL_SCANCODE_2]) inv.selected = 1;
    if(in.key[SDL_SCANCODE_3]) inv.selected = 2;
    if(in.key[SDL_SCANCODE_4]) inv.selected = 3;
    return inv;
}

Inventory i_highlite(Inventory inv, const Input in, const int xres)
{
    inv.hilited = inside(inv, in, xres) ? in.y / inv.width : -1;
    return inv;
}

static Inventory check_tile_change(Inventory inv)
{
    inv.change = false;
    if(inv.hilited != -1 && inv.hilited != inv.last)
        inv.change = true;
    inv.last = inv.hilited;
    return inv;
}

Inventory i_what_is(Inventory inv, const Scroll sc, const Timer tm)
{
    inv = check_tile_change(inv);
    if(inv.change && inv.hilited < inv.items.max)
    {
        const int a = tm.renders;
        const int b = tm.renders + 90;
        const Item it = inv.items.item[inv.hilited];

        if(it.c == SCROLL)
        {
            char* const squares = s_scroll_to_str(sc, it.index);
            t_set_title(a, b, true,
                "%s\n"
                "%s\n"
                "%s\n"
                "%s\n",
                it.cstr, it.desc, it.name, squares);
            free(squares);
        }
        else
        if(it.c == LETTER)
            t_set_title(a, b, true,
                "%s\n"
                "%s\n",
                it.name, it.desc);
        else
            t_set_title(a, b, true,
                "%s\n"
                "%s\n"
                "%s\n"
                "Damage: %0.1f\n",
                it.cstr, it.desc, it.name, (double) it.damage);
    }
    return inv;
}
