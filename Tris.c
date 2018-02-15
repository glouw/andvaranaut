#include "Tris.h"

#include "Points.h"
#include "Direction.h"
#include "Map.h"
#include "Flags.h"
#include "util.h"

static Tris tsnew(const int max)
{
    const Tris ts = { xtoss(Tri, max), 0, max };
    return ts;
}

static Tris tsadd(Tris tris, const Tri tri, const char* why)
{
    if(tris.count == tris.max)
        xbomb("tris size limitation reached: %s\n", why);
    tris.tri[tris.count++] = tri;
    return tris;
}

// Verbose pixel equal.
static int peql(const Point a, const Point b)
{
    return a.x == b.x && a.y == b.y;
}

static int incircum(const Tri t, const Point p)
{
    const float ax = t.a.x - p.x;
    const float ay = t.a.y - p.y;
    const float bx = t.b.x - p.x;
    const float by = t.b.y - p.y;
    const float cx = t.c.x - p.x;
    const float cy = t.c.y - p.y;
    const float det =
        (ax * ax + ay * ay) * (bx * cy - cx * by) -
        (bx * bx + by * by) * (ax * cy - cx * ay) +
        (cx * cx + cy * cy) * (ax * by - bx * ay);
    return det > 0.0f;
}

// Collects all edges from given triangles.
static Tris ecollect(Tris edges, const Tris in, const Flags flags)
{
    for(int i = 0; i < in.count; i++)
    {
        const Tri tri = in.tri[i];
        const Tri ab = { tri.a, tri.b, flags.zer };
        const Tri bc = { tri.b, tri.c, flags.zer };
        const Tri ca = { tri.c, tri.a, flags.zer };
        edges = tsadd(edges, ab, "ab");
        edges = tsadd(edges, bc, "bc");
        edges = tsadd(edges, ca, "ca");
    }
    return edges;
}

// Returns true if edge ab of two triangles are alligned.
static int alligned(const Tri a, const Tri b)
{
    return (peql(a.a, b.a) && peql(a.b, b.b)) || (peql(a.a, b.b) && peql(a.b, b.a));
}

// Flags alligned edges.
static void emark(Tris edges, const Flags flags)
{
    for(int i = 0; i < edges.count; i++)
    {
        const Tri edge = edges.tri[i];
        for(int j = 0; j < edges.count; j++)
        {
            if(i == j)
                continue;
            const Tri other = edges.tri[j];
            if(alligned(edge, other))
                edges.tri[j].c = flags.one;
        }
    }
}

// Creates new triangles from unique edges and appends to tris.
static Tris ejoin(Tris tris, const Tris edges, const Point p, const Flags flags)
{
    for(int j = 0; j < edges.count; j++)
    {
        const Tri edge = edges.tri[j];
        if(peql(edge.c, flags.zer))
        {
            const Tri tri = { edge.a, edge.b, p };
            tris = tsadd(tris, tri, "ejoin tsadd problem");
        }
    }
    return tris;
}

static int outob(const Point p, const int w, const int h)
{
    return p.x < 0 || p.y < 0 || p.x >= w || p.y >= h;
}

static Tris delaunay(const Points ps, const int w, const int h, const int max, const Flags flags)
{
    Tris in = tsnew(max);
    Tris out = tsnew(max);
    Tris tris = tsnew(max);
    Tris edges = tsnew(max);
    // Shallow copies are exploited here for quick array concatentations.
    // In doing so, the original tris triangle is lost. This dummy pointer
    // will keep track of it for freeing at a later date.
    Tri* dummy = tris.tri;
    // The super triangle will snuggley fit over the screen.
    const Tri super = { { (float) -w, 0.0f }, { 2.0f * w, 0.0f }, { w / 2.0f, 2.0f * h } };
    tris = tsadd(tris, super, "delaunay tsadd problem with super");
    for(int j = 0; j < ps.count; j++)
    {
        in.count = out.count = edges.count = 0;
        const Point p = ps.point[j];
        // For all triangles...
        for(int i = 0; i < tris.count; i++)
        {
            const Tri tri = tris.tri[i];
            // Get triangles where point lies inside their circumcenter...
            if(incircum(tri, p))
                in = tsadd(in, tri, "delaunay tsadd problem with in");
            // And get triangles where point lies outside of their circumcenter.
            else out = tsadd(out, tri, "delaunay tsadd problem with out");
        }
        // Collect all triangle edges where point was inside circumcenter.
        edges = ecollect(edges, in, flags);
        // Flag edges that are non-unique.
        emark(edges, flags);
        // Construct new triangles with unique edges.
        out = ejoin(out, edges, p, flags);
        // Update triangle list.
        // FAST SHALLOW COPY - ORIGINAL POINTER LOST.
        tris = out;
    }
    free(dummy);
    free(in.tri);
    free(edges.tri);
    return tris;
}

// Random points.
static Points prand(const int w, const int h, const int max, const int grid, const int border, const Points misc)
{
    const int a = 0;
    const int b = misc.count;
    Points ps = xpsnew(max);
    // Copy over misc points first.
    for(int i = a; i < b; i++)
        ps = xpsadd(ps, misc.point[i], "");
    for(int i = b; i < max; i++)
    {
        const Point p = {
            (float) (rand() % (w - border) + border / 2),
            (float) (rand() % (h - border) + border / 2),
        };
        ps.point[ps.count++] = xsnap(p, grid);
    }
    return ps;
}

static float len(const Tri edge)
{
    return xmag(xsub(edge.b, edge.a));
}

