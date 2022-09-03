#!/bin/bash

set -e
file=res.txt
rm -f $file
make
echo 'M\tP\tNP\tTime' > $file
for M in 16 160 1600 
    do
    for P in 16 160 1600
    do
        for NP in $(seq 1 16); do
            for run in $(seq 1 10); do
                echo $run
                mpirun -n $NP ./lab_add $P $M >>$file
                #make run P=$P M=$M NP=$NP >>$file
            done
        done
    done
done
