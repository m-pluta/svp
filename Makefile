# Main variables
CC = gcc
CFLAGS = -I. -Werror -Wpedantic -Wall -Wextra

# Main runme
all:  main.c
	$(CC) -o runme main.c $(CFLAGS)

# Test Suite
test: main.o tests/test.c
	$(CC) -o test tests/test.c main.o $(CFLAGS)
	./test

# Object files
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


clean:
	rm -rf runme test *.o

run:
	make all
	./runme [3.0 2.0] [1.0 -1.0]
	./runme [3.0 -70.0 2.0] [1.0 0.0 -1.0]

.PHONY: all test clean

# cpplint --filter=-legal,-build --recursive .