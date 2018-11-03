#include "Field.h"

#include "Atom.h"
#include "util.h"

#include <string.h>
#include <float.h>

Field f_prepare(const Map map, const float aura)
{
    static Field zero;
    Field field = zero;
    field.res = 2;
    field.rows = field.res * map.rows;
    field.cols = field.res * map.cols;
    field.mesh = xtoss(float*, field.rows);
    field.aura = field.res * aura;
    for(int j = 0; j < field.rows; j++)
        field.mesh[j] = xtoss(float, field.cols);
    return field;
}

int f_on(const Field field, const int y, const int x)
{
    return y >= 0 && x >= 0 && y < field.rows && x < field.cols;
}

static void box(const Field field, const int y, const int x, const int w)
{
    Atom* const atoms = xtoss(Atom, 8 * w);
    int count = 0;
    const int t = y - w; // Top.
    const int b = y + w; // Bottom.
    const int l = x - w; // Left.
    const int r = x + w; // Right.
    for(int j = t; j <= b; j++)
    for(int i = l; i <= r; i++)
        if((i == l || j == t || i == r || j == b)
        && f_on(field, j, i)
        && field.mesh[j][i] == 0.0f)
            atoms[count++] = a_materialize(field, j, i);
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

Point f_force(const Field field, const Point from, const Point to, const Map map)
{
    const Point dead = { 0.0f, 0.0f };
    const float dist = p_mag(p_sub(from, to));
    if(dist < 1.33f || dist > field.aura / field.res)
        return dead;
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
    const int len = xlen(v);
    float grads[len];
    for(int i = 0; i < len; i++)
        grads[i] = 0.0f;
    for(int i = 0; i < len; i++)
    {
        const Point dir = p_add(v[i], from);
        const int y = field.res * from.y, yy = field.res * dir.y;
        const int x = field.res * from.x, xx = field.res * dir.x;
        if(f_on(field, yy, xx)) grads[i] = field.mesh[yy][xx] - field.mesh[y][x];
    }
    const Point grad = v[largest(grads, len)];
    const Point where = p_add(grad, from);
    const int xx = where.x;
    const int yy = where.y;
    return map.walling[yy][xx] != ' ' ? dead : grad;
}

void f_diffuse(const Field field, const Point where)
{
    const int y = field.res * where.y;
    const int x = field.res * where.x;
    for(int w = 1; w <= field.aura; w++)
        box(field, y, x, w);
}

void f_examine(const Field field)
{
    for(int j = 0; j < field.rows; j++)
    {
        for(int i = 0; i < field.cols; i++)
            printf("%d ", field.mesh[j][i] > 0.0f);
        putchar('\n');
    }
    putchar('\n');
}

void f_ruin(const Field field)
{
    for(int j = 0; j < field.rows; j++)
        free(field.mesh[j]);
    free(field.mesh);
}
