# Compiler
CC := mpicc

# Compiler options
FLAGS := -std=c99 -Wall -Werror

# Dependency
DEPS := matrix.h

# Object files
OBJS := matrix.o

%.o: %.c $(DEPS)
	$(CC) $(FLAGS) -c $<

lab2: main.o $(OBJS)
	$(CC) -o $@ $^

clean:
	$(RM) *.o *.exe *.out lab*