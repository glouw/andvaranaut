#include "Portals.h"

#include "Util.h"
#include "String.h"

Portals populate(const char* const name)
{
    char* const path = concat("config/", name);
    FILE* const file = fopen(path, "r");
    const int count = lns(file);
    Portal* const portal = toss(Portal, count);
    for(int i = 0; i < count; i++)
    {
        Point where = { 0.0, 0.0 };
        char* const line = readln(file);
        char* const location = strtok(line, " ");
        sscanf(location, "%f,%f", &where.x, &where.y);
        portal[i].where = where;
        portal[i].name = build(strtok(NULL, " #"));
        free(line);
    }
    free(path);
    fclose(file);
    const Portals portals = { count, portal };
    return portals;
}

void destroy(const Portals portals)
{
    for(int i = 0; i < portals.count; i++)
        free(portals.portal[i].name);
    free(portals.portal);
}

int isportal(const int ascii)
{
    return ascii >= 'a' && ascii <= 'z';
}
