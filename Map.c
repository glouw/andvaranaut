#include "Map.h"
#include "Util.h"

static char** get(FILE* const file, const int rows)
{
    char** block = (char**) calloc(rows, sizeof(char*));
    for(int row = 0; row < rows; row++)
        block[row] = readln(file);
    return block;
}

Map open(const char* const name)
{
    char* const path = concat("maps/", name);
    FILE* const file = fopen(path, "r");
    // Three blocks per map: a ceiling block, a wall block, and a floor block
    const int rows = lns(file) / 3;
    char** const ceiling = get(file, rows);
    char** const walling = get(file, rows);
    char** const floring = get(file, rows);
    const Map map = { rows, ceiling, walling, floring };
    fclose(file);
    free(path);
    return map;
}

void close(const Map map)
{
    for(int row = 0; row < map.rows; row++)
    {
        free(map.ceiling[row]);
        free(map.walling[row]);
        free(map.floring[row]);
    }
    free(map.ceiling);
    free(map.walling);
    free(map.floring);
}

Map reopen(const Map map, const char* const name)
{
    close(map);
    return open(name);
}
