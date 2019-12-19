#!/bin/bash
if [ $1 = "mpi.c" ]
then
    mpicc $1
    mpirun -np $2 ./a.out
else
    gcc -fopenmp $1
    ./a.out
fi