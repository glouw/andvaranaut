#include "Test.h"

#include "Util.h"
#include "String.h"

static void __trim()
{
    char str[] = " lorem  ";
    puts(str);
    printf(".%s.\n", trim(str));
}

static void readme()
{
    FILE* const file = fopen("README", "r");
    const int lines = lns(file );
    for(int i = 0; i < lines; i++) puts(readln(file));
}

void test()
{
    __trim();
    readme();
}
