#include "Map.h"

#include "util.h"

#include <stdlib.h>
#include <stdio.h>

static void print(const Map map)
{
    for(int row = 0; row < map.rows; row++) puts(map.ceiling[row]);
    for(int row = 0; row < map.rows; row++) puts(map.walling[row]);
    for(int row = 0; row < map.rows; row++) puts(map.floring[row]);
}

static char** get(FILE* const file, const int rows)
{
    char** block = (char**) calloc(rows, sizeof(char*));
    for(int row = 0; row < rows; row++)
        block[row] = readln(file);
    return block;
}

Map open(const char* const path)
{
    FILE* const file = fopen(path, "r");
    const int rows = lns(file) / 3;
    char** const ceiling = get(file, rows);
    char** const walling = get(file, rows);
    char** const floring = get(file, rows);
    const Map map = { rows, ceiling, walling, floring };
    fclose(file);
    print(map);
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

Map reopen(const Map map, const char* const path)
{
    close(map);
    return open(path);
}
