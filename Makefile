CC = gcc
CFLAGS = -I. -Werror -Wpedantic -Wall -Wextra -lm -Ofast

# List of source files
SRCS = src/main.c src/vector.c src/vector2d.c src/gram_schmidt.c src/schorr_euchner.c

# List of header files
HDRS = src/vector.h src/vector2d.h src/gram_schmidt.h src/schorr_euchner.h

# List of object files
OBJS = $(SRCS:.c=.o)

# The main target
all: runme

# Target to build the executable
runme: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

# Test Suite
test: main.o tests/test.c
	$(CC) -o test tests/test.c main.o $(CFLAGS)
	./test

# Rule to build object files from source files
%.o: %.c $(HDRS)
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean rule to remove generated files
clean:
	rm -rf runme $(OBJS)

run:
	make all
	./runme [3.0 2.0] [1.0 -1.0]
	./runme [3.0 -70.0 2.0] [1.0 0.0 -1.0] [0.0 -7.0 9.0]
	./runme [87, 247] [147, 122]
	./runme [3164876632.0 1.0 0.0 0.0] [2493630228.0 0.0 1.0 0.0] [1060187369.0 0.0 0.0 1.0] [2699093087.0 0.0 0.0 0.0]
	make clean

.PHONY: all test clean

# cpplint --filter=-legal,-build --recursive .