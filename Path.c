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

static bool on(const Path path, const int y, const int x)
{
    return y >= 0 && x >= 0 && y < path.rows && x < path.cols && path.clear[y][x];
}

static float boxavg(const Path path, const int y, const int x)
{
    float sum = 0.0;
    for(int j = y - 1; j <= y + 1; j++)
    for(int i = x - 1; i <= x + 1; i++)
    {
        // Out of bounds check
        if(!on(path, j, i))
            continue;
        // Do not sum middle of box
        if(j == y && i == x)
            continue;
        sum += path.density[j][i];
    }
    return sum / 8.0;
}

typedef struct
{
    int y; // Notice y and x are in reverse
    int x; // here, contrary to the Point type.
    float val;
}
Atom;

static Atom materialize(const Path path, const int j, const int i)
{
    Atom atom = { j, i, boxavg(path, j, i) };
    return atom;
}

static void boxrun(const Path path, const int y, const int x, const int w)
{
    Atom* const atoms = toss(Atom, 8 * w);
    int count = 0;
    const int t = y - w; // Top
    const int b = y + w; // Bottom
    const int l = x - w; // Left
    const int r = x + w; // Right
    // Calculate diffusable atoms
    for(int j = t; j <= b; j++)
    for(int i = l; i <= r; i++)
        // If on outside of box...
        if((i == l || j == t || i == r || j == b)
        // And on a walkable part of the path...
        && on(path, j, i)
        // And no antiobject is in the way...
        && path.density[j][i] == 0.0)
            // Then materialize an atom with the diffusion box average
            atoms[count++] = materialize(path, j, i);
    // Transfer diffused atoms in one go
    for(int a = 0; a < count; a++)
        path.density[atoms[a].y][atoms[a].x] = atoms[a].val;
    free(atoms);
}

void diffuse(const Path path, const int y, const int x)
{
    for(int w = 1; w < max(path.rows, path.cols); w++)
        boxrun(path, y, x, w);
}

// Needs some sort of spline calculations for a litle more realism
Point way(const Path path, const Point to, const Point from)
{
    // Reached
    if(eql(to, from, 1.0))
        return pt(0, 0);
    // Else, try all immediate directions
    const Point points[] = {
        add(from, pt(+1, -0)), // E
        add(from, pt(+1, +1)), // SE
        add(from, pt(+0, +1)), // S
        add(from, pt(-1, +1)), // SW
        add(from, pt(-1, +0)), // W
        add(from, pt(-1, -1)), // NW
        add(from, pt(+0, -1)), // N
        add(from, pt(+1, -1)), // NE
    };
    float max = 0.0;
    int index = 0;
    for(int i = 0; i < len(points); i++)
    {
        const int yy = points[i].y, y = from.y;
        const int xx = points[i].x, x = from.x;
        const float gradient = path.density[yy][xx] - path.density[y][x];
        if(path.clear[yy][xx] && gradient > max)
        {
            max = gradient;
            index = i;
        }
    }
    return unt(sub(points[index], from));
}

void examine(const Path path)
{
    for(int i = 0; i < path.rows; i++)
    {
        for(int j = 0; j < path.cols; j++)
            printf("%7.4f", path.density[i][j]);
        putchar('\n');
    }
    putchar('\n');
}

void ruin(const Path path)
{
    for(int i = 0; i < path.rows; i++)
    {
        free(path.density[i]);
        free(path.clear[i]);
    }
    free(path.density);
    free(path.clear);
}
