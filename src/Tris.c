#include "Tris.h"

#include "Points.h"
#include "Sorter.h"
#include "Map.h"
#include "Flags.h"
#include "util.h"

static Tris make(const int max)
{
    const Tris ts = { u_toss(Tri, max), 0, max };
    return ts;
}

static Tris append(Tris tris, const Tri tri)
{
    if(tris.count == tris.max)
        u_bomb("tris size limitation reached\n");

    tris.tri[tris.count++] = tri;
    return tris;
}

static int reverse_eql(const Tri a, const Tri b)
{
    return p_same(a.a, b.b) && p_same(a.b, b.a);
}

static int forward_eql(const Tri a, const Tri b)
{
    return p_same(a.a, b.a) && p_same(a.b, b.b);
}

static int alligned(const Tri a, const Tri b)
{
    return forward_eql(a, b)
        || reverse_eql(a, b);
}

static void mark(Tris edges, const Flags flags)
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

static Tris join(Tris tris, const Tris edges, const Point p, const Flags flags)
{
    for(int j = 0; j < edges.count; j++)
    {
        const Tri edge = edges.tri[j];
        if(p_same(edge.c, flags.zer))
        {
            const Tri tri = { edge.a, edge.b, p };
            tris = append(tris, tri);
        }
    }
    return tris;
}

static int out_of_bounce(const Point p, const int w, const int h)
{
    return p.x < 0 || p.y < 0 || p.x >= w || p.y >= h;
}

static int in_circumcenter(const Tri t, const Point p)
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

static Tris collect(Tris edges, const Tris in, const Flags flags)
{
    for(int i = 0; i < in.count; i++)
    {
        const Tri tri = in.tri[i];
        const Tri ab = { tri.a, tri.b, flags.zer };
        const Tri bc = { tri.b, tri.c, flags.zer };
        const Tri ca = { tri.c, tri.a, flags.zer };
        edges = append(edges, ab);
        edges = append(edges, bc);
        edges = append(edges, ca);
    }
    return edges;
}

//
// https://en.wikipedia.org/wiki/Delaunay_triangulation
//

static Tris triangulate(const Points ps, const int w, const int h, const int max, const Flags flags)
{
    Tris in = make(max);
    Tris out = make(max);
    Tris tris = make(max);
    Tris edges = make(max);

    Tri* dummy = tris.tri;

    const Tri super = {
        { (float) -w, 0.0f }, { 2.0f * w, 0.0f }, { w / 2.0f, 2.0f * h }
    };

    tris = append(tris, super);
    for(int j = 0; j < ps.count; j++)
    {
        in.count = out.count = edges.count = 0;
        const Point p = ps.point[j];

        for(int i = 0; i < tris.count; i++)
        {
            const Tri tri = tris.tri[i];

            if(in_circumcenter(tri, p))
                in = append(in, tri);
            else out = append(out, tri);
        }

        edges = collect(edges, in, flags);

        mark(edges, flags);

        out = join(out, edges, p, flags);

        tris = out;
    }
    free(dummy);
    free(in.tri);
    free(edges.tri);

    return tris;
}

static Points prand(const int w, const int h, const int max, const int grid, const int border, const Points extra)
{
    Points ps = p_new(max);
    ps = p_cat(ps, extra);

    for(int i = ps.count; i < ps.max; i++)
    {
        const Point p = {
            (float) (rand() % (w - border) + border / 2),
            (float) (rand() % (h - border) + border / 2),
        };
        const Point snapped = p_mid(p_snap(p, grid));
        if(!p_find(ps, snapped))
            ps = p_append(ps, snapped);
    }
    return ps;
}

static void sort(const Tris edges)
{
    qsort(edges.tri, edges.count, sizeof(Tri), s_descend_tris);
}

static int connected(const Point a, const Point b, const Tris edges, const Flags flags)
{
    Points todo = p_new(edges.max);
    Points done = p_new(edges.max);
    Tris reach = make(edges.max);

    todo = p_append(todo, a);

    int connection = false;
    while(todo.count != 0 && connection != true)
    {
        const Point removed = todo.point[--todo.count];
        done = p_append(done, removed);

        //
        // Get reachable edges from current point.
        //

        reach.count = 0;
        for(int i = 0; i < edges.count; i++)
        {
            const Tri edge = edges.tri[i];

            if(p_same(edge.c, flags.one))
                continue;

            if(p_same(edge.a, removed))
                reach = append(reach, edge);
        }

        //
        // For all reachable edges
        //

        for(int i = 0; i < reach.count; i++)
        {
            //
            // Was the destination reached?
            //

            if(p_same(reach.tri[i].b, b))
            {
                connection = true;
                break;
            }

            //
            // Otherwise add point of reachable edge to todo list.
            //

            if(!p_find(done, reach.tri[i].b))
                todo = p_append(todo, reach.tri[i].b);
        }
    }
    free(todo.point);
    free(reach.tri);
    free(done.point);

    return connection;
}

