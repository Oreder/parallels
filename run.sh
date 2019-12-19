#!/bin/bash
if [ $1 = "mpi.c" ]
then
    mpicc $1
    mpirun -np $3 ./a.out $2
else
    if [ $1 = "openmp.c" ]
    then
        gcc -fopenmp $1
        ./a.out $3 $2
    else
        gcc $1
        ./a.out $2
    fi
fi