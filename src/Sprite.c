#include "Sprite.h"

#include "util.h"

#include <ctype.h>

Sprite xzsprite(void)
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

// Flower.
static Sprite _a_(Sprite s)
{
    s.width = 0.60f;
    return s;
}

// Outlaw.
static Sprite _b_(Sprite s)
{
    s.speed = 0.033f;
    s.acceleration = 0.0025f;
    s.width = 0.66f;
    static const char* const sentences[] = {
        "Hey there!",
        "How are you doing today?",
        "MY",
        "SENTENCES",
        "WILL",
        "LOOP",
        "This is my last message.",
    };
    for(int i = 0; i < xlen(sentences); i++)
        s.speech = xspappend(s.speech, sentences[i]);
    return s;
}

// Lootbag.
static Sprite _d_(Sprite s)
{
    s.width = 1.00f;
    s.health = 1.0f;
    return s;
}

// Tree trunk.
static Sprite _e_(Sprite s)
{
    s.width = 1.00f;
    s.health = 1.0f;
    return s;
}

// Tree leaves.
static Sprite _f_(Sprite s)
{
    s.width = 1.00f;
    s.health = 1.0f;
    return s;
}

Sprite xsregistrar(const int ascii, const Point where)
{
    Sprite s = born(ascii, where);
    switch(s.ascii)
    {
    case 'a': return _a_(s);
    case 'b': return _b_(s);
    case 'd': return _d_(s);
    case 'e': return _e_(s);
    case 'f': return _f_(s);
    }
    return s;
}

int xsissprite(const int ascii)
{
    return isalpha(ascii);
}

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

int xismute(Sprite* const sprite)
{
    return sprite->speech.count == 0;
}

void xstick(Sprite* const sprite, const Timer tm)
{
    sprite->speech.ticks = tm.ticks;
}
