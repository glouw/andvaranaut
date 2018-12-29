#pragma once

typedef struct
{
    const char* sentences[8];
    int index;
    int count;
    int ticks;
}
Speech;

Speech s_use_greet(void);

Speech s_use_grateful(void);
