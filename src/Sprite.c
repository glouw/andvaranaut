#include "Sprite.h"

#include "Speech.h"
#include "Rect.h"
#include "util.h"

#include <ctype.h>

Sprite xsregistrar(const int ascii, const Point where)
{
    // Unfortunately, these tables are built everytime a new sprite is created, but the benefit is that all sprite
    // stats are centralized in one place for quick comparison and edits.
    const Sprite
    lower[] = {
        { where, 'a', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'b', IDLE, 0.0, where, xzpoint(), 0.04f, 0.0025f, false, 0, 200.0f, xzrect(), xspgeneric() }, // Outlaw.
        { where, 'c', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'd', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'e', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'f', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'g', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'h', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'i', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'j', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'k', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'l', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'm', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'n', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'o', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'p', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'q', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'r', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 's', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 't', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'u', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'v', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'w', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'x', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'y', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'z', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
    },
    upper[] = {
        { where, 'A', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'B', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'C', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'D', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'E', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'F', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'G', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'H', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'I', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'J', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'K', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'L', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'M', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'N', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'O', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'P', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'Q', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'R', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'S', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'T', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'U', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'V', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'W', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'X', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'Y', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
        { where, 'Z', IDLE, 0.0, where, xzpoint(), 0.0f , 0.0f   , false, 0, 200.0f, xzrect(), xspzero()    },
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
