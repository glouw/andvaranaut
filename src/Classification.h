#pragma once

#include "Surfaces.h"

typedef enum
{
    AMMO,
    AMULET,
    ARMOR,
    BOOK,
    BOOT,
    CHEST,
    FLESH,
    FOOD,
    GLOVE,
    HAT,
    KEY,
    LIGHT,
    LONGWEP,
    MEDWEP,
    MONEY,
    MUSIC,
    POTION,
    RING,
    ROCK,
    SCROLL,
    SHIELD,
    SHORTWEP,
    TOOL,
    WAND,
    NONE,
    CLASSIFICATIONS,
}
Classification;

int xcindex(const Classification);

int xcindices(const Classification, const Surfaces);

int xismelee(const Classification);

int xisranged(const Classification);

int xismagic(const Classification);
