#pragma once

typedef struct
{
    int* casting;
    int** castables;
    int grid;
    int squares;
    int scrolls;
}
Scroll;

Scroll xscnew();
