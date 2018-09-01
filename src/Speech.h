#pragma once

typedef struct
{
    char** sentences;
    int count;
    int max;
}
Speech;

Speech xzspeech(void);

Speech xspnew(void);

Speech xspappend(Speech sp, const char* sentence);
