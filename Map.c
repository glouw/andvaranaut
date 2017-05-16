#include "Map.h"

#include "Util.h"
#include "String.h"

static char** get(FILE* const file, const int rows)
{
    char** block = toss(char*, rows);
    for(int row = 0; row < rows; row++)
        block[row] = readln(file);
    return block;
}

Map open(const char* const name)
{
    char* const path = concat("maps/", name);
    FILE* const file = fopen(path, "r");
    Map map;
    map.rows = lns(file) / 3;
    map.ceiling = get(file, map.rows);
    map.walling = get(file, map.rows);
    map.floring = get(file, map.rows);
    fclose(file);
    free(path);
    return map;
}

void dump(const Map map, const char* const name)
{
    char* const path = concat("maps/", name);
    remove(path);
    FILE* const file = fopen(path, "w");
    for(int row = 0; row < map.rows; row++) fprintf(file, "%s\n", map.ceiling[row]);
    for(int row = 0; row < map.rows; row++) fprintf(file, "%s\n", map.walling[row]);
    for(int row = 0; row < map.rows; row++) fprintf(file, "%s\n", map.floring[row]);
    fclose(file);
    free(path);
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
