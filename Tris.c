#include "Tris.h"

#include "Points.h"
#include "Direction.h"
#include "Map.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

const Point zer = { 0.0f, 0.0f };

const Point one = { 1.0f, 1.0f };

static Tris tsnew(const int max)
{
    const Tris ts = { (Tri*) malloc(sizeof(Tri) * max), 0, max };
    return ts;
}

static Tris tsadd(Tris tris, const Tri tri, const char* why)
{
    if(tris.count == tris.max)
    {
        printf("tris size limitation reached: %s\n", why);
        exit(1);
    }
    static int flag;
    if(flag == 0 && tris.count / (float) tris.max > 0.75f)
    {
        printf("warning: tris size reaching 75%% capacity: %s\n", why);
        flag = 1;
    }
    tris.tri[tris.count++] = tri;
    return tris;
}

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
static Tris ecollect(Tris edges, const Tris in)
{
    for(int i = 0; i < in.count; i++)
    {
        const Tri tri = in.tri[i];
        const Tri ab = { tri.a, tri.b, zer };
        const Tri bc = { tri.b, tri.c, zer };
        const Tri ca = { tri.c, tri.a, zer };
        edges = tsadd(edges, ab, "ab");
        edges = tsadd(edges, bc, "bc");
        edges = tsadd(edges, ca, "ca");
    }
    return edges;
}

// Returns true if edge ab of two triangles are alligned.
static int alligned(const Tri a, const Tri b)
{
    return (peql(a.a, b.a) && peql(a.b, b.b))
        || (peql(a.a, b.b) && peql(a.b, b.a));
}

// Flags alligned edges.
static void emark(Tris edges)
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
                edges.tri[j].c = one;
        }
    }
}

// Creates new triangles from unique edges and appends to tris.
static Tris ejoin(Tris tris, const Tris edges, const Point p)
{
    for(int j = 0; j < edges.count; j++)
    {
        const Tri edge = edges.tri[j];
        if(peql(edge.c, zer))
        {
            const Tri tri = { edge.a, edge.b, p };
            tris = tsadd(tris, tri, "ejoin");
        }
    }
    return tris;
}

static int outob(const Point p, const int w, const int h)
{
    return p.x < 0 || p.y < 0 || p.x >= w || p.y >= h;
}

static Tris delaunay(const Points ps, const int w, const int h, const int max)
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
    tris = tsadd(tris, super, "super");
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
                in = tsadd(in, tri, "in");
            // And get triangles where point lies outside of their circumcenter.
            else out = tsadd(out, tri, "out");
        }
        // Collect all triangle edges where point was inside circumcenter.
        edges = ecollect(edges, in);
        // Flag edges that are non-unique.
        emark(edges);
        // Construct new triangles with unique edges.
        out = ejoin(out, edges, p);
        // Update triangle list.
        // FAST SHALLOW COPY - ORIGINAL POINTER LOST.
        tris = out;
    }
    free(dummy);
    free(in.tri);
    free(edges.tri);
    return tris;
}

static Points psnew(const int max)
{
    const Points ps = { (Point*) malloc(sizeof(Point) * max), 0, max };
    return ps;
}

static Points psadd(Points ps, const Point p, const char* why)
{
    if(ps.count == ps.max)
    {
        printf("points size limitation reached: %s\n", why);
        exit(1);
    }
    ps.point[ps.count++] = p;
    return ps;
}

static Points prand(const int w, const int h, const int max, const int grid, const int border)
{
    Points ps = psnew(max);
    for(int i = 0; i < max; i++)
    {
        const Point p = {
            (float) (rand() % (w - border) + border / 2),
            (float) (rand() % (h - border) + border / 2),
        };
        const Point snapped = {
            roundf(p.x / grid) * grid,
            roundf(p.y / grid) * grid,
        };
        ps.point[ps.count++] = snapped;
    }
    return ps;
}

static Point sub(const Point a, const Point b)
{
    Point out;
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    return out;
}

static float mag(const Point a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}

static float len(const Tri edge)
{
    return mag(sub(edge.b, edge.a));
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
            return 1;
    return 0;
}

