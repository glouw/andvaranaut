#pragma once

#include "Hero.h"
#include "Attack.h"
#include "Input.h"
#include "Sprite.h"
#include "Field.h"
#include "Gauge.h"
#include "Inventory.h"
#include "Sorter.h"
#include "Method.h"
#include "Fire.h"

typedef struct
{
    Sprite* sprite;
    int count;
    int max;

    // How the sprites were last hurt.
    Method last;
}
Sprites;

Sprites s_spawn(const int max);

Sprites s_lay(Sprites, const Map, const Overview, const Timer);

void s_pull(const Sprites, const Hero);

void s_push(const Sprites, const Hero);

void s_turn(const Sprites, const float yaw);

void s_sort(const Sprites, Sorter);

Sprites s_hero_damage_sprites(Sprites, const Hero, const Timer);

Hero s_caretake(const Sprites, Hero, const Map, const Field, const Fire, const Timer, const Input);

Sprites s_gen(const int max, const Map);

Sprites s_spread_fire(Sprites, const Fire, const Map, const Timer);

Map s_count_agents(const Sprites, Map);

Sprites s_populate(Sprites, const Map, const Timer);
