make -C src clean

OPT=-j10

for i in 00 01 10 11
do
    make $OPT -C src CLANG=${i:0:1} CC=${i:1:2}
    make $OPT -C src clean
done
