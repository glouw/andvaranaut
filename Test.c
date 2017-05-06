#include "Test.h"

#include "Util.h"

void test()
{
    FILE* const readme = fopen("README", "r");
    const int lines = lns(readme);
    for(int i = 0; i < lines; i++) puts(readln(readme));
}
