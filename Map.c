#include "Map.h"

#include "Sprites.h"

#include "util.h"

void xmprint(char** block, const int rows, const int cols)
{
    for(int row = 0; row < rows; row++)
    for(int col = 0; col < cols; col++)
        printf("%c%s", block[row][col], col == cols - 1 ? "\n" : "");
    putchar('\n');
}

void xmdump(const Map map)
{
    xmprint(map.ceiling, map.rows, map.cols);
    xmprint(map.walling, map.rows, map.cols);
    xmprint(map.floring, map.rows, map.cols);
}

static char** mreset(char** block, const int rows, const int cols, const int blok)
{
    for(int row = 0; row < rows; row++)
    for(int col = 0; col < cols; col++)
        block[row][col] = blok;
    return block;
}

static char** mnew(const int rows, const int cols, const int blok)
{
    char** block = xtoss(char*, rows);
    for(int row = 0; row < rows; row++)
        block[row] = xtoss(char, cols);
    return mreset(block, rows, cols, blok);
}

Map xmgen(const int rows, const int cols, const Points trapdoors)
{
    Map map;
    xzero(map);
    map.rows = rows;
    map.cols = cols;
    map.ceiling = mnew(map.rows, map.cols, '#');
    map.walling = mnew(map.rows, map.cols, '#');
    map.floring = mnew(map.rows, map.cols, '"');
    map.trapdoors = trapdoors;
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
    if(xout(map, ov.where))
        return;
    const int ascii = ov.selected + ' ';
    // If the ascii is an alpha character then it is a sprite
    if(xissprite(ascii))
        return;
    // Otherwise place the ascii character.
    const int x = ov.where.x;
    const int y = ov.where.y;
    if(ov.party == FLORING) map.floring[y][x] = ascii;
    if(ov.party == WALLING) map.walling[y][x] = ascii;
    if(ov.party == CEILING) map.ceiling[y][x] = ascii;
}

void xroom(const Map map, const Point where, const int w, const int h, const Party p)
{
    for(int i = -w; i <= w; i++)
    for(int j = -h; j <= h; j++)
    {
        const int xx = where.x + i;
        const int yy = where.y + j;
        switch(p)
        {
        case WALLING: map.walling[yy][xx] = ' '; break;
        case CEILING: map.ceiling[yy][xx] = ' '; break;
        case FLORING: map.floring[yy][xx] = ' '; break;
        default:
            break;
        }
    }
}

static void platform(const Map map, const int x, const int y, const Party party)
{
    for(int j = -1; j <= 1; j++)
    for(int k = -1; k <= 1; k++)
    {
        const int yy = j + y;
        const int xx = k + x;
        switch(party)
        {
        case FLORING: map.floring[yy][xx] = '"'; break;
        case CEILING: map.ceiling[yy][xx] = '#'; break;
        default:
            xbomb("platform: party not supported\n");
            break;
        }
    }
}

static void trapdoor(const Map map, const int x, const int y, const Party party)
{
    switch(party)
    {
    case FLORING: map.floring[y][x] = '~'; break;
    case CEILING: map.ceiling[y][x] = '~'; break;
    default:
        xbomb("trapdoor: party not supported\n");
        break;
    }
}

void xtrapdoors(const Map map, const Points trapdoors, const Party party)
{
    for(int i = 0; i < trapdoors.count; i++)
    {
        const Point where = trapdoors.point[i];
        const int x = where.x;
        const int y = where.y;
        platform(map, x, y, party);
        trapdoor(map, x, y, party);
    }
}

void xcorridor(const Map map, const Point a, const Point b)
{
    const Point step = xsub(b, a);
    const Point delta = {
        step.x > 0.0f ? 1.0f : step.x < 0.0f ? -1.0f : 0.0f,
        step.y > 0.0f ? 1.0f : step.y < 0.0f ? -1.0f : 0.0f,
    };
    const int sx = abs(step.x);
    const int sy = abs(step.y);
    const int dx = delta.x;
    const int dy = delta.y;
    int x = a.x;
    int y = a.y;
    for(int i = 0; i < sx; i++) map.walling[y][x += dx] = ' ';
    for(int i = 0; i < sy; i++) map.walling[y += dy][x] = ' ';
}
