#include "Sprite.h"

#include <ctype.h>
#include "util.h"

static Sprite born(const Point where)
{
    Sprite sprite;
    xzero(sprite);
    sprite.where = where;
    sprite.last = where;
    sprite.state = IDLE;
    sprite.health = 1000.0f;
    return sprite;
}

// Flower.
static Sprite _a_(const Point where)
{
    Sprite sprite = born(where);
    sprite.ascii = 'a';
    sprite.width = 0.60f;
    return sprite;
}

// Outlaw.
static Sprite _b_(const Point where)
{
    Sprite sprite = born(where);
    sprite.ascii = 'b';
    sprite.speed = 0.033f;
    sprite.acceleration = 0.0025f;
    sprite.width = 0.66f;
    return sprite;
}

// Loot bag.
static Sprite _d_(const Point where)
{
    Sprite sprite = born(where);
    sprite.ascii = 'd';
    sprite.width = 1.00f;
    sprite.health = 1.0f;
    return sprite;
}

// Tree Trunk.
static Sprite _e_(const Point where)
{
    Sprite sprite = born(where);
    sprite.ascii = 'e';
    sprite.width = 1.00f;
    sprite.health = 1.0f;
    return sprite;
}

// Tree Leaves.
static Sprite _f_(const Point where)
{
    Sprite sprite = born(where);
    sprite.ascii = 'f';
    sprite.width = 1.00f;
    sprite.health = 1.0f;
    return sprite;
}

static Sprite _unknown_(const Point where, const int ascii)
{
    Sprite sprite = born(where);
    sprite.ascii = ascii;
    return sprite;
}

Sprite xsregistrar(const int ascii, const Point where)
{
    switch(ascii)
    {
    case 'a': return _a_(where);
    case 'b': return _b_(where);
    case 'd': return _d_(where);
    case 'e': return _e_(where);
    case 'f': return _f_(where);
    default:
        return _unknown_(where, ascii);
    }
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
