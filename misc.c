#include "misc.h"

#include <stdlib.h>

int fl(const double x)
{
    return (int) x - (x < (int) x);
}

int cl(const double x)
{
    return (int) x + (x > (int) x);
}

double dec(const double x)
{
    return x - (int) x;
}

int newlines(FILE* const fp)
{
    char* line = NULL;
    unsigned reads = 0;
    int lines = 0;
    while(getline(&line, &reads, fp) != -1) lines++;
    free(line);
    rewind(fp);
    return lines;
}
