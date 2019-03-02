# !/bin/bash

#
# To ensure long term portability, Andvaranaut is to be compiled with all major C and C++ compilers.
#
# To ensure the random number generator does not break world generation,
# Andvaranaut is to be started and exited a number of times for each compilation.
#

test()
{
    for i in {0..25..1}
    do
        ./andvaranaut -x 512
    done

    echo "SUCCESS"
}

main()
{
    set -e

    for i in 11 10 01 00
    do
        make -C src clean

        make -j 10 -C src CLANG=${i:0:1} CPP=${i:1:2}

        test
    done
}

main
