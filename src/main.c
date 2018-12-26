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

    const World world = w_make(32);

    Hero hero = h_birth(args.focal, world.map, 0);

    Overview ov = o_init();

    Flow current = f_start(-1.0f / 6.0f);

    Flow clouds = f_start(9.0f);

    Field field = f_prepare(world.map[hero.floor], hero.aura);

    Fire fire = f_kindle(world.map[hero.floor]);

    Scroll scroll = s_new();

    Input in = i_ready(args.msen);

    Theme theme = NO_THEME;

    Timer tm = t_new();

    Sdl sdl = s_setup(args);

    const Text yel = t_build("art/gui/SDS_8x8.ttf", 24, sdl.yel, sdl.blk);
    const Text red = t_build("art/gui/SDS_8x8.ttf", 24, sdl.red, sdl.blk);

    t_clear_title();

    for(int renders = 0, fps = 0; args.xres == 512 ? renders < 5 : !in.done; renders++)
    {
        const int t0 = SDL_GetTicks();

        tm = t_tick(tm, hero.gauge.count && i_can_block(i_get_equipped(hero.inventory)), renders);

        t_advance_title(renders);

        theme = m_get_theme(theme, world.map[hero.floor], hero.where, tm);

        if(i_using_world_edit_mode(in))
        {
            SDL_SetRelativeMouseMode(SDL_FALSE);

            s_render_overlay(sdl, ov, world.sprites[hero.floor], world.map[hero.floor], tm);

            ov = o_update(ov, in, sdl.xres);

            m_edit(world.map[hero.floor], ov);

            world.sprites[hero.floor] = s_lay(world.sprites[hero.floor], world.map[hero.floor], ov);
        }
        else
        {
            world.map[hero.floor] = s_count_agents(world.sprites[hero.floor], world.map[hero.floor]);

            m_place_barricades(world.map[hero.floor]);

            hero = h_teleporting(hero, world.map[hero.floor], in, tm);

            current = f_stream(current);

            clouds = f_stream(clouds);

            if(hero.teleporting)
            {
                hero = h_teleport(hero, world.map[hero.floor], hero.where);

                f_ruin(field);

                field = f_prepare(world.map[hero.floor], hero.aura);

                f_extinguish(fire);

                fire = f_kindle(world.map[hero.floor]);
            }

            s_caretake(world.sprites[hero.floor], hero, world.map[hero.floor], field, fire, tm);

            hero = w_interact(world, hero, tm);

            world.sprites[hero.floor] = s_spread_fire(world.sprites[hero.floor], fire, world.map[hero.floor], tm);

            ov = o_pan(ov, hero.where, sdl.xres, sdl.yres);

            hero.inventory = i_select(hero.inventory, in);

            s_render_playing(sdl, yel, hero, world.sprites[hero.floor], world.map[hero.floor], current, clouds, tm);

            if(i_using_inventory(in))
            {
                SDL_SetRelativeMouseMode(SDL_FALSE);

                hero.inventory = i_highlite(hero.inventory, in, sdl.xres);

                hero.inventory = i_what_is(hero.inventory, scroll, tm);
            }
            else
            if(i_using_lookup(in))
            {
                const int key = i_get_key(in);
                s_draw_room_lookup(sdl, yel, red, world.map[hero.floor].rooms, key);
                hero = h_transport(hero, world.map[hero.floor], key);
            }
            else
            {
                SDL_SetRelativeMouseMode(SDL_TRUE);

                t_clear_title_when_linger();

                hero = h_sustain(hero, world.map[hero.floor], in, current, world.sprites[hero.floor].last, tm);

                hero = s_draw_gauge(sdl, hero, scroll);

                world.sprites[hero.floor] = s_hero_damage_sprites(world.sprites[hero.floor], hero, tm);
            }

            f_clear(fire);
        }
        t_show_title(yel, sdl);

        t_scrib(yel, sdl.renderer, sdl.xres, sdl.yres, BOT_RITE, 0xFF, 0, "%d", fps);

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
