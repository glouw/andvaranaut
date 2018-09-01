#include "Speech.h"

#include "util.h"

Speech xzspeech(void)
{
    static Speech sp;
    return sp;
}

Speech xspnew(void)
{
    return xzspeech();
}

Speech xspappend(Speech sp, const char* sentence)
{
    // Resize if no size.
    if(sp.max == 0)
        xretoss(sp.sentences, char*, sp.max = 1);

    // Resize if capped.
    if(sp.count >= sp.max)
        xretoss(sp.sentences, char*, sp.max *= 2);

    // Append.
    sp.sentences[sp.count++] = (char*) sentence;

    return sp;
}