static void reverse_delete(Tris edges, const int w, const int h, const Flags flags)
{
    sort(edges);
    for(int i = 0; i < edges.count; i++)
    {
        Tri* edge = &edges.tri[i];

        //
        // Flag out of bound edges.
        //

        if(out_of_bounce(edge->a, w, h)
        || out_of_bounce(edge->b, w, h))
        {
            edge->c = flags.one;
            continue;
        }

        //
        // Break connection.
        //

        edge->c = flags.one;

        //
        // If two points are not connected in anyway then reconnect.
        // Occasionally a loop will be created as edges are doubled (sibling reversed)
        // and not perfectly paired when sorted by magnitude.
        // This benefits the map design; perfectly linear dungeons are boring.
        //

        if(!connected(edge->a, edge->b, edges, flags))
            edge->c = flags.zer;
    }
}

static void remove_dups(const Tris edges, const Flags flags)
{
    for(int i = 0; i < edges.count; i++)
    for(int j = 0; j < edges.count; j++)
    {
        if(p_same(edges.tri[j].c, flags.one)) continue;
        if(p_same(edges.tri[i].c, flags.one)) continue;

        if(reverse_eql(edges.tri[i], edges.tri[j]))
            edges.tri[j].c = flags.one;
    }
}

//
// #############################################
// #           #####################           #
// #           #####################           #
// #  r o o m     c o r r i d o r     r o o m  #
// #           #####################           #
// #           #####################           #
// #############################################
//

static void place_bone(const Map map, const Tri e, const int w, const int h)
{
    m_place_room(map, e.a, w, h, WALLING);
    m_place_room(map, e.b, w, h, WALLING);

    if(u_d2() == 0) m_place_room(map, e.a, w, h, CEILING);
    if(u_d2() == 0) m_place_room(map, e.b, w, h, CEILING);

    m_lay_corridor(map, e.a, e.b);
}

static void carve(const Map map, const Tris edges, const Flags flags)
{
    for(int i = 0; i < edges.count; i++)
    {
        const Tri e = edges.tri[i];

        if(p_same(e.c, flags.one))
            continue;

        const int size = m_max(map) - m_min(map);
        const int w = m_min(map) + rand() % size;
        const int h = m_min(map) + rand() % size;
        place_bone(map, e, w, h);
    }
}

static Points get_traps(const Points rooms, const int count)
{
    Points trapdoors = p_new(count);
    for(int i = 0; i < count; i++)
        trapdoors = p_add_unique(trapdoors, rooms);
    return trapdoors;
}

static void generate_simple(const Map map, const Points rooms)
{
    const int min = m_min(map);
    const Point mid = rooms.point[0];
    m_place_room(map, mid, min, min, WALLING);
    m_place_room(map, mid, min, min, CEILING);
}

static void generate_complex(const Map map, const Points rooms, const int w, const int h)
{
    const Flags flags = { { 0.0, 0.0 }, { 1.0, 1.0 } };

    const Tris tris = triangulate(rooms, w, h, 3 * rooms.max, flags);

    const Tris edges = collect(make(3 * tris.max), tris, flags);

    reverse_delete(edges, w, h, flags);

    remove_dups(edges, flags);

    carve(map, edges, flags);

    free(tris.tri);
    free(edges.tri);
}

Map t_generate(const Points extra, const int w, const int h, const int grid, const int traps, const int floor)
{
    const int max = traps + (10 + u_d10());

    const int border = 2 * grid;

    const Points rooms = prand(w, h, max, grid, border, extra);

    const Points trapdoors = get_traps(rooms, u_min(traps, rooms.count));

    const Map map = m_generate(h, w, trapdoors, rooms, grid, floor);

    //
    // Complex generation only works with more than one room.
    //

    rooms.count > 1 ?  generate_complex(map, rooms, w, h) : generate_simple(map, rooms);

    return map;
}
