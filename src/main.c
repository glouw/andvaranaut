#include "World.h"
#include "Title.h"
#include "Fire.h"
#include "util.h"

int main(int argc, char* argv[])
{
    srand(time(NULL));

    i_test();

    s_test();

    Timer tm = t_new();

    const Args args = a_parse(argc, argv);

    const World world = w_make(32, tm);

    const Room start = w_get_starting_room(world);

    Hero hero = h_birth(args.focal, start);

    Overview ov = o_init();

    Flow current = f_start(-1.0f / 6.0f);

    Flow clouds = f_start(9.0f);

    Field field = f_prepare(world.map[hero.floor], hero.aura);

    Fire fire = f_kindle(world.map[hero.floor]);

    Scroll scroll = s_new();

    Input in = i_ready(args.msen);

    Theme theme = NO_THEME;

    Sdl sdl = s_setup(args);

    const Text yel = t_build("art/gui/SDS_8x8.ttf", 24, sdl.yel, sdl.blk);

    const Text red = t_build("art/gui/SDS_8x8.ttf", 24, sdl.red, sdl.blk);

    t_clear_title();

    for(int renders = 0, fps = 0; args.xres == 512 ? renders < 5 : !in.done; renders++)
    {
        const int t0 = SDL_GetTicks();

        tm = t_tick(tm, g_slowmo(hero.gauge), renders);

        t_advance_title(renders);

        theme = m_get_theme(theme, world.map[hero.floor], hero.where, tm);

        if(i_using_world_edit_mode(in))
        {
            SDL_SetRelativeMouseMode(SDL_FALSE);

            ov = o_update(ov, in, sdl.xres);

            m_edit(world.map[hero.floor], ov);

            world.sprites[hero.floor] = s_lay(world.sprites[hero.floor], world.map[hero.floor], ov, tm);

            s_render_overlay(sdl, ov, world.sprites[hero.floor], world.map[hero.floor], tm);
        }
        else // Playing.
        {
            ov = o_pan(ov, hero.where, sdl.xres, sdl.yres);

            world.map[hero.floor] = s_count_agents(world.sprites[hero.floor], world.map[hero.floor]);

            m_place_barricades(world.map[hero.floor]);

            current = f_stream(current, tm);

            clouds = f_stream(clouds, tm);

            hero = h_teleporting(hero, world.map[hero.floor], in, tm);

            if(hero.teleporting)
            {
                hero = h_teleport(hero, world.map[hero.floor], hero.where);

                f_ruin(field);

                field = f_prepare(world.map[hero.floor], hero.aura);

                f_extinguish(fire);

                fire = f_kindle(world.map[hero.floor]);
            }
            hero = s_caretake(world.sprites[hero.floor], hero, world.map[hero.floor], field, fire, tm, in);

            world.sprites[hero.floor] = s_spread_fire(world.sprites[hero.floor], fire, world.map[hero.floor], tm);

            s_render_playing(sdl, yel, hero, world.sprites[hero.floor], world.map[hero.floor], current, clouds, tm, in);

            hero.inventory = i_unhilite(hero.inventory);

            hero.inventory = i_select(hero.inventory, in);

            if(i_using_inventory(in))
            {
                hero.inventory = i_handle(hero.inventory, in, scroll, tm, sdl.xres);
            }
            else if(i_using_lookup(in))
            {
                hero = w_transport(world, hero, sdl, red, yel, in);
            }
            else // Combat.
            {
                SDL_SetRelativeMouseMode(SDL_TRUE);

                t_clear_title_when_linger();

                hero = s_draw_gauge(sdl, hero, scroll);

                hero = h_sustain(hero, world.map[hero.floor], in, current, world.sprites[hero.floor].last, tm);

                world.sprites[hero.floor] = s_hero_damage_sprites(world.sprites[hero.floor], hero, tm);
            }
            f_clear(fire);
        }
        t_show_title(yel, sdl);

        t_printf(yel, sdl.renderer, sdl.xres, sdl.yres, BOT_RITE, 0xFF, 0, "%d", fps);

        s_present(sdl);

        in = i_pump(in);

        const int t1 = SDL_GetTicks();

        const int ms = 1000.0f / args.fps - (t1 - t0);

        SDL_Delay(ms < 0 ? 0 : ms);

        const int t2 = SDL_GetTicks();

        if(tm.rise)
            fps = 1000.0f / (t2 - t0);
    }
    return 0;
}
