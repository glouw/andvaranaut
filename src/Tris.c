#include "Tris.h"

#include "Points.h"
#include "Map.h"
#include "Flags.h"
#include "util.h"

static Tris tsnew(const int max)
{
    const Tris ts = { xtoss(Tri, max), 0, max };
    return ts;
}

static Tris tsadd(Tris tris, const Tri tri)
{
    if(tris.count == tris.max)
        xbomb("tris size limitation reached\n");

    tris.tri[tris.count++] = tri;
    return tris;
}

static int reveql(const Tri a, const Tri b)
{
    return xpsame(a.a, b.b) && xpsame(a.b, b.a);
}

static int foreql(const Tri a, const Tri b)
{
    return xpsame(a.a, b.a) && xpsame(a.b, b.b);
}

static int alligned(const Tri a, const Tri b)
{
    return foreql(a, b) || reveql(a, b);
}

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

static Tris ejoin(Tris tris, const Tris edges, const Point p, const Flags flags)
{
    for(int j = 0; j < edges.count; j++)
    {
        const Tri edge = edges.tri[j];
        if(xpsame(edge.c, flags.zer))
        {
            const Tri tri = { edge.a, edge.b, p };
            tris = tsadd(tris, tri);
        }
    }
    return tris;
}

static int outob(const Point p, const int w, const int h)
{
    return p.x < 0 || p.y < 0 || p.x >= w || p.y >= h;
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

static Tris ecollect(Tris edges, const Tris in, const Flags flags)
{
    for(int i = 0; i < in.count; i++)
    {
        const Tri tri = in.tri[i];
        const Tri ab = { tri.a, tri.b, flags.zer };
        const Tri bc = { tri.b, tri.c, flags.zer };
        const Tri ca = { tri.c, tri.a, flags.zer };
        edges = tsadd(edges, ab);
        edges = tsadd(edges, bc);
        edges = tsadd(edges, ca);
    }
    return edges;
}

static Tris delaunay(const Points ps, const int w, const int h, const int max, const Flags flags)
{
    Tris in = tsnew(max);
    Tris out = tsnew(max);
    Tris tris = tsnew(max);
    Tris edges = tsnew(max);

    // Note:
    // Shallow copies are exploited here for quick array concatentations.
    // In doing so, the original tris triangle is lost. This dummy pointer
    // will keep track of it for later freeing.
    Tri* dummy = tris.tri;

    // The super triangle will snuggley fit over the screen.
    const Tri super = { { (float) -w, 0.0f }, { 2.0f * w, 0.0f }, { w / 2.0f, 2.0f * h } };
    tris = tsadd(tris, super);
    for(int j = 0; j < ps.count; j++)
    {
        in.count = out.count = edges.count = 0;
        const Point p = ps.point[j];

        for(int i = 0; i < tris.count; i++)
        {
            const Tri tri = tris.tri[i];

            // Get triangles where point lies inside their circumcenter.
            if(incircum(tri, p))
                in = tsadd(in, tri);
            // Get triangles where point lies outside of their circumcenter.
            else out = tsadd(out, tri);
        }

        // Collect all triangle edges where point was inside circumcenter.
        edges = ecollect(edges, in, flags);

        // Flag edges that are non-unique.
        emark(edges, flags);

        // Construct new triangles with unique edges.
        out = ejoin(out, edges, p, flags);

        // Update triangle list. (See note above on shallow copying).
        tris = out;
    }
    free(dummy);
    free(in.tri);
    free(edges.tri);

    return tris;
}

static Points prand(const int w, const int h, const int max, const int grid, const int border, const Points extra)
{
    Points ps = xpsnew(max);
    ps = xpscat(ps, extra);

    for(int i = ps.count; i < ps.max; i++)
    {
        const Point p = {
            (float) (rand() % (w - border) + border / 2),
            (float) (rand() % (h - border) + border / 2),
        };
        const Point snapped = xmid(xsnap(p, grid));
        if(!xpsfind(ps, snapped))
            ps = xpsadd(ps, snapped);
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

static void sort(const Tris edges)
{
    qsort(edges.tri, edges.count, sizeof(Tri), descending);
}

// Returns true if two points are connected.
static int connected(const Point a, const Point b, const Tris edges, const Flags flags)
{
    Points todo = xpsnew(edges.max);
    Points done = xpsnew(edges.max);
    Tris reach = tsnew(edges.max);
    todo = xpsadd(todo, a);

    int connection = false;
    while(todo.count != 0 && connection != true)
    {
        const Point removed = todo.point[--todo.count];
        done = xpsadd(done, removed);

        // Get reachable edges from current point.
        reach.count = 0;
        for(int i = 0; i < edges.count; i++)
        {
            const Tri edge = edges.tri[i];

            if(xpsame(edge.c, flags.one))
                continue;

            if(xpsame(edge.a, removed))
                reach = tsadd(reach, edge);
        }

        // For all reachable edges
        for(int i = 0; i < reach.count; i++)
        {
            // Was the destination reached?
            if(xpsame(reach.tri[i].b, b))
            {
                connection = true;
                break;
            }
            // Otherwise add point of reachable edge to todo list.
            if(!xpsfind(done, reach.tri[i].b))
                todo = xpsadd(todo, reach.tri[i].b);
        }
    }
    free(todo.point);
    free(reach.tri);
    free(done.point);

    return connection;
}

// Reverse Delete (C) Kruskal 1956.
static void revdel(Tris edges, const int w, const int h, const Flags flags)
{
    sort(edges);
    for(int i = 0; i < edges.count; i++)
    {
        Tri* edge = &edges.tri[i];

        // Flag out of bound edges.
        if(outob(edge->a, w, h)
        || outob(edge->b, w, h))
        {
            edge->c = flags.one;
            continue;
        }

        // Break connection.
        edge->c = flags.one;

        // If two points are not connected in anyway then reconnect.
        // Occasionally a loop will be created as edges are doubled (sibling reversed)
        // and not perfectly paired when sorted by magnitude.
        // This benefits the map design; perfectly linear dungeons are boring.
        if(!connected(edge->a, edge->b, edges, flags)) edge->c = flags.zer;
    }
}

static void mdups(const Tris edges, const Flags flags)
{
    for(int i = 0; i < edges.count; i++)
    for(int j = 0; j < edges.count; j++)
    {
        if(xpsame(edges.tri[j].c, flags.one)) continue;
        if(xpsame(edges.tri[i].c, flags.one)) continue;

        if(reveql(edges.tri[i], edges.tri[j]))
            edges.tri[j].c = flags.one;
    }
}

// #############################################
// #           #####################           #
// #           #####################           #
// #  r o o m     c o r r i d o r     r o o m  #
// #           #####################           #
// #           #####################           #
// #############################################
static void bone(const Map map, const Tri e, const int w, const int h)
{
    xmroom(map, e.a, w, h, WALLING);
    xmroom(map, e.b, w, h, WALLING);

    if(xd2() == 0) xmroom(map, e.a, w, h, CEILING);
    if(xd2() == 0) xmroom(map, e.b, w, h, CEILING);

    xmcorridor(map, e.a, e.b);
}

static void carve(const Map map, const Tris edges, const Flags flags)
{
    for(int i = 0; i < edges.count; i++)
    {
        const Tri e = edges.tri[i];

        if(xpsame(e.c, flags.one))
            continue;

        const int size = xmrmax(map) - xmrmin(map);
        const int w = xmrmin(map) + rand() % size;
        const int h = xmrmin(map) + rand() % size;
        bone(map, e, w, h);
    }
}

Map xtgen(const Points extra)
{
    const int w = 150;
    const int h = 350;
    const int grid = 30;
    const int max = 20 * (1 + xd2());
    const int ntraps = 2;
    const int border = 2 * grid;

    if(xodd(grid))
        xbomb("Grid size must be even\n");

    const Flags flags = {
        { 0.0, 0.0 },
        { 1.0, 1.0 },
    };

    Points rooms = prand(w, h, max, grid, border, extra);
    const Tris tris = delaunay(rooms , w, h, 9 * max, flags);
    const Tris edges = ecollect(tsnew(27 * max), tris, flags);
    revdel(edges, w, h, flags);

    // Randomly select which rooms will become trapdoors.
    Points trapdoors = xpsnew(ntraps);
    for(int i = 0; i < ntraps; i++)
        trapdoors = xpsadduq(trapdoors, rooms);

    // Build a map with trapdoors and carve out rooms.
    const Map map = xmgen(h, w, trapdoors, rooms, grid);
    mdups(edges, flags);
    carve(map, edges, flags);

    free(tris.tri);
    free(edges.tri);

    return map;
}
