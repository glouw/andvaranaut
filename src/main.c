#include "World.h"
#include "Title.h"
#include "util.h"

int main(int argc, char* argv[])
{
#if 1
    // Unit tests.
    i_test();

    xstest();
#endif

    srand(true ? time(NULL) : 0);

    const Args args = a_parse(argc, argv);

    World wd = xwinit(32);

    const int floor = 0;

    const Point start = wd.map[floor].trapdoors.point[0];

    Gauge gg = g_new();

    Hero me = h_born(args.focal, start, floor, gg);

    Overview ov = o_init();

    Flow current = f_start(-1.0f / 6.0f);

    Flow clouds = f_start(9.0f);

    Field fd = f_prepare(wd.map[me.floor], me.aura);

    Inventory inv = i_create();

    Scroll sc = xscnew();

    Input in = i_ready(args.msen);

    Theme theme = NO_THEME;

    Timer tm = xtmnew();

    Sdl sdl = xsetup(args);

    const Text text = xtxnew("art/gui/SDS_8x8.ttf", 24, sdl.yel, sdl.blk);

    xttinit();

    // Game loop. X-Resolution 512 reserved for performance testing.
    for(int renders = 0, fps = 0; args.xres == 512 ? renders < 20 : !in.done; renders++)
    {
        const int t0 = SDL_GetTicks();

        const int ticks = renders / 10;

        tm = xtmtick(tm, renders, ticks);

        xttadvance(renders);

        theme = m_theme(theme, wd.map[me.floor], me.where, tm);

        // World edit mode.
        if(in.key[SDL_SCANCODE_TAB])
        {
            SDL_SetRelativeMouseMode(SDL_FALSE);

            ov = o_update(ov, in, sdl.xres);

            m_edit(wd.map[me.floor], ov);

            wd.sprites[me.floor] = xlay(wd.sprites[me.floor], wd.map[me.floor], ov);

            xview(sdl, ov, wd.sprites[me.floor], wd.map[me.floor], tm);
        }
        // Play Mode.
        else
        {
            me = h_teleporting(me, wd.map[me.floor], in, tm);
            if(me.teleporting)
            {
                me = h_teleport(me, wd.map[me.floor]);

                f_ruin(fd);
                fd = f_prepare(wd.map[me.floor], me.aura);
            }
            ov = o_backpan(ov, me.where, sdl.xres, sdl.yres);

            current = f_stream(current);

            clouds = f_stream(clouds);

            me = xcaretake(wd.sprites[me.floor], me, wd.map[me.floor], fd, gg, tm);

            inv = i_select(inv, in);

            inv = i_highlite(inv, in, sdl.xres);

            xrender(sdl, text, me, wd.sprites[me.floor], wd.map[me.floor], current, clouds, tm);

            xdinv(sdl, inv);

            xdbars(sdl, me, tm);

            xdmap(sdl, wd.map[me.floor], me.where);

            wd.map[me.floor] = xscount(wd.sprites[me.floor], wd.map[me.floor]);

            m_barricade(wd.map[me.floor]);

            // Inventory management.
            if(i_invuse(in))
            {
                SDL_SetRelativeMouseMode(SDL_FALSE);

                inv = i_whatis(inv, sc, tm);
            }
            // World Interaction.
            else
            {
                SDL_SetRelativeMouseMode(SDL_TRUE);

                xttstuckclear();

                const Attack atk = xdgauge(sdl, gg, inv, sc);

                // TODO: Maybe pass in item wind rate.
                gg = g_wind(gg, in, tm);

                me = h_sustain(me, wd.map[me.floor], in, current, wd.sprites[me.floor].last);

                wd.sprites[me.floor] = xhurt(wd.sprites[me.floor], atk, me, in, inv, tm);
            }
        }
        xttshow(text, sdl);

        xdfps(sdl, text, "%d", fps);

        xpresent(sdl);

        in = i_pump(in);

        // Caps framerate in software.
        const int t1 = SDL_GetTicks();
        const int ms = 1000.0f / args.fps - (t1 - t0);
        SDL_Delay(ms < 0 ? 0 : ms);

        const int t2 = SDL_GetTicks();
        if(tm.rise)
            fps = 1000.0f / (t2 - t0);
    }

    // No need to free anything - the OS will do it with a quicker exit.
    return 0;
}
