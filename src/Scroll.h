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

Scroll xscnew();

int xsindex(const Scroll);

void xsclear(const Scroll);
