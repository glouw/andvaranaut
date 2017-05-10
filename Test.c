#include "Test.h"

#include "Util.h"
#include "String.h"

static void _build()
{
    char* strings[] = { "each", "must", "be", "built", "printed", "and", "freed" };
    for(int i = 0; i < len(strings); i++)
    {
        char* const string = build(strings[i]);
        printf("%s\n", string);
        free(string);
    }
}

static void _concat()
{
    char* strings[] = { "each", "must", "concatenate", "another", "string", "and", "freed" };
    for(int i = 0; i < len(strings); i++)
    {
        char* const string = concat(strings[i], "...");
        printf("%s\n", string);
        free(string);
    }
}

static void _match()
{
    char* strings[] = { "each", "of", "these", "must", "match", "themselves" };
    for(int i = 0; i < len(strings); i++)
        printf("%s\n", boolean(match(strings[i], strings[i])));
}

static void _trim()
{
    char strings[][80] = { " lorem ", "  ipsum  ", "morel ", " muspi", " notice the spaces " };
    for(int i = 0; i < len(strings); i++)
        printf("_%s_\n", trim(strings[i]));
}

static void _floating()
{
    char* strings[] = { "1.0", "2.0", "-1.2", "+42.0", "+0.0", "-0.0", "INF" };
    for(int i = 0; i < len(strings); i++)
        printf("%f\n", floating(strings[i]));
}

static void __String()
{
    _build();
    _concat();
    _match();
    _trim();
    _floating();
}

static void readme()
{
    FILE* const file = fopen("README", "r");
    const int lines = lns(file );
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
    __String();
    readme();
}
