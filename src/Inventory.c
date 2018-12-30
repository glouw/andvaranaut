#include "Inventory.h"

#include "Title.h"
#include "util.h"

#include <SDL2/SDL.h>

Inventory i_create(void)
{
    const Inventory inv = {
        i_build(10), 0, 32, -1, false, false, -1, i_none()
    };
    const Identification ids[] = {
        /* 0 */ {  0, SHORTWEP },
        /* 1 */ {  0, WAND     },
        /* 2 */ {  8, AMMO     },
        /* 3 */ {  0, SHIELD   },
        /* 4 */ { 14, AMMO     },
        /* 5 */ {  3, SCROLL   },
        /* 6 */ {  1, SCROLL   },
        /* 7 */ {  9, SCROLL   },
        /* 8 */ {  1, LETTER   },
        /* 9 */ {  0, LETTER   },
    };
    for(int i = 0; i < u_len(ids); i++)
        i_add(inv.items, i_new(ids[i]));

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

Inventory i_hilite(Inventory inv, const Input in, const int xres)
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

static int dragging(const Inventory inv)
{
    return inv.drag != -1;
}

Inventory i_what_is(Inventory inv, const Scroll sc, const Timer tm)
{
    if(dragging(inv))
        t_clear_title_when_linger();
    else
    {
        inv = check_tile_change(inv);

        if(inv.change
        && inv.hilited < inv.items.max)
        {
            const Item it = inv.items.item[inv.hilited];

            if(it.id.clas == SCROLL)
            {
                char* const squares = s_scroll_to_str(sc, it.id.index);
                t_set_title(tm.renders, 90, true,
                    "%s\n"
                    "%s\n"
                    "%s\n"
                    "%s\n",
                    it.cstr, it.desc, it.name, squares);
                free(squares);
            }
            else
            if(it.id.clas == LETTER)
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
    }
    return inv;
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
        if(dragging(inv))
        {
            if(inside(inv, in, xres))
            {
                const int to = calc_index(inv, in);
                const Item temp = inv.items.item[to];
                inv.items.item[to] = inv.items.item[inv.drag];
                inv.items.item[inv.drag] = temp;
            }
            else // Drop.
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

Inventory i_unhilite(Inventory inv)
{
    inv.hilited = -1;

    return inv;
}
