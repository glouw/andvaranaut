#include "Weapon.h"

#include "SDL2/SDL.h"

float reach(const Weapon weapon)
{
    switch(weapon)
    {
        case HANDS:
            return 0.00;
        case LSWORD:
            return 0.75;
        default:
            break;
    }
    return 0.00;
}

float power(const Weapon weapon)
{
    switch(weapon)
    {
        case HANDS:
            return 0.00;
        case LSWORD:
            return 0.10;
        default:
            break;
    }
    return 0.00;
}

Weapon wield(const Weapon weapon, const Input input)
{
    Weapon temp = weapon;
    if(input.key[SDL_SCANCODE_1])
        temp = HANDS;
    if(input.key[SDL_SCANCODE_2])
        temp = LSWORD;
    return temp;
}
