#include "World.h"
#include "Title.h"
#include "util.h"

int main(int argc, char* argv[])
{
#if 1 // Unit tests.
    xittest();

    xstest();
#endif

    srand(true ? time(NULL) : 0);

    const Args args = xparse(argc, argv);

    World wd = xwinit(32);

    const int floor = 0;

    const Point start = wd.map[floor].trapdoors.point[0];

    Hero me = xspawn(args.focal, start, floor);

    Overview ov = xinit();

    Flow current = xstart(-1.0f / 6.0f);

    Flow clouds = xstart(9.0f);

    Gauge gg = xgnew();

    Field fd = xprepare(wd.map[me.floor], me.aura);

    Inventory inv = xinvnew();

    Scroll sc = xscnew();

    Input in = xready(args.msen);

    Theme theme = NO_THEME;

    Timer tm = xtmnew();

    Sdl sdl = xsetup(args);

    const Text text = xtxnew("art/gui/SDS_8x8.ttf", 24, sdl.red, sdl.blk);

    xttinit();

    // Game loop. X-Resolution 512 reserved for performance testing.
    for(int renders = 0, fps = 0; args.xres == 512 ? renders < 20 : !in.done; renders++)
    {
        const int t0 = SDL_GetTicks();

        const int ticks = renders / 10;

        tm = xtmtick(tm, renders, ticks);

        xttadvance(renders);

        theme = xmthemett(theme, wd.map[me.floor], me.where, tm);

        // World edit mode.
        if(in.key[SDL_SCANCODE_TAB])
        {
            SDL_SetRelativeMouseMode(SDL_FALSE);

            ov = xupdate(ov, in, sdl.xres);

            xmedit(wd.map[me.floor], ov);

            wd.sprites[me.floor] = xlay(wd.sprites[me.floor], wd.map[me.floor], ov);

            xview(sdl, ov, wd.sprites[me.floor], wd.map[me.floor], tm);
        }
        // Play Mode.
        else
        {
            me = xteleporting(me, wd.map[me.floor], in, tm);
            if(me.teleporting)
            {
                me = xteleport(me, wd.map[me.floor]);

                xruin(fd);
                fd = xprepare(wd.map[me.floor], me.aura);
            }
            ov = xbackpan(ov, me.where, sdl.xres, sdl.yres);

            current = xstream(current);

            clouds = xstream(clouds);

            me = xcaretake(wd.sprites[me.floor], me, wd.map[me.floor], fd, tm);

            inv = xinvselect(inv, in);

            inv = xinvhilite(inv, in, sdl.xres);

            xrender(sdl, text, me, wd.sprites[me.floor], wd.map[me.floor], current, clouds, tm);

            xdinv(sdl, inv);

            xdbars(sdl, me, tm);

            xdmap(sdl, wd.map[me.floor], me.where);

            wd.map[me.floor] = xscount(wd.sprites[me.floor], wd.map[me.floor]);

            xmbarricade(wd.map[me.floor]);

            // Inventory management.
            if(xinvuse(in))
            {
                SDL_SetRelativeMouseMode(SDL_FALSE);

                inv = xwhatis(inv, sc, tm);
            }
            // World Interaction.
            else
            {
                SDL_SetRelativeMouseMode(SDL_TRUE);

                xttstuckclear();

                const Attack atk = xdgauge(sdl, gg, inv, sc);

                // TODO: Maybe pass in item wind rate.
                gg = xgwind(gg, in);

                me = xsustain(me, wd.map[me.floor], in, current, wd.sprites[me.floor].last);

                wd.sprites[me.floor] = xhurt(wd.sprites[me.floor], atk, me, in, inv, tm);
            }
        }
        xttshow(text, sdl);

        xdfps(sdl, text, "%d", fps);

        xpresent(sdl);

        in = xpump(in);

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
