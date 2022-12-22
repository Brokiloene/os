// child1

#include <stdio.h>    // STDOUT_FILENO & size_t
#include <sys/mman.h> // mmap & shm_open
#include <fcntl.h>    // O_xxxx   
#include <ctype.h>    // tolower
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
		check_err(sem_wait(&shr_str->p_to_c1), "sem_wait error");
		if (shr_str->size == STOP) {
			check_err(sem_post(&shr_str->c1_to_c2), "sem_post error");
			break;
		}
		for (int i = 0; i < shr_str->size; ++i) {
			shr_str->str[i] = tolower(shr_str->str[i]);
		}
		check_err(sem_post(&shr_str->c1_to_c2), "sem_post error");
	}
	shm_unlink(shmpath);

	return 0;
}
