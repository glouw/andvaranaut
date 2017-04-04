#include "test.h"
#include "play.h"

int main(const int argc, const char* argv[])
{
    argc == 2 ? play(argv) : test();
}
