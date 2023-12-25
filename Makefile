# Main variables
CC = gcc
CFLAGS = -I. -Werror -Wpedantic -Wall -Wextra

# Main program
all:  main.c
	$(CC) -o program main.c $(CFLAGS)

# Test Suite
test: main.o tests/test.c
	$(CC) -o test tests/test.c main.o $(CFLAGS)
	./test

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf main test *.o

.PHONY: all test clean

# cpplint --filter=-legal,-build --recursive .