static int descending(const void* a, const void* b)
{
    const Tri ea = *(const Tri*) a;
    const Tri eb = *(const Tri*) b;
    return len(ea) < len(eb) ? 1 : len(ea) > len(eb) ? -1 : 0;
}

static void sort(const Tris edges, const Direction direction)
{
    qsort(edges.tri, edges.count, sizeof(Tri), direction);
}

static int psfind(const Points ps, const Point p)
{
    for(int i = 0; i < ps.count; i++)
        if(peql(ps.point[i], p))
            return true;
    return false;
}

static int connected(const Point a, const Point b, const Tris edges, const Flags flags)
{
    Points todo = xpsnew(edges.max);
    Points done = xpsnew(edges.max);
    Tris reach = tsnew(edges.max);
    todo = xpsadd(todo, a, "first todo");
    int connection = false;
    while(todo.count != 0 && connection != true)
    {
        const Point removed = todo.point[--todo.count];
        done = xpsadd(done, removed, "done a point");
        // Get reachable edges from current point.
        reach.count = 0;
        for(int i = 0; i < edges.count; i++)
        {
            const Tri edge = edges.tri[i];
            if(peql(edge.c, flags.one))
                continue;
            if(peql(edge.a, removed))
                reach = tsadd(reach, edge, "connected tsadd problem with reach");
        }
        // For all reachable edges
        for(int i = 0; i < reach.count; i++)
        {
            // Was the destination reached?
            if(peql(reach.tri[i].b, b))
            {
                connection = true;
                break;
            }
            // Otherwise add point of reachable edge to todo list.
            if(!psfind(done, reach.tri[i].b))
                todo = xpsadd(todo, reach.tri[i].b, "connected tsadd problem with todo");
        }
    }
    free(todo.point);
    free(reach.tri);
    free(done.point);
    return connection;
}

// Graph Theory Reverse Delete algorithm. Kruskal 1956.
static void revdel(Tris edges, const int w, const int h, const Flags flags)
{
    sort(edges, descending);
    for(int i = 0; i < edges.count; i++)
    {
        Tri* edge = &edges.tri[i];
        if(outob(edge->a, w, h)
        || outob(edge->b, w, h))
        {
            edge->c = flags.one;
            continue;
        }
        // Break the connection.
        edge->c = flags.one;
        // If two points are not connected in anyway then reconnect.
        // Occasionally it will create a loop because true connectivity
        // checks all edges. Thankfully, the occasional loop benefits
        // the dungeon design else the explorer will get bored dead end after dead end.
        if(!connected(edge->a, edge->b, edges, flags)) edge->c = flags.zer;
    }
}

static void mdups(const Tris edges, const Flags flags)
{
    for(int i = 0; i < edges.count; i++)
    for(int j = 0; j < edges.count; j++)
    {
        if(peql(edges.tri[j].c, flags.one))
            continue;
        if(peql(edges.tri[i].c, flags.one))
            continue;
        if(peql(edges.tri[i].a, edges.tri[j].b)
        && peql(edges.tri[i].b, edges.tri[j].a))
            edges.tri[j].c = flags.one;
    }
}

// ####################################### This is what a bone looks like
// #  r   ################################ when generated from an edge.
// #  o   #####################          # This here is for wallings, but
// #  o      c o r r i d o r    r o o m  # apply a chance percentage to the
// #  m   #####################          # ceiling and floorings to have the same
// #      ################################ thing happen.
// #######################################
static void bone(const Map map, const Tri e, const int w, const int h)
{
    xroom(map, e.a, w, h, WALLING);
    xroom(map, e.b, w, h, WALLING);
    if(xd2() == 0) xroom(map, e.a, w, h, CEILING);
    if(xd2() == 0) xroom(map, e.b, w, h, CEILING);
    if(xd8() == 0) xroom(map, e.a, w / 2, h / 2, FLORING);
    if(xd8() == 0) xroom(map, e.b, w / 2, h / 2, FLORING);
    xcorridor(map, e.a, e.b);
}

// Carve all bones from solid map.
static void carve(const Map map, const Tris edges, const Flags flags, const int grid)
{
    for(int i = 0; i < edges.count; i++)
    {
        const Tri e = edges.tri[i];
        if(peql(e.c, flags.one))
            continue;
        if(xout(map, e.a)) xbomb("map: point a was out of bounds in map carving");
        if(xout(map, e.b)) xbomb("map: point b was out of bounds in map carving");
        const int min = 2;
        const int size = grid / 2 - min;
        const int w = min + rand() % size;
        const int h = min + rand() % size;
        bone(map, e, w, h);
    }
}

Map xtgenerate(const Points misc)
{
    const int w = 160;
    const int h = 105;
    const int grid = 20;
    const int max = 20 * (1 + (xd2() == 0));
    // The triangle type is reused for edges by omitting the third point.
    // The third point is then reused for a flag. For duplication removal our out of bounds checks.
    const Flags flags = { { 0.0, 0.0 }, { 1.0, 1.0 } };
    const int border = 2 * grid;
    const Points ps = prand(w, h, max, grid, border, misc);
    const Tris tris = delaunay(ps, w, h, 9 * max, flags);
    const Tris edges = ecollect(tsnew(27 * max), tris, flags);
    revdel(edges, w, h, flags);
    const Points trapdoors = xpspop(ps, 5);
    const Map map = xmgen(h, w, trapdoors);
    mdups(edges, flags);
    carve(map, edges, flags, grid);
    free(tris.tri);
    free(ps.point);
    free(edges.tri);
    return map;
}
