#include "Path.h"

#include "util.h"

#include <string.h>

Path prepare(const Map map)
{
    Path path;
    path.rows = map.rows;
    path.cols = strlen(map.walling[0]);
    path.density = toss(float*, path.rows);
    path.clear = toss(int*, path.rows);
    for(int i = 0; i < path.rows; i++)
    {
        path.density[i] = wipe(float, path.cols);
        path.clear[i] = wipe(int, path.cols);
    }
    return path;
}

static void examine(const Path path)
{
    for(int i = 0; i < path.rows; i++)
    {
        for(int j = 0; j < path.cols; j++)
            printf("%5.1f", path.density[i][j]);
        putchar('\n');
    }
    putchar('\n');
}

void ruin(const Path path)
{
    examine(path);
    for(int i = 0; i < path.rows; i++)
    {
        free(path.density[i]);
        free(path.clear[i]);
    }
    free(path.density);
    free(path.clear);
}
