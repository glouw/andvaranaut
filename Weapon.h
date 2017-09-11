#pragma once

#include "Input.h"

typedef enum
{
    HANDS,
    LSWORD,
    WEAPONS
}
Weapon;

float reach(const Weapon weapon);

float power(const Weapon weapon);

Weapon wield(const Weapon weapon, const Input input);
