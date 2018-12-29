#include "Inventory.h"

#include "Title.h"
#include "util.h"

#include <SDL2/SDL.h>

Inventory i_create(void)
{
    const Inventory inv = { i_build(10), 0, 32, -1, false, false, -1, i_none() };
    const Item noobits[] = {
        i_new(SHORTWEP, 0),
        i_new(WAND, 0),
        i_new(AMMO, 8),
        i_new(SHIELD, 0),
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

static int calc_index(const Inventory inv, const Input in)
{
    return in.y / inv.width;
}

static int inside(const Inventory inv, const Input in, const int xres)
{
    return in.x > xres - inv.width && calc_index(inv, in) < inv.items.max;
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
    inv.hilited = inside(inv, in, xres) ? calc_index(inv, in) : -1;
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
        const Item it = inv.items.item[inv.hilited];

        if(it.clas == SCROLL)
        {
            char* const squares = s_scroll_to_str(sc, it.index);
            t_set_title(tm.renders, 90, true,
                "%s\n"
                "%s\n"
                "%s\n"
                "%s\n",
                it.cstr, it.desc, it.name, squares);
            free(squares);
        }
        else
        if(it.clas == LETTER)
            t_set_title(tm.renders, 90, true,
                "%s\n"
                "%s\n",
                it.name, it.desc);
        else
            t_set_title(tm.renders, 90, true,
                "%s\n"
                "%s\n"
                "%s\n"
                "Damage: %0.1f\n",
                it.cstr, it.desc, it.name, (double) it.damage);
    }
    return inv;
}

Item i_get_equipped(const Inventory inv)
{
    return inv.items.item[inv.selected];
}

static Inventory drag(Inventory inv, const Input in, const int xres)
{
    if(in.ld)
        if(inside(inv, in, xres))
            inv.drag = calc_index(inv, in);

    return inv;
}

static Inventory swap(Inventory inv, const Input in, const int xres)
{
    if(in.lu)
    {
        if(inv.drag != -1)
        {
            if(inside(inv, in, xres))
            {
                const int to = calc_index(inv, in);
                const Item temp = inv.items.item[to];
                inv.items.item[to] = inv.items.item[inv.drag];
                inv.items.item[inv.drag] = temp;
            }
            else
            {
                inv.trade = inv.items.item[inv.drag];
                inv.items.item[inv.drag] = i_none();
            }
        }
        inv.drag = -1;
    }
    return inv;
}

Inventory i_manage(Inventory inv, const Input in, const int xres)
{
    inv = drag(inv, in, xres);
    inv = swap(inv, in, xres);

    return inv;
}
