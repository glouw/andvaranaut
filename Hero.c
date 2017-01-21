// Copyright (C) 2016-2017 Gustav Louw
// 
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Hero.h"

#include "SDL2/SDL.h"

Hero
Hero_Move(Hero hero, Map map)
{
    const uint8_t* const key = SDL_GetKeyboardState(NULL);
    // Keyboard state update
    SDL_PumpEvents();
    // <CTRL+D> to exit
    if(key[SDL_SCANCODE_LCTRL] && key[SDL_SCANCODE_D]) hero.health = -1.0;
    if(key[SDL_SCANCODE_RCTRL] && key[SDL_SCANCODE_D]) hero.health = -1.0;
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
