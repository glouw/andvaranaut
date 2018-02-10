#include "Trapdoor.h"

#include "util.h"

Trapdoor xtdnew(const Points ps)
{
    Trapdoor td;
    xzero(td);
    td.a = xmid(ps.point[rand() % ps.count]);
    td.b = xmid(ps.point[rand() % ps.count]);
    td.c = xmid(ps.point[rand() % ps.count]);
    td.d = xmid(ps.point[rand() % ps.count]);
    td.e = xmid(ps.point[rand() % ps.count]);
    return td;
}

Point xtdrand(const Trapdoor td)
{
    switch(rand() % 5)
    {
        default:
        case 0: return td.a; break;
        case 1: return td.b; break;
        case 2: return td.c; break;
        case 3: return td.d; break;
        case 4: return td.e; break;
    }
}
