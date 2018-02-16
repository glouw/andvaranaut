#include "Tris.h"
#include "Floors.h"
#include "util.h"

int main(int argc, char* argv[])
{
    const int s0 = SDL_GetTicks();
    srand(time(0));
    const Args args = xparse(argc, argv);
    const int top = 0;
    Floors floors = xfsinit(32);
    Map map = floors.map[top];
    Hero hero = xspawn(args.focal, xpsrand(map.trapdoors), top);
    Sprites sprites = xsgen();
    Sdl sdl = xsetup(args);
    Input input = xready(args.msen);
    Overview overview = xinit();
    Flow current = xstart();
    Flow clouds = xstart();
    Gauge gauge = xgnew();
    const int s1 = SDL_GetTicks();
    printf("setup time: %d milliseconds\n", s1 - s0);
    xmprint(map.walling, map.rows, map.cols);
    // X-Resolution 512 reserved for performance testing.
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
            clouds = xstream(clouds);
            const Attack attack = xgpower(gauge, input, hero.wep);
            gauge = xgwind(gauge, hero.wep, input);
            hero = xsustain(hero, map, input, current);
            sprites = xcaretake(sprites, hero, input, map, attack, ticks);
            xrender(sdl, hero, sprites, map, current, clouds, ticks);
            xdgauge(sdl, gauge);
            xdmeters(sdl, hero, ticks);
            if(xteleporting(hero, map, input, ticks))
            {
                // TODO: Maps must be renamed to Zone.
                // Maps will include sprites.
                hero = xteleport(hero, map);
                map = floors.map[hero.floor];
                xmprint(map.walling, map.rows, map.cols);
            }
        }
        xpresent(sdl);
        input = xpump(input);
        const int t1 = SDL_GetTicks();
        const int ms = 1000.0 / args.fps - (t1 - t0);
        SDL_Delay(ms < 0 ? 0 : ms);
    }
    xfsclose(floors);
    xgfree(gauge);
    xrelease(sdl);
    xkill(sprites);
    return 0;
}
