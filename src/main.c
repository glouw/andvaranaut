#include "Tris.h"
#include "World.h"
#include "Inventory.h"
#include "Scroll.h"
#include "Font.h"
#include "Title.h"
#include "Timer.h"
#include "util.h"

int main(int argc, char* argv[])
{
    // The one and only random seeder. Keep seed constant to keep the same map for testing.
    srand(false ? 128 : time(0));

    // Parses command line arguments. Uses game defaults if no arguments are passed in.
    const Args args = xparse(argc, argv);

    // Initializes the world with however many floors specified.
    World wd = xwinit(32);

    // The floor the hero starts on.
    const int floor = 0;

    // Spawns the hero.
    const Point start = wd.map[floor].trapdoors.point[0];
    Hero me = xspawn(args.focal, start, floor);

    // Prepares the overview for the map editor.
    Overview ov = xinit();

    // Prepares the cloud and water currents. Their heights are specified as arguments.
    Flow current = xstart(-1.0f / 6.0f);
    Flow clouds = xstart(10.0f);

    // Prepares the attack gauge.
    Gauge gg = xgnew();

    // Prepares the pathfinder.
    Field fd = xprepare(wd.map[me.floor], me.aura);

    // Prepare the inventory.
    Inventory inv = xinvnew();

    // Prepare the magic scrolls.
    Scroll sc = xscnew();

    // Prepares game input (keyboard, mouse).
    Input in = xready(args.msen);

    // Prepares game output (sound, audio, video).
    Sdl sdl = xsetup(args);

    // Prepares general display font.
    Font fill = xfbuild("art/gui/SDS_8x8.ttf", 32, sdl.red, 0);
    Font line = xfbuild("art/gui/SDS_8x8.ttf", 32, sdl.blk, 1);

    // Titles present new areas to the player. The title type is the only heap type as it used deep within the engine.
    xttinit(0, 180, "Andvaranaut\nPre-Alpha");

    // Game loop. X-Resolution 512 reserved for performance testing. Exits with certain keypress or 'X' window button.
    for(int renders = 0; args.xres == 512 ? renders < args.fps : !in.done; renders++)
    {
        const int t0 = SDL_GetTicks();
        const int ticks = renders / (args.fps / 6);

        const Timer tm = xtmnew(renders, ticks);

        // Advance the title.
        xttadvance(renders);

        /* Edit Mode */
        if(in.key[SDL_SCANCODE_TAB])
        {
            // Brings up the ouse cursor.
            SDL_SetRelativeMouseMode(SDL_FALSE);

            // Pans the overview.
            ov = xupdate(ov, in, sdl.xres);

            // Edit walls, celiings, and floors.
            xmedit(wd.map[me.floor], ov);

            // Lays down new sprites.
            wd.sprites[me.floor] = xlay(wd.sprites[me.floor], wd.map[me.floor], ov);

            // Draws the overview to the screen backbuffer.
            xview(sdl, ov, wd.sprites[me.floor], wd.map[me.floor], tm);
        }
        /* Play Mode */
        else
        {
            // Hero teleportation (up and down floors via trapdoors).
            if(xteleporting(me, wd.map[me.floor], in, tm))
            {
                me = xteleport(me, wd.map[me.floor]);

                // Old pathfinder freed and a new pathfinder for the new floor is prepared.
                xruin(fd);
                fd = xprepare(wd.map[me.floor], me.aura);

                // Set buffer title buffer.
                xttset(renders, renders + 120, "Floor %d", me.floor);
            }
            // Overview backpanning keeps overview up to date with hero location.
            ov = xbackpan(ov, me.where, sdl.xres, sdl.yres);

            // Current and cloud movement.
            current = xstream(current);
            clouds = xstream(clouds);

            // Sprite updater for current floor. Returns an updated hero if sprite interaction occured.
            me = xcaretake(wd.sprites[me.floor], me, wd.map[me.floor], fd, tm);

            // Inventory selection.
            inv = xinvselect(inv, in);
            inv = xinvhilite(inv, in, sdl.xres);

            // Renders to screen backbuffer floors, ceiling, walls, and then sprites.
            xrender(sdl, me, wd.sprites[me.floor], wd.map[me.floor], current, clouds, tm);

            // Draws to screen backbuffer the inventory panel.
            xdinv(sdl, inv);

            // Draws to screen backbuffer health, fatigue, and mana bars.
            xdbars(sdl, me, tm);

            // Draws to screen backbuffer the game map.
            xdmap(sdl, wd.map[me.floor], me.where);

            // Inventory management.
            if(xinvuse(in))
            {
                // Brings up the mouse cursor.
                SDL_SetRelativeMouseMode(SDL_FALSE);

                // Brings up a description of whats pointed at in the inventory.
                xwhatis(inv, tm);

                // TODO: Manage inventory here.
                // TODO: Draw inventory here.
            }
            // World Interaction.
            else
            {
                // Hides the mouse cursor.
                SDL_SetRelativeMouseMode(SDL_TRUE);

                // NOTE: Attack must be calculated before gauge is wound.
                const Attack atk = xdgauge(sdl, gg, inv, sc);

                // TODO: Maybe pass in item wind rate.
                gg = xgwind(gg, in);

                // Hero self sustain.
                me = xsustain(me, wd.map[me.floor], in, current);

                // Sprite damaging. May add or remove sprites to game word.
                wd.sprites[me.floor] = xhurt(wd.sprites[me.floor], atk, me, in, inv, tm);

                // Add recoil to the hero yaw if the last attack done to sprites was ranged.
                me = xrecoil(me, wd.sprites[me.floor].last);
            }
        }

        // Present the title to the middle of the screen.
        xttshow(fill, line, sdl);

        // Presents screen backbuffer to screen.
        xpresent(sdl);

        // Updates input.
        in = xpump(in);

        // Caps framerate. Only effective if VSYNC is off.
        const int t1 = SDL_GetTicks();
        const int ms = 1000.0f / args.fps - (t1 - t0);
        SDL_Delay(ms < 0 ? 0 : ms);
    }

    // No need to free dynamic pooled memory. The OS will do for us which gives us a quick exit.
    return 0;
}
