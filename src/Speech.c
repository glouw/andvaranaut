#include "Speech.h"

#include "util.h"

static Speech append(Speech sp, const char* const sentence)
{
    if(sp.count == u_len(sp.sentences))
        u_bomb("error: cannot append '%s': sprite sentence buffer overflow", sentence);
    sp.sentences[sp.count++] = sentence;
    return sp;
}

static Speech build(const char* const sentences[], const int len, const Timer tm)
{
    static Speech zero;
    Speech sp = zero;
    for(int i = 0; i < len; i++)
        sp = append(sp, sentences[i]);
    sp.ticks = tm.ticks;
    return append(sp, ""); // Extra padding.
}

Speech s_use_unwanted(const Timer tm)
{
    const char* const sentences[] = {
        "I do not want this.",
    };
    return build(sentences, u_len(sentences), tm);
}

Speech s_use_grateful(const Timer tm)
{
    const char* const sentences[] = {
        "Wow, I always wanted this!",
    };
    return build(sentences, u_len(sentences), tm);
}

Speech s_use_greet(const Timer tm)
{
    const char* const sentences[] = {
        "Hello there!",
        "How are you doing?",
        "Nice weather huh?",
    };
    return build(sentences, u_len(sentences), tm);
}
