#include "Blocks.h"

#include <stdlib.h>
#include <string.h>

static char** get(FILE* const file, const int rows)
{
    char* line = NULL;
    unsigned reads = 0;
    char** block = (char**) calloc(rows, sizeof(char*));
    for(int row = 0; row < rows; row++)
    {
        getline(&line, &reads, file);
        block[row] = strdup(line);
    }
    free(line);
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
