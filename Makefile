# Advent Of Code 2017 In C Makefile
# Usage: make help

# Directory structure
DEBUG_DIR    := out/debug
RELEASE_DIR  := out/release
SRC_DIR      := src/main
LIB_DIR      := src/lib
TEST_DIR     := src/test

# Compile commands
GCC_DEBUG    := gcc -std=c11 -g -O0 -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable
GCC_EXPAND   := gcc -std=c11 -E -P
GCC_RELEASE  := gcc -std=c11 -O3
VALGRIND     := valgrind --tool=memcheck --leak-check=full

# Source files
# SRC are compiled sources
# INCLUDE are files that are not compiled directly, but still need to be tracked for changes
DAYS_SRC     := $(shell find $(SRC_DIR) -name 'day*.c')
DAYS_DEBUG   := $(DAYS_SRC:$(SRC_DIR)/%.c=$(DEBUG_DIR)/%.o)
DAYS_RELEASE := $(DAYS_SRC:$(SRC_DIR)/%.c=$(RELEASE_DIR)/%.o)

LIB_SRC      := $(shell find $(LIB_DIR) -name '*.c' ! -name '*.template.c')
LIB_INCLUDE  := $(shell find $(LIB_DIR) -name '*.h' -o -name '*.template.*')

TEST_SRC     := $(shell find $(TEST_DIR) -name '*.c' ! -name '*.template.c')
TEST_INCLUDE := $(shell find $(TEST_DIR) -name '*.template.c') $(TEST_DIR)/unittest.h 

.DEFAULT_GOAL = run

help :
	@echo "+--------------------------------------------------------------------+"
	@echo "| Alcatraz Escapee (Alex O'Neill) Advent Of Code 2017 in C Solutions |"
	@echo "+--------------------------------------------------------------------+"
	@echo "Options:"
	@echo "  make day=XX        - Build and run day XX"
	@echo "  make all           - Build and run all days"
	@echo "  make test          - Build and run unit tests"
	@echo "  make check day=XX  - Build and run day XX with Valgrind"
	@echo "  make checkall      - Build and run all days with Valgrind"
	@echo "  make checktest     - Build and run unit tests with Valgrind"
	@echo "  make clean         - Cleans all compiled binaries"

.PHONY: run
run: out/release/day$(day).o
	-@if [ "$(day)" = "" ]; then \
		echo "No day provided - try with make day=XX" ; \
	else \
		out/release/day$(day).o ; \
	fi

.PHONY: all
all: $(RELEASE_DAYS)
	-@for day in $(RELEASE_DAYS) ; do \
        $$day ; \
    done

.PHONY: test
test : out/release/test.o
	-@out/release/test.o

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

.PHONY: clean
clean :
	rm -rf out

# Release Configuraton
$(RELEASE_DIR)/day%.o : src/main/day%.c $(LIB_SRC) $(LIB_INCLUDE)
	mkdir -p $(RELEASE_DIR)
	$(GCC_RELEASE) $< $(LIB_SRC) -o $@

$(RELEASE_DIR)/test.o : $(TEST_SRC) $(LIB_SRC) $(TEST_INCLUDE) $(LIB_INCLUDE)
	mkdir -p $(RELEASE_DIR)
	$(GCC_DEBUG) $(TEST_SRC) $(LIB_SRC) -o $(RELEASE_DIR)/test.o

# Debug Configuration
$(DEBUG_DIR)/day%.o : src/main/day%.c $(LIB_SRC) $(LIB_INCLUDE)
	mkdir -p $(DEBUG_DIR)
	$(GCC_DEBUG) $< $(LIB_SRC) -o $@

$(DEBUG_DIR)/test.o : $(TEST_SRC) $(LIB_SRC) $(TEST_INCLUDE) $(LIB_INCLUDE)
	mkdir -p $(DEBUG_DIR)
	$(GCC_DEBUG) $(TEST_SRC) $(LIB_SRC) -o $(DEBUG_DIR)/test.o
