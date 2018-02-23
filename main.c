#include "Tris.h"
#include "World.h"
#include "util.h"

int main(int argc, char* argv[])
{
    const int floor = 0;
    srand(time(0));
    const Args args = xparse(argc, argv);
    Sdl s = xsetup(args);
    World w = xwinit(32);
    const Point start = w.map[floor].trapdoors.point[0];
    Hero h = xspawn(args.focal, start, floor);
    Input in = xready(args.msen);
    Overview ov = xinit();
    Flow current = xstart(-1.0f / 6.0f);
    Flow clouds = xstart(10.0f);
    Gauge g = xgnew();
    Field f = xprepare(w.map[h.floor], h.aura);
    // X-Resolution 512 reserved for performance testing.
    for(int renders = 0; args.xres == 512 ? renders < args.fps : !in.done; renders++)
    {
        const int t0 = SDL_GetTicks();
        const int ticks = renders / (args.fps / 6);
        // Edit mode.
        if(in.key[SDL_SCANCODE_TAB])
        {
            SDL_SetRelativeMouseMode(SDL_FALSE);
            ov = xupdate(ov, in, s.xres, s.textures.count);
            xmedit(w.map[h.floor], ov);
            w.sprites[h.floor] = xlay(w.sprites[h.floor], w.map[h.floor], ov);
            xview(s, ov, w.sprites[h.floor], w.map[h.floor], ticks);
        }
        // Play mode.
        else
        {
            if(xteleporting(h, w.map[h.floor], in, ticks))
            {
                h = xteleport(h, w.map[h.floor]);
                xruin(f);
                f = xprepare(w.map[h.floor], h.aura);
            }
            ov = xbackpan(ov, h.where, s.xres, s.yres);
            current = xstream(current);
            clouds = xstream(clouds);
            xrender(s, h, w.sprites[h.floor], w.map[h.floor], current, clouds, ticks);
            h = xinventory(h, in);
            if(h.inventory)
            {
                SDL_SetRelativeMouseMode(SDL_FALSE);
                // TODO: Manage inventory here.
                // TODO: Draw inventory here.
            }
            else
            {
                SDL_SetRelativeMouseMode(SDL_TRUE);
                const Attack attack = xgpower(g, in, h.wep);
                g = xgwind(g, h.wep, in);
                h = xsustain(h, w.map[h.floor], in, current);
                xdgauge(s, g);
                w.sprites[h.floor] = xcaretake(w.sprites[h.floor], h, in, w.map[h.floor], attack, f, ticks);
            }
            xdmap(s, w.map[h.floor], h.where);
        }
        xpresent(s);
        in = xpump(in);
        const int t1 = SDL_GetTicks();
        const int ms = 1000.0 / args.fps - (t1 - t0);
        SDL_Delay(ms < 0 ? 0 : ms);
    }
    xwclose(w);
    xgfree(g);
    xrelease(s);
    xruin(f);
    return 0;
}
