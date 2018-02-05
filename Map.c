#include "Map.h"

#include "Sprites.h"

#include "util.h"

static void mprint(char** block, const int rows, const int cols)
{
    for(int row = 0; row < rows; row++)
    for(int col = 0; col < cols; col++)
        printf("%c%s", block[row][col], col == cols - 1 ? "\n" : "");
    putchar('\n');
}

void xmdump(const Map map)
{
    mprint(map.ceiling, map.rows, map.cols);
    mprint(map.walling, map.rows, map.cols);
    mprint(map.floring, map.rows, map.cols);
}

static char** mreset(char** block, const int rows, const int cols)
{
    for(int row = 0; row < rows; row++)
    for(int col = 0; col < cols; col++)
        block[row][col] = '#';
    return block;
}

static char** mnew(const int rows, const int cols)
{
    char** block = xtoss(char*, rows);
    for(int row = 0; row < rows; row++)
        block[row] = xtoss(char, cols);
    return mreset(block, rows, cols);
}

Map xmgen(const int rows, const int cols)
{
    Map map;
    xzero(map);
    map.rows = rows;
    map.cols = cols;
    map.ceiling = mnew(map.rows, map.cols);
    map.walling = mnew(map.rows, map.cols);
    map.floring = mnew(map.rows, map.cols);
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

int xisportal(const Map map, const Point where)
{
    return xblok(where, map.floring) == '~'
        || xblok(where, map.ceiling) == '~';
}

int xout(const Map map, const Point where)
{
    return (int) where.x >= map.cols || (int) where.x < 0
        || (int) where.y >= map.rows || (int) where.y < 0;
}

void xedit(const Map map, const Overview ov)
{
    // Placing - Out of bounds check.
    if(xout(map, ov.where)) return;
    const int ascii = ov.selected + ' ';
    // If the ascii is an alpha character then it is a sprite
    if(xissprite(ascii)) return;
    // Otherwise place the ascii character.
    const int x = ov.where.x;
    const int y = ov.where.y;
    if(ov.party == FLORING) map.floring[y][x] = ascii;
    if(ov.party == WALLING) map.walling[y][x] = ascii;
    if(ov.party == CEILING) map.ceiling[y][x] = ascii;
}
