#pragma once

typedef struct
{
    const char* sentences[64];
    int count;
    int ticks;
}
Speech;

Speech xspzero(void);

Speech xspdefeated(void);
