#include "Sprite.h"

#include "Speech.h"
#include "Rect.h"
#include "util.h"

#include <ctype.h>

Sprite xsregistrar(const int ascii, const Point where)
{
    // Lower case sprites: [a - z]
    const Sprite lower[] = {
        { where, 'a', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'b', IDLE, 0.0, where, xzpoint(), 0.04f, 0.0025f, false, 0, 200.0f, xzrect(), xspgeneric() }, // Outlaw
        { where, 'c', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'd', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'e', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'f', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
    };

    // Upper case sprites: [A - Z]
    const Sprite upper[] = {
        { where, 'A', IDLE, 0.0, where, xzpoint(), 0.0f, 0.0f, false, 0, 200.0f, xzrect(), xspzero() }, // Placeholder.
    };

    return islower(ascii) ? lower[ascii - 'a'] : upper[ascii - 'A'];
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
