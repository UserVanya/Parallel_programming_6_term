#!/bin/bash

set -e
file=res.txt
rm -f $file

echo 'n\tTime' > $file
for N in $(seq 1 16); do
    for run in $(seq 1 10); do
        echo $run
        mpirun -n $N ./lab >> $file
    done
done