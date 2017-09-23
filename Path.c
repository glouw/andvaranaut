#include "Path.h"

#include "util.h"

#include <string.h>

Path prepare(const Map map)
{
    Path path;
    path.rows = map.rows;
    path.cols = strlen(map.walling[0]);
    path.hill = toss(float*, path.rows);
    for(int i = 0; i < path.rows; i++)
        path.hill[i] = toss(float, path.cols);
    return path;
}

static void examine(const Path path)
{
    for(int i = 0; i < path.rows; i++) {
    for(int j = 0; j < path.cols; j++)
        printf("%5.1f", path.hill[i][j]);
    putchar('\n');
    }
}

#if 0
static float sum(const Path path, const int x, const int y)
{
    return 0.0;
}

static void box(const Path path, const int x, const int y)
{
}
#endif

// Collaborative diffusion
void find(const Path path, const Map map, const Hero hero, const Sprites sprites)
{
    // Mark wall anti-objects as negative values. Mark free space as zero
    for(int i = 0; i < path.rows; i++)
    for(int j = 0; j < path.cols; j++)
        path.hill[i][j] = map.walling[i][j] - ' ' > 0 ? -1.0 : 0.0;
    // Mark sprite anti-objects as negative values
    for(int i = 0; i < sprites.count; i++)
    {
        const int x = sprites.sprite[i].where.x;
        const int y = sprites.sprite[i].where.y;
        path.hill[y][x] = -1.0;
    }
    // Mark the hero object as a postive value
    const int x = hero.where.x;
    const int y = hero.where.y;
    path.hill[y][x] = 1.0;
}

void ruin(const Path path)
{
    examine(path);
    for(int i = 0; i < path.rows; i++)
        free(path.hill[i]);
    free(path.hill);
}

Path redo(const Path path, const Map map)
{
    ruin(path);
    return prepare(map);
}
