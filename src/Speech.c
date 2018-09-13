#include "Speech.h"

#include "util.h"

Speech xspzero(void)
{
    static Speech sp;
    return sp;
}

static Speech build(const char* const sentences[], const int len)
{
    Speech sp = xspzero();
    for(int i = 0; i < len; i++)
        sp = xspappend(sp, sentences[i]);
    return sp;
}

Speech xspgeneric(void)
{
    const char* const sentences[] = {
        "Hey there!",
        "How are you doing today?",
        "This is my last message.",
    };
    return build(sentences, xlen(sentences));
}

Speech xspappend(Speech sp, const char* const sentence)
{
    if(sp.max == 0)
        xretoss(sp.sentences, char*, sp.max = 1);
    if(sp.count >= sp.max)
        xretoss(sp.sentences, char*, sp.max *= 2);
    sp.sentences[sp.count++] = (char*) sentence;
    return sp;
}
