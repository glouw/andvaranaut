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

Scroll s_new(void);

int s_index(const Scroll);

void s_clear(const Scroll);

char* s_scroll_to_str(const Scroll, const int scindex);
