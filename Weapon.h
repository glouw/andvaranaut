#pragma once

typedef enum
{
    LSWORD,
    HANDS,
    WEAPONS
}
Weapon;

float reach(const Weapon weapon);
float power(const Weapon weapon);
