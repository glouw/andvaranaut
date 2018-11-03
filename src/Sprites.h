#pragma once

#include "Hero.h"
#include "Attack.h"
#include "Sprite.h"
#include "Field.h"
#include "Gauge.h"
#include "Inventory.h"
#include "Method.h"

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

void s_orient(const Sprites, const Hero);

void s_placeback(const Sprites, const Hero);

Sprites s_lay(Sprites, const Map, const Overview);

Sprites s_hurt(Sprites, const Attack, const Hero, const Input, const Inventory, const Timer);

Hero s_caretake(const Sprites, const Hero, const Map, const Field, const Gauge, const Timer);

Sprites s_gen(const int max, const Map);

Sprites s_populate(Sprites, const Map);

Map s_count(const Sprites, Map);
