# Advent Of Code 2017 In C Makefile
# Usage: make all|help|day=XX

OUT_DIR = out/
CC = gcc -std=c11 -Wall -Wextra
DAYS = $(addprefix $(OUT_DIR), \
		day01.o \
		day02.o \
	)


INCLUDE = \
	src/aoc.h \
	src/utils/intarraylist.h

SRC = \
	src/aoc.c \
	src/utils/intarraylist.c

.DEFAULT_GOAL = run

help :
	@echo "+--------------------------------------------------------------------+"
	@echo "| Alcatraz Escapee (Alex O'Neill) Advent Of Code 2017 in C Solutions |"
	@echo "+--------------------------------------------------------------------+"
	@echo "To build and run a particular day, run 'make day=XX'"
	@echo "For all days, run 'make all'"

.PHONY: one
run: out/day$(day).o ; -@out/day$(day).o

.PHONY: all
all: $(DAYS)
	-@for day in $(DAYS) ; do \
        $$day ; \
    done

out/day%.o : src/days/day%.c $(SRC) $(INCLUDE)
	mkdir -p out
	$(CC) $< $(SRC) -o $@

.PHONY: clean
clean :
	rm -rf out/*.o
