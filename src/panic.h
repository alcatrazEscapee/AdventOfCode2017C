// Implements panic methods
// These immediately exit the program, hopefully with an identifiable and informative error message.

#ifndef PANIC_H
#define PANIC_H

#include <stdio.h> // printf

#define PANIC(args...) do { printf("PANIC!\n"); printf(args); exit(1); } while (0)
#define PANIC_IF_NULL(value, args...) do { if (value == NULL) { printf("PANIC! %s is NULL!\n", #value); printf(args); exit(1); }} while (0)

#endif