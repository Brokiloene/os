// PrimeCount - Решето Эратосфена
// GCD - Наивный алгоритм. Пытаться разделить числа на все числа, что меньше A и B.
#include <stdio.h> // printf
#include <stdlib.h> // calloc & free & exit & EXIT_FAILURE

#include "lib5.h"

void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int PrimeCount(int a, int b)
{
	printf("(lib5_v2)\n");
	int *erat = (int *) calloc(b + 1, sizeof(int));
	if (erat == NULL) {
		printf("calloc error\n"); 
		exit(EXIT_FAILURE);
	}
	for (int i = 2; i * i <= b; ++i) {
		for (int j = i * i; j <= b; j += i) {
			erat[j] = 1;
		}
	}
	int res = 0;
	for (int i = a; i <= b; ++i) {
		if (erat[i] == 0) res += 1;
	}
	free(erat);
	return res;
}

int GCD(int a, int b)
{
	printf("(lib5_v2)\n");
	if (a > b) swap(&a, &b);
	for (int i = a; i > 0; --i) {
		if (a % i == 0 && b % i == 0) {
			return i;
		}
	}
	return 1;
}