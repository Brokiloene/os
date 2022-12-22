// var16 foo1-3(PrimeCount) foo2-4(GCD) 1-make

#include <stdio.h>
#include <dlfcn.h> // dll manipulation
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

const char* DLL_1 = "lib5_v1.so";
const char* DLL_2 = "lib5_v2.so";

int main(int argc, char const *argv[])
{
	
	void *handle = dlopen(DLL_1, RTLD_LAZY);
	check_not_null(handle, "dll open error\n");

	int (*PrimeCount)(int, int);
	int (*GCD)(int, int);
	PrimeCount = dlsym(handle, "PrimeCount");
	GCD = dlsym(handle, "GCD");

	char *err = dlerror();
	check_null(err, err);

	int mode, a, b;
	int cur_lib = 1;

	while(scanf("%d", &mode) > 0) {
		switch(mode) {
			case 0:
				check_ans(dlclose(handle), 0, "error with closing dll\n");
				if (cur_lib == 1) {
					handle = dlopen(DLL_2, RTLD_LAZY);
					cur_lib = 0;
					printf("switch lib from %s to %s\n", DLL_1, DLL_2);
				} else {
					handle = dlopen(DLL_1, RTLD_LAZY);
					cur_lib = 1;
					printf("switch lib from %s to %s\n", DLL_2, DLL_1);
				}
				check_not_null(handle, "dll open error\n");
				PrimeCount = dlsym(handle, "PrimeCount");
				GCD = dlsym(handle, "GCD");
				char *err = dlerror();
				check_null(err, err);
				break;
			case 1:
				check_ans(scanf("%d %d", &a, &b), 2, "scanf error\n");
				printf("PrimeCount = %d\n", PrimeCount(a, b));
				break;
			case 2:
				check_ans(scanf("%d %d", &a, &b), 2, "scanf error\n");
				printf("GCD = %d\n", GCD(a, b));
				break;
			default:
				check_ans(dlclose(handle), 0, "error with closing dll\n");
				return 0;
		}
	}

	return 0;
}
