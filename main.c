#include "Sdl.h"
#include "util.h"

int main(const int argc, const char* argv[])
{
    srand(time(0));
    xlicense();
    const Args args = xparse(argc, argv);
    // Data init.
    Hero hero = xspawn(args.focal);
    Map map = xopen(hero.floor);
    Sprites sprites = xwake(hero.floor);
    Sdl sdl = xsetup(args);
    Input input = xready();
    Overview overview = xinit(sdl.xres, sdl.yres);
    // A level editor ships with the engine. The level editor will swap the player's renderering
    // view with an overhead view of the sprites and the map. The user can lay down floor, wall,
    // and ceiling tiles while moving and adding new sprites.
    bool editing = false;
    // Game loop.
    for(int renders = 0; args.xres == 512 ? renders < args.fps : !input.key[SDL_SCANCODE_F1]; renders++)
    {
        const int t0 = SDL_GetTicks();
        const int ticks = renders / (args.fps / 5);
        // Edit mode.
        if(input.key[SDL_SCANCODE_F2]) editing = true;
        if(input.key[SDL_SCANCODE_F3]) editing = false;
        if(editing)
        {
            // The mouse cursor must shown when editing.
            SDL_SetRelativeMouseMode(SDL_FALSE);
            overview = xupdate(overview, input, sdl.xres, sdl.textures.count);
            xview(sdl, overview, map, ticks);
            xedit(map, overview);
        }
        // Play mode.
        else
        {
            // The mouse cursor must be removed when playing.
            SDL_SetRelativeMouseMode(SDL_TRUE);
            // Data update.
            if(xteleporting(hero, map, input, ticks))
            {
                hero = xteleport(hero, map);
                map = xreopen(map, hero.floor);
                sprites = xrewake(sprites, hero.floor);
            }
            hero = xsustain(hero, map, input);
            xcaretake(sprites, hero, input, map);
            // Video output.
            xrender(sdl, hero, sprites, map, ticks);
        }
        // Update the screen with the final rendered frame.
        xpresent(sdl);
        // User input.
        input = xpump(input);
        // FPS lock.
        // The renderer will most likely be using VSYNC.
        // If the renderer is not using VSYNC, this software time delay will create a soft VSYNC effect.
        const int t1 = SDL_GetTicks();
        const int ms = 1000.0 / args.fps - (t1 - t0);
        SDL_Delay(ms < 0 ? 0 : ms);
    }
    // Cleanup.
    xrelease(sdl);
    xclose(map);
    xkill(sprites);
}
