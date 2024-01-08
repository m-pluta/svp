CC = gcc
CFLAGS = -I. -Werror -Wpedantic -Wall -Wextra -lm -O3
DFLAGS = -pg -O1

TESTCASE = [41 3 55 139 186] [62 128 99 8 88] [25 54 158 58 200] [225 160 102 19 29] [12 4 69 155 39]

# Directory variable
DIR = src/

# List of source files
SRCS = $(wildcard $(DIR)*.c)

# List of object files
OBJS = $(SRCS:.c=.o)

# The main target
all: cpplint runme

# Target to build the executable
runme: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

again:
	make clean
	make all

callgrind: clean
	make clean
	$(CC) -o runme $(SRCS) $(CFLAGS) $(DFLAGS)
	valgrind --tool=callgrind ./runme $(TESTCASE)
	kcachegrind callgrind.out.*

hyperfine:
	make clean
	make runme
	hyperfine "./runme $(TESTCASE)" > hyperfine.txt

#Simple test suite
test:
	@make --no-print-directory all
	@echo "Starting tests"
	@make --no-print-directory do_run EXPECTED="1.414214" LATTICE="[3.0 2.0] [1.0 -1.0]"
	@make --no-print-directory do_run EXPECTED="1.414214" LATTICE="[3.0 -70.0 2.0] [1.0 0.0 -1.0] [0.0 -7.0 9.0]"
	@make --no-print-directory do_run EXPECTED="138.654246" LATTICE="[87, 247] [147, 122]"
	@make --no-print-directory do_run EXPECTED="115.0" LATTICE="[3164876632.0 1.0 0.0 0.0] [2493630228.0 0.0 1.0 0.0] [1060187369.0 0.0 0.0 1.0] [2699093087.0 0.0 0.0 0.0]"
	@echo "Tests finished"
	@make --no-print-directory clean

do_run:
	@echo $(LATTICE)
	@./runme $(LATTICE)
	@echo -n "Expected: "
	@echo $(EXPECTED)
	@echo -n "Actual:   "
	@cat result.txt
	@echo

# Clean rule to remove generated files
clean:
	rm -rf runme $(OBJS) result.txt gmon.out callgrind.out.*

clean-test:
	rm -rf test-gen.csv test-result.csv

run-test:
	python3 test-gen-all.py
	python3 test-run.py

cpplint:
	cpplint $(SRCS)

.PHONY: all runme again callgrind hyperfine test do_run clean clean-test run-test