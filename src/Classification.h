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

int c_get_index(const Classification);

int c_max_indices(const Classification);

int c_is_melee(const Classification);

int c_is_range(const Classification);

int c_is_magic(const Classification);

int c_is_consumable(const Classification);

Classification c_rand(void);
