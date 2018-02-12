#include "Tris.h"
#include "util.h"

int main(int argc, char* argv[])
{
    srand(time(0));
    const Args args = xparse(argc, argv);
    Map map = xtgenerate();
    Hero hero = xspawn(args.focal, xpsrand(map.trapdoors));
    Sprites sprites = xsgen();
    Sdl sdl = xsetup(args);
    Input input = xready(args.msen);
    Overview overview = xinit();
    Current current = xstart();
    Gauge gauge = xgnew();
    for(int renders = 0; args.xres == 512 ? renders < args.fps : !input.done; renders++)
    {
        const int t0 = SDL_GetTicks();
        const int ticks = renders / (args.fps / 6);
        if(input.key[SDL_SCANCODE_LSHIFT])
        {
            // Edit mode.
            SDL_SetRelativeMouseMode(SDL_FALSE);
            overview = xupdate(overview, input, sdl.xres, sdl.textures.count);
            xedit(map, overview);
            sprites = xlay(sprites, map, overview);
            xview(sdl, overview, sprites, map, ticks);
        }
        else
        {
            // Play mode.
            SDL_SetRelativeMouseMode(SDL_TRUE);
            current = xstream(current);
            const Attack attack = xgpower(gauge, input, hero.wep);
            gauge = xgwind(gauge, hero.wep, input);
            if(xteleporting(hero, map, input, ticks))
            {
                printf("teleporting\n");
            }
            hero = xsustain(hero, map, input, current);
            sprites = xcaretake(sprites, hero, input, map, attack, ticks);
            xrender(sdl, hero, sprites, map, current, ticks);
            xdgauge(sdl, gauge);
            xdmeters(sdl, hero, ticks);
        }
        xpresent(sdl);
        input = xpump(input);
        const int t1 = SDL_GetTicks();
        const int ms = 1000.0 / args.fps - (t1 - t0);
        SDL_Delay(ms < 0 ? 0 : ms);
    }
    xgfree(gauge);
    xrelease(sdl);
    xclose(map);
    xkill(sprites);
    return 0;
}
