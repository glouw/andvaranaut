#pragma once

typedef struct
{
    int* casting;
    int** castables;
    int width;
    int squares;
    int scrolls;
}
Scroll;

Scroll xscnew(void);

int xsindex(const Scroll);

void xsclear(const Scroll);

char* xsstr(const Scroll, const int scindex);
