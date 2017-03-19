#include "Map.h"

#include <stdlib.h>

Map open(const char* const path)
{
    FILE* const fp = fopen(path, "r");
    const Meta meta = retrieve(fp);
    const Blocks blocks = build(fp, meta.rows);
    fclose(fp);
    return (Map) { meta, blocks };
}

void close(const Map map)
{
    for(int row = 0; row < map.meta.rows; row++)
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
