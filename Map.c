#include "Map.h"

#include "util.h"

#include <stdlib.h>

// There are three blocks per map, one for the
// ceiling, one for the walling, and another for the
// flooring. Rows defines the number of rows per block,
// so the number of rows is equal to the number of newlines
// divided by three

Map open(const char* const path)
{
    FILE* const fp = fopen(path, "r");
    const int rows = newlines(fp) / 3;
    const Blocks blocks = build(fp, rows);
    fclose(fp);
    const Map map = { rows, blocks };
    return map;
}

void close(const Map map)
{
    for(int row = 0; row < map.rows; row++)
    {
        free(map.blocks.ceiling[row]);
        free(map.blocks.walling[row]);
        free(map.blocks.floring[row]);
    }
    free(map.blocks.ceiling);
    free(map.blocks.walling);
    free(map.blocks.floring);
}

Map reopen(const Map map, const char* const path)
{
    close(map);
    return open(path);
}
