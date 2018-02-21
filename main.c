#include "Tris.h"
#include "World.h"
#include "util.h"

int main(int argc, char* argv[])
{
    const int floor = 0;
    const int floors = 32;
    srand(time(0));
    const Args args = xparse(argc, argv);
    World world = xwinit(floors);
    Hero hero = xspawn(args.focal, world.map[floor].trapdoors.point[0], floor);
    Sdl sdl = xsetup(args);
    Input input = xready(args.msen);
    Overview overview = xinit();
    Flow current = xstart(-1.0f / 6.0f);
    Flow clouds = xstart(10.0f);
    Gauge gauge = xgnew();
    Field field = xprepare(world.map[hero.floor], hero.aura);
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
            xmedit(world.map[hero.floor], overview);
            world.sprites[hero.floor] = xlay(world.sprites[hero.floor], world.map[hero.floor], overview);
            xview(sdl, overview, world.sprites[hero.floor], world.map[hero.floor], ticks);
        }
        else
        {
            // Play mode.
            SDL_SetRelativeMouseMode(SDL_TRUE);
            overview = xbackpan(overview, hero.where, sdl.xres, sdl.yres);
            current = xstream(current);
            clouds = xstream(clouds);
            const Attack attack = xgpower(gauge, input, hero.wep);
            gauge = xgwind(gauge, hero.wep, input);
            hero = xsustain(hero, world.map[hero.floor], input, current);
            world.sprites[hero.floor] = xcaretake(world.sprites[hero.floor], hero, input, world.map[hero.floor], attack, field, ticks);
            xrender(sdl, hero, world.sprites[hero.floor], world.map[hero.floor], current, clouds, ticks);
            xdgauge(sdl, gauge);
            xdmap(sdl, world.map[hero.floor], hero.where);
            if(xteleporting(hero, world.map[hero.floor], input, ticks))
            {
                hero = xteleport(hero, world.map[hero.floor]);
                xruin(field);
                field = xprepare(world.map[hero.floor], hero.aura);
            }
        }
        xpresent(sdl);
        input = xpump(input);
        const int t1 = SDL_GetTicks();
        printf("%f\n", 1000.0 / (t1 - t0));
        const int ms = 1000.0 / args.fps - (t1 - t0);
        SDL_Delay(ms < 0 ? 0 : ms);
    }
    xwclose(world);
    xgfree(gauge);
    xrelease(sdl);
    xruin(field);
    return 0;
}
