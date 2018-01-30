#include "Map.h"

#include "Sprites.h"

#include "util.h"

// TODO: Add functionality to expand (retoss) map when editing.

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

static int scheck(const Map map, const int row, const int col, const int s)
{
    for(int j = row; j < row + s; j++)
    for(int i = col; i < col + s; i++)
    {
        // Check if square is out of map bounds.
        if(i <= 0 || i >= map.cols - 1) return false;
        if(j <= 0 || j >= map.rows - 1) return false;
        // Check if another square to left, right, top, or bottom.
        if(i == col && map.walling[j][i - 1] == ' ') return false; // Left.
        if(j == row && map.walling[j - 1][i] == ' ') return false; // Top.
        if(i == col + s - 1 && map.walling[j][i + 1] == ' ') return false; // Right.
        if(j == row + s - 1 && map.walling[j + 1][i] == ' ') return false; // Bottom.
        // Otherwise, check if current tile was carved.
        if(map.walling[j][i] == ' ') return false;
    }
    return true;
}

static void square(const Map map, const int row, const int col, const int s)
{
    for(int j = row; j < row + s; j++)
    for(int i = col; i < col + s; i++)
    {
        if(i == col && j == row) continue;
        if(i == col + s - 1 && j == row + s - 1) continue;
        if(i == col + s - 1 && j == row) continue;
        if(i == col && j == row + s - 1) continue;
        map.walling[j][i] = ' ';
    }
}

static void psquare(const Map map, const int y, const int x, const int s)
{
    if(scheck(map, y, x, s)) square(map, y, x, s);
}

static Map carved(const Map map, const Point where)
{
    square(map, where.x - 1, where.y - 1, 8);
    const int tries = 100;
    for(int i = 0; i < tries; i++)
    {
        const int y = rand() % map.rows;
        const int x = rand() % map.cols;
        const int s = rand() % 10 + 5;
        psquare(map, y, x, s);
    }
    for(int row = 0; row < map.rows; row++)
    for(int col = 0; col < map.cols; col++)
        printf("%c%s", map.walling[row][col], col == map.cols - 1 ? "\n" : "");
    return map;
}

Map xmgen(const Point where)
{
    Map map;
    xzero(map);
    map.rows = map.cols = 64;
    map.ceiling = mnew(map.rows, map.cols);
    map.walling = mnew(map.rows, map.cols);
    map.floring = mnew(map.rows, map.cols);
    return carved(map, where);
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
    return where.x >= map.cols || where.x < 0.0f
        || where.y >= map.rows || where.y < 0.0f;
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
