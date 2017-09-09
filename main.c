#include "Sprites.h"
#include "Sdl.h"
#include <stdio.h>
#include <stdlib.h>

int main(const int argc, const char* argv[])
{
    puts("Water copyright (c) 2017 Gustav Louw, Cameron Bauer");
    puts("This program comes with ABSOLUTELY NO WARRANTY.");
    puts("This is free software, and you are welcome to redistribute it");
    puts("under certain conditions.");
    const int res = argc == 2 ? strtol(argv[1], NULL, 0) : 512;
    const int fps = 60;
    // Data init
    Hero hero = spawn();
    Map map = open(hero.floor);
    Sprites sprites = wake(hero.floor);
    Sdl sdl = setup(res, fps);
    Input input = ready();
    for(int renders = 0; res == 128 ? renders < fps : !input.key[SDL_SCANCODE_F1]; renders++)
    {
        const int t0 = SDL_GetTicks();
        const int ticks = renders / (fps / 5);
        // Data update
        if(teleporting(hero, map, input, ticks))
        {
            hero = teleport(hero, map);
            map = reopen(map, hero.floor);
            sprites = rewake(sprites, hero.floor);
        }
        hero = sustain(hero, map, input);
        sprites = caretake(sprites, hero, input, map, ticks);
        // Video output
        render(sdl, hero, sprites, map, ticks);
        // User input
        input = pump(input);
        // FPS lock
        const int t1 = SDL_GetTicks();
        const int ms = 1000.0 / fps - (t1 - t0);
        SDL_Delay(ms < 0 ? 0 : ms);
    }
    release(sdl);
    close(map);
    kill(sprites);
}
