#include "Speech.h"

#include "util.h"

static Speech init(const char* const str, const Timer tm)
{
    static Speech zero;
    Speech sp = zero;

    sp.ticks = tm.ticks;
    sp.count = u_char_count(str, '\n');

    sp.sentences = u_toss(char*, sp.count);
    for(int i = 0; i < sp.count; i++)
        sp.sentences[i] = u_str_dup("");

    return sp;
}

static Speech parse(Speech sp, const char* const str)
{
    char* const dup = u_str_dup(str);

    const char* const delim = "\n";

    int i = 0;
    for(char* sentence = strtok(dup, delim); sentence; sentence = strtok(NULL, delim))
        sp.sentences[i++] = u_str_dup(sentence);

    free(dup);

    return sp;
}

static Speech build(const char* const str, const Timer tm)
{
    const Speech sp = init(str, tm);
    return parse(sp, str);
}

static void kill(const Speech sp)
{
    if(sp.sentences)
    {
        for(int i = 0; i < sp.count; i++)
            free(sp.sentences[i]);
        free(sp.sentences);
    }
}

Speech s_swap(const Speech sp, const char* const str, const Timer tm)
{
    kill(sp);
    return build(str, tm);
}