static int connected(const Point a, const Point b, const Tris edges)
{
    Points todo = psnew(edges.max);
    Points done = psnew(edges.max);
    Tris reach = tsnew(edges.max);
    todo = psadd(todo, a, "first todo");
    int connection = 0;
    while(todo.count != 0 && connection != 1)
    {
        const Point removed = todo.point[--todo.count];
        done = psadd(done, removed, "done a point");
        // Get reachable edges from current point.
        reach.count = 0;
        for(int i = 0; i < edges.count; i++)
        {
            const Tri edge = edges.tri[i];
            if(peql(edge.c, one))
                continue;
            if(peql(edge.a, removed))
                reach = tsadd(reach, edge, "reach");
        }
        // For all reachable edges
        for(int i = 0; i < reach.count; i++)
        {
            // Destination reached.
            if(peql(reach.tri[i].b, b))
            {
                connection = 1;
                break;
            }
            // Otherwise add todo list.
            if(!psfind(done, reach.tri[i].b))
                todo = psadd(todo, reach.tri[i].b, "todo reachable");
        }
    }
    free(todo.point);
    free(reach.tri);
    free(done.point);
    return connection;
}

static void revdel(Tris edges, const int w, const int h)
{
    sort(edges, descending);
    for(int i = 0; i < edges.count; i++)
    {
        Tri* edge = &edges.tri[i];
        if(outob(edge->a, w, h)
        || outob(edge->b, w, h))
        {
            edge->c = one;
            continue;
        }
        // Break the connection.
        edge->c = one;
        // If two points are not connected in anyway then reconnect.
        // Occasionally it will create a loop because true connectivity
        // checks all edges. Thankfully, the occasional loop benefits
        // the dungeon design else the explorer will get bored dead end after dead end.
        if(!connected(edge->a, edge->b, edges)) edge->c = zer;
    }
}

static void mdups(const Tris edges)
{
    for(int i = 0; i < edges.count; i++)
    for(int j = 0; j < edges.count; j++)
    {
        if(peql(edges.tri[j].c, one))
            continue;
        if(peql(edges.tri[i].c, one))
            continue;
        if(peql(edges.tri[i].a, edges.tri[j].b)
        && peql(edges.tri[i].b, edges.tri[j].a))
            edges.tri[j].c = one;
    }
}

static void carve(const Map map, const Tris edges, const int scale)
{
    for(int i = 0; i < edges.count; i += 1)
    {
        const Tri e = edges.tri[i];
        if(peql(e.c, one))
            continue;
        const int x0 = e.a.x / scale;
        const int y0 = e.a.y / scale;
        const int x1 = e.b.x / scale;
        const int y1 = e.b.y / scale;
        const Point a = { x0, y0 };
        const Point b = { x1, y1 };
        if(xout(map, a))
            printf("%d %d: %f %f\n", map.rows, map.cols, a.y, a.x);
        if(xout(map, b))
            printf("%d %d: %f %f\n", map.rows, map.cols, a.y, a.x);
        // Draw line.
        map.walling[y0][x0] = ' ';
        map.walling[y1][x1] = ' ';
    }
}

Map xtgenerate(const Sdl sdl)
{
    srand(time(0));
    // Choose to generate at roughl screen pixel
    // size so that triangles can be debugged with simple line drawings.
    const int w = (sdl.xres / 2) + rand() % (sdl.xres / 2);
    const int h = (sdl.yres / 2) + rand() % (sdl.yres / 2);
    // Lets rock.
    // Border and grid can remain constant. No need to randomize.
    // Will give bigger rooms on map edge if map and width height small.
    const int border = 50;
    const int grid = 20;
    const int max = 50 + rand() % 50;
    // Points are randomly placed in an array.
    const Points ps = prand(w, h, max, grid, border);
    // Points are placed into a triangle mesh with delaunay triangulation.
    const Tris tris = delaunay(ps, w, h, 9 * max);
    // Edges are colleted.
    const Tris edges = ecollect(tsnew(27 * max), tris);
    // The revere-delete algorithm is used to obtain a minimum spanning tree from edges.
    // Kruskal et al. (C) 1956.
    revdel(edges, w, h);
    // Duplicate edges are removed and a map is carved. Map is scaled to screen.
    const int scale = 10;
    const Map map = xmgen(h / scale, w / scale);
    printf("%d %d %d %d\n", w, h, map.rows, map.cols);
    mdups(edges);
    carve(map, edges, scale);
    free(tris.tri);
    free(ps.point);
    free(edges.tri);
    xmdump(map);
    return map;
}
