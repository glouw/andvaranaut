#include "Sprite.h"

#include <ctype.h>
#include "util.h"

Sprite xzsprite()
{
    static Sprite sprite;
    return sprite;
}

static Sprite born(const int ascii, const Point where)
{
    Sprite s = xzsprite();
    s.ascii = ascii;
    s.where = where;
    s.last = where;
    s.state = IDLE;
    s.health = 1000.0f;
    return s;
}

Sprite xsregistrar(const int ascii, const Point where)
{
    Sprite s = born(ascii, where);
    switch(s.ascii)
    {
    // Flower.
    case 'a':
        s.width = 0.60f;
        break;

    // Outlaw.
    case 'b':
        s.speed = 0.033f;
        s.acceleration = 0.0025f;
        s.width = 0.66f;
        break;

    // Loot bag.
    case 'd':
        s.width = 1.00f;
        s.health = 1.0f;
        break;

    // Tree trunk.
    case 'e':
        s.width = 1.00f;
        s.health = 1.0f;
        break;

    // Tree leaves.
    case 'f':
        s.ascii = 'f';
        s.width = 1.00f;
        s.health = 1.0f;
        break;
    }
    return s;
}

int xishurt(const State state)
{
    return state == HURTN
        || state == HURTE
        || state == HURTS
        || state == HURTW;
}

int xisdead(const State state)
{
    return state == DEADN
        || state == DEADE
        || state == DEADS
        || state == DEADW;
}

int xisalive(const State state)
{
    return !xisdead(state);
}

int xsissprite(const int ascii)
{
    return isalpha(ascii);
}

// Cannot be hurt with melee and ranged weapons.
int xiscosmetic(const int ascii)
{
    return ascii == 'a';
}

int xisinanimate(const int ascii)
{
    return ascii == 'd';
}

int xisuseless(const Sprite* const sprite)
{
    return xisdead(sprite->state) || xiscosmetic(sprite->ascii);
}

int xisstuck(const Sprite* const sprite)
{
    return xisdead(sprite->state) || xishurt(sprite->state);
}

void xsplace(Sprite* const sprite, const Point to)
{
    sprite->last = sprite->where;
    sprite->where = to;
}
