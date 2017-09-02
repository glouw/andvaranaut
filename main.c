#include "Hero.h"
#include "Map.h"
#include "Sprites.h"
#include "Sdl.h"
#include "Input.h"

int main(const int argc, const char* argv[])
{
    puts("Water copyright (c) 2017 Gustav Louw, Cameron Bauer, Omar Hefnawi");
    puts("This program comes with ABSOLUTELY NO WARRANTY.");
    puts("This is free software, and you are welcome to redistribute it");
    puts("under certain conditions.");
    // Early exit if no command line arguments
    if(argc == 1)
        return 1;
    // Rock and roll
    const int res = strtol(argv[1], NULL, 0);
    const int fps = 60;
    Hero hero = spawn();
    Map map = open(hero.floor);
    Sprites sprites = wake(hero.floor);
    Sdl sdl = setup(res, fps);
    Input input = ready();
    for(int renders = 0; res == 128 ? renders < fps : !input.key[SDL_SCANCODE_F1]; renders++)
    {
        const int t0 = SDL_GetTicks();
        // Data
        if(teleporting(hero, map, input, sdl))
        {
            hero = teleport(hero, map);
            map = reopen(map, hero.floor);
            sprites = rewake(sprites, hero.floor);
        }
        hero = sustain(hero, map, input);
        sprites = caretake(sprites, hero, input, sdl, map);
        // Output
        render(sdl, hero, sprites, map);
        sdl = tick(sdl, renders);
        const int t1 = SDL_GetTicks();
        const int ms = 1000.0 / sdl.fps - (t1 - t0);
        SDL_Delay(ms < 0 ? 0 : ms);
        // Input
        input = pump(input);
    }
    release(sdl);
    close(map);
    kill(sprites);
}
