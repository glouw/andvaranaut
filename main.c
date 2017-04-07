#include "Test.h"
#include "Play.h"

int main(const int argc, const char* argv[])
{
    argc == 2 ? play(argv) : test();
}
