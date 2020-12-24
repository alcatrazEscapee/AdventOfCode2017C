# Advent Of Code 2017 In C Makefile
# Usage: make all|help|day=XX

OUT_DIR = out/
CC = gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable
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
	src/days/aoc.h \
	src/class.h \
	src/panic.h \
	src/strings.h \
	src/utils.h

SRC = \
	src/collections/intarraylist.c \
	src/collections/arrayhashmap.c \
	src/math/integer.c \
	src/math/vectors.c \
	src/days/aoc.c \
	src/strings.c \
	src/utils.c

# Test sources
# These are always #include-ed directly in the main unittest file
# However, we still need to track them for modifications so we know when to re-make the test binary
TEST_SRC = \
	test/math/testinteger.c \
	test/teststrings.c \
	test/unittest.c


.DEFAULT_GOAL = run

help :
	@echo "+--------------------------------------------------------------------+"
	@echo "| Alcatraz Escapee (Alex O'Neill) Advent Of Code 2017 in C Solutions |"
	@echo "+--------------------------------------------------------------------+"
	@echo "Options:"
	@echo "  make day=XX        - Run day XX"
	@echo "  make all           - Run all days"
	@echo "  make clean         - Cleans all compiled binaries"
	@echo "  make test          - Run unit tests"
	@echo "  make check day=XX  - Run day XX with Valgrind memcheck"
	@echo "  make checkall      - Run all days with Valgrind memcheck"
	@echo "  make checktest     - Run unit tests with Valgrind memcheck"

.PHONY: run
run: out/day$(day).o
	-@if [ "$(day)" = "" ]; then \
		echo "No day provided - try with make day=XX" ; \
	else \
		out/day$(day).o ; \
	fi

.PHONY: out/day.o

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
	-@if [ "$(day)" = "" ]; then \
		echo "No day provided - try with make check day=XX" ; \
	else \
		$(VALGRIND) out/day$(day).o ; \
	fi

.PHONY: checkall
checkall : $(DAYS)
	for day in $(DAYS) ; do \
		$(VALGRIND) $$day ; \
	done

.PHONY: checktest
checktest : out/test.o
	-@$(VALGRIND) out/test.o

out/day%.o : src/days/day%.c $(SRC) $(INCLUDE)
	mkdir -p out
	$(CC) $< $(SRC) -o $@

out/test.o : $(TEST_SRC) test/unittest.h $(SRC) $(INCLUDE)
	mkdir -p out
	$(CC) test/unittest.c $(SRC) -o out/test.o
