#ifndef _LAB_5_
#define _LAB_5_

#include <stdio.h> // perror
#include <stdlib.h> // EXIT_FAILURE & exit

#define check_ans(foo, ok, msg) do { \
if (foo != ok) { perror(msg); exit(EXIT_FAILURE); } \
} while(0);

#define check_null(foo, msg) do { \
if (foo != NULL) { perror(msg); exit(EXIT_FAILURE); } \
} while(0);

#define check_not_null(foo, msg) do { \
if (foo == NULL) { perror(msg); exit(EXIT_FAILURE); } \
} while(0);

int PrimeCount(int A, int B);
int GCD(int A, int B);

#endif