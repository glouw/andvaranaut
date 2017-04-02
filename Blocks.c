#include "Blocks.h"

#include "util.h"

#include <stdlib.h>

static char** get(FILE* const file, const int rows)
{
    char** block = (char**) calloc(rows, sizeof(char*));
    for(int row = 0; row < rows; row++)
        block[row] = readln(file);
    return block;
}

Blocks build(FILE* const file, const int rows)
{
    const Blocks blocks = {
        get(file, rows),
        get(file, rows),
        get(file, rows),
    };
    return blocks;
}
