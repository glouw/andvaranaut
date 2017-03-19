#include "Meta.h"

#include <stdlib.h>

Meta retrieve(FILE* const fp)
{
    char* line = NULL;
    unsigned reads = 0;
    int rows = 0;
    int inside = 0;
    getline(&line, &reads, fp);
    sscanf(line, "%d %d", &rows, &inside);
    free(line);
    return (Meta) { rows, inside };
}
