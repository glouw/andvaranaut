#include "Portals.h"

#include "util.h"

#include <stdlib.h>
#include <string.h>

Portals populate(const char* const path)
{
    FILE* const fp = fopen(path, "r");
    const int count = newlines(fp);
    Portal* const portal = (Portal*) calloc(count, sizeof(*portal));
    for(int i = 0; i < count; i++)
    {
        char* line = NULL;
        unsigned reads = 0;
        Point where = { 0.0, 0.0 };
        getline(&line, &reads, fp);
        line = strtok(line, " ");
        sscanf(line, "%f,%f", &where.x, &where.y);
        portal[i].where = where;
        line = strtok(NULL, " #");
        portal[i].blocks = strdup(line);
    }
    fclose(fp);
    const Portals portals = { portal, count };
    return portals;
}

void destroy(const Portals portals)
{
    for(int i = 0; i < portals.count; i++)
        free(portals.portal[i].blocks);
    free(portals.portal);
}
