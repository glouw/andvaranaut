#pragma once

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

// Fast math floor
int fl(const float x);

// Fast math ceil
int cl(const float x);

// Fast math decimal
float dec(const float x);

// Returns the number of lines in a file
int lns(FILE* const file);

// Returns a line from a file - forwards file pointer
char* readln(FILE* const file);

// Prints an error message and exits
void bomb(const char* const message, ...);

// Returns true if number is odd
bool odd(const int a);

// Makes a number even
int balance(const int a);

// Takes two strings and returns a concatenated string
char* concat(const char* const a, const char* const b);

/*
 * Some macros
 */

#define len(a) ((int) (sizeof(a) / sizeof(*a)))

// Type safe ease of use malloc
#define toss(t, n) ((t*) malloc((n) * sizeof(t)))

// Type safe ease of use realloc
#define retoss(ptr, t, n) (ptr = (t*) realloc((ptr), (n) * sizeof(t)))

// Ease of use memset for zeroing out an array
#define zero(a) (memset(&(a), 0, sizeof(a)))

// Generic type macro for finding max of two type values
#define max(a, b) ((a) > (b) ? (a) : (b))

// Generic type macro for finding min of two type values
#define min(a, b) ((a) < (b) ? (a) : (b))

// Minimum array size needed for an integer to be converted to a string
#define MINTS ((CHAR_BIT * sizeof(int) - 1) / 3 + 2)
