#include "Sprite.h"

#include "Speech.h"
#include "Frame.h"
#include "util.h"

#include <assert.h>
#include <ctype.h>

//
// Enemy sprites or non-living things.
//

static const Sprite lower[] = {
    /* Flower  */ { 'a', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* Dwarf   */ { 'b', 0, 0, 8, 6, 1.0f, 0.038f, 0.0032f, 800.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* Ember   */ { 'c', 1, 1, 4, 0, 1.0f, 0.000f, 0.0000f,   1.0f, 0.50f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* Lootbag */ { 'd', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f,   1.0f, 0.00f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'e', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'f', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'g', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'h', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'i', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'j', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'k', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'l', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'm', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'n', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'o', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'p', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'q', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'r', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 's', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 't', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'u', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'v', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'w', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'x', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'y', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'z', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
};

//
// Important sprites with quest lines.
//

static const Sprite upper[] = {
    /* */         { 'A', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* Tutor   */ { 'B', 0, 0, 8, 6, 1.0f, 0.038f, 0.0032f, 800.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,LETTER}, {1,LETTER}, "You've woken up.\nClimb down the trapdoors.\nBring me Andvari's Gift.\n\n", "Thanks.\n\n", "..." },
    /* */         { 'C', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'D', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'E', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'F', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'G', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'H', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'I', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'J', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'K', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'L', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'M', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'N', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'O', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'P', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'Q', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'R', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'S', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'T', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'U', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'V', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'W', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'X', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'Y', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
    /* */         { 'Z', 0, 0, 4, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, 0, 0, {NULL,0,0,0}, {0,NONE  }, {0,NONE  }, "", "", "" },
};

static void check(const Sprite table[], const int len, const char first, const char last)
{
    for(int i = 0; i < len; i++)
        assert(table[i].ascii == first + i);
    assert(len == last - first + 1);
}

void s_test(void)
{
    check(lower, u_len(lower), 'a', 'z');
    check(upper, u_len(upper), 'A', 'Z');
}

Sprite s_register(const int ascii, const Point where, const Timer tm)
{
    Sprite sprite = islower(ascii) ? lower[ascii - 'a'] : upper[ascii - 'A'];
    sprite.where = sprite.last = where;
    sprite.speech = s_swap(sprite.speech, sprite.quest_start, tm);
    return sprite;
}

int s_sprite(const int ascii)
{
    return isalpha(ascii);
}

int s_cosmetic(const int ascii)
{
    return ascii == 'a';
}

int s_firey(const int ascii)
{
    return ascii == 'c';
}

int s_inanimate(const int ascii)
{
    return ascii == 'd' || s_firey(ascii);
}

int s_useless(const Sprite* const sprite)
{
    return s_dead(sprite->state) || s_cosmetic(sprite->ascii);
}

int s_not_agent(const Sprite* const sprite)
{
    return s_useless(sprite) || s_inanimate(sprite->ascii);
}

int s_stuck(const Sprite* const sprite)
{
    return s_dead(sprite->state) || s_hurting(sprite->state) || s_stunned(sprite->state);
}

void s_place(Sprite* const sprite, const Point to)
{
    sprite->last = sprite->where;
    sprite->where = to;
}

int s_muted(const Sprite* const sprite)
{
    return sprite->speech.count == 0;
}

int s_busy(const Sprite* const sprite, const Timer tm)
{
    return tm.ticks < sprite->busy_ticks;
}

void s_go_busy(Sprite* const sprite, const Timer tm, const int ticks, const State state)
{
    sprite->busy_ticks = tm.ticks + ticks;
    sprite->state = state;
}

int s_impulse(const Sprite* const sprite, const Timer tm)
{
    return s_attacking(sprite->state) && tm.rise;
}

int s_evil_act(const Sprite* const sprite, const Timer tm)
{
    return sprite->evil && tm.fall;
}

int s_will_rage(const Sprite* const sprite, const Timer tm)
{
    return sprite->state == IDLE && s_evil_act(sprite, tm);
}

int s_must_spread(const Sprite* const sprite, char** const floring)
{
    return s_firey(sprite->ascii) && s_alive(sprite->state) && p_char(sprite->where, floring) == '(';
}

//
// TODO: Different ticks for each sprite.
//

void s_parried(Sprite* const sprite, const Point velocity, const Timer tm)
{
    const Point dir = p_unit(velocity);
    if((sprite->state == ATTACK_N && p_south(dir))
    || (sprite->state == ATTACK_S && p_north(dir))
    || (sprite->state == ATTACK_W && p_east (dir))
    || (sprite->state == ATTACK_E && p_west (dir)))
    {
        const int ticks = 1 + (rand() % sprite->stun_ticks);
        const int frames = ticks * FRAMES;
        s_go_busy(sprite, tm, frames, STUNNED);
    }
}
