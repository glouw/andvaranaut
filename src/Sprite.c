#include "Sprite.h"

#include "Speech.h"
#include "util.h"

#include <assert.h>
#include <ctype.h>

static void check(const Sprite table[], const int len, const char first, const char last)
{
    for(int i = 0; i < len; i++)
        assert(table[i].ascii == first + i);
    assert(len == last - first + 1);
}

static Sprite initspeech(Sprite sprite)
{
    switch(sprite.ascii)
    {
    case 'b':
        sprite.speech = xspsherrif();
        sprite.angers = xspyulia();
        break;
    default:
        sprite.speech = xspzero();
        sprite.angers = xspzero();
        break;
    }
    return sprite;
}

Sprite xsregistrar(const int ascii, const Point where)
{
    static const Sprite
    lower[] = {
        { { 0,0 }, 'a', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'b', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.04f, 0.0025f, false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, false, }, // Outlaw.
        { { 0,0 }, 'c', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'd', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'e', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'f', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'g', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'h', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'i', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'j', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'k', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'l', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'm', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'n', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'o', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'p', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'q', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'r', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 's', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 't', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'u', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'v', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'w', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'x', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'y', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
        { { 0,0 }, 'z', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true , },
    },
    upper[] = {
        { { 0,0 }, 'A', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'B', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'C', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'D', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'E', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'F', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'G', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'H', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'I', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'J', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'K', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'L', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'M', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'N', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'O', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'P', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'Q', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'R', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'S', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'T', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'U', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'V', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'W', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'X', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'Y', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
        { { 0,0 }, 'Z', IDLE, 0.0, { 0,0 }, { 0,0 }, 0.0f , 0.0f   , false, 0, 200.0f, { 0,0,0,0 }, { {0},0,0 }, { {0},0,0 }, true, },
    };
    check(lower, xlen(lower), 'a', 'z');
    check(upper, xlen(upper), 'A', 'Z');

    // Sprite table is static so these dynamic fields must be set here.
    Sprite sprite = islower(ascii) ? lower[ascii - 'a'] : upper[ascii - 'A'];
    sprite.where = sprite.last = where;
    return initspeech(sprite);
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

int xnocount(const Sprite* const sprite)
{
    return xisuseless(sprite) || xisinanimate(sprite->ascii);
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
