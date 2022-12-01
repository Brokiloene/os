#ifndef _LAB4_UTILS_
#define _LAB4_UTILS_

#include <stdio.h>    // perror
#include <stdlib.h>   // EXIT_FAILURE
#include <semaphore.h>// sem_t

#define STOP -1
#define MAX_SIZE 1024

#define check_err(foo, msg) do { int __res = foo; \
if (__res == -1) { perror(msg); exit(EXIT_FAILURE); } \
} while(0);

typedef struct {
	sem_t p_to_c1;
	sem_t c1_to_c2;
	sem_t c2_to_p;
	int size;
	char str[MAX_SIZE];
} Shared_str;

#endif
