#include "Map.h"

#include <stdlib.h>

Map open(const char* const name)
{
    const Map map = { build(name), wake(name) };
    return map;
}

void close(const Map map)
{
    for(int row = 0; row < map.blocks.rows; row++)
    {
        free(map.blocks.ceiling[row]);
        free(map.blocks.walling[row]);
        free(map.blocks.floring[row]);
    }
    free(map.blocks.ceiling);
    free(map.blocks.walling);
    free(map.blocks.floring);
}

Map reopen(const Map map, const char* const name)
{
    close(map);
    return open(name);
}
