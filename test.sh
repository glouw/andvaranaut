#!/bin/bash

set -e

run()
{
    for i in {0..25..1}
    do
        ./andvaranaut -x 512
    done

    echo "SUCCESS"
}

main()
{
    for i in 11 10 01 00
    do
        make -C src clean

        make -j 10 -C src CLANG=${i:0:1} CPP=${i:1:2}

        run
    done
}

main
