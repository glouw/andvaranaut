#pragma once

typedef struct
{
    char** sentences;
    int count;
    int max;
    int ticks;
}
Speech;

Speech xspzero(void);

Speech xspgeneric(void);

Speech xspappend(Speech, const char* sentence);
