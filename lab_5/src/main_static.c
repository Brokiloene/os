// var16 foo1-3(PrimeCount) foo2-4(GCD) 1-make

#include <stdio.h>

#include "lib5.h"



int main(int argc, char const *argv[])
{
	int mode, a, b;
	while(scanf("%d", &mode) > 0) {
		switch(mode) {
			case 1:
				check_ans(scanf("%d %d", &a, &b), 2, "scanf error\n");
				printf("PrimeCount = %d\n", PrimeCount(a, b));
				break;
			case 2:
				check_ans(scanf("%d %d", &a, &b), 2, "scanf error\n");
				printf("GCD = %d\n", GCD(a, b));
				break;
			default:
				return 0;
		}

	}
	return 0;
}
