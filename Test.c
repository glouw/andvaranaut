#include "Util.h"

#include <stdlib.h>

static void _strcon()
{
    char* cat = strcon("hi there", ", my friend");
    puts(cat);
    free(cat);
}

static void _readln()
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

static void _util()
{
    _readln();
    _strcon();
}

void test()
{
    _util();
}
