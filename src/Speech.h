#pragma once

typedef struct
{
    char** sentences;
    int count;
    int max;
}
Speech;

Speech xzsp();

Speech xspnew();

Speech xspappend(Speech sp, const char* sentence);
