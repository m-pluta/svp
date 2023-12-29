CC = gcc
CFLAGS = -I. -Werror -Wpedantic -Wall -Wextra

# List of source files
SRCS = main.c vector.c basis.c

# List of header files
HDRS = vector.h basis.h

# List of object files
OBJS = $(SRCS:.c=.o)

# The main target
all: runme

# Target to build the executable
runme: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Test Suite
test: main.o tests/test.c
	$(CC) -o test tests/test.c main.o $(CFLAGS)
	./test

# Rule to build object files from source files
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -rf runme $(OBJS)

run:
	make all
	./runme [3.0 2.0] [1.0 -1.0]
	./runme [3.0 -70.0 2.0] [1.0 0.0 -1.0] [0.0 -7.0 9.0]
	make clean

.PHONY: all test clean

# cpplint --filter=-legal,-build --recursive .