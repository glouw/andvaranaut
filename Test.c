#include "Types.h"
#include "Sprites.h"
#include "Util.h"

// ----------------------------------------------------------------------------
// Util
// ----------------------------------------------------------------------------
static void _string()
{
    char* const str = string("testing a new string ok");
    puts(str);
    free(str);
}

static void _concat()
{
    char* const str = concat("hi there", ", my friend");
    puts(str);
    free(str);
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

static void _Util()
{
    _readln();
    _concat();
    _string();
}

// ----------------------------------------------------------------------------
// Sprites
// ----------------------------------------------------------------------------
static void _Sprites()
{
    Sprites sprites = wake("outside");
    prints(sprites);
    // Swapped
    sprites = swap(sprites, "start");
    prints(sprites);
    // Pasted surface sprites
    Hero hero;
    hero.where.x = 2.0;
    hero.where.y = 3.0;
    hero.angle.theta = 1.0 * acosf(-1.0) / 2.0; // PI / 2
    hero.angle.percent = 0.25;
    const Sprites temp = update(sprites, hero);
    prints(temp);
    // Cleanup
    sleep(temp);
    sleep(sprites);
}

void test()
{
    _Util();
    _Sprites();
}
