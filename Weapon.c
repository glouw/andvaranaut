#include "Weapon.h"

float reach(const Weapon weapon)
{
    switch(weapon)
    {
        case HANDS:
            return 0.00;
        case LSWORD:
            return 0.75;
        default:
            return 0.00;
    }
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
            return 0.00;
    }
}
