#include "Test.h"

#include "Util.h"
#include "String.h"

static void _concat()
{
    const char* strings[] = {
        "each", "must", "concatenate", "another", "string", "and", "freed"
    };
    for(int i = 0; i < len(strings); i++)
    {
        char* const string = concat(strings[i], "...");
        printf("%s\n", string);
        free(string);
    }
}

static void _match()
{
    const char* strings[] = {
        "each", "of", "these", "must", "match", "themselves"
    };
    for(int i = 0; i < len(strings); i++)
        printf("%s\n", boolean(match(strings[i], strings[i])));
}

static void _trim()
{
    char strings[][80] = {
        " lorem ", "  ipsum  ", "morel ", " muspi", " notice the spaces "
    };
    for(int i = 0; i < len(strings); i++)
        printf("_%s_\n", trim(strings[i]));
}

static void __String()
{
    _concat();
    _match();
    _trim();
}

static void license()
{
    puts(
    "Water copyright (c) 2017 Gustav Louw, Cameron Bauer, Omar Hefnawi\n"
    "This program comes with ABSOLUTELY NO WARRANTY.\n"
    "This is free software, and you are welcome to redistribute it\n"
    "under certain conditions.\n\n"
    "Select a resolution and start playing:\n"
    "\t./water 512\n");
}

extern void test()
{
    (void) __String;
    license();
}
