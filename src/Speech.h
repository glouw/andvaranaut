#pragma once

typedef struct
{
    const char* sentences[64];
    int index;
    int count;
    int ticks;
}
Speech;

Speech xspzero(void);

Speech xspgreeting(void);

Speech xspmercy(void);
