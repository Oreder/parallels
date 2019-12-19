# Compiler
CC := mpicc

# Compiler options
FLAGS := -Wall -Werror

# Dependency
DEPS := vector.h matrix.h LU.h equations.h

# Object files
OBJS := vector.o matrix.o LU.o equations.o

%.o: %.c $(DEPS)
	$(CC) $(FLAGS) -c $<

lab2: main.o $(OBJS)
	$(CC) -o $@ $^

clean:
	$(RM) *.o *.exe *.out lab*