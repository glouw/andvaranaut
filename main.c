#include "Sdl.h"
#include "Args.h"
#include "util.h"

int main(const int argc, const char* argv[])
{
    xlicense();
    const Args args = xparse(argc, argv);
    // Data init
    Hero hero = xspawn(args.focal);
    Map map = xopen(hero.floor);
    Sprites sprites = xwake(hero.floor);
    Sdl sdl = xsetup(args.xres, args.yres, args.fps);
    Input input = xready();
    for(int renders = 0; args.xres == 512 ? renders < args.fps : !input.key[SDL_SCANCODE_F1]; renders++)
    {
        const int t0 = SDL_GetTicks();
        const int ticks = renders / (args.fps / 5);
        // Data update
        if(xteleporting(hero, map, input, ticks))
        {
            hero = xteleport(hero, map);
            map = xreopen(map, hero.floor);
            sprites = xrewake(sprites, hero.floor);
        }
        hero = xsustain(hero, map, input);
        xcaretake(sprites, hero, input, map);
        // Video output
        xrender(sdl, hero, sprites, map, ticks);
        // User input
        input = xpump(input);
        // FPS lock
        const int t1 = SDL_GetTicks();
        const int ms = 1000.0 / args.fps - (t1 - t0);
        SDL_Delay(ms < 0 ? 0 : ms);
    }
    // Cleanup
    xrelease(sdl);
    xclose(map);
    xkill(sprites);
}
