#include "Sdl.h"
#include "util.h"

int main(const int argc, const char* argv[])
{
    srand(time(0));
    const Args args = xparse(argc, argv);
    // Data init.
    Hero hero = xspawn(args.focal);
    Map map = xopen(hero.floor);
    Sprites sprites = xwake(hero.floor);
    Sdl sdl = xsetup(args);
    Input input = xready(args.msen);
    Overview overview = xinit();
    Current current = xstart();
    Gauge gauge = xgnew(args.gfac);
    // Game loop.
    for(int renders = 0; args.xres == 512 ? renders < args.fps : !input.key[SDL_SCANCODE_END]; renders++)
    {
        const int t0 = SDL_GetTicks();
        const int ticks = renders / (args.fps / 6);
        // Edit mode. User holds in the left shift button.
        if(input.key[SDL_SCANCODE_LSHIFT])
        {
            // The mouse cursor is shown when editing.
            SDL_SetRelativeMouseMode(SDL_FALSE);
            // Map and sprite editing.
            overview = xupdate(overview, input, sdl.xres, sdl.textures.count);
            xedit(map, overview);
            sprites = xlay(sprites, map, overview);
            // Saving map and sprites.
            if(input.key[SDL_SCANCODE_F5])
            {
                xmsave(map, hero.floor, ticks);
                xssave(sprites, hero.floor, ticks);
            }
            // Render.
            xview(sdl, overview, sprites, map, ticks);
        }
        // Play mode.
        else
        {
            // The mouse cursor is removed when playing.
            SDL_SetRelativeMouseMode(SDL_TRUE);
            // Data update.
            current = xstream(current);
            const Attack attack = xgpower(gauge, input); // return Attack type
            if(attack.power > 0.0)
                printf("%f %f %f\n", attack.power, attack.dir.x, attack.dir.y);
            gauge = xgwind(gauge, hero.wep, input);
            if(xteleporting(hero, map, input, ticks))
            {
                hero = xteleport(hero, map);
                map = xreopen(map, hero.floor);
                sprites = xrewake(sprites, hero.floor);
            }
            hero = xsustain(hero, map, input, current);
            xcaretake(sprites, hero, input, map, attack, ticks);
            // Render.
            xrender(sdl, hero, sprites, map, current, ticks);
            xdgauge(sdl, gauge);
            xdmeters(sdl, hero, ticks);
        }
        // Update the screen with the final rendered frame.
        xpresent(sdl);
        // User input.
        input = xpump(input);
        // FPS lock; normally handled with VSYNC.
        // This software time delay will create a soft delay en-lieu of VSYNC if VSYNC is not present.
        const int t1 = SDL_GetTicks();
        const int ms = 1000.0 / args.fps - (t1 - t0);
        SDL_Delay(ms < 0 ? 0 : ms);
    }
    // Cleanup.
    xgfree(gauge);
    xrelease(sdl);
    xclose(map);
    xkill(sprites);
}
