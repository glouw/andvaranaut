#pragma once

typedef struct
{
    char** sentences;
    int count;
    int max;
    int ticks;
}
Speech;

Speech xspappend(Speech, const char* sentence);
