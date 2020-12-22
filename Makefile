# Advent Of Code 2017 In C Makefile
# Usage: make all|help|dayXX

OUT_DIR = out
CC = gcc -std=c11 -Wall -Wextra
DAYS = \
	day01

INCLUDE = \
	src/aoc.h \
	src/utils/intarraylist.h

SRC = \
	src/utils/intarraylist.c

.DEFAULT_GOAL = all

.PHONY: all
all : $(DAYS)

help :
	@echo "+--------------------------------------------------------------------+"
	@echo "| Alcatraz Escapee (Alex O'Neill) Advent Of Code 2017 in C Solutions |"
	@echo "+--------------------------------------------------------------------+"
	@echo "To build and run a particular day, run 'make dayXX'"
	@echo "To build all days, run 'make all'"

.PHONY: day01
day01 : out/day01.o ; -@out/day01.o
out/day01.o : src/days/day01.c $(SRC) $(INCLUDE)
	mkdir -p out
	$(CC) src/days/day01.c $(SRC) -o out/day01.o

.PHONY: clean
clean :
	rm -rf out
