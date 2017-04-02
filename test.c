#include "util.h"

#include <stdlib.h>

static void _util()
{
    FILE* file = fopen("test/longline.test", "r");
    const int lines = lns(file);
    for(int i = 0; i < lines; i++)
    {
        char* const line = readln(file);
        puts(line);
        free(line);
    }
    fclose(file);
}

void test()
{
    _util();
}
