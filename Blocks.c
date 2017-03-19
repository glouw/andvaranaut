#include "Blocks.h"

#include <stdlib.h>
#include <string.h>

static char** get(FILE* const fp, const int rows)
{
    char* line = NULL;
    unsigned reads = 0;
    char** block = calloc(rows, sizeof(char*));
    for(int row = 0; row < rows; row++)
    {
        getline(&line, &reads, fp);
        block[row] = strdup(line);
    }
    free(line);
    return block;
}

Blocks build(FILE* const fp, const int rows)
{
    return (Blocks) {
        get(fp, rows),
        get(fp, rows),
        get(fp, rows),
    };
}
