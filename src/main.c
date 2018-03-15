#include "Tris.h"
#include "World.h"
#include "Inventory.h"
#include "util.h"

int main(int argc, char* argv[])
{
    // The only random seeder in the game.
    // Keep seed at zero to keep the same map for testing.
    srand(true ? 0 : time(0));
    const int floor = 0;
    const Args args = xparse(argc, argv);
    World wd = xwinit(32);
    const Point start = wd.map[floor].trapdoors.point[0];
    Hero me = xspawn(args.focal, start, floor);
    Input in = xready(args.msen);
    Overview ov = xinit();
    Flow current = xstart(-1.0f / 6.0f);
    Flow clouds = xstart(10.0f);
    Gauge gg = xgnew();
    Field fd = xprepare(wd.map[me.floor], me.aura);
    Inventory inv = xinvnew();
    Sdl sdl = xsetup(args);
    // X-Resolution 512 reserved for performance testing.
    for(int renders = 0; args.xres == 512 ? renders < args.fps : !in.done; renders++)
    {
        const int t0 = SDL_GetTicks();
        const int ticks = renders / (args.fps / 6);
        // Edit mode.
        // Maybe remove one day? Not really needed with random map generation.
        if(in.key[SDL_SCANCODE_TAB])
        {
            SDL_SetRelativeMouseMode(SDL_FALSE);
            ov = xupdate(ov, in, sdl.xres, sdl.textures.count);
            xmedit(wd.map[me.floor], ov);
            wd.sprites[me.floor] = xlay(wd.sprites[me.floor], wd.map[me.floor], ov);
            xview(sdl, ov, wd.sprites[me.floor], wd.map[me.floor], ticks);
        }
        // Play mode.
        else
        {
            if(xteleporting(me, wd.map[me.floor], in, ticks))
            {
                me = xteleport(me, wd.map[me.floor]);
                xruin(fd);
                fd = xprepare(wd.map[me.floor], me.aura);
            }
            ov = xbackpan(ov, me.where, sdl.xres, sdl.yres);
            current = xstream(current);
            clouds = xstream(clouds);
            me = xcaretake(wd.sprites[me.floor], me, wd.map[me.floor], fd, ticks);
            inv = xinvselect(inv, in);
            xrender(sdl, me, wd.sprites[me.floor], wd.map[me.floor], current, clouds, ticks);
            xdinv(sdl, inv);
            xdbars(sdl, me, ticks);
            if(xinvuse(in))
            {
                SDL_SetRelativeMouseMode(SDL_FALSE);
                xwhatis(inv, in, sdl.xres);
                // TODO: Manage inventory here.
                // TODO: Draw inventory here.
            }
            else
            {
                const Classification cl = inv.items.item[inv.selected].c;
                SDL_SetRelativeMouseMode(SDL_TRUE);
                const Attack attack = xgpower(gg, in, cl);
                gg = xgwind(gg, cl, in);
                me = xsustain(me, wd.map[me.floor], in, current);
                xdgauge(sdl, gg);
                wd.sprites[me.floor] = xhurt(wd.sprites[me.floor], attack, me, in, inv, sdl.surfaces, ticks);
            }
            xdmap(sdl, wd.map[me.floor], me.where);
        }
        xpresent(sdl);
        in = xpump(in);
        const int t1 = SDL_GetTicks();
        const int ms = 1000.0f / args.fps - (t1 - t0);
        SDL_Delay(ms < 0 ? 0 : ms);
    }
    xwclose(wd);
    xgfree(gg);
    xrelease(sdl);
    xruin(fd);
    return 0;
}
