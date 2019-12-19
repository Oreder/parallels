# Parallels

Computing the inverse matrix base on the LU decomposition

+ Original without parallels
```bash
    make lab1
    ./lab1
```

+ Parallel by [mpi](https://mpitutorial.com/tutorials/)
```bash
    make lab2 CC=mpicc
    mpiexec -n 16 ./lab2
```

 ## References
1. [Using MPI: Portable Parallel Programing Advances](https://doc.lagout.org/science/0_Computer%20Science/5_Parallel%20and%20Distributed/Using%20MPI-2%20-%20Advanced%20Features.pdf)
2. [MPI in two hours](https://userinfo.surfsara.nl/systems/shared/mpi/mpi-intro)
3. Shared memory for windows ([utexas.edu](http://pages.tacc.utexas.edu/~eijkhout/pcse/html/mpi-shared.html)|[stackoverflow](https://stackoverflow.com/questions/39912588/can-i-use-mpi-with-shared-memory))