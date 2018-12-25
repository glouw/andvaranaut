#!/bin/bash

set -e

OPT=-j10

build()
{
    for i in 11 10 01 00
    do
        make -C src clean
        make $OPT -C src CLANG=${i:0:1} CC=${i:1:2}
    done
}

run()
{
    for i in {0..100..1}
    do
        ./andvaranaut -x 512
    done
}

fin()
{
    echo "SUCCESS"
}

build
run
fin
