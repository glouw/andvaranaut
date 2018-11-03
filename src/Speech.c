#include "Speech.h"

#include "util.h"

static Speech append(Speech sp, const char* const sentence)
{
    if(sp.count == u_len(sp.sentences))
        u_bomb("error: cannot append '%s': sprite sentence buffer overflow", sentence);
    sp.sentences[sp.count++] = sentence;
    return sp;
}

static Speech build(const char* const sentences[], const int len)
{
    static Speech zero;
    Speech sp = zero;
    for(int i = 0; i < len; i++)
        sp = append(sp, sentences[i]);
    return sp;
}

Speech s_greet(void)
{
    const char* const sentences[] = {
        "Hello there!",
        ""
    };
    return build(sentences, u_len(sentences));
}
