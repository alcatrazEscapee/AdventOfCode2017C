# Advent Of Code 2017 In C Makefile
# Usage: make all|help|dayXX

OUT_DIR = out
DAYS = $(addprefix $(OUT_DIR)/, \
	day01 \
	day02 \
	)

.DEFAULT_GOAL = all

all : $(DAYS)

help :
	@echo "+--------------------------------------------------------------------+"
	@echo "| Alcatraz Escapee (Alex O'Neill) Advent Of Code 2017 in C Solutions |"
	@echo "+--------------------------------------------------------------------+"
	@echo "To build a particular day, run 'make dayXX'"
	@echo "To build all days, run 'make all'"

.PHONY day01 : out/day01
out/day01 : main.c
	@echo Building day01
	@mkdir -p out
	cc main.c -o out/day01

.PHONY day02 : out/day02
out/day02 : main.c
	@echo Building day02
	@mkdir -p out
	cc main.c -o out/day02

.PHONY clean :
	rm -rf out
