# Advent Of Code 2017 In C Makefile
# Usage: make all|help|day=XX

DEBUG_DIR = out/debug/
RELEASE_DIR = out/release/
CC_DEBUG = gcc -std=c11 -g -O0 -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable
CC_EXPAND = gcc -std=c11 -E -P
CC_RELEASE = gcc -std=c11 -O3
VALGRIND = valgrind --tool=memcheck --leak-check=full

DAYS = \
		day01.o \
		day02.o \
		day03.o \
		day04.o \
		day05.o

DEBUG_DAYS = $(addprefix $(DEBUG_DIR), $(DAYS))
RELEASE_DAYS = $(addprefix $(RELEASE_DIR), $(DAYS))

INCLUDE = \
	src/collections/arrayhashmap.h \
	src/collections/arrayhashset.h \
	src/collections/arraylist.h \
	src/collections/intarraylist.h \
	src/math/integer.h \
	src/math/vectors.h \
	src/days/aoc.h \
	src/lib.h \
	src/sorting.h \
	src/strings.h \
	src/utils.h \
	src/void.h

SRC = \
	src/collections/arrayhashmap.c \
	src/collections/arrayhashset.c \
	src/collections/arraylist.c \
	src/collections/intarraylist.c \
	src/math/integer.c \
	src/math/vectors.c \
	src/days/aoc.c \
	src/lib.c \
	src/sorting.c \
	src/strings.c \
	src/utils.c \
	src/void.c

# Test sources
# These are always #include-ed directly in the main unittest file
# However, we still need to track them for modifications so we know when to re-make the test binary
TEST_SRC = \
	test/collections/testarrayhashmap.c \
	test/collections/testarraylist.c \
	test/collections/testintarraylist.c \
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
	@echo "  make check day=XX  - Run day XX with Valgrind"
	@echo "  make checkall      - Run all days with Valgrind"
	@echo "  make checktest     - Run unit tests with Valgrind"
	@echo "  make expand day=XX - Runs the preprocessor against day XX"

.PHONY: run
run: out/release/day$(day).o
	-@if [ "$(day)" = "" ]; then \
		echo "No day provided - try with make day=XX" ; \
	else \
		out/release/day$(day).o ; \
	fi

.PHONY: out/day.o

.PHONY: all
all: $(RELEASE_DAYS)
	-@for day in $(RELEASE_DAYS) ; do \
        $$day ; \
    done

.PHONY: clean
clean :
	rm -rf out

.PHONY: test
test : out/debug/test.o
	-@out/debug/test.o

.PHONY: check
check : out/debug/day$(day).o
	-@if [ "$(day)" = "" ]; then \
		echo "No day provided - try with make check day=XX" ; \
	else \
		$(VALGRIND) out/debug/day$(day).o ; \
	fi

.PHONY: checkall
checkall : $(DEBUG_DAYS)
	for day in $(DEBUG_DAYS) ; do \
		$(VALGRIND) $$day ; \
	done

.PHONY: checktest
checktest : out/debug/test.o
	-@$(VALGRIND) out/debug/test.o

.PHONY: expand
expand :
	-@if [ "$(day)" = "" ]; then \
		echo "No day provided - try with make expand day=XX " ; \
	else \
		mkdir -p out/debug ; \
		$(CC_EXPAND) src/days/day$(day).c >> out/debug/day$(day).c ; \
	fi

out/debug/day%.o : src/days/day%.c $(SRC) $(INCLUDE)
	mkdir -p out/debug
	$(CC_DEBUG) $< $(SRC) -o $@

out/debug/test.o : $(TEST_SRC) test/unittest.h $(SRC) $(INCLUDE)
	mkdir -p out/debug
	$(CC_DEBUG) test/unittest.c $(SRC) -o out/debug/test.o

out/release/day%.o : src/days/day%.c $(SRC) $(INCLUDE)
	mkdir -p out/release
	$(CC_RELEASE) $< $(SRC) -o $@
