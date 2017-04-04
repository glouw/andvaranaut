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

static void print(const Blocks blocks)
{
    for(int row = 0; row < blocks.rows; row++)
        puts(blocks.ceiling[row]);
    for(int row = 0; row < blocks.rows; row++)
        puts(blocks.walling[row]);
    for(int row = 0; row < blocks.rows; row++)
        puts(blocks.floring[row]);
}

Blocks build(const char* const name)
{
    char* const path = strcon(name, ".blocks");
    FILE* const file = fopen(path, "r");
    // Three blocks per file
    const int rows = lns(file) / 3;
    const Blocks blocks = {
        rows,
        get(file, rows),
        get(file, rows),
        get(file, rows),
    };
    fclose(file);
    free(path);
    print(blocks);
    return blocks;
}
