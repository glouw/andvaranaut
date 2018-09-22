#include "Speech.h"

#include "util.h"

Speech xspzero(void)
{
    static Speech sp;
    return sp;
}

static Speech xspappend(Speech sp, const char* const sentence)
{
    if(sp.count == xlen(sp.sentences))
        xbomb("error: cannot append '%s': sprite sentence buffer overflow", sentence);
    sp.sentences[sp.count++] = sentence;
    return sp;
}

static Speech build(const char* const sentences[], const int len)
{
    Speech sp = xspzero();
    for(int i = 0; i < len; i++)
        sp = xspappend(sp, sentences[i]);
    return sp;
}

Speech xspanger(void)
{
    const char* const sentences[] = {
        "My sword!",
        "Julia!",
        "",
        "JULIA!",
        "",
    };
    return build(sentences, xlen(sentences));
}

Speech xspdefeated(void)
{
    const char* const sentences[] = {
        "Swing your sword",
        "I can go no further",
        "",
    };
    return build(sentences, xlen(sentences));
}
