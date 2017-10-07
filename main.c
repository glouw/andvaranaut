#include "Sdl.h"
#include "Args.h"
#include "util.h"

int main(const int argc, const char* argv[])
{
    license();
    const Args args = parse(argc, argv);
    // Data init
    Hero hero = spawn(args.scale);
    Map map = xopen(hero.floor);
    Sprites sprites = wake(hero.floor);
    Sdl sdl = setup(args.xres, args.yres, args.fps);
    Input input = ready();
    for(int renders = 0; args.xres == 512 ? renders < args.fps : !input.key[SDL_SCANCODE_F1]; renders++)
    {
        const int t0 = SDL_GetTicks();
        const int ticks = renders / (args.fps / 5);
        // Data update
        if(teleporting(hero, map, input, ticks))
        {
            hero = teleport(hero, map);
            map = xreopen(map, hero.floor);
            sprites = rewake(sprites, hero.floor);
        }
        hero = sustain(hero, map, input);
        caretake(sprites, hero, input, map);
        // Video output
        render(sdl, hero, sprites, map, ticks);
        // User input
        input = pump(input);
        // FPS lock
        const int t1 = SDL_GetTicks();
        const int ms = 1000.0 / args.fps - (t1 - t0);
        SDL_Delay(ms < 0 ? 0 : ms);
    }
    // Cleanup
    release(sdl);
    xclose(map);
    kill(sprites);
}
