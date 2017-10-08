#pragma once

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

// Fast math floor
int xfl(const float x);

// Fast math ceil
int xcl(const float x);

// Fast math decimal
float xdec(const float x);

// Returns the number of lines in a file
int xlns(FILE* const file);

// Returns a line from a file - forwards file pointer
char* xreadln(FILE* const file);

// Prints an error message and exits
void xbomb(const char* const message, ...);

// Returns true if number is odd
bool xodd(const int a);

// Makes a number even
int xbalance(const int a);

// Takes two strings and returns a concatenated string
char* xconcat(const char* const a, const char* const b);

// Prints the license
void xlicense();

// Get the lenght of an array
#define xlen(a) ((int) (sizeof(a) / sizeof(*a)))

// Type safe ease of use calloc
#define xwipe(t, n) ((t*) calloc((n), sizeof(t)))

// Type safe ease of use malloc
#define xtoss(t, n) ((t*) malloc((n) * sizeof(t)))

// Type safe ease of use realloc
#define xretoss(ptr, t, n) (ptr = (t*) realloc((ptr), (n) * sizeof(t)))

// Ease of use memset for zeroing out an array
#define xzero(a) (memset(&(a), 0, sizeof(a)))

// Generic type macro for finding max of two type values
#define xmax(a, b) ((a) > (b) ? (a) : (b))

// Generic type macro for finding min of two type values
#define xmin(a, b) ((a) < (b) ? (a) : (b))

// Minimum array size needed for an integer to be converted to a string
#define MINTS ((CHAR_BIT * sizeof(int) - 1) / 3 + 2)
