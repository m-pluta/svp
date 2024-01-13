CC=gcc
CFLAGS=-I. -Werror -Wpedantic -Wall -Wextra -lm -O3
DFLAGS=-pg -O1

TESTCASE=[41 3 55 139 186] [62 128 99 8 88] [25 54 158 58 200] [225 160 102 19 29] [12 4 69 155 39]

DIR=src/
TEST_DIR=tests/
SRCS=$(wildcard $(DIR)*.c)
OBJS=$(SRCS:.c=.o)
DEPS=$(OBJS:.o=.d)

# The main target
all: cpplint runme

# Target to build the executable
runme: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

again: clean runme

callgrind: runme
	$(CC) -o runme $(SRCS) $(CFLAGS) $(DFLAGS)
	valgrind --tool=callgrind ./runme $(TESTCASE)
	kcachegrind callgrind.out.*
	make clean

hyperfine: runme
	hyperfine "./runme $(TESTCASE)"
	make clean

memory: runme
	memusage -T ./runme $(TESTCASE)
	valgrind ./runme $(TESTCASE)
	valgrind --tool=dhat ./runme $(TESTCASE)
	make clean

# Run test suite
test: runme
	$(CC) $(TEST_DIR)test.c -o test $(CFLAGS)
	./test
	make clean

clean:
	rm -rf runme test $(OBJS) result.txt gmon.out callgrind.out.* dhat.out.* $(TEST_DIR)hyperfine.csv

clean-test-result:
	rm -rf $(TEST_DIR)test-result.csv

# Requires 'fplll' and 'bc' commands in bash
run-test: runme
	python3 $(TEST_DIR)test-gen-all.py
	python3 $(TEST_DIR)test-run.py
	make clean

cpplint:
	cpplint --filter=-legal,-build,-readability/casting $(SRCS) $(wildcard $(DIR)*.h)

.PHONY: all runme again callgrind hyperfine test do_run clean clean-test run-test