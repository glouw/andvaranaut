#include "Field.h"

#include "util.h"

#include <string.h>
#include <float.h>

Field prepare(const Map map)
{
    Field field;
    field.rows = map.rows;
    field.cols = map.cols;
    field.mesh = toss(float*, field.rows);
    for(int j = 0; j < field.rows; j++)
        field.mesh[j] = wipe(float, field.cols);
    return field;
}

static bool on(const Field field, const int y, const int x)
{
    return y >= 0 && x >= 0 && y < field.rows && x < field.cols;
}

static float boxavg(const Field field, const int y, const int x)
{
    float sum = 0.0;
    for(int j = y - 1; j <= y + 1; j++)
    for(int i = x - 1; i <= x + 1; i++)
    {
        // Out of bounds check
        if(!on(field, j, i))
            continue;
        // Do not sum middle of box
        if(j == y && i == x)
            continue;
        sum += field.mesh[j][i];
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

static Atom materialize(const Field field, const int j, const int i)
{
    Atom atom = { j, i, boxavg(field, j, i) };
    return atom;
}

static void boxrun(const Field field, const int y, const int x, const int w)
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
        // And on a walkable part of the field...
        && on(field, j, i)
        // And no antiobject is in the way...
        && field.mesh[j][i] == 0.0)
            // Then materialize an atom with the diffusion box average
            atoms[count++] = materialize(field, j, i);
    // Transfer diffused atoms in one go
    for(int a = 0; a < count; a++)
        field.mesh[atoms[a].y][atoms[a].x] = atoms[a].val;
    free(atoms);
}

void diffuse(const Field field, const int y, const int x)
{
    for(int w = 1; w < max(field.rows, field.cols); w++) boxrun(field, y, x, w);
}

Point force(const Field field, const Point p)
{
    const Point points[] = {
        {  1, -0 }, // E
        {  1,  1 }, // SE
        {  0,  1 }, // S
        { -1,  1 }, // SW
        { -1,  0 }, // W
        { -1, -1 }, // NW
        {  0, -1 }, // N
        {  1, -1 }, // NE
    };
    float max = -FLT_MAX;
    int index = 0;
    for(int i = 0; i < len(points); i++)
    {
        const Point dir = add(points[i], p);
        const int y = p.y, yy = dir.y;
        const int x = p.x, xx = dir.x;
        const float gradient = field.mesh[yy][xx] - field.mesh[y][x];
        if(gradient > max) max = gradient, index = i;
    }
    return points[index];
}

void examine(const Field field)
{
    for(int j = 0; j < field.rows; j++)
    {
        for(int i = 0; i < field.cols; i++)
            printf("%7.4f", field.mesh[j][i]);
        putchar('\n');
    }
    putchar('\n');
}

void ruin(const Field field)
{
    for(int j = 0; j < field.rows; j++)
        free(field.mesh[j]);
    free(field.mesh);
}
