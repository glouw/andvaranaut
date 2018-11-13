#include "World.h"
#include "Title.h"
#include "Fire.h"
#include "util.h"

int main(int argc, char* argv[])
{
    i_test();

    s_test();

    srand(time(NULL));

    const Args args = a_parse(argc, argv);

    const World wd = w_make(32);

    const int floor = 0;

    const Point start = wd.map[floor].trapdoors.point[0];

    Gauge gg = g_new();

    Hero me = h_birth(args.focal, start, floor, gg);

    Overview ov = o_init();

    Flow current = f_start(-1.0f / 6.0f);

    Flow clouds = f_start(9.0f);

    Field field = f_prepare(wd.map[me.floor], me.aura);

    Fire fire = f_kindle(wd.map[me.floor]);

    Inventory inv = i_create();

    Scroll sc = s_new();

    Input in = i_ready(args.msen);

    Theme theme = NO_THEME;

    Timer tm = t_new();

    Sdl sdl = s_setup(args);

    const Text text = t_build("art/gui/SDS_8x8.ttf", 24, sdl.yel, sdl.blk);

    t_clear_title();

    // X-Resolution 512 reserved for performance testing.
    for(int renders = 0, fps = 0; args.xres == 512 ? renders < 20 : !in.done; renders++)
    {
        const int t0 = SDL_GetTicks();

        const int ticks = renders / 10;

        tm = t_tick(tm, renders, ticks);

        t_advance_title(renders);

        theme = m_get_theme(theme, wd.map[me.floor], me.where, tm);

        if(i_using_world_edit_mode(in))
        {
            SDL_SetRelativeMouseMode(SDL_FALSE);

            s_render_overlay(sdl, ov, wd.sprites[me.floor], wd.map[me.floor], tm);

            ov = o_update(ov, in, sdl.xres);

            m_edit(wd.map[me.floor], ov);

            wd.sprites[me.floor] = s_lay(wd.sprites[me.floor], wd.map[me.floor], ov);
        }
        else
        {
            s_render_playing(sdl, text, me, wd.sprites[me.floor], wd.map[me.floor], current, clouds, inv, tm);

            me = h_teleporting(me, wd.map[me.floor], in, tm);

            if(me.teleporting)
            {
                me = h_teleport(me, wd.map[me.floor]);

                f_ruin(field);
                field = f_prepare(wd.map[me.floor], me.aura);

                f_extinguish(fire);
                fire = f_kindle(wd.map[me.floor]);
            }
            ov = o_pan(ov, me.where, sdl.xres, sdl.yres);

            current = f_stream(current);

            clouds = f_stream(clouds);

            me = s_caretake(wd.sprites[me.floor], me, wd.map[me.floor], field, gg, fire, tm);

            wd.sprites[me.floor] = s_spread_fire(wd.sprites[me.floor], fire, wd.map[me.floor], tm);

            wd.map[me.floor] = s_count_agents(wd.sprites[me.floor], wd.map[me.floor]);

            m_place_barricades(wd.map[me.floor]);

            inv = i_select(inv, in);

            if(i_using_inventory(in))
            {
                SDL_SetRelativeMouseMode(SDL_FALSE);

                inv = i_highlite(inv, in, sdl.xres);

                inv = i_what_is(inv, sc, tm);
            }
            else
            {
                SDL_SetRelativeMouseMode(SDL_TRUE);

                t_clear_title_when_linger();

                const Attack atk = s_draw_gauge(sdl, gg, inv, sc);

                // TODO: Maybe pass in item wind rate.
                gg = g_wind(gg, in, tm);

                me = h_sustain(me, wd.map[me.floor], in, current, wd.sprites[me.floor].last);

                wd.sprites[me.floor] = s_hurt(wd.sprites[me.floor], atk, me, in, inv, tm);
            }

            f_clear(fire);
        }
        t_show_title(text, sdl);

        s_draw_fps(sdl, text, "%d", fps);

        s_present(sdl);

        in = i_pump(in);

        // Caps framerate in software.
        const int t1 = SDL_GetTicks();
        const int ms = 1000.0f / args.fps - (t1 - t0);
        SDL_Delay(ms < 0 ? 0 : ms);

        const int t2 = SDL_GetTicks();
        if(tm.rise)
            fps = 1000.0f / (t2 - t0);
    }

    // No need to free anything - the OS will do it with a quicker exit.
    return 0;
}
