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
    field.anti = -1000.0;
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
        // Do not sum wall anti objects
        if(field.mesh[j][i] == field.anti)
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

static void box(const Field field, const int y, const int x, const int w)
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
    // Cleanup
    free(atoms);
}

static bool peak(float* const gradients, const int size)
{
    bool peaking = true;
    for(int i = 0; i < size - 1; i++)
    {
        // Do not compare zero gradients
        if(gradients[i + 0] == 0.0
        || gradients[i + 1] == 0.0)
            continue;
        if(gradients[i + 0] != gradients[i + 1])
            peaking = false;
    }
    return peaking;
}

static int largest(float* const gradients, const int size)
{
    float max = FLT_MIN;
    int index = 0;
    for(int i = 0; i < size; i++)
    {
        // Do not check zero gradients
        if(gradients[i] == 0.0)
            continue;
        if(gradients[i] > max)
            max = gradients[i], index = i;
    }
    return index;
}

Point force(const Field field, const Point from, const Point to)
{
    // Zero acceleration vector
    const Point dead = { 0.0, 0.0 };
    // Accleration vectors by direction
    const Point vectors[] = {
        {  1, -0 }, // E
        {  1,  1 }, // SE
        {  0,  1 }, // S
        { -1,  1 }, // SW
        { -1,  0 }, // W
        { -1, -1 }, // NW
        {  0, -1 }, // N
        {  1, -1 }, // NE
    };
    const int size = len(vectors);
    // Acceleration field gradients, one for each acceleration vector
    float gradients[size];
    zero(gradients);
    for(int i = 0; i < size; i++)
    {
        const Point dir = add(vectors[i], from);
        const int y = from.y, yy = dir.y;
        const int x = from.x, xx = dir.x;
        // Do not worry about calculating the gradients for anti objects
        if(field.mesh[yy][xx] == field.anti)
            continue;
        gradients[i] = field.mesh[yy][xx] - field.mesh[y][x];
    }
    // Once all gradients are found, return the zero acceleration vector
    // if <from> is at a a peak of the acceleration mesh,
    // or if <from> is close enough to the <to> destination
    return peak(gradients, size) || eql(to, from, 2.5) ? dead
        // Otherwise, return the acceleration vector of the largest gradient
        : vectors[largest(gradients, size)];
}

void diffuse(const Field field, const Point where)
{
    const int y = where.y;
    const int x = where.x;
    for(int w = 1; w < max(field.rows, field.cols); w++)
        box(field, y, x, w);
}

void deposit(const Field field, const Point p, const float val)
{
    const int y = p.y;
    const int x = p.x;
    field.mesh[y][x] += val;
}

void examine(const Field field)
{
    for(int j = 0; j < field.rows; j++)
    {
        for(int i = 0; i < field.cols; i++)
            printf("%10.4f", field.mesh[j][i]);
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
