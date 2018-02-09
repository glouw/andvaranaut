#include "Field.h"

#include "util.h"

#include <string.h>
#include <float.h>

Field xprepare(const Map map, const float aura)
{
    Field field;
    field.res = 2; /* Whatever feels best. */
    field.rows = field.res * map.rows;
    field.cols = field.res * map.cols;
    field.mesh = xtoss(float*, field.rows);
    field.aura = field.res * aura;
    for(int j = 0; j < field.rows; j++)
        field.mesh[j] = xwipe(float, field.cols);
    return field;
}

int xon(const Field field, const int y, const int x)
{
    return y >= 0 && x >= 0 && y < field.rows && x < field.cols;
}

static float average(const Field field, const int y, const int x)
{
    float sum = 0.0f;
    int sums = 0;
    for(int j = y - 1; j <= y + 1; j++)
    for(int i = x - 1; i <= x + 1; i++)
    {
        // Out of bounds check.
        if(!xon(field, j, i))
            continue;
        // Do not sum middle of box.
        if(j == y && i == x)
            continue;
        sum += field.mesh[j][i];
        sums++;
    }
    return sum / sums;
}

typedef struct
{
    int y; // Notice y and x are in reverse
    int x; // here, contrary to the Point type.
    float val;
}
Atom;

static Atom materialize(const Field field, const int y, const int x)
{
    const Atom atom = { y, x, average(field, y, x) };
    return atom;
}

static void box(const Field field, const int y, const int x, const int w)
{
    Atom* const atoms = xtoss(Atom, 8 * w);
    int count = 0;
    const int t = y - w; // Top.
    const int b = y + w; // Bottom.
    const int l = x - w; // Left.
    const int r = x + w; // Right.
    // Calculate diffusable atoms.
    for(int j = t; j <= b; j++)
    for(int i = l; i <= r; i++)
        // If on outside of box...
        if((i == l || j == t || i == r || j == b)
        // And on a walkable part of the field...
        && xon(field, j, i)
        // And no antiobject is in the way...
        && field.mesh[j][i] == 0.0f)
            // Then materialize an atom with the diffusion box average.
            atoms[count++] = materialize(field, j, i);
    // Transfer diffused atoms in one go.
    for(int a = 0; a < count; a++)
        field.mesh[atoms[a].y][atoms[a].x] = atoms[a].val;
    free(atoms);
}

static int largest(float* gradients, const int size)
{
    float max = -FLT_MAX;
    int index = 0;
    for(int i = 0; i < size; i++)
        if(gradients[i] > max)
            max = gradients[i], index = i;
    return index;
}

Point xforce(const Field field, const Point from, const Point to, const Map map)
{
    // Return the zero acceleration vector if the <from> point
    // is close enough to the destination <to> point or if it is far enough away.
    const Point dead = { 0.0f, 0.0f };
    const float dist = xmag(xsub(from, to));
    if(dist < 1.33f || dist > (field.aura / field.res) - 1)
        return dead;
    // Otherwise, calculate the accleration vectors by direction.
    const Point v[] = {
        { +1.0f, -0.0f }, // E
        { +1.0f, +1.0f }, // SE
        { +0.0f, +1.0f }, // S
        { -1.0f, +1.0f }, // SW
        { -1.0f, +0.0f }, // W
        { -1.0f, -1.0f }, // NW
        { +0.0f, -1.0f }, // N
        { +1.0f, -1.0f }, // NE
    };
    // And calculate the acceleration field gradients.
    float grads[xlen(v)];
    xzero(grads);
    for(int i = 0; i < xlen(v); i++)
    {
        const Point dir = xadd(v[i], from);
        const int y = field.res * from.y, yy = field.res * dir.y;
        const int x = field.res * from.x, xx = field.res * dir.x;
        if(xon(field, yy, xx)) grads[i] = field.mesh[yy][xx] - field.mesh[y][x];
    }
    /* TODO:
     * Check map here before moving.
     * Return dead if map wall is in the way. */
    const Point grad = v[largest(grads, xlen(v))];
    const Point where = xadd(grad, from);
    const int xx = where.x;
    const int yy = where.y;
    return map.walling[yy][xx] != ' ' ? dead : grad;
}

void xdiffuse(const Field field, const Point where)
{
    const int y = field.res * where.y;
    const int x = field.res * where.x;
    for(int w = 1; w <= field.aura; w++)
        box(field, y, x, w);
}

void xexamine(const Field field)
{
    for(int j = 0; j < field.rows; j++)
    {
        for(int i = 0; i < field.cols; i++)
            printf("%13.3f", (double) field.mesh[j][i]);
        putchar('\n');
    }
    putchar('\n');
}

void xruin(const Field field)
{
    for(int j = 0; j < field.rows; j++)
        free(field.mesh[j]);
    free(field.mesh);
}
