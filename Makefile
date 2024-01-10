CC = gcc
CFLAGS = -I. -Werror -Wpedantic -Wall -Wextra -lm -O3 -march=native
DFLAGS = -pg -O1

TESTCASE = [41 3 55 139 186] [62 128 99 8 88] [25 54 158 58 200] [225 160 102 19 29] [12 4 69 155 39]

# Directory variable
DIR = src/
TEST_DIR = tests/

# List of source files
SRCS = $(wildcard $(DIR)*.c)

# List of object files
OBJS = $(SRCS:.c=.o)

# The main target
all: runme

# Target to build the executable
runme: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

again:
	make clean
	make runme

callgrind: clean
	make again
	$(CC) -o runme $(SRCS) $(CFLAGS) $(DFLAGS)
	valgrind --tool=callgrind ./runme $(TESTCASE)
	kcachegrind callgrind.out.*

hyperfine:
	make again
	hyperfine "./runme $(TESTCASE)" > hyperfine.txt

memusage:
	make again
	memusage -T ./runme $(TESTCASE) |&grep -E -o 'heap total: [0-9]+' >> out 2>&1

#Simple test suite
test:
	make runme
	$(CC) -o test $(TEST_DIR)test.c $(CFLAGS)
	./test
	make clean


# Clean rule to remove generated files
clean:
	rm -rf runme test $(OBJS) result.txt gmon.out callgrind.out.*

clean-test:
	rm -rf $(TEST_DIR)test-gen.csv $(TEST_DIR)test-result.csv

run-test:
	python3 $(TEST_DIR)test-gen-all.py
	python3 $(TEST_DIR)test-run.py

cpplint:
	cpplint $(SRCS)

.PHONY: all runme again callgrind hyperfine test do_run clean clean-test run-test