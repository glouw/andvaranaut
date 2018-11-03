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

int c_index(const Classification);

int c_indices(const Classification);

int c_ismelee(const Classification);

int c_isrange(const Classification);

int c_ismagic(const Classification);

int c_isconsume(const Classification);

Classification c_rand(void);
