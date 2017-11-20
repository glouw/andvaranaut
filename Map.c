#include "Map.h"

#include "Sprites.h"

#include "util.h"

static char** get(FILE* const file, const int rows)
{
    char** block = xtoss(char*, rows);
    for(int row = 0; row < rows; row++)
        block[row] = xreadln(file);
    return block;
}

Map xopen(const int level)
{
    char which[MINTS];
    sprintf(which, "%d", level);
    char* const path = xconcat("maps/lvl", which);
    FILE* const file = fopen(path, "r");
    Map map;
    xzero(map);
    map.rows = xlns(file) / 3;
    map.ceiling = get(file, map.rows);
    map.walling = get(file, map.rows);
    map.floring = get(file, map.rows);
    // Assumes all row columns are same size.
    map.cols = strlen(map.walling[0]);
    fclose(file);
    free(path);
    return map;
}

void xclose(const Map map)
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

Map xreopen(const Map map, const int level)
{
    xclose(map);
    return xopen(level);
}

bool xisportal(const Map map, const Point where)
{
    return xblok(where, map.floring) == '~' || xblok(where, map.ceiling) == '~';
}

bool xout(const Map map, const float x, const float y)
{
    return x >= map.cols || x < 0.0
        || y >= map.rows || y < 0.0;
}

void xedit(const Map map, const Overview ov)
{
    if(xout(map, ov.x, ov.y)) return;
    const int ascii = ov.selected + ' ';
    // If the ascii is an alpha character then it is a sprite
    if(xissprite(ascii)) return;
    // Otherwise place the ascii character.
    const int x = ov.x;
    const int y = ov.y;
    if(ov.party == FLORING) map.floring[y][x] = ascii;
    if(ov.party == WALLING) map.walling[y][x] = ascii;
    if(ov.party == CEILING) map.ceiling[y][x] = ascii;
}
