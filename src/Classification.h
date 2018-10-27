#pragma once

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
    LETTER,
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
    CLASSIFICATIONS
}
Classification;

int xcindex(const Classification);

int xcindices(const Classification);

int xismelee(const Classification);

int xisrange(const Classification);

int xismagic(const Classification);

int xisconsumable(const Classification);

Classification xclrand(void);
