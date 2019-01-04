#pragma once

#include "Timer.h"

typedef struct
{
    const char* sentences[8];
    int index;
    int count;
    int ticks;
}
Speech;

Speech s_use_greet(const Timer);

Speech s_use_unwanted(const Timer);

Speech s_use_grateful(const Timer);

Speech s_use_inv_full(const Timer);

Speech s_use_no_give(const Timer);
