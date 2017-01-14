#include "Hero.h"

#include "SDL2/SDL.h"

Hero
Hero_Move(Hero hero, Map map)
{
    const uint8_t* const key = SDL_GetKeyboardState(NULL);
    // Keyboard state update
    SDL_PumpEvents();
    // <CTRL+D> to exit
    if(key[SDL_SCANCODE_LCTRL] && key[SDL_SCANCODE_D]) hero.health = -1;
    if(key[SDL_SCANCODE_RCTRL] && key[SDL_SCANCODE_D]) hero.health = -1;
    // Hero rotation <h, l>
    if(key[SDL_SCANCODE_H]) hero.theta -= hero.d0;
    if(key[SDL_SCANCODE_L]) hero.theta += hero.d0;
    // Hero movement <w, a, s, d>
    Hero temp = hero;
    const Point direction = { hero.dx * cos(hero.theta), hero.dy * sin(hero.theta) };
    if(key[SDL_SCANCODE_W]) temp.where.x += direction.x, temp.where.y += direction.y;
    if(key[SDL_SCANCODE_S]) temp.where.x -= direction.x, temp.where.y -= direction.y;
    if(key[SDL_SCANCODE_A]) temp.where.y -= direction.x, temp.where.x += direction.y;
    if(key[SDL_SCANCODE_D]) temp.where.y += direction.x, temp.where.x -= direction.y;
    // Hero collision detection
    return Point_Collision(temp.where, map.walling) ? hero : temp;
}
