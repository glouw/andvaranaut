#pragma once

#include "Hero.h"
#include "Attack.h"
#include "Sprite.h"
#include "Field.h"
#include "Inventory.h"
#include "Method.h"

typedef struct
{
    Sprite* sprite;
    int count;
    int max;
    Method last;
}
Sprites;

Sprites xzsprites();

Sprites xsnew(const int max);

void xkill(const Sprites);

void xorient(const Sprites, const Hero);

void xplback(const Sprites, const Hero);

Sprites xlay(Sprites, const Map, const Overview);

Sprites xhurt(Sprites, const Attack, const Hero, const Input, const Inventory, const Timer);

Hero xcaretake(const Sprites, const Hero, const Map, const Field, const Timer);
