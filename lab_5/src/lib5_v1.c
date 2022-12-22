// PrimeCount - Наивный алгоритм. Проверить делимость текущего числа на все предыдущие числа.
// GCD - Алгоритм Евклида

#include "lib5.h"

void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int PrimeCount(int a, int b)
{
	printf("(lib5_v1)\n");
	int res = 0;
	for (int i = a; i <= b; ++i) {
		for (int j = 2; j < i; ++j) {
			if (i % j == 0 && i != j) {
				res -= 1;
				break;
			}
		}
		res += 1;
	}
	return res;
}

int GCD(int a, int b)
{
	printf("(lib5_v1)\n");
	while(b > 0) {
		if (a >= b) a = a % b;
		swap(&a, &b);
	}
	return a;
}

