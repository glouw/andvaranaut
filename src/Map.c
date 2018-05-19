#include "Map.h"

#include "Sprites.h"
#include "util.h"

#include <math.h>

Map xzmap()
{
    static Map map;
    return map;
}

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

static char** reset(char** block, const int rows, const int cols, const int blok)
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
    return reset(block, rows, cols, blok);
}

Map xmgen(const int rows, const int cols, const Points trapdoors, const Points interests, const int grid)
{
    const Sheer md = {
        0.0f,
        md.a + (float) (rand() % 2)
    };
    const Sheer up = {
        md.b + 1.0f,
        up.a + (float) (rand() % 2)
    };

    Map map = xzmap();
    map.rows = rows;
    map.cols = cols;
    map.ceiling = mnew(map.rows, map.cols, '#');
    map.walling = mnew(map.rows, map.cols, '#');
    map.floring = mnew(map.rows, map.cols, '"');
    map.trapdoors = trapdoors;
    map.interests = interests;
    map.upper = up;
    map.middle = md;
    map.grid = grid;

    return map;
}

void xmclose(const Map map)
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

int xmisportal(char** block, const Point where)
{
    return xblok(where, block) == '~';
}

int xmout(const Map map, const Point where)
{
    return (int) where.x >= map.cols || (int) where.x < 0
        || (int) where.y >= map.rows || (int) where.y < 0;
}

void xmedit(const Map map, const Overview ov)
{
    // Placing - Out of bounds check.
    if(xmout(map, ov.where))
        return;
    const int ascii = ov.selected + ' ';
    // If the ascii is an alpha character then it is a sprite
    if(xsissprite(ascii))
        return;
    // Otherwise place the ascii character.
    const int x = ov.where.x;
    const int y = ov.where.y;
    if(ov.party == FLORING) map.floring[y][x] = ascii;
    if(ov.party == WALLING) map.walling[y][x] = ascii;
    if(ov.party == CEILING) map.ceiling[y][x] = ascii;
}

void xmroom(const Map map, const Point where, const int w, const int h, const Party p)
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
            xbomb("room: party not supported\n");
            break;
        }
    }
}

static void supports(const Map map, const int x, const int y)
{
    map.walling[y + 1][x + 1] = map.floring[y + 1][x + 1] =
    map.walling[y + 1][x - 1] = map.floring[y + 1][x - 1] =
    map.walling[y - 1][x - 1] = map.floring[y - 1][x - 1] =
    map.walling[y - 1][x + 1] = map.floring[y - 1][x + 1] = '#';
}

static void platform(const Map map, const int x, const int y, const Party p)
{
    for(int j = -1; j <= 1; j++)
    for(int k = -1; k <= 1; k++)
    {
        const int yy = j + y;
        const int xx = k + x;
        switch(p)
        {
        case FLORING: map.floring[yy][xx] = '"'; break;
        case CEILING: map.ceiling[yy][xx] = '#'; break;
        default:
            xbomb("platform: party not supported\n");
            break;
        }
    }
}

static void trapdoor(const Map map, const int x, const int y, const Party p)
{
    switch(p)
    {
    case FLORING: map.floring[y][x] = '~'; break;
    case CEILING: map.ceiling[y][x] = '~'; break;
    default:
        xbomb("trapdoor: party not supported\n");
        break;
    }
}

void xmtrapdoors(const Map map, const Points trapdoors, const Party p)
{
    for(int i = 0; i < trapdoors.count; i++)
    {
        const Point where = trapdoors.point[i];
        const int x = where.x;
        const int y = where.y;
        platform(map, x, y, p);
        supports(map, x, y);
        trapdoor(map, x, y, p);
    }
}

void xmcorridor(const Map map, const Point a, const Point b)
{
    const Point step = xsub(b, a);
    const Point delta = {
        step.x > 0.0f ? 1.0f : step.x < 0.0f ? -1.0f : 0.0f,
        step.y > 0.0f ? 1.0f : step.y < 0.0f ? -1.0f : 0.0f,
    };
    const int sx = fabsf(step.x);
    const int sy = fabsf(step.y);
    const int dx = delta.x;
    const int dy = delta.y;
    int x = a.x;
    int y = a.y;
    for(int i = 0; i < sx; i++) map.walling[y][x += dx] = ' ';
    for(int i = 0; i < sy; i++) map.walling[y += dy][x] = ' ';
}
