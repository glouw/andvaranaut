#include "Sprite.h"

#include "Speech.h"
#include "util.h"

#include <assert.h>
#include <ctype.h>

static Sprite init_speech(Sprite sprite)
{
    static Speech zero;
    switch(sprite.ascii)
    {
    case 'b':
        sprite.speech = s_greet();
        break;
    default:
        sprite.speech = zero;
        break;
    }
    return sprite;
}

// Be sure to map these sprites to their surface paths in Surfaces.c.
static const Sprite lower[] = {
    /* Flower  */ { 'a', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* Dwarf   */ { 'b', 0, 0, 1.0f, 0.038f, 0.0032f, 800.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* Ember   */ { 'c', 1, 1, 1.0f, 0.000f, 0.0000f,   1.0f, 0.50f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* Lootbag */ { 'd', 0, 0, 1.0f, 0.000f, 0.0000f,   1.0f, 0.00f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'e', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'f', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'g', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'h', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'i', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'j', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'k', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'l', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'm', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'n', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'o', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'p', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'q', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'r', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 's', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 't', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'u', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'v', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'w', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'x', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'y', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'z', 0, 0, 1.0f, 0.000f, 0.0000f, 200.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
},
upper[] = {
    /* */         { 'A', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'B', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'C', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'D', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'E', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'F', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'G', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'H', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'I', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'J', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'K', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'L', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'M', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'N', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'O', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'P', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'Q', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'R', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'S', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'T', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'U', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'V', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'W', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'X', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'Y', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
    /* */         { 'Z', 0, 0, 1.0f, 0.000f, 0.0000f, 000.0f, 0.20f, 0.0f, {0,0}, {0,0}, {0,0}, IDLE, {0,0,0,0}, {{0},0,0,0}, 0 },
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

Sprite s_register(const int ascii, const Point where)
{
    Sprite sprite = islower(ascii) ? lower[ascii - 'a'] : upper[ascii - 'A'];
    sprite.where = sprite.last = where;
    return init_speech(sprite);
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
    return ascii == 'd';
}

int s_useless(const Sprite* const sprite)
{
    return s_dead(sprite->state) || s_cosmetic(sprite->ascii);
}

int s_no_count(const Sprite* const sprite)
{
    return s_useless(sprite) || s_inanimate(sprite->ascii);
}

int s_stuck(const Sprite* const sprite)
{
    return s_dead(sprite->state) || s_hurting(sprite->state);
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

int s_attacking(const Sprite* const sprite)
{
    return sprite->state == ATTACK_N
        || sprite->state == ATTACK_E
        || sprite->state == ATTACK_S
        || sprite->state == ATTACK_W;
}

int s_stunned(const Sprite* const sprite, const Timer tm)
{
    return tm.ticks < sprite->stunticks;
}

void s_stun(Sprite* const sprite, const Timer tm)
{
    sprite->stunticks = tm.ticks + 5;
}

int s_impulse(Sprite* const sprite, const Timer tm)
{
    return s_attacking(sprite) && tm.rise;
}
