#include "Sprite.h"

#include <ctype.h>
#include "util.h"

static Sprite born(const int ascii, const Point where)
{
    Sprite s;
    xzero(s);
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

int xsissprite(const int ascii)
{
    return isalpha(ascii);
}

int xsiscosmetic(const Sprite* sprite)
{
    return sprite->ascii == 'a';
}

void xsplace(Sprite* const sprite, const Point to)
{
    sprite->last = sprite->where;
    sprite->where = to;
}
