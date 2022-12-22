// child2

#include <sys/mman.h> // mmap & shm_open
#include <fcntl.h>    // O_xxxx 
#include <semaphore.h>// sem_t
 
#include "lab4_utils.h"

int main(int argc, char const *argv[])
{
	char *shmpath = "/shmlab4";
	int fd = shm_open(shmpath, O_RDWR, 0);
	check_err(fd, "shm_open error");

	Shared_str *shr_str = mmap(NULL, sizeof(*shr_str), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (shr_str == MAP_FAILED) {check_err(-1, "mmap error")};

	while(1) {
		/* waiting */
		check_err(sem_wait(&shr_str->c1_to_c2), "sem_wait error");
		if (shr_str->size == STOP) {
			break;
		}
		for (int i = 0, end = shr_str->size - 1; i < end; i++) {
			if (shr_str->str[i] == ' ' && shr_str->str[i] == shr_str->str[i + 1]) {
				for (int j = i, end = shr_str->size - 1; j < end; j++) {
					shr_str->str[j] = shr_str->str[j + 1];
				}
			shr_str->size--;
			i--;
			}
		}
		check_err(sem_post(&shr_str->c2_to_p), "sem_post error");
	}
	shm_unlink(shmpath);

	return 0;
}
