# Compiler
CC := gcc

# Compiler options
FLAGS := -std=c99 -Wall -Werror

# Dependency
DEPS := alloc.h io.h LU.h

# Object files
OBJS := alloc.o io.o LU.o

%.o: %.c $(DEPS)
	$(CC) $(FLAGS) -c $<

lab1: source.o $(OBJS)
	$(CC) -o $@ $^

clean:
	$(RM) *.o *.exe *.out lab*