# Advent Of Code 2017 In C Makefile
# Usage: make all|help|day=XX

OUT_DIR = out/
CC = gcc -std=c11 -Wall -Wextra -Wno-unused-parameter
VALGRIND = valgrind --tool=memcheck

DAYS = $(addprefix $(OUT_DIR), \
		day01.o \
		day02.o \
		day03.o \
	)


INCLUDE = \
	src/collections/intarraylist.h \
	src/collections/arrayhashmap.h \
	src/math/integer.h \
	src/math/vectors.h \
	src/aoc.h \
	src/class.h \
	src/panic.h \
	src/utils.h

SRC = \
	src/collections/intarraylist.c \
	src/collections/arrayhashmap.c \
	src/math/integer.c \
	src/math/vectors.c \
	src/aoc.c \
	src/utils.c

TEST_SRC = \
	test/unittest.c

TEST_INCLUDE = \
	test/unittest.h

.DEFAULT_GOAL = run

help :
	@echo "+--------------------------------------------------------------------+"
	@echo "| Alcatraz Escapee (Alex O'Neill) Advent Of Code 2017 in C Solutions |"
	@echo "+--------------------------------------------------------------------+"
	@echo "To build and run a particular day, run 'make day=XX'"
	@echo "For all days, run 'make all'"

.PHONY: run
run: out/day$(day).o ; -@out/day$(day).o

.PHONY: all
all: $(DAYS)
	-@for day in $(DAYS) ; do \
        $$day ; \
    done

.PHONY: clean
clean :
	rm -rf out/*.o

.PHONY: test
test : out/test.o
	-@out/test.o

.PHONY: check
check : out/day$(day).o
	-@$(VALGRIND) out/day$(day).o

.PHONY: checkall
checkall : $(DAYS)
	for day in $(DAYS) ; do \
		$(VALGRIND) $$day ; \
	done

out/day%.o : src/days/day%.c $(SRC) $(INCLUDE)
	mkdir -p out
	$(CC) $< $(SRC) -o $@

out/test.o : $(TEST_SRC) $(TEST_INCLUDE) $(SRC) $(INCLUDE)
	mkdir -p out
	$(CC) $(TEST_SRC) $(SRC) -o out/test.o
